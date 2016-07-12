//
//  TCPSocket.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/7/11.
//
//

#include "TCPSocket.hpp"

#include <sys/socket.h>

#include <netdb.h>

#include <arpa/inet.h>






//struct addrinfo {
//	int	ai_flags;	/* AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST */
//    int	ai_family;	/* PF_xxx */
//    int	ai_socktype;	/* SOCK_xxx */
//    int	ai_protocol;	/* 0 or IPPROTO_xxx for IPv4 and IPv6 */
//    socklen_t ai_addrlen;	/* length of ai_addr */
//    char	*ai_canonname;	/* canonical name for hostname */
//    struct	sockaddr *ai_addr;	/* binary address */
//    struct	addrinfo *ai_next;	/* next structure in linked list */
//};

bool TCPSocket::isIpv6Network(const std::string& domainStr/* = "www.baidu.com"*/){
    bool isIpv6Net = false;
    
    struct addrinfo* result = nullptr,*cur = nullptr;
    
    /*
    struct sockaddr_in6 addr_in6;
    //bzero(&addr_in6, sizeof(addr_in6));
    memset(&addr_in6, 0, sizeof(addr_in6));
    addr_in6.sin6_family = AF_INET6;
    */
    
    //IPv4中使用gethostbyname()函数完成主机名到地址解析，这个函数仅仅支持IPv4
    //IPv6中引入了getaddrinfo()的新API，它是协议无关的，既可用于IPv4也可用于IPv6
    int ret = getaddrinfo(domainStr.c_str(), nullptr, nullptr, &result);
    if (ret == 0) {
        for (cur = result; cur != nullptr; cur = cur->ai_next) {
            switch (cur->ai_family) {//ai 表示 addrinfo(命名规则)
                case AF_INET6:
                    log("ipv6!!!!!");
                    isIpv6Net = true;
                    break;
                case AF_INET:
                    log("ipv4!!!!!");
                    break;
                default:
                    break;
            }
        }
    }
    //getaddrinfo返回的所有存储空间都是动态获取的，这些存储空间必须通过调用freeaddrinfo返回给系统
    freeaddrinfo(result);
    return isIpv6Net;
}

//客户端使用时，要把这个字符串ip转成网络字节序（32位整数）
std::string TCPSocket::domain2Ip(const char* pDomain){
    if (isIpv6Network()) {
        struct addrinfo hint;
        memset(&hint, 0, sizeof(hint));
        hint.ai_family = AF_INET6;
        hint.ai_flags = AI_V4MAPPED;
        
        struct addrinfo* result = nullptr;
        getaddrinfo(pDomain, nullptr, &hint, &result);
        
        if (result != nullptr) {
            char hostname[1025] = "";
            //The getnameinfo function converts an address(struct sockaddr) into a host name and a service name
            getnameinfo(result->ai_addr, result->ai_addrlen, hostname, sizeof(hostname), nullptr, 0, 0);
            char ipv6[128] = "";
            memcpy(ipv6, hostname, sizeof(ipv6));
            
            freeaddrinfo(result);
            return ipv6;
        }
        
        
    } else {
        struct hostent *pHostent = gethostbyname(pDomain);
        /*
        //我的写法
        if (pHostent != NULL) {
            char ip[16] = {0};
            
            //inet_ntop  它不仅可以用于转换IPv4地址，还可以用来转换IPv6的地址
            inet_ntop(pHostent->h_addrtype, (pHostent->h_addr_list)[0], ip, sizeof(ip));
            
            return ip;
        }
        */
        
        
        
        //网上写法
        if (pHostent != NULL) {
            unsigned char* addr = (unsigned char*)(pHostent->h_addr_list)[0];
            if (addr != NULL) {
                char ip[16] = {0};
                //%u 十进制无符号整数
                sprintf(ip, "%u.%u.%u.%u", addr[0], addr[1], addr[2], addr[3]);
                return ip;
            }
        }
    }
    return "";
}

//兼容ipv6&ipv4
//1.socket初始化
//sockfd = socket((isIpv6Network() ? AF_INET6 : AF_INET), SOCK_STREAM, IPPROTO_TCP);



//2.socket connect
//connect 第二个参数地址
/*
struct sockaddr* getSockAddr(){
    return (isIpv6Network() ? (struct sockaddr*)&addr_in6 : (struct sockaddr*)&addr_in);
}
 */
//connect 第三个参数地址大小
/*
 int getSockLength(){
    return (isIpv6Network() ? sizeof(sockaddr_in6) : sizeof(sockaddr_in));
 }
 */
//int ret = connect(sockfd, getSockAddr(), getSockLength());


//Socket发送接口及接收数据接口都不需要去处理，另外需要考虑的是Socket的重连，及网络切换的问题
