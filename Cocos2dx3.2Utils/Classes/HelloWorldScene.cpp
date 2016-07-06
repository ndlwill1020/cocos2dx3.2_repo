#include "HelloWorldScene.h"
#include "StringUtils.hpp"



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
    
//    auto helloWorld = Sprite::create("HelloWorld.png");
//    helloWorld->setPosition(visibleSize.width / 2, visibleSize.height / 2);
//    this->addChild(helloWorld);

    
    //会卡。。。
    blurSprite = BlurSprite::create("HelloWorld.png");
    blurSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(blurSprite);

    
    
    
    
    auto list=StringTools::split("Zhangxl,aihaoguangfan,basketball,swim",",");
    for(int i=0;i<list.size();i++){
        log("%s",list.at(i).c_str());
    }
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    blurSprite->setBlurRadius(12.0f);
}

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


