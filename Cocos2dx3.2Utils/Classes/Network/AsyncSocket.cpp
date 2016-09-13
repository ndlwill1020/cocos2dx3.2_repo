//
//  AsyncSocket.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/9/9.
//
//

#include "AsyncSocket.hpp"
#include <errno.h>
#include <sys/socket.h>

AsyncSocket::AsyncSocket():IOSocket(){

}

AsyncSocket::~AsyncSocket(){
    onClose();
}

bool AsyncSocket::onCreate(){
    bool ret = IOSocket::onCreate();
    if (ret) {
        if (Net::setNonblcok(sockfd, true) < 0) {
            ret = false;
            onClose();
        }
    }
    return ret;
}

bool AsyncSocket::onConnect(const char* domainStr, int port, unsigned long timeout_ms){
    if (sockfd == INVALID_SOCKET) {
        return false;
    }
    bool ret = false;
    Net::tcpconnect(sockfd, domainStr, port);
    do {
        fd_set wfds;
        FD_ZERO(&wfds);
        FD_SET(sockfd, &wfds);
        
        struct timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;
        
        int selectValue = 0;
        selectValue = select(sockfd + 1, NULL, &wfds, NULL, &tv);
        
        if (selectValue == 0) {
            fprintf(stderr, "connect timeout. return:%d", selectValue);
            break;
        } else if (selectValue == -1) {
            int errorno = Net::getError(sockfd);
            if (errorno != 0) {
                fprintf(stderr, "connect error. errorno:%d  global errno:%d", errorno, errno);
                break;
            }
        }
        
        
        ret = true;
    } while (0);
    return ret;
}

long AsyncSocket::onReceive(void* pBuf, int bufLen, unsigned long timeout_ms){
    if (sockfd == INVALID_SOCKET) {
        return -1;
    }
    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
    
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);
    
    fd_set errorfds;
    FD_ZERO(&errorfds);
    FD_SET(sockfd, &errorfds);
    
    select(sockfd + 1, &readfds, NULL, &errorfds, &tv);
    long recvValue = -2;
    if (FD_ISSET(sockfd, &readfds)) {
        recvValue = recv(sockfd, pBuf, bufLen, 0);
    } else if (FD_ISSET(sockfd, &errorfds)) {
        return -1;
    }
    
    /*
     recvValue:
     0标识socket关闭
     -1标识recv错误
     -2标识没有可读数据
     >0标识接收数据成功
     */
    return recvValue;
}

long AsyncSocket::onForceReceive(void* pBuf, int bufLen, unsigned long timeout_ms){
    Net::setNonblcok(sockfd, false);
    long ret = onReceive(pBuf, bufLen, timeout_ms);
    Net::setNonblcok(sockfd, true);
    return ret;
}

long AsyncSocket::onSend(const void* pBuf, int bufLen, unsigned long timeout_ms){
    if (pBuf == NULL || sockfd == INVALID_SOCKET) {
        return -1;
    }
    
    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
    
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(sockfd, &writefds);
    
    fd_set errorfds;
    FD_ZERO(&errorfds);
    FD_SET(sockfd, &errorfds);
    
    select(sockfd + 1, NULL, &writefds, &errorfds, &tv);
    long sendValue = 0;
    if (FD_ISSET(sockfd, &writefds)) {
        sendValue = send(sockfd, pBuf, bufLen, 0);
        if (sendValue != bufLen) {
            return -1;
        }
    } else if (FD_ISSET(sockfd, &errorfds)) {
        //has error! need reconnect
        return -2;
    }
    return sendValue;
}

#pragma mark------------------------------------------------------------------------
bool AsyncSocket::onConnect(const char* domainStr, int port){
    return onConnect(domainStr, port, ASYNC_SOCKET_TIMEOUT_MS);
}

long AsyncSocket::onReceive(void* pBuf, int bufLen){
    return onReceive(pBuf, bufLen, ASYNC_SOCKET_TIMEOUT_MS);
}

long AsyncSocket::onForceReceive(void* pBuf, int bufLen){
    return onForceReceive(pBuf, bufLen, ASYNC_SOCKET_TIMEOUT_MS);
}

long AsyncSocket::onSend(const void* pBuf, int bufLen){
    return onSend(pBuf, bufLen, ASYNC_SOCKET_TIMEOUT_MS);
}