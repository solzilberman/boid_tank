#pragma once
//$Header$
// **********************************************************************************
//                                  Flock.h
// **********************************************************************************
// FlockC
//
// @Author: Shlomi Zilberman
// @Created: 03-21-2022
//
/**
 * Flock manager which updates boid props and renders scene
 */
// **********************************************************************************
// **********************************************************************************

#include <stdio.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <helpers.h>

#include <cmath>
#include <fstream>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>

#include "spatialdiv.h"

// GEOMETREY HELPERS
std::vector<GLfloat> boidVerts = {
    0.000000, 0.000000, 0.000000,  0.000000, 0.000000, -1.000000, 0.000000, 1.000000, -1.000000,
    0.000000, 1.000000, -1.000000, 0.000000, 1.000000, 0.000000,  0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000,  0.000000, 0.000000, -1.000000, 2.000000, 0.500000, -0.500000,
    0.000000, 1.000000, -1.000000, 0.000000, 1.000000, 0.000000,  2.000000, 0.500000, -0.500000,
    0.000000, 0.000000, 0.000000,  0.000000, 1.000000, 0.000000,  2.000000, 0.500000, -0.500000,
    0.000000, 0.000000, -1.000000, 0.000000, 1.000000, -1.000000, 2.000000, 0.500000, -0.500000};

std::vector<GLfloat> boidNorms = {
    -1.000000, 0.000000,  0.000000,  -1.000000, 0.000000,  0.000000,  -1.000000, 0.000000,
    0.000000,  -1.000000, 0.000000,  0.000000,  -1.000000, 0.000000,  0.000000,  -1.000000,
    0.000000,  0.000000,  0.500000,  -2.000000, -0.000000, 0.500000,  -2.000000, -0.000000,
    0.500000,  -2.000000, -0.000000, 0.500000,  2.000000,  -0.000000, 0.500000,  2.000000,
    -0.000000, 0.500000,  2.000000,  -0.000000, 0.500000,  -0.000000, 2.000000,  0.500000,
    -0.000000, 2.000000,  0.500000,  -0.000000, 2.000000,  0.500000,  -0.000000, -2.000000,
    0.500000,  -0.000000, -2.000000, 0.500000,  -0.000000, -2.000000};

// nhoods
std::vector<glm::vec3> moore = {
    glm::vec3(0.f, 0.f, 0.f),

    glm::vec3(1.f, -1.f, -1.f),  glm::vec3(1.f, -1.f, 0.f),  glm::vec3(1.f, -1.f, 1.f),
    glm::vec3(1.f, 0.0f, -1.f),  glm::vec3(1.f, 0.0f, 0.f),  glm::vec3(1.f, 0.0f, 1.f),
    glm::vec3(1.f, 1.0f, -1.f),  glm::vec3(1.f, 1.0f, 0.f),  glm::vec3(1.f, 1.0f, 1.f),

    glm::vec3(-1.f, -1.f, -1.f), glm::vec3(-1.f, -1.f, 0.f), glm::vec3(-1.f, -1.f, 1.f),
    glm::vec3(-1.f, 0.0f, -1.f), glm::vec3(-1.f, 0.0f, 0.f), glm::vec3(-1.f, 0.0f, 1.f),
    glm::vec3(-1.f, 1.0f, -1.f), glm::vec3(-1.f, 1.0f, 0.f), glm::vec3(-1.f, 1.0f, 1.f),

    glm::vec3(0.0f, -1.f, -1.f), glm::vec3(0.0f, -1.f, 0.f), glm::vec3(0.0f, -1.f, 1.f),
    glm::vec3(0.0f, 0.0f, -1.f), glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(0.0f, 0.0f, 1.f),
    glm::vec3(0.0f, 1.0f, -1.f), glm::vec3(0.0f, 1.0f, 0.f), glm::vec3(0.0f, 1.0f, 1.f)};

std::vector<glm::vec3> von = {glm::vec3(0.f, 0.f, 0.f),  glm::vec3(1.f, 0.f, 0.f),
                              glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f),
                              glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 1.f),
                              glm::vec3(0.f, 0.f, -1.f)};

std::vector<glm::vec3> basic = {
    glm::vec3(0.f, 0.f, 0.f),
};

class FlockC {
   public:
    // graphics props
    GLFWwindow *window;
    int windowWidth;
    int windowHeight;
    float guiOffset = 400.f;
    bool ompEnabled = false;

