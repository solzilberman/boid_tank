#pragma once
#include "skybox.h"
class EngineC {
   public:
    FlockC *flock;
    ShaderC *shader;
    CameraC *camera;
    SkyBoxC *skb;
    TerrainC *terrain;
    int windowWidth;
    int windowHeight;
    bool skyboxEnabled = false;
    bool terrainEnabled = true;
    bool povModeEnabled = false;
    bool shouldDisablePov = false;
    glm::vec3 eye = glm::vec3(10.f, 75.f, 550.f);
    glm::vec3 target = glm::vec3(0, 0, -1);
    EngineC(int ww, int wh, FlockC *fl) {
        // render props
        windowWidth = ww;
        windowHeight = wh;
        shader = new ShaderC("shaders/phong.vert", "shaders/phong.frag");
        camera = new CameraC(eye, target, glm::vec3(0, 1, 0), 45.f,
                             (float)ww / (float)wh, 3.f, 5000.f);
        flock = fl;
        skb = new SkyBoxC();
        terrain = new TerrainC();
    };

    void renderBoids() {
        for (int i = 0; i < flock->countBoids; i++) {
            // calc object transform
            glm::mat4 mod = flock->boids[i]->scl;
            glm::mat4 tr = glm::translate(glm::mat4(1.f), flock->boids[i]->pos);
            mod = tr * flock->boids[i]->rot * flock->boids[i]->scl;
            // set shader props
            shader->setMat4(shader->params->modelParameter, mod);
            shader->setMat4(shader->params->viewParameter, camera->view);
            shader->setMat4(shader->params->projParameter, camera->projection);
            shader->setMat3(
                shader->params->modelViewNParameter,
                glm::mat3(glm::transpose(glm::inverse(camera->view * mod))));
            // mat props
            shader->setVec3(shader->params->kaParameter,
                            flock->boids[i]->mat->ka);
            shader->setVec3(shader->params->kdParameter,
                            flock->boids[i]->mat->kd);
            shader->setVec3(shader->params->ksParameter,
                            flock->boids[i]->mat->ks);
            shader->setFloat(shader->params->shParameter,
                             flock->boids[i]->mat->sh);
            // draw
            flock->boids[i]->vb->use();
            glDrawArrays(GL_TRIANGLES, 0, flock->boids[i]->vb->sizeV);
        }
    }

