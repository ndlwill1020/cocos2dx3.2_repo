//
//  CommonTools.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/8/18.
//
//

#include "CommonTools.hpp"

/*
 timeval tv;
 gettimeofday(&tv, NULL);
 unsigned long rand_seed = tv.tv_sec * 1000 + tv.tv_usec / 1000;
 srand(rand_seed);
 */
//得到start-end的随机数 包括start end
int CommonTools::getRandom(int end, int start){
    int random = CCRANDOM_0_1() * ( end - start + 1 ) + start;
    return random;
}