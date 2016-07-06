//
//  BlurSprite.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/7/5.
//
//

#include "BlurSprite.hpp"

BlurSprite::BlurSprite()
:fBlurRadius_(0.0f)
,fBlurSampleNum_(0.0f)
{

}

BlurSprite::~BlurSprite(){
    
}

BlurSprite* BlurSprite::create(const std::string& fileName, const float fRadius/* = 8.0f*/, const float fSampleNum/* = 8.0f*/){
    BlurSprite* pRet = new (std::nothrow) BlurSprite();
    if (pRet && pRet->init(fileName, fRadius, fSampleNum)) {
        pRet->autorelease();
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool BlurSprite::init(const std::string& fileName, const float fRadius, const float fSampleNum){
    if (!Sprite::init()) {
        return false;
    }
    //必须先赋值  initWithFile会调用他们
    fBlurRadius_ = fRadius;
    fBlurSampleNum_ = fSampleNum;
    this->initWithFile(fileName);
    
    
    return true;
}

bool BlurSprite::initWithTexture(Texture2D *texture, const Rect& rect)
{
    log("override initWithTexture");
    if (Sprite::initWithTexture(texture, rect))
    {
#if CC_ENABLE_CACHE_TEXTURE_DATA
        log("############################");
        auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
            setGLProgram(nullptr);
            initGLProgram();
        });
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif
        initGLProgram();
        return true;
    }
    return false;
}

void BlurSprite::initGLProgram(){
    /*
    const char* ccPositionTextureColor_noMVP_vert = STRINGIFY(
                                                              attribute vec4 a_position;
                                                              attribute vec2 a_texCoord;
                                                              attribute vec4 a_color;
                                                              
                                                              \n#ifdef GL_ES\n
                                                              varying lowp vec4 v_fragmentColor;
                                                              varying mediump vec2 v_texCoord;
                                                              \n#else\n
                                                              varying vec4 v_fragmentColor;
                                                              varying vec2 v_texCoord;
                                                              \n#endif\n
                                                              
                                                              void main()
    {
        gl_Position = CC_PMatrix * a_position;
        v_fragmentColor = a_color;
        v_texCoord = a_texCoord;
    }
                                                              );
     */
    auto fileUtils = FileUtils::getInstance();
    auto fragmentFullPath = fileUtils->fullPathForFilename("blur.fsh");
    const GLchar* fragmentSource = (const GLchar*)(fileUtils->getStringFromFile(fragmentFullPath).c_str());
    //link   updateUnifrom
    auto program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragmentSource);
    CHECK_GL_ERROR_DEBUG();
    //GLProgramStateCache::getInstance()->getGLProgramState(glprogram)   有直接调用，没有insert program
    auto programState = GLProgramState::getOrCreateWithGLProgram(program);
    this->setGLProgramState(programState);
    
    auto textureSize = this->getTexture()->getContentSizeInPixels();
    /*
     uniform vec2 resolution;
     uniform float blurRadius;
     uniform float sampleNum;
     */
    this->getGLProgramState()->setUniformVec2("resolution", textureSize);
    this->getGLProgramState()->setUniformFloat("blurRadius", fBlurRadius_);
    this->getGLProgramState()->setUniformFloat("sampleNum", fBlurSampleNum_);
    
}

void BlurSprite::setBlurRadius(float fRadius){
    fBlurRadius_ = fRadius;
    getGLProgramState()->setUniformFloat("blurRadius", fBlurRadius_);
}

void BlurSprite::setBlurSampleNum(float fSampleNum){
    fBlurSampleNum_ = fSampleNum;
    getGLProgramState()->setUniformFloat("sampleNum", fBlurSampleNum_);
}