    // physics props
    glm::vec3 windForce;
    bool windEnabled = true;
    float gravity;
    float dt;

    // object props
    CageC *cage;
    std::vector<ShapeC *> geoms;
    VertexBufferInstancedC *vbiPrey;
    VertexBufferInstancedC *vbiPred;
    ShaderC *shaderI;
    bool enableInstancedRendering = true;

    // boid props
    std::vector<BoidC *> boids;
    int boidType = FISH;
    float maxSpeed = 1.0f;
    float boidSize;
    float cageSize = 500.f;
    float cageSizeX = 300.f;
    float cageSizeY = 300.f;
    float cageSizeZ = 300.f;
    float halfCageSize = cageSize / 2.f;
    int countBoids;
    int countGeoms;
    int boidLifetime;
    bool obstacleCourseEnabled = false;
    bool cageEnabled = false;
    glm::vec3 boidColor = glm::vec3(0.f, 0.f, 0.f);

    // subdiv props
    int nbins = 100.f;
    UniformSubC *spatialGrid = new UniformSubC(nbins, cageSize, basic);
    std::vector<int> currSearchSpace;

    // flock props
    glm::vec3 steeringVec = glm::vec3(0.f, 0.f, 0.f);
    float steeringCoeff = 0.1f;

    // cohesion
    float cohesionCoeff = 1e-5f;
    float cohesionRadius = cageSize / 5.f;
    bool shouldApplyCohesionRule = true;

    // adhesion props
    float adhesionCoeff = 1e-5f;
    float adhesionRadius = cageSize / 5.f;
    bool shouldApplyAdhesionRule = true;

    // sep props
    float sepCoeff = 1e-3f;
    float sepRadius = 22.5f;
    bool shouldApplySepRule = true;

    // mouse and physics
    float followCoeff = 8e-5f;
    bool followMouse = false;
    bool invertMouse = false;

    // collision and obstacle props
    float collisionCoeff = 0.15f;
    float maxLookAhead = 15.f;

    // predetor prey props
    float xAvoidYCoeff = .005f;
    float xAvoidYRadius = cageSize / 2.f;
    float xAttackYCoeff = .001f;
    float xAttackYRadius = cageSize / 2.f;
    bool predPreyEnabled = true;
    int preyType = boidType;
    int predType = PEPE;
    glm::vec3 preyColor = boidType == PYRAMID ? glm::vec3(0,0,0) : glm::vec3(0.8, .75f, 0.0); 
    glm::vec3 predColor = glm::vec3(0, 1, 0);
    // metrics
    std::ofstream myfile;
    bool shouldLogMetrics = false;

    FlockC(int c, int ww, int wh, int pl, float dt, GLFWwindow *win) {
        if (!c) {
            std::cerr << "CANNOT INIT FLOCK WITH SIZE 0" << std::endl;
            exit(1);
        }
        // render props
        window = win;
        windowWidth = ww;
        windowHeight = wh;
        // boid props
        boidSize = 0.15f;
        gravity = -0.01f;
        countBoids = c;
        countGeoms = 0;
        boidLifetime = pl;
        this->dt = dt;

        // world props
        cage = new CageC(cageSize);
        windForce = glm::vec3(0.f);

        // init boids
        emitRandomBoid();
        // add obstacle
        ShapeC *obs1 = new CubeC(glm::vec3(50.f, 10.f, 0), glm::vec3(10.f, 10.f, 10.f));
        ShapeC *obs2 = new CubeC(glm::vec3(-20.f, -15.f, -5.f), glm::vec3(10.f, 20.f, 10.f));
        ShapeC *obs3 = new CubeC(glm::vec3(-20.f, 25.f, -5.f), glm::vec3(30.f, 10.f, 10.f));
        addGeom(obs1);
        /*addGeom(obs2);
        addGeom(obs3);*/
        shaderI = new ShaderC("shaders/inst.vert", "shaders/inst.frag");

        // metrics
        if (shouldLogMetrics) {
            myfile.open("log.txt");
            myfile << "count boids=" + std::to_string(countBoids) + "\n";
            myfile << "cage size=" + std::to_string(cageSize) + "\n";
            myfile << "num bins=" + std::to_string(nbins * nbins * nbins) + "\n";
        }
        if (preyType == PYRAMID) {
            vbiPrey = new VertexBufferInstancedC(boidVerts, boidNorms);
            loadModel("models/dolphin.txt", 1);  // pred
        } else {
            loadModel("models/fish.txt", 0);     // prey
            loadModel("models/pepe.txt", 1);  // pred
        }
    }

