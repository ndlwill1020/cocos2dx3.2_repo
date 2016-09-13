//
//  SocketBase.cpp
//  
//
//  Created by macuser on 16/9/7.
//
//

#include "SocketBase.hpp"

#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>



namespace Net {
    SOCKET tcpsocket4server(const char* domainStr, int port, int reuseaddr){
        SOCKET sockfd = INVALID_SOCKET;
        sockfd = socket((isIpv6Network() ? AF_INET6 : AF_INET), SOCK_STREAM, 0);
        if (sockfd < 0) {
            fprintf(stderr, "socket() error!\n");
            return INVALID_SOCKET;
        }
        
        if (reuseaddr) {
            int optval = 1;
            setReuseaddr(sockfd, optval);
        }
        
        if (isIpv6Network()) {
            struct sockaddr_in6 addr_in6;
            memset(&addr_in6, 0, sizeof(addr_in6));
            addr_in6.sin6_family = AF_INET6;
            addr_in6.sin6_port = htons((short)port);
            if (inet_pton(AF_INET6, domain2Ip(domainStr).c_str(), &addr_in6.sin6_addr) <= 0) {
                return -1;
            }
            int ret = bind(sockfd, (struct sockaddr*)&addr_in6, sizeof(addr_in6));
            if (ret < 0) {
                fprintf(stderr, "bind address error! ret = %d \n", ret);
                closesocket(sockfd);
                return INVALID_SOCKET;
            }
        } else {
            struct sockaddr_in addr_in;
            memset(&addr_in, 0, sizeof(addr_in));
            addr_in.sin_family = AF_INET;
            addr_in.sin_port = htons((short)port);
            if (inet_pton(AF_INET, domain2Ip(domainStr).c_str(), &addr_in.sin_addr.s_addr) <= 0) {
                return -1;
            }
            int ret = bind(sockfd, (struct sockaddr*)&addr_in, sizeof(addr_in));
            if (ret < 0) {
                fprintf(stderr, "bind address error! ret = %d \n", ret);
                closesocket(sockfd);
                return INVALID_SOCKET;
            }
        }
        
        return sockfd;
    }
    
    SOCKET tcpsocket4client(){
        SOCKET sockfd = INVALID_SOCKET;
        sockfd = socket((isIpv6Network() ? AF_INET6 : AF_INET), SOCK_STREAM, 0);
        if (sockfd < 0) {
            fprintf(stderr, "socket() error!\n");
            return INVALID_SOCKET;
        }
        return sockfd;
    }
    
    int tcpconnect(SOCKET sockfd, const char* domainStr, int port){
        if (sockfd == INVALID_SOCKET) {
            return -1;
        }
        
        int ret = -1;
        if (isIpv6Network()) {
            struct sockaddr_in6 addr_in6;
            memset(&addr_in6, 0, sizeof(addr_in6));
            addr_in6.sin6_family = AF_INET6;
            addr_in6.sin6_port = htons((short)port);
            if (inet_pton(AF_INET6, domain2Ip(domainStr).c_str(), &addr_in6.sin6_addr) <= 0) {
                return -1;
            }
            ret = connect(sockfd, (struct sockaddr*)&addr_in6, sizeof(addr_in6));
        } else {
            struct sockaddr_in addr_in;
            memset(&addr_in, 0, sizeof(addr_in));
            addr_in.sin_family = AF_INET;
            addr_in.sin_port = htons((short)port);
            if (inet_pton(AF_INET, domain2Ip(domainStr).c_str(), &addr_in.sin_addr.s_addr) <= 0) {
                return -1;
            }
            ret = connect(sockfd, (struct sockaddr*)&addr_in, sizeof(addr_in));
        }
        
        //需要test一下
//        struct addrinfo *result, hints;
//        memset(&hints, 0, sizeof(struct addrinfo));
//        hints.ai_socktype = SOCK_STREAM;
//        char portStr[16];
//        sprintf(portStr, "%d", port);
//        if (isIpv6Network()) {
//            hints.ai_family = AF_INET6;
//        } else {
//            hints.ai_family = AF_INET;
//        }
//        ret = getaddrinfo(domainStr, portStr, &hints, &result);
//        if (ret != 0) {
//            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
//            return -1;
//        }
//        ret = connect(sockfd, result->ai_addr, result->ai_addrlen);
//        freeaddrinfo(result);
        
        
        if (ret < 0) {
            fprintf(stderr, "socket connect failed\n");
        }
        return ret;
    }
    
