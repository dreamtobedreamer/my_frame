#ifndef MyEasyTcpClient_H
#define MyEasyTcpClient_H

#include "MyNet.h"
#include "MyVec.h"


/**
 * 限制：
 *      1. 读写一帧的大小不能超过65535字节
 */
class MyDataParser
{
public:
    MyDataParser();
    virtual ~MyDataParser();

    /**
     * @brief GetDataFromSocket 从 Socket 读数据
     */
    void GetDataFromSocket(const char* inBuf, int inLen);

    /**
     * @brief GetFrame 获得一帧的数据
     * @param outBuf   获得数据指针
     * @return 0： 没有消息，大于0：获得消息的长度
     */
    int GetFrame(char **outBuf);

    /**
     * @brief WriteFrame
     * @param inBuf   输入的数据
     * @param inLen   输入的长度
     * @param outBuf  加工过的数据
     * @return        加工过的数据的长度
     */
    int WriteFrame(const char* inBuf, int inLen, char** outBuf);

    static void Test();

protected:
    void CreateFrameBuffer();
    void DestoryFrameBuffer();
private:
    /* 将收到的数据缓存到该数组中，然后等待解析成帧供客户端解析 */
    MyVec<char> mRecvDataCache;
    char* mFrameBuffer;
    char* mWriteBuffer;
};

class MyEasyTcpClient
{
public:
    MyEasyTcpClient(MyAddrInfo& inAddrInfo);
    virtual ~MyEasyTcpClient();

    int Connect();
    int SetReuseSock();
    int32_t Send(const char* inData, size_t inLen);
    int32_t Recv(void* inData, size_t inLen);
protected:
    int Socket();
    int SetNonBlocking(bool b);
private:
    bool Init();
    void Exit();
private:
    MyAddrInfo mAddrInfo;
    SOCKET mSock;
};

#endif