    ~FlockC() { myfile.close(); }

    /************** simulation functionality **************/
    glm::vec3 calculateTendency(int index) {
        /*
        Calculates additional velocity of boid (@ index i) to move towards
        mouse/target.
        @in: index of boid which whose velocity should be updated
        @out: v3 velocity
       */
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        x -= guiOffset;
        float rx = x / windowWidth;
        float ry = (windowHeight - y) / windowHeight;
        rx = rx * cageSize - (cageSize / 2.f);
        ry = ry * cageSize - (cageSize / 2.f);
        glm::vec3 vv = glm::vec3(rx, ry, 0);
        if (invertMouse) vv.x = -vv.x;
        geoms[0]->translate = vv;
        geoms[0]->updateModel();
        return (vv - boids[index]->pos);
    }

    glm::vec3 calculateTypeXAvoidTypeY(int index) {
        glm::vec3 c = glm::vec3(0.f);
        for (auto i : currSearchSpace) {
            // only sees forward
            float inFront = glm::dot(boids[i]->pos - boids[index]->pos, boids[index]->velocity);
            if (i == index || boids[i]->type != predType) continue;
            // loop pred, passed in prey
            float d = glm::distance2(boids[i]->pos, boids[index]->pos);
            if (d <= xAvoidYRadius * xAvoidYRadius) {
                c = c - (boids[i]->pos - boids[index]->pos);
            }
        }
        return c;
    }

    glm::vec3 calculateTypeXAttacksTypeY(int index) {
        glm::vec3 c = glm::vec3(0.f);
        float k = 0;
        int closestPreyInd = -1;
        float closestDist = -INFINITY;
        for (auto i : currSearchSpace) {
            // only sees forward
            float inFront = glm::dot(boids[i]->pos - boids[index]->pos, boids[index]->velocity);
            if (i == index || boids[i]->type != preyType) continue;
            // loop pred, passed in prey
            float d = glm::distance2(boids[i]->pos, boids[index]->pos);
            if (d <= xAttackYRadius * xAttackYRadius) {
                k += 1;
                c = c + boids[i]->pos;
            }
        }
        if (k == 0) return glm::vec3(0.f, 0.f, 0.f);
        return ((c / k) - boids[index]->pos);
    }

    glm::vec3 collisionAvoidance(int index) {
        // calc pos next interval (assume continue on curr path)
        glm::vec3 ahead = boids[index]->pos + glm::normalize(boids[index]->velocity) * maxLookAhead;
        glm::vec3 aheadH =
            boids[index]->pos + glm::normalize(boids[index]->velocity) * maxLookAhead * .5f;

        int closestObsInd = -1;
        float closestObsDist = -INFINITY;
        for (int i = 0; i < countGeoms; i++) {
            // check if ahead inside
            float d = glm::distance(geoms[i]->boundingSphere->center, ahead);
            float dH = glm::distance(geoms[i]->boundingSphere->center, aheadH);
            if (d <= geoms[i]->boundingSphere->radius || dH <= geoms[i]->boundingSphere->radius) {
                // collision will happen
                if (d > closestObsDist) {
                    closestObsInd = i;
                    closestObsDist = dH <= geoms[i]->boundingSphere->radius ? dH : d;
                }
            }
        }

        if (closestObsInd < 0) {
            return glm::vec3(0.f, 0.f, 0.f);
        }

        glm::vec3 ret = ahead - geoms[closestObsInd]->boundingSphere->center;
        ret = glm::normalize(ret);
        return ret;
    }

