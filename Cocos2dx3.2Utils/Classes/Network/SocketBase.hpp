//
//  SocketBase.hpp
//  
//
//  Created by macuser on 16/9/7.
//
//

#ifndef SocketBase_hpp
#define SocketBase_hpp
#include "cocos2d.h"
USING_NS_CC;
#include <string>

#define SOCKET int
#define closesocket close
#define INVALID_SOCKET -1

//using namespace std;

namespace Net {
    SOCKET tcpsocket4server(const char* domainStr, int port, int reuseaddr = 0);
    SOCKET tcpsocket4client();
    int tcpconnect(SOCKET sockfd, const char* domainStr, int port);
    
    /*
     usage:
     setNonblcok(sockfd, O_NONBLOCK, true);//开启O_NONBLOCK
     setNonblcok(sockfd, O_NONBLOCK, false);//关闭O_NONBLOCK
     success:0 fail:-1
     */
    int setNonblcok(SOCKET sockfd, int flags, bool isOn);
    int setKeepAlive(SOCKET sockfd, int optval = 1);
    
    int setReuseaddr(SOCKET sockfd, int optval);//for server
    
    //判断是否IPv6网络
    bool isIpv6Network(const std::string& domainStr = "www.baidu.com");
    //域名解析函数(域名字符串－》ip字符串)
    std::string domain2Ip(const char* pDomain);
    
    //对于服务器来说，在bind以后就可以调用getsockname来获取本地地址和端口
    int getLocalAddr(SOCKET sockfd, char* ip, int* port);//得到本地地址
    
    
    int getPeerAddr(SOCKET sockfd, char* ip, int* port);//获得客户端的ip和端口   for server
}


#endif /* SocketBase_hpp */
