//
//  IOSocket.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/9/9.
//
//

#include "IOSocket.hpp"
#include <sys/socket.h>


IOSocket::IOSocket(){
    sockfd = INVALID_SOCKET;
}

IOSocket::~IOSocket(){
    onClose();
}

//ms毫秒 us微妙 s秒
void IOSocket::sleep(unsigned long ms){
    struct timeval tv;
    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;
    select(0, 0, 0, 0, &tv);
}

bool IOSocket::onCreate(/*const char* domainStr, int port*/){
    if (sockfd != INVALID_SOCKET) {
        return false;
    }
    sockfd = Net::tcpsocket4client();
    return (sockfd != INVALID_SOCKET);
}

void IOSocket::onClose(){
    if (sockfd == INVALID_SOCKET) {
        return;
    }
    closesocket(sockfd);
    sockfd = INVALID_SOCKET;
}

bool IOSocket::onShutdown(int howto){
    if (sockfd == INVALID_SOCKET) {
        return false;
    }
    if (shutdown(sockfd, howto) == 0) {
        return true;
    } else {
        return false;
    }
}