    glm::vec3 applyAllBasicRulesSinglePass(int index) {
        // coh, adh, sep
        int kc = 0, ka = 0;
        glm::vec3 cm, cv, cs;

        for (const auto &i : currSearchSpace) {
            glm::vec3 sub = boids[i]->pos - boids[index]->pos;
            float inFront = sub.x * boids[index]->velocity.x + sub.y * boids[index]->velocity.y +
                            sub.z * boids[index]->velocity.z;
            if (i == index || inFront > 0.f || boids[i]->type != boids[index]->type) continue;
            float d = glm::distance2(boids[i]->pos, boids[index]->pos);

            // coh
            if (d <= cohesionRadius * cohesionRadius) {
                kc += 1;
                cm += boids[i]->pos;
            }
            // adh
            if (d <= adhesionRadius * adhesionRadius) {
                ka += 1.f;
                cv += boids[i]->velocity;
            }
            // sep
            if (d <= sepRadius * sepRadius) {
                cs = cs - (boids[i]->pos - boids[index]->pos);
            }
        }

        // cohesion
        if (kc == 0 || !shouldApplyCohesionRule)
            cm = glm::vec3(0.f, 0.f, 0.f);
        else {
            cm /= (kc);
            cm = ((cm - boids[index]->pos));
        }
        cm *= cohesionCoeff;

        // adh
        if (ka == 0 || !shouldApplyCohesionRule)
            cv *= 0.f;
        else {
            cv /= (ka);
        }
        cv *= adhesionCoeff;

        // sep
        cs *= shouldApplySepRule;
        cs *= sepCoeff;

        return cm + cv + cs;
    }

    void calcCurrentSearchSpace(int index) {
        currSearchSpace.clear();
        currSearchSpace.reserve(countBoids);
        // gets voxel x,y,z of boid
        for (const auto &k :
             spatialGrid->fastQ[spatialGrid->getCurrentKeyFromVec3(boids[index]->pos)]) {
            for (const auto &i : spatialGrid->points[k]) {
                currSearchSpace.push_back(i);
            }
        }
        // if (shouldLogMetrics)
        //   myfile << std::to_string(indices.size()) + "\n";
    }
    void updateBoids() {
        /*
        Update position, velocity, rotation of all boids.
        Executes each time step of simulation
        */
        // for each type
        for (int i = 0; i < countBoids; i++) {
            // calc nhood of boids[i]
            calcCurrentSearchSpace(i);

            // apply flock rules
            boids[i]->velocity += applyAllBasicRulesSinglePass(i);

            // additional rules
            if (followMouse) boids[i]->velocity += calculateTendency(i) * followCoeff;
            if (windEnabled) boids[i]->velocity += steeringVec * steeringCoeff;
            if (obstacleCourseEnabled) boids[i]->velocity += collisionAvoidance(i) * collisionCoeff;

            //// pred prey sim
            if (predPreyEnabled) {
                if (boids[i]->type == preyType)
                    boids[i]->velocity += calculateTypeXAvoidTypeY(i) * xAvoidYCoeff;
                if (boids[i]->type == predType)
                    boids[i]->velocity += calculateTypeXAttacksTypeY(i) * xAttackYCoeff;
            }

            // if (tvel == YAXIS) {
            //    tvel += glm::vec3(.01f, 0.f, 0.f);
            //}

            // update vel
            boids[i]->velocity = limitVel(boids[i]->velocity, maxSpeed);
            if (!cageEnabled) boids[i]->velocity += keepBoidInBounds(boids[i]->pos);

            //// update lookat matrix
            glm::mat4 rt =
                glm::transpose(glm::lookAt(glm::vec3(0.f, 0.f, 0.f), boids[i]->velocity, YAXIS));
            boids[i]->rot = glm::rotate(rt, boids[i]->rotAdjust, YAXIS);
            // update pos
            boids[i]->pos += boids[i]->velocity;
            if (cageEnabled) {
                boids[i]->pos = forcePacmanWorld(boids[i]->pos);
            }
        }
        // update spatial subdivision
        spatialGrid->clearPoints();
        spatialGrid->populate(boids);
    }

    /************** settings functionality **************/
    void resetFlockProperties() {
        steeringVec = glm::vec3(0.f, 0.f, 0.f);
        steeringCoeff = 0.01f;
        cohesionCoeff = .0015f;
        adhesionCoeff = .01f;
        sepCoeff = 1.f;
        sepRadius = 7.5f;
        followCoeff = 0.001f;
        followMouse = false;
    }

    /************** geom functionality **************/
    void addGeom(ShapeC *shape) {
        geoms.push_back(shape);
        countGeoms += 1;
    }

