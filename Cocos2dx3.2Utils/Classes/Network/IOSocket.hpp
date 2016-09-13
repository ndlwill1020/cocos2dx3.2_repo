//
//  IOSocket.hpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/9/9.
//
//

#ifndef IOSocket_hpp
#define IOSocket_hpp
#include "SocketBase.hpp"

class IOSocket{
protected:
    IOSocket();
    ~IOSocket();
    
    static void sleep(unsigned long ms);
    
    virtual bool onCreate(/*const char* domainStr, int port*/);
    virtual void onClose();
    virtual bool onShutdown(int howto);
    
    virtual bool onConnect(const char* domainStr, int port) = 0;
    virtual long onReceive(void* pBuf, int bufLen) = 0;
    virtual long onForceReceive(void* pBuf, int bufLen) = 0;
    virtual long onSend(const void* pBuf, int bufLen) = 0;
protected:
    SOCKET sockfd;
};

#endif /* IOSocket_hpp */
