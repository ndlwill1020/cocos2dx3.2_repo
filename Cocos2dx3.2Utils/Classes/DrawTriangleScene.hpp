//
//  DrawTriangleScene.hpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/8/3.
//
//

#ifndef DrawTriangleScene_hpp
#define DrawTriangleScene_hpp

#include "cocos2d.h"
USING_NS_CC;

class DrawTriangleScene : public Layer{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(DrawTriangleScene);
    
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
    void onDraw();
    //virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
private:
    CustomCommand _command;
    
    GLuint vao;
    GLuint vertexVBO;
    GLuint colorVBO;
    
protected:
    DrawTriangleScene();
    ~DrawTriangleScene();
};

#endif /* DrawTriangleScene_hpp */
