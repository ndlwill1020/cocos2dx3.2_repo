//
//  DrawCubeScene.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/8/3.
//
//

#include "DrawCubeScene.hpp"

DrawCubeScene::DrawCubeScene(){

}

DrawCubeScene::~DrawCubeScene(){

}

Scene* DrawCubeScene::createScene(){
    auto scene = Scene::create();
    auto layer = DrawCubeScene::create();
    scene->addChild(layer);
    return scene;
}

bool DrawCubeScene::init(){
    if (!Layer::init()) {
        return false;
    }
    winSize = Director::getInstance()->getWinSize();
    
    //test for 改进顶点数据结构  画4边形
    /*3.改进顶点数据结构:
    现在顶点属性包含位置(position)和颜色(color)，将来还会有法线（normal），纹理坐标（texture coordinate）等数据。
    如果每一种类型的顶点数据都分开来存放，一来不利于管理，二来也会产生内存碎片。
    
    cocos2d-x里面所用的方法，比如一个Quard的定义如下：
    struct V3F_C4B_T2F
    {
        //! vertices (3F)
        Vec3     vertices;            // 12 bytes
        //! colors (4B)
        Color4B      colors;              // 4 bytes
        // tex coords (2F)
        Tex2F        texCoords;           // 8 bytes
    };
    
    ----------------------------------
     */
    /*
    auto program = GLProgram::createWithFilenames("myVertexShader.vert", "myFragmentShader.frag");
    this->setGLProgram(program);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    Vertex data[] =
    {
        { {-1,-1},{0,1,0,1} },
        { {1,-1},{0,1,0,1} },
        { {-1,1},{0,1,0,1} },
        { {1,1},{0,1,0,1} }
    };
    
    glGenBuffers(1, &vertexVBO);//包括 顶点坐标和颜色
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    
    GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Position));
    
    
    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Color));

    
    GLubyte indices[] = {0, 1, 2, 2, 3, 1};
    GLuint indexVBO;
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
     */

    
    /**************************************我是分割线**************************************/
    //test for 画立方体
    auto program = GLProgram::createWithFilenames("drawCubeVertexShader.vert", "drawCubeFragmentShader.frag");
    this->setGLProgram(program);
    
    //vao纪录的状态包含  //#
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    Vertex data[] =
    {
        // Front
        {{1, -1, 0}, {1, 0, 0, 1}},
        {{1, 1, 0}, {0, 1, 0, 1}},
        {{-1, 1, 0}, {0, 0, 1, 1}},
        {{-1, -1, 0}, {0, 0, 0, 1}},
        // Back
        {{1, 1, -2}, {1, 0, 0, 1}},
        {{-1, -1, -2}, {0, 1, 0, 1}},
        {{1, -1, -2}, {0, 0, 1, 1}},
        {{-1, 1, -2}, {0, 0, 0, 1}},
        // Left
        {{-1, -1, 0}, {1, 0, 0, 1}},
        {{-1, 1, 0}, {0, 1, 0, 1}},
        {{-1, 1, -2}, {0, 0, 1, 1}},
        {{-1, -1, -2}, {0, 0, 0, 1}},
        // Right
        {{1, -1, -2}, {1, 0, 0, 1}},
        {{1, 1, -2}, {0, 1, 0, 1}},
        {{1, 1, 0}, {0, 0, 1, 1}},
        {{1, -1, 0}, {0, 0, 0, 1}},
        // Top
        {{1, 1, 0}, {1, 0, 0, 1}},
        {{1, 1, -2}, {0, 1, 0, 1}},
        {{-1, 1, -2}, {0, 0, 1, 1}},
        {{-1, 1, 0}, {0, 0, 0, 1}},
        // Bottom
        {{1, -1, -2}, {1, 0, 0, 1}},
        {{1, -1, 0}, {0, 1, 0, 1}},
        {{-1, -1, 0}, {0, 0, 1, 1}},
        {{-1, -1, -2}, {0, 0, 0, 1}}
    };
    
    
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);//#
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    
    GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(positionLocation);//#
    //# 
    glVertexAttribPointer(positionLocation,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, Position));
    //glDisableVertexAttribArray(<#GLuint index#>)
    
    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
    glEnableVertexAttribArray(colorLocation);//#
    //#
    glVertexAttribPointer(colorLocation,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, Color));
    
    GLubyte indices[] = {
        // Front
        0, 1, 2,
        2, 3, 0,
        // Back
        4, 5, 6,
        4, 5, 7,
        // Left
        8, 9, 10,
        10, 11, 8,
        // Right
        12, 13, 14,
        14, 15, 12,
        // Top
        16, 17, 18,
        18, 19, 16,
        // Bottom
        20, 21, 22,
        22, 23, 20
    };
    
    
    //索引vbo
    GLuint indexVBO;
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);//#
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) , indices, GL_STATIC_DRAW);
    
    // 所有包含的状态被关闭  所以上面无需调用glDisableVertexAttribArray
    glBindVertexArray(0);
    //在上面那句话的后面调用，不然他们会被vao包含
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    
    return true;
}

