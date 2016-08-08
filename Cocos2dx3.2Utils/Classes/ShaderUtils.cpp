//
//  ShaderUtils.cpp
//  Cocos2dx3.2Utils
//
//  Created by macuser on 16/8/5.
//
//

#include "ShaderUtils.hpp"

const char* strokeShaderName = "stroke_shader";

namespace ShaderUtils {
    GLProgramState *getTextureStrokeProgramState(float strokeWidth, Color3B strokeColor, Size textureSize){
        auto program = GLProgramCache::getInstance()->getGLProgram(strokeShaderName);
        if (!program) {
            std::string fragmentShaderSource = FileUtils::getInstance()->getStringFromFile("strokeFragmentShader.frag");
            program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragmentShaderSource.c_str());
            
            
            const float cosArray[12] = {1, 0.866, 0.5, 0, -0.5, -0.866, -0.1, -0.866, -0.5, 0, 0.5, 0.866};
            const float sinArray[12] = {0, 0.5, 0.866, 1, 0.866, 0.5, 0, -0.5, -0.866, -1, -0.866, -0.5};
            GLint cosArrayLocation = program->getUniformLocation("cosArray");
            program->setUniformLocationWith4fv(cosArrayLocation, cosArray, 3);
            GLint sinArrayLocation = program->getUniformLocation("sinArray");
            program->setUniformLocationWith4fv(sinArrayLocation, sinArray, 3);
            
            
            /*
             GLint glUniform{1,2,3,4}fv(GLint location, GLsizei count, GLfloat *v);
             参数：
             ·location – the previously queried location
             ·count – the number of elements in the array
             ·v – an array of floats
             */
            
            /*
            GLint cosArrayLocation = glGetUniformLocation(program->getProgram(), "cosArray");
            glUniform4fv(cosArrayLocation, 3, cosArray);//4f-4float v-指针  4*3=12
            GLint sinArrayLocation = glGetUniformLocation(program->getProgram(), "sinArray");
            glUniform4fv(sinArrayLocation, 3, sinArray);//4f-4float v-指针  4*3=12
             */
            
            GLProgramCache::getInstance()->addGLProgram(program, strokeShaderName);
        }
        
        
        auto programState = GLProgramState::create(program);
        
        //programState->setUniformFloat(glGetUniformLocation(program->getProgram(), "u_strokeWidth"), strokeWidth);
       
        //const float cosArray[12] = {1, 0.866, 0.5, 0, -0.5, -0.866, -0.1, -0.866, -0.5, 0, 0.5, 0.866};
        //const float sinArray[12] = {0, 0.5, 0.866, 1, 0.866, 0.5, 0, -0.5, -0.866, -1, -0.866, -0.5};
        
        
        programState->setUniformFloat("u_strokeWidth", strokeWidth);
        programState->setUniformVec3("u_strokeColor", Vec3(strokeColor.r / 255.0f, strokeColor.g / 255.0f, strokeColor.b / 255.0f));
        programState->setUniformVec2("u_texSize", Vec2(textureSize.width, textureSize.height));
        
        
        
        return programState;
    }
}