    /************** boid functionality **************/
    void setColorBasedOnNHood(int index, float q) {
        if (boids[index]->type == FISH) {
            boids[index]->mat->ka = glm::vec3(.20f, q, 0.f);
            boids[index]->mat->kd = glm::vec3(.20f, q, 0.f);
        } else if (boids[index]->type == PEPE) {
            boids[index]->mat->ka = glm::vec3(q, .75f, .2f);
            boids[index]->mat->kd = glm::vec3(q, .75f, .2f);
        } else if (boids[index]->type == CRANE) {
            boids[index]->mat->ka = glm::vec3(1.f, 1.f, 1.f);
            boids[index]->mat->kd = glm::vec3(1.f, 1.f, 1.f);
        } else if (boids[index]->type == BIRD) {
            boids[index]->mat->ka = glm::vec3(0.f, 0.f, 0.f);
            boids[index]->mat->kd = glm::vec3(0.f, 0.f, 0.f);
        } else if (boids[index]->type == PYRAMID) {
            boids[index]->mat->ka = glm::vec3(0.f, 0.f, 0.f);
            boids[index]->mat->kd = glm::vec3(0.f, 0.f, 0.f);
        } else {
            boids[index]->mat->ka = glm::vec3(1.f - q, 0.3f, 0);
            boids[index]->mat->kd = glm::vec3(1.f - q, 0.3f, 0);
            boids[index]->mat->ks = glm::vec3(.1f, .1f, .1f);
        }
    }

    void killBoid(int d) {
        std::vector<BoidC *> tmp;
        BoidC *destroy = 0;
        for (int i = 0; i < boids.size(); i++) {
            if (i != d) {
                tmp.push_back(boids[i]);
            } else {
                destroy = boids[i];
                countBoids -= 1;
            }
        }
        if (destroy) delete destroy;
        boids = tmp;
    }

    void emitRandomBoid() {
        srand(time(0));
        for (int i = 0; i < countBoids; i++) {
            float rng = cageSize / 4.f;
            float x = randFloat(-rng, rng);
            float y = randFloat(-rng, rng);
            float z = randFloat(-rng, rng);
            glm::vec3 pos(x, y, z);

            float vx = randFloat(maxSpeed / 2.f, maxSpeed);
            float vy = randFloat(maxSpeed / 4.f, maxSpeed / 2.f);
            float vz = randFloat(maxSpeed / 4.f, maxSpeed / 2.f);
            glm::vec3 vel(vx, vy, vz);
            // BoidC::boidType tp = randFloat() > .5f ? BoidC::FISH :
            // BoidC::DOLPHIN; pos, radius, vel, range
            BoidC *tmp = new BoidC(glm::normalize(pos)*250.f, boidSize * 1.f, glm::vec3(1,1,1)-glm::normalize(vel), boidLifetime, boidType);
            // random color
            /*      glm::vec3 amb = glm::vec3(.8f, .25f, 0.f);
                  glm::vec3 diff = glm::vec3(.2f, .2f, .2f);*/
            glm::vec3 amb = glm::vec3(boidColor);
            glm::vec3 diff = glm::vec3(boidColor);
            tmp->mat->ka = amb;
            tmp->mat->kd = diff;
            tmp->mat->ks = glm::vec3(.2f, .2f, .2f);
            spatialGrid->insert(glm::vec3(pos.x, pos.y, pos.z), i);
            boids.push_back(tmp);
        }
    }

    void emitBoidXYZFromFloat(float x, float y, float z) {
        float vx = randFloat(-.2f, .2f);
        float vy = randFloat(-.2f, .2f);
        float vz = randFloat(-.2f, .2f);
        emitBoid(glm::vec3(x, y, z), glm::vec3(vx, vy, vz));
    }

    void emitBoidXYZFromVector(glm::vec3 p) {
        float vx = randFloat(-.2f, .2f);
        float vy = randFloat(-.2f, .2f);
        float vz = randFloat(-.2f, .2f);
        emitBoid(glm::vec3(p.x, p.y, p.z), glm::vec3(vx, vy, vz));
    }

    void emitBoidOnClick(float x, float y) {
        float vx = randFloat(.1f, .5f);
        float vy = randFloat(-.2f, .2f);
        float vz = randFloat(-.2f, .2f);
        emitBoid(glm::vec3(0, 0, 0), glm::vec3(vx, vy, vz));
    }

