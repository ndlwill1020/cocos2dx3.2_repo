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
    static std::vector<std::string> split(std::string str,const char* symbol);
};

#endif /* StringUtils_hpp */
