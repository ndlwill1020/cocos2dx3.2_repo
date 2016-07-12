//
//  TCPSocket.hpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/7/11.
//
//

#ifndef TCPSocket_hpp
#define TCPSocket_hpp

#include "cocos2d.h"
USING_NS_CC;
//IPv4是“.”来分割的，但是IPv6是“:”来分割的

class TCPSocket{
public:
    //判断是否IPv6网络
    static bool isIpv6Network(const std::string& domainStr = "www.baidu.com");
    
    //域名解析函数
    static std::string domain2Ip(const char* pDomain);
};

#endif /* TCPSocket_hpp */