    void emitBoidType(int t) {
        BoidC *tmp =
            new BoidC(glm::vec3(randFloat(), randFloat(), randFloat()), boidSize,
                      glm::vec3(randFloat() * .5f, randFloat(), randFloat()), boidLifetime, t);
        // random color
        glm::vec3 amb = glm::vec3(.0f, 1, .0f);
        glm::vec3 diff = glm::vec3(.3, 1, .3);
        tmp->mat->ka = amb;
        tmp->mat->kd = diff;
        tmp->mat->ks = glm::vec3(1.f, 1.f, 1.f);
        boids.push_back(tmp);
        countBoids += 1;
    }

    void emitBoid(glm::vec3 pos, glm::vec3 vel) {
        BoidC *tmp = new BoidC(pos, boidSize, vel, boidLifetime, predType);
        // random color
        glm::vec3 amb = glm::vec3(randFloat(), randFloat(), randFloat());
        glm::vec3 diff = glm::vec3(randFloat(), randFloat(), randFloat());
        tmp->mat->ka = amb;
        tmp->mat->kd = diff;
        tmp->mat->ks = glm::vec3(1.f, 1.f, 1.f);
        boids.push_back(tmp);
        countBoids += 1;
    }

    // load verts and norms from obj
    void loadModel(std::string fn, int type) {
        std::vector<float> verts;
        std::vector<float> norms;
        // read in file
        std::ifstream infile = std::ifstream(fn);
        std::string line;
        while (getline(infile, line)) {
            // process pair (a,b)
            std::stringstream ss(line);
            std::string fl;
            std::vector<float> vns;
            while (getline(ss, fl, ' ')) {
                vns.push_back(std::stof(fl));
            }
            glm::vec3 v = glm::vec3(vns[0], vns[1], vns[2]);
            glm::vec3 n = glm::normalize(glm::vec3(vns[3], vns[4], vns[5]));
            verts.push_back(v.x);
            verts.push_back(v.y);
            verts.push_back(v.z);
            norms.push_back(n.x);
            norms.push_back(n.y);
            norms.push_back(n.z);
        }
        if (type == 0) {
            vbiPrey = new VertexBufferInstancedC(verts, norms);
        } else {
            vbiPred = new VertexBufferInstancedC(verts, norms);
        }
        verts.clear();
        norms.clear();
    }

    // helpers
    glm::vec3 forcePacmanWorld(glm::vec3 v) {
        float compx = cage->scale[0] / 2.f;
        float compy = cage->scale[1] / 2.f;
        float compz = cage->scale[2] / 2.f;
        if (fabs(v.x) > compx) {
            v.x = v.x > 0 ? -compx + 2.f : compx - 2.f;
        }
        if (fabs(v.y) > compy) {
            v.y = v.y > 0 ? -compy + 2.f : compy - 2.f;
        }
        if (fabs(v.z) > compz) {
            v.z = v.z > 0 ? -compz + 2.f : compz - 2.f;
        }
        return v;
    };

    // bound glm vec 3 to -x,x (pacman like world)
    glm::vec3 keepBoidInBounds(glm::vec3 v) {
        float dt = .1f;
        glm::vec3 ret;
        float compx = cage->scale[0] / 2.f - 30.f;
        float compy = cage->scale[1] / 2.f - 30.f;
        float compz = cage->scale[2] / 2.f - 30.f;
        if (fabs(v.x) > compx) {
            ret.x += v.x > 0 ? -dt : dt;
        }
        if (fabs(v.y) > compy) {
            ret.y += v.y > 0 ? -dt : dt;
        }
        if (fabs(v.z) > compz) {
            ret.z += v.z > 0 ? -dt : dt;
        }
        return ret;
    };

    // bound velocity to [-ms,ms]
    glm::vec3 limitVel(glm::vec3 v, float ms) {
        /*if (fabs(v.x) > ms || fabs(v.y) > ms || fabs(v.z) > ms)
        {
        }*/
        float dt = .2f;
        if (glm::length(v) <= ms) {
            if (aboutEq(v.x, 0.f, 1e-1) && aboutEq(v.y, 0.f, 1e-1) && aboutEq(v.z, 0.f, 1e-1)) {
                v.x += v.x < 0 ? -dt : dt;
            }
            return v;
        }

        v = glm::normalize(v);
        if (aboutEq(v.x, 0.f, 1e-1) && aboutEq(v.y, 0.f, 1e-1) && aboutEq(v.z, 0.f, 1e-1)) {
            v.x += v.x < 0 ? -dt : dt;
        }
        v *= ms;
        return v;
    };
};
