#ifndef MYTFTP_H
#define MYTFTP_H
#include "Common.h"
#include "MyUdp.h"
#include "MyBase64.h"
#include "MyThread.h"
#include <semaphore.h>
/* TFTP protocol:
 * data1:
 *     0x0001 RRQ(read)
 *     0x0002 WRQ(write)(2byte) FILENAME(nbyte) 0 MODE(nbyte) 0
 * data2:
 *     0x0003 block_num(2byte) data(0-512byte)
 * data3:(ACK)
 *     0x0004 block_num(2byte)
 * data4:(error)
 *     0x0005 error_num(2byte) error_msg(nbyte) 0
 */
namespace my_master {

#define TFTP_BUF_SIZE 1024
#define TFTP_DATA_SIZE 512

#define TFTP_HEAD_SIZE 2
#define TFTP_BLOCKNUM_SIZE 2
#define TFTP_ERRNUM_SIZE 2
class MyTFTP : public my_master::MyUdp , public my_master::MyThread
{
public:
    typedef struct
    {
        uint16_t block_num;
        FILE* fp;
    }recv_t;
    typedef struct
    {
        std::string filename;
        MyAddrInfo info;
        FILE* fp;
        sem_t event;

        uint16_t block_num;
        int send_block;

        char* file_buf;
        int file_len;
    }send_t;

    MyTFTP(std::string ip,uint16_t port, bool isServer = true);
    ~MyTFTP();
    ///////////////////////////////////////////////
    /// override MyUdp virtual method
    virtual void* CallBackFunc(MyEvent *ev);
    ///////////////////////////////////////////////
    /// override MyThread virtual method
    virtual void Run();
    virtual void OnInit();
    virtual void OnExit();

    void SetRootDir(std::string path); // /home/kpli/
    void SendFile(MyAddrInfo& info, std::string filename); // TODO...
    void GetFile(MyAddrInfo& info, std::string filename);  // TODO...
private:
    uint16_t GetHead(char* buf);
    uint16_t GetBlockNum(char* buf);
    int HandleMsg(MyAddrInfo& info,char* buf, int len);
    int HandleMsg1(MyAddrInfo& info,char* buf, int len);
    int HandleMsg2(MyAddrInfo& info,char* buf, int len);
    int HandleMsg3(MyAddrInfo& info,char* buf, int len);
    int HandleMsg4(MyAddrInfo& info,char* buf, int len);
    int HandleMsg5(MyAddrInfo& info,char* buf, int len);

    int BuildRRQ(std::string filename);
    int BuildWRQ(std::string filename);
    int BuildData(uint16_t block_num, char* buf, uint16_t len); // len (0-512byte)
    int BuildACK(uint16_t block_num);
    int BuildErrMsg(uint16_t err_num, std::string errMsg);

    void InitFileTrans(std::string filename, MyAddrInfo &info);
    void ReadFile(std::string filename);
    int GetFileData(char** buf);
    void CloseFileTrans();

    char m_buf[TFTP_BUF_SIZE];
    std::string m_path;
    recv_t m_recv;
    send_t m_send;
};

/* client send file, server recv file:
        server: (recv file)
            recv w request
            send ack

            recv data
            send ack

            until data < 512
            close file
        client: (send file)
            initFileTrans
            build W request
            wait ack

            loop:
                send data
                wait ack

            CloseFileTrans


        server: (send file)
            handle r request (if isExist)
            send ack

            invoke send method
                init m_send
                begin thread
                    send file data


 */

} // end namespace
#endif // MYTFTP_H
