#include "HelloWorldScene.h"
#include "StringUtils.hpp"
#include "TCPSocket.hpp"

#include "ShaderUtils.hpp"


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //当文件（如数据库文件，游戏配置文件）的md5不符时，退出游戏
    //进行MD5数值对比，也就是MD5校验。由于MD5加密不可逆算，如果数值一样，那就表示文件没有被修改的。反之，则被修改了

    
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto bg = LayerColor::create(Color4B::GREEN);
    bg->setPosition(Vec2::ZERO);
    this->addChild(bg, -1);
    
    //test for shadow
//    auto role = Sprite::create("role.png");
//    role->setScale(2.0f);
//    role->setPosition(visibleSize.width / 2, visibleSize.height / 2);
//    auto program = GLProgram::createWithFilenames("shadow.vert", "shadow.frag");
//    role->setGLProgram(program);
//    this->addChild(role, 1);
    
    //test for stroke  效率不高～
//    auto logo = Sprite::create("role.png");
//    logo->setPosition(visibleSize.width / 2, visibleSize.height / 2);
//    logo->setGLProgramState(ShaderUtils::getTextureStrokeProgramState(5.0f, Color3B::RED, logo->getContentSize()));
//    this->addChild(logo, 1);
    
    //test for hsv
//    auto cocos = Sprite::create("HelloWorld.png");
//    cocos->setPosition(visibleSize.width / 2, visibleSize.height / 2);
//    cocos->setGLProgramState(ShaderUtils::getHSVProgramState(120.0f, 0.0f, 0.0f));
//    this->addChild(cocos, 1);

    auto cocos = Sprite::create("HelloWorld.png");
    cocos->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    //test for gray
    //std::string fragmentShaderSource = FileUtils::getInstance()->getStringFromFile("gray.frag");
    
    //test for ic_flower_blur
    //std::string fragmentShaderSource = FileUtils::getInstance()->getStringFromFile("ice_flower_blur.frag");
    
    //test for GaussianBlur
    //std::string fragmentShaderSource = FileUtils::getInstance()->getStringFromFile("GaussianBlur.frag");
    
    //test for Emboss
    std::string fragmentShaderSource = FileUtils::getInstance()->getStringFromFile("Emboss.frag");
    cocos->setGLProgram(GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragmentShaderSource.c_str()));
    this->addChild(cocos, 1);
    
    
    //test for NetWork
    //TCPSocket::isIpv6Network();
    
    
    //test for blurSprite
    //会卡。。。
//    blurSprite = BlurSprite::create("HelloWorld.png");
//    blurSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
//    this->addChild(blurSprite);


    //test for StringTools
//    auto list=StringTools::split("Zhangxl,aihaoguangfan,basketball,swim",',');
//    for(int i=0;i<list.size();i++){
//        log("%s",list.at(i).c_str());
//    }
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    if (blurSprite) {
        blurSprite->setBlurRadius(12.0f);
    }
}



//============================================================

//CallFuncN * create(const std::function<void(Node*)>& func);
RenderTexture* HelloWorld::ScreenShot(const bool bIsSave, std::function<void(cocos2d::RenderTexture*, const std::string&)> pFuncCallback)
{
    auto winSize = Director::getInstance()->getWinSize();
    RenderTexture *renderTexture = RenderTexture::create(winSize.width, winSize.height);
    
    renderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    Director::getInstance()->getRunningScene()->visit();
    renderTexture->end();
    //Director::getInstance()->getRenderer()->render();
    if (bIsSave) {
        static int index = 0;
        renderTexture->saveToFile(StringUtils::format("ScreenShot_%.2d.png",index++), Image::Format::PNG, true);
    } else {
        if (pFuncCallback != nullptr) {
            pFuncCallback(renderTexture, "");
        }
    }
    return renderTexture;
}

void HelloWorld::ScreenShot_Callback(RenderTexture* renderTesture, const std::string& str){

}


