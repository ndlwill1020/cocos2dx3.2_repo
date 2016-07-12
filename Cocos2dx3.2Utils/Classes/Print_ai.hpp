//
//  Print_ai.hpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/7/12.
//
//

#ifndef Print_ai_hpp
#define Print_ai_hpp

//打印 addrinfo 的函数——print_ai

#include <sys/socket.h>
#include <netdb.h>

extern void print_ai(struct addrinfo *ailist);
extern void print_flags(const struct addrinfo *aip);
extern void print_family(const struct addrinfo *aip);
extern void print_socktype(const struct addrinfo *aip);
extern void print_protocol(const struct addrinfo *aip);

#endif /* Print_ai_hpp */
