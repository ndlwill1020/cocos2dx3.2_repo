//
//  TestShaderScene.hpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/7/5.
//
//

#ifndef TestShaderScene_hpp
#define TestShaderScene_hpp

#include "cocos2d.h"
USING_NS_CC;

class TestShaderScene : public Layer{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(TestShaderScene);
    
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    void onDraw(const Mat4 &transform, uint32_t flags);
protected:
    TestShaderScene();
    ~TestShaderScene();
private:
    CustomCommand _customCommand;
};
#endif /* TestShaderScene_hpp */
