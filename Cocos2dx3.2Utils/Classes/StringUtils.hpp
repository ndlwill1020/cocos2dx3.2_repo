//
//  StringUtils.hpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/7/5.
//
//

#ifndef StringUtils_hpp
#define StringUtils_hpp

#include "cocos2d.h"
USING_NS_CC;

class StringTools{
public:
    /**
     *  字符串拆分
     *
     *  @param str    要被拆分的字符串
     *  @param symbol 按照这个字符拆分
     *
     *  @return 拆分后的字符串数组
     */
    static std::vector<std::string> split(std::string str,const char symbol);
    
    
};

#endif /* StringUtils_hpp */
