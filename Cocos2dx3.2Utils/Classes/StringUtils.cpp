//
//  StringUtils.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/7/5.
//
//

#include "StringUtils.hpp"

#include <string>

/*
 size_t find (const string& str, size_t pos = 0) const;

 size_t find (const char* s, size_t pos = 0) const;

 size_t find (const char* s, size_t pos, size_t n) const;

 size_t find (char c, size_t pos = 0) const;
 */
std::vector<std::string> StringTools::split(std::string str,const char symbol){
    std::vector<std::string> result;
    
    //    std::string s=std::string("ffff");
    //    log("%s",s.c_str());
    
    //    Value v=Value("ss");
    //    log("%s",v.asString().c_str());
    
    //size_t实际上是unsigned int   C语言
    size_t size = strlen(str.c_str());
    log("size:%zu",size);
    
    size_t startIndex = 0;
    size_t endIndex = 0;
    endIndex = str.find(symbol);//从0开始
    
    log("endIndex:%zu",endIndex);
    
    std::string splitStr="";
    
    //static const size_type npos = -1;
    while(endIndex != std::string::npos){
        splitStr = str.substr(startIndex, endIndex);//从指定位置开始，并指定长度  c++
        result.push_back(splitStr);
        str = str.substr(endIndex + 1, size - endIndex - 1);
        log("str:%s",str.c_str());
        endIndex = str.find(symbol);
        log("endIndex:%zu",endIndex);
    }
    
    //compare函数在>时返回1，<时返回-1，==时返回0
    if(str.compare("") != 0){  //c++
        result.push_back(str);
    }
    return result;
}

