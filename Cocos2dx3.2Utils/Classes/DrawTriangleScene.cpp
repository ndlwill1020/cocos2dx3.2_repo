//
//  DrawTriangleScene.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/8/3.
//
//



/*
 uniform:
 
 如果.frag里面有个uniform vec4 u_color;
 GLuint uColorLocation = glGetUniformLocation(program->getProgram(), "u_color");
 float uColor[] = {1.0, 0.0, 0.0, 1.0};
 //除非你显式地调用glUniformXXX函数来修改这个uniform的值，否则它的值恒定不变
 glUniform4fv(uColorLocation,1, uColor);
 */

/*
 CC_MVPMatrix:
 
 查看源码GLProgram::compileShader()
 CC_MVPMatrix是一个mat4类型的uniform,在shader代码被编译之前，它由cocos2d-x框架插入进来的
 这里除了插入CC_MVPMatrix以外，还插入了其它一些uniform。只要你在后面的main函数里面不使用这些变量，最终shader program里面是看不到它们的。（被优化掉了）
 
 CC_MVPMatrix本质上是一个变换矩阵，用来把一个世界坐标系中的点转换到Clipping space
 
 3D物体从建模到最终显示到屏幕上面要经历以下几个阶段：
 对象空间(Object Space)
 世界空间(World Space)
 照相机空间(Camera Space/Eye Space)
 裁剪空间(Clipping Space)  
 设备空间（normalized device space）
 视口空间(Viewport)
 从对象空间到世界空间的变换通常叫做Model-To-World变换，从世界空间到照相机空间的变换叫做World-To-View变换，而从照相机空间到裁剪空间的变换叫做View-To-Projection。合起来，就是我们常常提到的MVP变换
 
 实际开发过程中，我们往往会把MV变换放到一起，一般做法如下：
 gl_Position = P * MV * ObjectPosition;

修改CC_MVPMatrix:
1.先通过glGetUniformLocation来获取这个uniform的入口，然后调用glUniformMatrix4fv来给它传值就行了
2.在cocos2d-x里面，我们可以通过修改矩阵栈里面的ModelView和Projection栈顶元素，从而修改ModelView和Projection矩阵，最终达到修改CC_MVPMatrix的目的。
 在onDraw函数的最开头加入下列代码：
 //pushMatrix会把原来栈顶上的元素拷贝一份并压入栈，这样我们后续对于此矩阵的操作可以通过调用popMatrix来撤销影响
 Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
 //把ModelView和Projection矩阵都重置成了单位矩阵
 Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
 Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
 Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
在onDraw函数返回前加入下列代码：
 Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
 Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

 //更新CC_MVPMatrix
 program->setUniformsForBuiltins();
 */

#include "DrawTriangleScene.hpp"

DrawTriangleScene::DrawTriangleScene(){

}

DrawTriangleScene::~DrawTriangleScene(){

}

Scene* DrawTriangleScene::createScene(){
    auto scene = Scene::create();
    auto layer = DrawTriangleScene::create();
    scene->addChild(layer);
    return scene;
}

bool DrawTriangleScene::init(){
    if (!Layer::init()) {
        return false;
    }
    //test 1 ,2
    //1./*使用cocos的shader*/  从Cocos2D-X的shader缓存中取出一个带有position和color顶点属性的shader，然后传给这个Layer
    //this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
    //2.使用自己的shader
    auto program = GLProgram::createWithFilenames("myVertexShader.vert", "myFragmentShader.frag");
    this->setGLProgram(program);
    
    //为了不让这些生成和绑定VBO和VAO的操作在每一帧都被执行，我们需要把它放在初始化函数里面
    //test 2
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    //
    auto winSize = Director::getInstance()->getWinSize();
    //不修改CC_MVPMatrix，传入的是屏幕坐标
//    float vertices[] = {0, 0, \
//        winSize.width, 0, \
//        winSize.width / 2, winSize.height};
    
    
    //修改CC_MVPMatrix为单位矩阵后，传入的坐标范围（－1，1），中心点（0，0）
    float vertices[] = {-1, -1, \
                        1, -1, \
                        0, 1};
    
    //grb
    float colors[] = {0, 1, 0, 1, \
                        1, 0, 0, 1, \
                            0, 0, 1, 1};
    
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
    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color"); //获得attribute的入口
    glEnableVertexAttribArray(colorLocation);//传递数据之前，首先要告诉OpenGL
    //glVertexAttribPointer的最后一个参数，这里传递的都是(GLvoid*)0。而不像之前一样传的是vertex和color的数组地址。这一点是使用VBO和不使用VBO的区别
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);//传递数据
    
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    return true;
}

void DrawTriangleScene::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags){
    Layer::visit(renderer, parentTransform, parentFlags);
    _command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(DrawTriangleScene::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}

void DrawTriangleScene::onDraw(){
    //test 1  没使用vbo
    /*使用cocos的shader*/
    /*
    auto program = this->getGLProgram();
    program->use();//GL::useProgram(_program);
    
    //设置该shader的一些内置uniform,主要是MVP，即model-view-project矩阵
    program->setUniformsForBuiltins();
    
    auto winSize = Director::getInstance()->getWinSize();
    //三角形的三个顶点
    float vertices[] = {0, 0, \
                        winSize.width, 0, \
                        winSize.width / 2, winSize.height};
    
    float colors[] = {0, 1, 0, 1, \
                        1, 0, 0, 1, \
                            0, 0, 1, 1};
    
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors);
    //绘制三角形，所谓的draw call就是指这个函数调用
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //通知cocos2d-x 的renderer，让它在合适的时候调用这些OpenGL命令
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
    //如果出错了，可以使用这个函数来获取出错信息
    CHECK_GL_ERROR_DEBUG();
    */
    
    
    //test 2  使用vbo&vao
    
    //修改CC_MVPMatrix:
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);//重置成了单位矩阵
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    
    /*VBO & VAO*/
    //VBO，全名Vertex Buffer Object。它是GPU里面的一块缓冲区，当我们需要传递数据的时候，可以先向GPU申请一块内存，然后往里面填充数据。最后，再通过调用glVertexAttribPointer把数据传递给Vertex Shader
    //VAO，全名为Vertex Array Object，它的作用主要是记录当前有哪些VBO，每个VBO里面绑定的是什么数据，还有每一个vertex attribute绑定的是哪一个VBO
    auto program = this->getGLProgram();
    program->use();//GL::useProgram(_program);
    //设置该shader的一些内置uniform,主要是MVP，即model-view-project矩阵
    program->setUniformsForBuiltins();
    
    //use vao  因为vao记录了每一个顶点属性和缓冲区的状态，所以只需要绑定就可以使用了
    glBindVertexArray(vao);
    //绘制三角形，所谓的draw call就是指这个函数调用
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    //通知cocos2d-x 的renderer，让它在合适的时候调用这些OpenGL命令
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
    //如果出错了，可以使用这个函数来获取出错信息
    CHECK_GL_ERROR_DEBUG();
    
    //撤销
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
