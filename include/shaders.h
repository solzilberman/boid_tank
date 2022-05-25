#ifndef __shaders_h__
#define __shaders_h__
#include "GL/glfw3.h"
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

class LightC
{
public:
    glm::vec4 lPos; // position
    glm::vec3 la;   // ambient
    glm::vec3 ld;   // diffuse
    glm::vec3 ls;   // specular
    LightC();
    void setPos(glm::vec4 par)
    {
        lPos = par;
    };
    void setLa(glm::vec3 par)
    {
        la = par;
    };
    void setLs(glm::vec3 par)
    {
        ls = par;
    };
    void setLd(glm::vec3 par)
    {
        ld = par;
    };
};

class ShaderParamsC
{
public:
    GLint modelParameter;      // modeling matrix
    GLint modelViewNParameter; // modeliview for normals
    GLint viewParameter;       // viewing matrix
    GLint projParameter;       // projection matrix
    // material
    GLint kaParameter; // ambient material
    GLint kdParameter; // diffuse material
    GLint ksParameter; // specular material
    GLint shParameter; // shinenness material
    // light
    GLuint laParameter;   // light amb
    GLuint ldParameter;   // light diffuse
    GLuint lsParameter;   // light specular
    GLuint lPosParameter; // light pos
};

class ShaderC
{
public:
    ShaderParamsC *params;
    LightC *light;
    LightC *light2;
    LightC *light3;
    GLuint program;
    ShaderC(std::string vs, std::string fs);
    void initProgram(std::string vs, std::string fs);
    void use();

    // set uniform methods
    void setBool(GLint param, bool val);
    void setInt(GLint param, int val);
    void setFloat(GLint param, float val);
    void setVec2(GLint param, glm::vec2 v);
    void setVec3(GLint param, glm::vec3 v);
    void setVec4(GLint param, glm::vec4 v);
    void setMat3(GLint param, glm::mat3 m);
    void setMat4(GLint param, glm::mat4 m);
};

LightC::LightC()
{
    lPos = glm::vec4(0.f,0.f,0.f,1);
    la = glm::vec3(1, 1, 1);
    ld = glm::vec3(1.f,1.f,1.f);
    ls = glm::vec3(1,0,0);
}

void shaderLog(GLint shader, GLenum eShaderType)
{
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

    const char *strShaderType = NULL;
    switch (eShaderType)
    {
        case GL_VERTEX_SHADER:
            strShaderType = "vertex";
            break;
        case GL_GEOMETRY_SHADER_EXT:
            strShaderType = "geometry";
            break;
        case GL_FRAGMENT_SHADER:
            strShaderType = "fragment";
            break;
        case GL_LINK_STATUS:
            strShaderType = "link";
            break;
    }
    fprintf(stderr,
            "Compile or link failure in %s shader:\n%s\n",
            strShaderType,
            strInfoLog);
    fprintf(stderr, "Press ENTER to exit\n");
    getchar();
    delete[] strInfoLog;
    exit(-1);
}

std::string findFile(const std::string &strFilename)
{
    std::ifstream testFile(strFilename.c_str());
    if (testFile.is_open())
        return strFilename;
    else
    {
        std::cout << "Could not find the file " << strFilename << std::endl;
        return std::string();
    }
}

std::string loadShader(const std::string &strShaderFilename)
{
    std::string strFilename = findFile(strShaderFilename);
    std::ifstream shaderFile(strFilename.c_str());
    std::stringstream shaderData;
    shaderData << shaderFile.rdbuf();
    shaderFile.close();

    return shaderData.str();
}

GLuint createShader(GLenum eShaderType, const std::string &strShader)
{
    GLuint shader = glCreateShader(eShaderType);
    const char *strData = strShader.c_str();
    glShaderSource(shader, 1, &strData, NULL);

    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
        shaderLog(shader, eShaderType);
    else
    {
        /*	   cout << "--------------------Shader------------ " << endl;
               cout << strData << endl;
               cout <<"------------------compiled OK-----------" << endl;*/
    }
    return shader;
}

