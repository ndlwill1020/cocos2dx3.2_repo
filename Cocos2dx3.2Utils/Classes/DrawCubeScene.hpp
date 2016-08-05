//
//  DrawCubeScene.hpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/8/3.
//
//

#ifndef DrawCubeScene_hpp
#define DrawCubeScene_hpp

/*
//test for 改进顶点数据结构  画4边形
typedef struct {
    float Position[2];
    float Color[4];
} Vertex;
 */


//test for 画立方体
typedef struct {
    float Position[3];
    float Color[4];
} Vertex;



#include "cocos2d.h"
USING_NS_CC;

class DrawCubeScene : public Layer{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(DrawCubeScene);
    
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
    void onDraw();
    //virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
private:
    CustomCommand _command;
    Size winSize;
    
    /*
    //test for 改进顶点数据结构  画4边形
    GLuint vao;
    GLuint vertexVBO;//包括 顶点坐标和颜色
     */
    
    //test for 画立方体
    GLuint vao;
    GLuint vertexVBO;
    
protected:
    DrawCubeScene();
    ~DrawCubeScene();
};


#endif /* DrawCubeScene_hpp */
