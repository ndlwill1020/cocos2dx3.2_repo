//
//  TestShaderScene.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/7/5.
//
//

#include "TestShaderScene.hpp"

TestShaderScene::TestShaderScene(){

}

TestShaderScene::~TestShaderScene(){

}

Scene* TestShaderScene::createScene(){
    auto scene = Scene::create();
    auto layer = TestShaderScene::create();
    scene->addChild(layer);
    return scene;
}

bool TestShaderScene::init(){
    if (!Layer::init()) {
        return false;
    }
    
    
    return true;
}

void TestShaderScene::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags){
    log("draw!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(TestShaderScene::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void TestShaderScene::onDraw(const Mat4 &transform, uint32_t flags){
    log("onDraw!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    CHECK_GL_ERROR_DEBUG();
}

