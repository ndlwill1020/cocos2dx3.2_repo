//
//  DrawTextureScene.hpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/8/4.
//
//

#ifndef DrawTextureScene_hpp
#define DrawTextureScene_hpp

#include "cocos2d.h"
USING_NS_CC;

class DrawTextureScene : public Layer{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(DrawTextureScene);
    
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
    void onDraw();
    
private:
    CustomCommand _command;
   
    GLuint vao;
    GLuint vertexVBO;
    GLuint textureID;
    
protected:
    DrawTextureScene();
    ~DrawTextureScene();
};


#endif /* DrawTextureScene_hpp */