    //int flags
    int setNonblcok(SOCKET sockfd, bool isOn){
        if (sockfd == INVALID_SOCKET) {
            return -1;
        }
        int originFlag = fcntl(sockfd, F_GETFL);
        if (originFlag < 0) {
            return -1;
        }
        if (isOn) {
            originFlag |= O_NONBLOCK;//flags
        } else {
            originFlag &= ~O_NONBLOCK;//flags
        }
        if (fcntl(sockfd, F_SETFL, originFlag) < 0) {
            return -1;
        } else {
            return 0;
        }
    }
    
    int setKeepAlive(SOCKET sockfd, int optval){
        if (sockfd == INVALID_SOCKET) {
            return -1;
        }
        
        if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) != 0) {
            return -1;
        }
        return 0;
    }
    
    int setReuseaddr(SOCKET sockfd, int optval){
        if (sockfd == INVALID_SOCKET) {
            return -1;
        }
        return setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    }
    
    
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
    bool isIpv6Network(const std::string& domainStr/* = "www.baidu.com"*/){
        bool isIpv6Net = false;
        struct addrinfo* result = nullptr,*cur = nullptr;
        /*
         struct sockaddr_in6 addr_in6;
         //bzero(&addr_in6, sizeof(addr_in6));
         memset(&addr_in6, 0, sizeof(addr_in6));
         addr_in6.sin6_family = AF_INET6;
         */
        
        //IPv4中使用gethostbyname()函数完成主机名到地址解析，这个函数仅仅支持IPv4
        //IPv6中引入了getaddrinfo()的新API，它是协议无关的，既可用于IPv4也可用于IPv6######
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
    std::string domain2Ip(const char* pDomain){
        if (isIpv6Network()) {//ipv6
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
            
            
        } else {//ipv4
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
    
    
    int getLocalAddr(SOCKET sockfd, char* ip, int* port){
        if (sockfd == INVALID_SOCKET) {
            return -1;
        }
        struct sockaddr addr;
        unsigned int addrLen = sizeof(addr);//size_t:unsigned int
        
        /*
         用于获取一个套接字的名字。
         它用于一个已捆绑或已连接套接字s，本地地址将被返回。
         本调用特别适用于如下情况：未调用bind()就调用了connect()，这时唯有getsockname()调用可以获知系统内定的本地地址
         */
        int ret = getsockname(sockfd, &addr, &addrLen);
        if (ret < 0) {
            return ret;
        }
        
        unsigned char* data = (unsigned char*)(addr.sa_data);
        if (port) {
            *port = ((unsigned int)data[0] << 8) | (data[1]);
        }
        
        if (ip) {
            sprintf(ip,"%d.%d.%d.%d",data[2],data[3],data[4],data[5]);
        }
        return 0;
    }
    
    //server
    int getPeerAddr(SOCKET sockfd, char* ip, int* port){
        if (sockfd == INVALID_SOCKET) {
            return -1;
        }
        struct sockaddr addr;
        unsigned int addrLen = sizeof(addr);
        //getpeername只有在链接建立以后才调用，否则不能正确获得对方地址和端口，所以他的参数描述字一般是链接描述字而非监听套接口描述字。
        int ret = getpeername(sockfd, &addr, &addrLen);
        if (ret < 0) {
            return ret;
        }
        
        unsigned char* data = (unsigned char*)(addr.sa_data);
        if (port) {
            *port = ((unsigned int)data[0] << 8) | (data[1]);
        }
        
        if (ip) {
            sprintf(ip,"%d.%d.%d.%d",data[2],data[3],data[4],data[5]);
        }
        return 0;
        
    }
    
    int getError(SOCKET sockfd){
        int so_error = 0;
        unsigned int len = sizeof(so_error);
        //reinterpret_cast 它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针
        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, reinterpret_cast<char *>(&so_error), &len);
        return so_error;
    }
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