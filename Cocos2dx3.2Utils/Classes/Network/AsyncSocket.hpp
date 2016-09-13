//
//  AsyncSocket.hpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/9/9.
//
//

#ifndef AsyncSocket_hpp
#define AsyncSocket_hpp
#include "IOSocket.hpp"

#define OUTPUT_DLL
#define ASYNC_SOCKET_TIMEOUT_MS 3000

class OUTPUT_DLL AsyncSocket : public IOSocket{
protected:
    AsyncSocket();
    ~AsyncSocket();
public:
    virtual bool onCreate();
    virtual bool onConnect(const char* domainStr, int port, unsigned long timeout_ms);
    virtual long onReceive(void* pBuf, int bufLen, unsigned long timeout_ms);
    virtual long onForceReceive(void* pBuf, int bufLen, unsigned long timeout_ms);
    virtual long onSend(const void* pBuf, int bufLen, unsigned long timeout_ms);
    
    virtual bool onConnect(const char* domainStr, int port);
    virtual long onReceive(void* pBuf, int bufLen);
    virtual long onForceReceive(void* pBuf, int bufLen);
    virtual long onSend(const void* pBuf, int bufLen);
};

#endif /* AsyncSocket_hpp */
