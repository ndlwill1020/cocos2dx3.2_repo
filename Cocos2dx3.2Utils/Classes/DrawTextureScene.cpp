//
//  DrawTextureScene.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/8/4.
//
//

#include "DrawTextureScene.hpp"

//添加纹理坐标属性并传到vertex shader里面去，然后把内存里面的纹理传给GPU，在fragment shader里面通过采样器，就可以根据vertex shader传递过来的纹理坐标把纹理上面的颜色值用插值的方式映射到每一个像素上去

/*
 //带纹理的立方体顶点数据
 #define TEX_COORD_MAX   1
 Vertex Vertices[] = {
 // Front
     { {1, -1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
     { {1, 1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
     { {-1, 1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
     { {-1, -1, 0}, {0, 0, 0, 1}, {0, 0}},
     // Back
     { {1, 1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
     { {-1, -1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
     { {1, -1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
     { {-1, 1, -2}, {0, 0, 0, 1}, {0, 0}},
     // Left
     { {-1, -1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
     { {-1, 1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
     { {-1, 1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
     { {-1, -1, -2}, {0, 0, 0, 1}, {0, 0}},
     // Right
     { {1, -1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
     { {1, 1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
     { {1, 1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
     { {1, -1, 0}, {0, 0, 0, 1}, {0, 0}},
     // Top
     { {1, 1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
     { {1, 1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
     { {-1, 1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
     { {-1, 1, 0}, {0, 0, 0, 1}, {0, 0}},
     // Bottom
     { {1, -1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
     { {1, -1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
     { {-1, -1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
     { {-1, -1, -2}, {0, 0, 0, 1}, {0, 0}}
 };
 */

DrawTextureScene::DrawTextureScene(){

}

DrawTextureScene::~DrawTextureScene(){

}

Scene* DrawTextureScene::createScene(){
    auto scene = Scene::create();
    auto layer = DrawTextureScene::create();
    scene->addChild(layer);
    return scene;
}

bool DrawTextureScene::init(){
    if (!Layer::init()) {
        return false;
    }
    
    auto program = GLProgram::createWithFilenames("drawTextureVertexShader.vert", "drawTextureFragmentShader.frag");
    this->setGLProgram(program);
    
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    typedef struct {
        float Position[2];
        float Color[4];
        float TexCoord[2];
    } Vertices;
    
    //纹理坐标的(0,0)点在图片的左上角,这个与OpenGL坐标系里左下角是(0,0)有所区别
    Vertices data[] =
    {
        { {-1,-1},{0,1,0,1},{0,1}},
        { {1,-1},{0,1,0,1},{1,1}},
        { {-1,1},{0,1,0,1},{0,0}},
        { {1,1},{0,1,0,1},{1,0}}
    };
    
    //纹理大小可根据Image得到，来设置渲染的顶点坐标
//    Vertices data[] =
//    {
//        { {-0.5,-0.5},{0,1,0,1},{0,1}},
//        { {0.5,-0.5},{0,1,0,1},{1,1}},
//        { {-0.5,0.5},{0,1,0,1},{0,0}},
//        { {0.5,0.5},{0,1,0,1},{1,0}}
//    };
    
    GLubyte indices[] = {0, 1, 2, 2, 3, 1};
    
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    
    GLuint positionLocation=glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertices),
                          (GLvoid *)offsetof(Vertices, Position));
    
    GLuint colorLocation=glGetAttribLocation(program->getProgram(), "a_color");
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertices),
                          (GLvoid *)offsetof(Vertices, Color));
    
    GLuint textureLocation=glGetAttribLocation(program->getProgram(), "a_uvCoord");
    glEnableVertexAttribArray(textureLocation);
    glVertexAttribPointer(textureLocation,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertices),
                          (GLvoid *)offsetof(Vertices, TexCoord));
    
    GLuint indexVBO;
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    
    //method1
    Image* image=new Image();
    std::string imagePath = FileUtils::getInstance()->fullPathForFilename("HelloWorld.png");
    image->initWithImageFile(imagePath);
    
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    //设置纹理在放大或者缩小的时候的插值方法(filter)以及在指定的纹理坐标超出(0-1)的范围的时候应该采用的策略
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    unsigned char* imageData = image->getData();
    int width = image->getWidth();
    int height = image->getHeight();
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 width,
                 height,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,//must be GL_UNSIGNED_BYTE
                 imageData);
    CC_SAFE_DELETE(image);
    
    /*
     //method2: the easier way
     Texture2D *texture = new Texture2D;
     texture->initWithImage(image);
     textureID = texture->getName();
     //method3: the easiest way
     Sprite *sprite = Sprite::create("HelloWorld.png");
     textureID = sprite->getTexture()->getName();
     */
    
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    return true;
}


void DrawTextureScene::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags){
    Layer::visit(renderer, parentTransform, parentFlags);
    _command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(DrawTextureScene::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}

void DrawTextureScene::onDraw(){
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    auto glProgram=getGLProgram();
    glProgram->use();
    //更新CC_MVPMatrix
    glProgram->setUniformsForBuiltins();
    
    
    GLuint textureLocation = glGetUniformLocation(glProgram->getProgram(), "CC_Texture0");
    // Set our sampler to user Texture Unit 0
    glUniform1i(textureLocation, 0);

//    GLuint uColorLocation = glGetUniformLocation(glProgram->getProgram(), "u_color");
//    GLfloat uColor[]={1.0,1.0,1.0,1.0};
//    glUniform4fv(uColorLocation, 1, uColor);
    
    glBindVertexArray(vao);
    
    //在调用draw call之前，我们需要绑定纹理
    
    GL::bindTexture2D(textureID);//activeTexture(GL_TEXTURE0); //glBindTexture(GL_TEXTURE_2D, textureID);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
    CHECK_GL_ERROR_DEBUG();
    
    
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    
}
