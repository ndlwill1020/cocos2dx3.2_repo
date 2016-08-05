//
//  DrawQuadScene.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/8/3.
//
//

#include "DrawQuadScene.hpp"
DrawQuadScene::DrawQuadScene(){
    
}

DrawQuadScene::~DrawQuadScene(){

}

Scene* DrawQuadScene::createScene(){
    auto scene = Scene::create();
    auto layer = DrawQuadScene::create();
    scene->addChild(layer);
    return scene;
}

bool DrawQuadScene::init(){
    if (!Layer::init()) {
        return false;
    }

    auto program = GLProgram::createWithFilenames("myVertexShader.vert", "myFragmentShader.frag");
    this->setGLProgram(program);
    
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    auto winSize = Director::getInstance()->getWinSize();
    
    //1.一般写法
    /*
    float vertices[] = {-1, -1, \
                        1, -1, \
                        -1, 1, \
                        -1, 1, \
                        1, 1, \
                        1, -1};
    */
    
    //2.vbo索引写法
    float vertices[] = {-1, -1, \
                        1, -1, \
                        -1, 1, \
                        1, 1};
    
    //如果我们只想画纯色的四边形，那么建议不要使用attribute，直接使用uniform并把该uniform的值传给gl_FragColor就行了
    float colors[] = {0, 1, 0, 1, \
                        0, 1, 0, 1, \
                        0, 1, 0, 1, \
                        0, 1, 0, 1, \
                        0, 1, 0, 1, \
                        0, 1, 0, 1};
    
    
    //vertices
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    //colors
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    
    //vbo索引写法
    GLubyte indices[] = {0, 1, 2,/*第一个三角形索引*/2, 3, 1/*第二个三角形索引*/};
    GLuint indexVBO;
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    return true;
}


void DrawQuadScene::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags){
    Layer::visit(renderer, parentTransform, parentFlags);
    _command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(DrawQuadScene::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}

void DrawQuadScene::onDraw(){
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    auto program = this->getGLProgram();
    program->use();
    program->setUniformsForBuiltins();
    
    glBindVertexArray(vao);
    
    //一般写法
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    
    //vbo索引写法  第4个参数：开始绘制的第一个索引数据在缓冲区的偏移
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid *)0);
    
    glBindVertexArray(0);
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
    CHECK_GL_ERROR_DEBUG();
    
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

