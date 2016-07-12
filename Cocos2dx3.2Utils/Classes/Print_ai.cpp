//
//  Print_ai.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/7/12.
//
//

#include "Print_ai.hpp"
#include <stdio.h>
#include <arpa/inet.h>

void print_ai(struct addrinfo *ailist)
{
    struct addrinfo     *aip = NULL;
    struct sockaddr_in  *sin_p = NULL;  // socket inet address pointer
    char                addr_in_p[INET_ADDRSTRLEN];
    const char          *p = NULL;
    
    for(aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        printf("flags: ");
        print_flags(aip);
        printf("\n");
        
        printf("family: ");
        print_family(aip);
        printf("\n");
        
        printf("socket type: ");
        print_socktype(aip);
        printf("\n");
        
        printf("protocol: ");
        print_protocol(aip);
        printf("\n");
        
        printf("host: %s\n", aip->ai_canonname ? aip->ai_canonname : "");
        
        if(aip -> ai_family == AF_INET)
        {
            sin_p = (struct sockaddr_in *)aip->ai_addr;
            p = inet_ntop(AF_INET, &sin_p->sin_addr, addr_in_p, INET_ADDRSTRLEN);
            printf("address: %s\n", p ? addr_in_p : "unknown");
            printf("port: %d\n", ntohs(sin_p->sin_port));    // Note:
        }
        
        printf("\n");
    }
}

void print_flags(const struct addrinfo *aip)
{
    if(aip->ai_flags == 0)
    {
        printf("0");
    }
    else
    {
        if(aip->ai_flags & AI_PASSIVE)
        {
            printf("AI_PASSIVE ");
        }
        if(aip->ai_flags & AI_CANONNAME)
        {
            printf("AI_CANONNAME ");
        }
        if(aip->ai_flags & AI_NUMERICHOST)
        {
            printf("AI_NUMERICHOST ");
        }
#if defined(AI_V4MAPPED)
        if(aip->ai_flags & AI_V4MAPPED)
        {
            printf("AI_V4MAPPED ");
        }
#endif
#if defined(AI_ALL)
        if(aip->ai_flags & AI_ALL)
        {
            printf("AI_ALL ");
        }
#endif
#if defined(AI_ADDRCONFIG)
        if(aip->ai_flags & AI_ADDRCONFIG)
        {
            printf("AI_ADDRCONFIG ");
        }
#endif
#if defined(AI_NUMERICSERV)
        if(aip->ai_flags & AI_NUMERICSERV)
        {
            printf("AI_NUMERICSERV ");
        }
#endif
    }
}

void print_family(const struct addrinfo *aip)
{
    switch(aip->ai_family)
    {
        case AF_INET:
            printf("inet");
            break;
        case AF_INET6:
            printf("inet6");
            break;
        case AF_UNIX:
            printf("unix");
            break;
        case AF_UNSPEC:
            printf("unspecified");
            break;
        default:
            printf("unknown");
    }
}

void print_socktype(const struct addrinfo *aip)
{
    switch(aip->ai_socktype)
    {
        case SOCK_STREAM:
            printf("stream");
            break;
        case SOCK_DGRAM:
            printf("datagram");
            break;
        case SOCK_SEQPACKET:
            printf("seqpacket");
            break;
        case SOCK_RAW:
            printf("raw");
            break;
        default:
            printf("unknown (%d)", aip->ai_socktype);
    }
}

void print_protocol(const struct addrinfo *aip)
{
    switch(aip->ai_protocol)
    {
        case 0:
            printf("default");
            break;
        case IPPROTO_TCP:
            printf("IPPROTO_TCP");
            break;
        case IPPROTO_UDP:
            printf("IPPROTO_UDP");
            break;
        case IPPROTO_RAW:
            printf("IPPROTO_RAW");  
            break;  
        default:  
            printf("unknown (%d)", aip->ai_protocol);  
    }  
}