GLuint createProgram(const std::vector<GLuint> &shaderList)
{
    GLuint program = glCreateProgram();

    for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glAttachShader(program, shaderList[iLoop]);

    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
        shaderLog(GL_LINK_STATUS, program);
    /*{
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }*/

    for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glDetachShader(program, shaderList[iLoop]);

    return program;
}

ShaderC::ShaderC(std::string vs, std::string fs)
{
    initProgram(vs, fs);
}

void ShaderC::initProgram(std::string vs, std::string fs)
{
    params = new ShaderParamsC();
    light = new LightC();
    light->lPos = glm::vec4(0, -50.f, 0, 1);
    light->ls = glm::vec3(1.f,1.f,1.f);
    light->la = glm::vec3(1.f,1.f,1.f);
    light->ld = glm::vec3(1.f,1.f,1.f);

    light2 = new LightC();
    light2->lPos = glm::vec4(50.f, 200.f, 300, 1);
    light2->ls = glm::vec3(1.f, 1.f, 1.f);
    light2->la = glm::vec3(1.f, 1.f, 1.f);
    light2->ld = glm::vec3(1.f, 1.f, 1.f);

    light3 = new LightC();
    light3->lPos = glm::vec4(50.f, 200.f, 300, 1);
    light3->ls = glm::vec3(1.f,1.f,1.f);
    light3->la = glm::vec3(1.f,1.f,1.f);
    light3->ld = glm::vec3(1.f,1.f,1.f);


    std::vector<GLuint> shaderList;

    // load and compile shaders
    shaderList.push_back(createShader(GL_VERTEX_SHADER, loadShader(vs)));
    shaderList.push_back(createShader(GL_FRAGMENT_SHADER, loadShader(fs)));

    // create the shader program and attach the shaders to it
    program = createProgram(shaderList);

    // delete shaders (they are on the GPU now)
    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
    params->modelParameter = glGetUniformLocation(program, "model");
    params->modelViewNParameter = glGetUniformLocation(program, "modelViewN");
    params->viewParameter = glGetUniformLocation(program, "view");
    params->projParameter = glGetUniformLocation(program, "proj");
    // now the material properties
    params->kaParameter = glGetUniformLocation(program, "mat.ka");
    params->kdParameter = glGetUniformLocation(program, "mat.kd");
    params->ksParameter = glGetUniformLocation(program, "mat.ks");
    params->shParameter = glGetUniformLocation(program, "mat.sh");
    // now the light properties
    params->laParameter =   glGetUniformLocation(program, "light.la");
    params->ldParameter =   glGetUniformLocation(program, "light.ld");
    params->lsParameter =   glGetUniformLocation(program, "light.ls");
    params->lPosParameter = glGetUniformLocation(program, "light.lPos");
}

void ShaderC::use()
{
    glUseProgram(program);
}

void ShaderC::setBool(GLint param, bool val)
{
    glUniform1i(param, (int)val);
}

void ShaderC::setInt(GLint param, int val)
{
    glUniform1i(param, val);
}

void ShaderC::setFloat(GLint param, float val)
{
    glUniform1f(param, val);
}

void ShaderC::setVec2(GLint param, glm::vec2 v)
{
    glUniform2f(param, v.x, v.y);
}

void ShaderC::setVec3(GLint param, glm::vec3 v)
{
    glUniform3f(param, v.x, v.y, v.z);
}

void ShaderC::setVec4(GLint param, glm::vec4 v)
{
    glUniform4f(param, v.x, v.y, v.z, v.w);
}

void ShaderC::setMat3(GLint param, glm::mat3 m)
{
    glUniformMatrix3fv(param, 1, GL_FALSE, &m[0][0]);
}

void ShaderC::setMat4(GLint param, glm::mat4 m)
{
    glUniformMatrix4fv(param, 1, GL_FALSE, &m[0][0]);
}

#endif