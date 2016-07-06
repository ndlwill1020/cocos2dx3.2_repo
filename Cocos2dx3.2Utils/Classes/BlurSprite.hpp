//
//  BlurSprite.hpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/7/5.
//
//

#ifndef BlurSprite_hpp
#define BlurSprite_hpp
#include "cocos2d.h"
USING_NS_CC;

class BlurSprite : public Sprite
{
protected:
    BlurSprite();
    ~BlurSprite();
public:
    //fRadius为模糊半径，此参数越大，模糊效果越明显；fSampleNum为模糊采样，此参数越大，模糊效果越细腻
    static BlurSprite* create(const std::string& fileName, const float fRadius = 8.0f, const float fSampleNum = 8.0f);
    virtual bool init(const std::string& fileName, const float fRadius = 8.0f, const float fSampleNum = 8.0f);
    
    virtual bool initWithTexture(Texture2D *texture, const Rect& rect);
    void initGLProgram();
    
    void setBlurRadius(float fRadius);
    void setBlurSampleNum(float fSampleNum);
    
private:
    float fBlurRadius_;
    float fBlurSampleNum_;
};

#endif /* BlurSprite_hpp */
