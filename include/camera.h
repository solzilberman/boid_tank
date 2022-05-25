#pragma once
#include "include/shaders.h"
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stdio.h>
#include <stdlib.h>

/*=================
Virtual Camera Class
 =================*/
class CameraC {
   public:
    ShaderC *shader;
    glm::vec3 eye;
    glm::vec3 target;
    glm::vec3 up;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;
    float fovy;
    float asp;
    float _near;
    float _far;

    CameraC() {}
    ~CameraC() {}
    CameraC(glm::vec3 eye, glm::vec3 target, glm::vec3 up, float fovy,
            float asp, float n, float f) {
        this->eye = eye;
        this->target = target;
        this->up = up;
        this->fovy = fovy;
        this->asp = asp;
        _near = n;
        _far = f;
        buildLookAt();
        buildProjection();
        buildModel();
    }

    void buildLookAt() { view = glm::lookAt(eye, target, up); }

    void buildProjection() {
        projection = glm::perspective(fovy, asp, _near, _far);
    }

    void buildModel() { model = glm::mat4(1.0f); }

    void bindShaders(ShaderC *shader) { shader = shader; }

    void updateEye(glm::vec3 newEye) {
        eye = newEye;
        buildLookAt();
    }

    void updateTarget(glm::vec3 newTarget) {
        target = newTarget;
        buildLookAt();
    }

    void updateFovy(float fvy) {
        fovy = fvy;
        buildProjection();
    }

    void updateAsp(float a) {
        asp = a;
        buildProjection();
    }
};