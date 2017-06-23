#include "MyTcp.h"
#include "MyApp.h"
#include "MyAllEvent.h"
#include "MyUdp.h"
using namespace my_master;
////////////////////////////////////////////////////
/// MyTcpServer
MyTcpServer::MyTcpServer(std::string ip, uint16_t port)
    :MySock(ip,port,SOCK_STREAM,true)
{}
MyTcpServer::~MyTcpServer()
{}
int MyTcpServer::Listen(int backlog)
{
    if(!IS_SERVER(m_tcp_ip_type))
        return 0;
    int res = -1;
    SetNonblock(true);
    res = listen(m_sock,backlog);
    assert(res == 0);
    return res;
}

int MyTcpServer::Accpet(struct sockaddr_in *addr)
{
    if(!IS_SERVER(m_tcp_ip_type))
        return 0;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int res = accept(m_sock,(sockaddr*)addr,&addrlen);
    return res; // file descriptor
}
////////////////////////////////////////////////////
/// MyTcpClient
MyTcpClient::MyTcpClient(std::string ip,uint16_t port)
    :MySock(ip,port,SOCK_STREAM,false)
{}
MyTcpClient::~MyTcpClient()
{}

int MyTcpClient::Read(char* buf,int len)
{
    if(IS_SERVER(m_tcp_ip_type))
        return 0;
    return read(m_sock,buf,len);
}

int MyTcpClient::Write(const char* buf, int len)
{
    if(IS_SERVER(m_tcp_ip_type))
        return 0;
    return write(m_sock,buf,len);
}
int MyTcpClient::Connect()
{
    if(IS_SERVER(m_tcp_ip_type))
        return -1;

    int ret;

    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(m_port);
    inet_pton(AF_INET, m_ip.c_str(), &m_addr.sin_addr);
    ret = connect(m_sock, (struct sockaddr*)&m_addr, sizeof(m_addr));
    if(ret != 0)
        perror("connect fail");
    assert(ret == 0);
    SetNonblock(true);
    return ret;
}

///////////////////////////////////////////////////////
/// MyTcpSocket
MyTcpSocket::MyTcpSocket(int fd, sockaddr_in addr)
{
    this->m_sock = fd;
    memcpy(&m_addr,&addr,sizeof(addr));
    Common::SetNonblock(fd,true);
}
MyTcpSocket::MyTcpSocket(const MyTcpSocket& other)
{
    memcpy(&m_addr,&other.m_addr,sizeof(m_addr));
    m_sock = other.m_sock;
    Common::SetNonblock(m_sock,true);
}

MyTcpSocket::~MyTcpSocket()
{
    close(m_sock);
}

std::string MyTcpSocket::GetIp()
{
    return MyNet::GetAddrIp(&m_addr);
}

unsigned short MyTcpSocket::GetPort()
{
    return MyNet::GetAddrPort(&m_addr);
}

void* MyTcpSocket::CallBackFunc(MyEvent *ev)
{
    MyAllEvent::BoardEvent(ev);
    return NULL;
}

MyTcpSocket& MyTcpSocket::operator=(MyTcpSocket& other)
{
    memcpy(&m_addr,&other.m_addr,sizeof(m_addr));
    m_sock = other.m_sock;
    return *this;
}

int MyTcpSocket::Read(char* buf,int len)
{
    return read(m_sock,buf,len);
}

int MyTcpSocket::Write(const char* buf, int len)
{
    return write(m_sock,buf,len);
}

/////////////////////////////////////////////////////////////
/// MyTcpFrame

MyTcpFrame::MyTcpFrame()
    :m_iscomplete(true),
      m_len(0)
{
}

MyTcpFrame::~MyTcpFrame(){}

int MyTcpFrame::EasyWrite(const char* buf, uint16_t len)
{
    int lenx = 0;
    int index = 0;
    int res = 0;

    char* temp = MySelfProtocol::GetBuf(&lenx);
    index += MySelfProtocol::BuildHeader(len,temp,lenx);
    memcpy(&temp[sizeof(len)],buf,len);
    index += len;
    res = WriteBuf(temp,index);
    MySelfProtocol::FreeBuf(temp);
    return (res - sizeof(len));
}

int MyTcpFrame::GetBuf1()
{
    char buf[1024] = {0};
    char buf_len[2] = {0};
    int res = 0, res1 = 1;

    while(1)
    {
        res = ReadBuf(buf,sizeof(buf));
        if(res == 0)
        {
            return Frame(NULL,0);
        }
        if(res < 0)
            break;
        for(int i = 0; i < res; ++i)
            m_datas.push(buf[i]);

        while(1)
        {
            if(m_iscomplete && m_datas.size() >= 2)
            {
                buf_len[0] = m_datas.front();
                m_datas.pop();
                buf_len[1] = m_datas.front();
                m_datas.pop();
                memcpy(&m_len,buf_len,sizeof(m_len));
                if(m_datas.size() >= m_len)
                {
                    char temp[m_len] = {0};
                    for(int i = 0; i < m_len; ++i)
                    {
                        temp[i] = m_datas.front();
                        m_datas.pop();
                    }
                    res1 = Frame(temp,m_len);
                    m_iscomplete = true;
                }else
                {
                    m_iscomplete = false;
                    break;
                }
            }else if(!m_iscomplete)
            {
                if(m_datas.size() >= m_len)
                {
                    char temp[m_len] = {0};
                    for(int i = 0; i < m_len; ++i)
                    {
                        temp[i] = m_datas.front();
                        m_datas.pop();
                    }
                    res1 = Frame(temp,m_len);
                    m_iscomplete = true;
                }else
                    break;
            }else
                break;
        } // end while
    } //end while
    return res1;
}

/* res:
 * =0 client quit
 * >0 data is coming
 * <0 end of file
*/
//ssize_t MyTcpFrame::ReadN(char* buf, size_t len)
//{

//}

//ssize_t MyTcpFrame::WriteN(const char* buf, size_t len)
//{

//}

//////////////////////////////////////////////////////////////////
/// MyEasyTcpSocket
MyEasyTcpSocket::MyEasyTcpSocket(int fd, sockaddr_in addr)
    :MyTcpSocket(fd,addr)
{}

MyEasyTcpSocket::~MyEasyTcpSocket(){}

void* MyEasyTcpSocket::CallBackFunc(MyEvent *ev)
{
    if(GetBuf1())
        MyApp::theApp->AddEvent(ev);
}

int MyEasyTcpSocket::ReadBuf(char* buf, int len)
{
    return Read(buf,len);
}

int MyEasyTcpSocket::WriteBuf(const char* buf, int len)
{
    return Write(buf,len);
}


///////////////////////////////////////////////////////////////////
/// MyEasyTcpClient
MyEasyTcpClient::MyEasyTcpClient(std::string ip, uint16_t port)
    :MyTcpClient(ip,port)
{}

MyEasyTcpClient::~MyEasyTcpClient(){}

int MyEasyTcpClient::ReadBuf(char* buf, int len)
{
    return Read(buf,len);
}

int MyEasyTcpClient::WriteBuf(const char* buf, int len)
{
    return Write(buf,len);
}

void* MyEasyTcpClient::CallBackFunc(MyEvent *ev)
{
    if(GetBuf1())
        MyApp::theApp->AddEvent(ev);
}















