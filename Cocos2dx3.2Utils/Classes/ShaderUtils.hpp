//
//  ShaderUtils.hpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/8/5.
//
//

#ifndef ShaderUtils_hpp
#define ShaderUtils_hpp
#include "cocos2d.h"
USING_NS_CC;

//描边算法
//1. 如果它是不透明的像素，则不管，维持原本颜色；2. 如果透明，是360度判断它四周有没有不透明的像素，如果有，则把它设成描边颜色，否则保持透明
namespace ShaderUtils {
    //描边宽度(像素),描边颜色，图片大小
    GLProgramState *getTextureStrokeProgramState(float strokeWidth, Color3B strokeColor, Size textureSize);
}

#endif /* ShaderUtils_hpp */