    void renderBoidsInst(int type) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        std::vector<float> m0;
        m0.reserve(flock->countBoids * 4 * sizeof(float));
        std::vector<float> m1;
        m1.reserve(flock->countBoids * 4 * sizeof(float));
        std::vector<float> m2;
        m2.reserve(flock->countBoids * 4 * sizeof(float));
        std::vector<float> m3;
        m3.reserve(flock->countBoids * 4 * sizeof(float));
        flock->shaderI->use();
        flock->shaderI->setMat4(flock->shaderI->params->viewParameter,
                                camera->view);
        flock->shaderI->setMat4(flock->shaderI->params->projParameter,
                                camera->projection);
        int skipType = type == 0 ? flock->preyType : flock->predType;
        int countBoids = 0;
        for (int i = 0; i < flock->countBoids; i++) {
            if (flock->boids[i]->type != skipType) continue;
            countBoids++;
            glm::mat4 tr = glm::translate(glm::mat4(1.f), flock->boids[i]->pos);
            glm::mat4 mod = tr * flock->boids[i]->rot * flock->boids[i]->scl;

            m0.push_back(mod[0][0]);
            m0.push_back(mod[0][1]);
            m0.push_back(mod[0][2]);
            m0.push_back(mod[0][3]);
            m1.push_back(mod[1][0]);
            m1.push_back(mod[1][1]);
            m1.push_back(mod[1][2]);
            m1.push_back(mod[1][3]);
            m2.push_back(mod[2][0]);
            m2.push_back(mod[2][1]);
            m2.push_back(mod[2][2]);
            m2.push_back(mod[2][3]);
            m3.push_back(mod[3][0]);
            m3.push_back(mod[3][1]);
            m3.push_back(mod[3][2]);
            m3.push_back(mod[3][3]);
        }
        if (type == 0) {
            flock->shaderI->setVec3(glGetUniformLocation(flock->shaderI->program, "objColor"),
                                    flock->preyColor);
            flock->vbiPrey->loadData(2, m0);
            flock->vbiPrey->loadData(3, m1);
            flock->vbiPrey->loadData(4, m2);
            flock->vbiPrey->loadData(5, m3);
            flock->vbiPrey->use();
            glDrawArraysInstanced(GL_TRIANGLES, 0, flock->vbiPrey->sizeV / 3,
                                  countBoids);
        } else {
            flock->shaderI->setVec3(glGetUniformLocation(flock->shaderI->program, "objColor"),
                                    flock->predColor);
            flock->vbiPred->loadData(2, m0);
            flock->vbiPred->loadData(3, m1);
            flock->vbiPred->loadData(4, m2);
            flock->vbiPred->loadData(5, m3);
            flock->vbiPred->use();
            glDrawArraysInstanced(GL_TRIANGLES, 0, flock->vbiPred->sizeV / 3, countBoids);   
        }
    }

    void renderCage() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        shader->setVec3(shader->params->kaParameter, flock->cage->mat->ka);
        shader->setVec3(shader->params->kdParameter, flock->cage->mat->kd);
        shader->setVec3(shader->params->ksParameter, flock->cage->mat->ks);
        shader->setFloat(shader->params->shParameter, flock->cage->mat->sh);
        shader->setMat4(shader->params->modelParameter, flock->cage->model);
        shader->setMat4(shader->params->viewParameter, camera->view);
        shader->setMat4(shader->params->projParameter, camera->projection);
        shader->setMat3(shader->params->modelViewNParameter,
                        glm::mat3(glm::transpose(
                            glm::inverse(camera->view * flock->cage->model))));
        flock->cage->vb->use();
        glDrawArrays(GL_LINES, 0, flock->cage->vb->sizeV);
    }

    void renderTerrain() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        terrain->shader->use();
        terrain->shader->setMat4(terrain->shader->params->modelParameter,
                                 terrain->model);
        terrain->shader->setMat4(terrain->shader->params->viewParameter,
                                 camera->view);
        terrain->shader->setMat4(terrain->shader->params->projParameter,
                                 camera->projection);
        /*shader->setMat3(shader->params->modelViewNParameter,
                        glm::mat3(glm::transpose(glm::inverse(camera->view *
           terrain->model))));*/
        terrain->vb->use();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, terrain->vb->sizeV);
    }

    void renderAllGeoms() {
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        for (int i = 0; i < flock->countGeoms; i++) {
            shader->setVec3(shader->params->kaParameter,
                            flock->geoms[i]->mat->ka);
            shader->setVec3(shader->params->kdParameter,
                            flock->geoms[i]->mat->kd);
            shader->setVec3(shader->params->ksParameter,
                            flock->geoms[i]->mat->ks);
            shader->setFloat(shader->params->shParameter,
                             flock->geoms[i]->mat->sh);
            shader->setMat4(shader->params->modelParameter,
                            flock->geoms[i]->model);
            shader->setMat4(shader->params->viewParameter, camera->view);
            shader->setMat4(shader->params->projParameter, camera->projection);
            shader->setMat3(shader->params->modelViewNParameter,
                            glm::mat3(glm::transpose(glm::inverse(
                                camera->view * flock->geoms[i]->model))));
            flock->geoms[i]->vb->use();
            glDrawArrays(GL_TRIANGLES, 0, flock->geoms[i]->vb->sizeV);
        }
    }

    void renderSkybox() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDepthFunc(GL_LEQUAL);
        glm::mat4 sv = glm::lookAt(glm::vec3(0,0,0), camera->eye, camera->up);
        skb->shader->use();
        skb->shader->setMat4(skb->shader->params->viewParameter, sv);
        skb->shader->setMat4(skb->shader->params->projParameter,
                             camera->projection);
        skb->Render();
        glDepthFunc(GL_LESS);
    }

    void renderScene() {
        if (povModeEnabled) {
            camera->updateEye(glm::vec3(flock->boids[0]->pos.x,flock->boids[0]->pos.y,flock->boids[0]->pos.z) +
                              20.f*glm::normalize(flock->boids[0]->velocity));
            camera->updateTarget(flock->boids[0]->pos);
            shouldDisablePov = true;
        } else if (shouldDisablePov) {
            camera->updateEye(eye);
            camera->updateTarget(target);
            shouldDisablePov = false;
        }

        shader->use();

        // light props
        shader->setVec3(shader->params->laParameter, shader->light->la);
        shader->setVec3(shader->params->ldParameter, shader->light->ld);
        shader->setVec3(shader->params->lsParameter, shader->light->ls);
        shader->setVec4(shader->params->lPosParameter, shader->light->lPos);

        ////light 2
        shader->setVec3(glGetUniformLocation(shader->program, "light2.la"),
                        shader->light2->la);
        shader->setVec3(glGetUniformLocation(shader->program, "light2.ld"),
                        shader->light2->ld);
        shader->setVec3(glGetUniformLocation(shader->program, "light2.ls"),
                        shader->light2->ls);
        shader->setVec4(glGetUniformLocation(shader->program, "light2.lPos"),
                        shader->light2->lPos);

        // light 3
        shader->setVec3(glGetUniformLocation(shader->program, "light3.la"),
                        shader->light3->la);
        shader->setVec3(glGetUniformLocation(shader->program, "light3.ld"),
                        shader->light3->ld);
        shader->setVec3(glGetUniformLocation(shader->program, "light3.ls"),
                        shader->light3->ls);
        shader->setVec4(glGetUniformLocation(shader->program, "light3.lPos"),
                        shader->light3->lPos);
        if (flock->obstacleCourseEnabled) renderAllGeoms();
        if (flock->cageEnabled) renderCage();
        if (terrainEnabled) renderTerrain();
        if (skyboxEnabled) renderSkybox();
        renderBoidsInst(0);
        renderBoidsInst(1);

        // update camera dir for flock
        flock->invertMouse =
            (camera->eye - camera->target).z > 0 ? false : true;
    }

    void stepSimulation() { flock->updateBoids(); }
};