void DrawCubeScene::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags){
    Layer::visit(renderer, parentTransform, parentFlags);
    _command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(DrawCubeScene::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}

void DrawCubeScene::onDraw(){
    /*
    //test for 改进顶点数据结构  画4边形
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    auto program = this->getGLProgram();
    program->use();
    program->setUniformsForBuiltins();
    
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid *)0);
    
    glBindVertexArray(0);
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
    CHECK_GL_ERROR_DEBUG();
    
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
     */
    
    /**************************************我是分割线**************************************/
    //test for 画立方体
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    //修改一个modelView矩阵，让它离我们的camera远一点
    
    /*
     可以通过modelView和projection矩阵来定义一个更好用的坐标系，然后基于这个坐标系去指定物体的坐标。 
     比如cocos2d-x里面，通过下列代码指定了自己的坐标系范围在(0~size.width)和(0~size.height)之间。
     case Projection::_3D:
     {
         float zeye = this->getZEye();
         Mat4 matrixPerspective, matrixLookup;
         loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
         Mat4::createPerspective(60, (GLfloat)size.width/size.height, 10, zeye+size.height/2, &matrixPerspective);
         multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, matrixPerspective);
     
         Vec3 eye(size.width/2, size.height/2, zeye), center(size.width/2, size.height/2, 0.0f), up(0.0f, 1.0f, 0.0f);
         Mat4::createLookAt(eye, center, up, &matrixLookup);
         multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, matrixLookup);
         loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
     break;
     }
     */

    Mat4 modelViewMatrix;
    //camera   1.位置位于   2.看着某点   3.头朝上（0,1,0），头朝下（0,-1,0）
    Mat4::createLookAt(Vec3(0, 0, 1), Vec3(0, 0, 0), Vec3(0,1,0), &modelViewMatrix);
    modelViewMatrix.translate(0, 0, -5);
    
    static float rotation = 0;
    modelViewMatrix.rotate(Vec3(1,1,1),CC_DEGREES_TO_RADIANS(rotation));
    rotation++;
    if (rotation > 360) {
        rotation = 0;
    }

    Mat4 projectionMatrix;
    Mat4::createPerspective(60, 480 / 320, 1.0, 42, &projectionMatrix);
    //Mat4::createPerspective(60, winSize.width / winSize.height, 1.0, 42, &projectionMatrix);
    Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, projectionMatrix);
    
    Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, modelViewMatrix);


    

    
    
    auto program = this->getGLProgram();
    program->use();
    program->setUniformsForBuiltins();
    
    glBindVertexArray(vao);
    
    GLuint uColorLocation = glGetUniformLocation(program->getProgram(), "u_color");
    float uColor[] = {1.0, 1.0, 1.0, 1.0};
    glUniform4fv(uColorLocation, 1, uColor);
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (GLvoid *)0);
    glBindVertexArray(0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 36);
    CHECK_GL_ERROR_DEBUG();
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}


