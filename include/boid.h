#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <stdio.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <helpers.h>
#include <fstream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static bool checkVectorNormApproxZero(glm::vec3 v) {
    /*if (lt(fabs(glm::sqrt(glm::dot(v, v))), 1e-4f, 1e-4))
    {
        return true;
    }*/
    if (lt(fabs(v.y), 1e-2, 1e-4)) return true;
    return false;
}

// default color consts
glm::vec3 bka = glm::vec3(0.f, 1.f, 1.f);
glm::vec3 bkd = glm::vec3(0.0f, 1.f, 1.f);
glm::vec3 bks = glm::vec3(0.f, 0.f, 0.f);

class BoidC {
   public:
    VertexBufferC *vb; //todo: delete
    MaterialC *mat;
    int type;
    std::vector<float> verts, norms; //todo: delete
    glm::mat4 model = glm::mat4(1.f);
    glm::mat4 rot = glm::mat4(1.f);
    float rotAdjust =
        0.f;  // adjusts model based on its initial orientation for steering
    glm::mat4 scl = glm::scale(glm::mat4(1.f), glm::vec3(1.f, 1.f, 1.f)); 
    // glm::scale(glm::mat4(1.f), glm::vec3(20.5f, 5.5f, 5.5f));
    glm::vec3 pos;
    glm::vec3 force;//todo: delete
    glm::vec3 velocity;
    // glm::quat dir = glm::quat(1.f, 0.f, 0.f, 0.f);
    float mass;//todo: delete
    float radius;//todo: delete
    bool stopped;
    int lifetime = 100.f;

    BoidC(glm::vec3 p, float r, glm::vec3 v, int lt, int tpe) {
        mat = new MaterialC(bka, bkd, bks, 10.f);
        pos = p;
        radius = r;//todo: delete
        velocity = v;
        lifetime = lt;
        stopped = false;
        type = tpe;
        // todo: cleanup
        switch (type) {
            case FISH: {
                glm::scale(glm::mat4(1.f), glm::vec3(0.15f, .15f, .15f));
                rotAdjust = 180.f;
                break;
            }
            case DOLPHIN: {
                rotAdjust = 0.f;
                scl =
                    glm::scale(glm::mat4(1.f), glm::vec3(0.075f, .075f, .075f));
                break;
            }
            case PEPE: {
                rotAdjust = 180.f;
                scl =
                    glm::scale(glm::mat4(1.f), glm::vec3(0.045f, .045f, .045f));
                break;
            }
            case PRED: {
                rotAdjust = 90.f;
                scl = glm::scale(glm::mat4(1.f), glm::vec3(2.f, 5.f, 5.f));
                break;
            }
            case CHICKEN:
                rotAdjust = 0.f;
                scl = glm::scale(glm::mat4(1.f), glm::vec3(0.01f, 0.01, 0.01));
                break;
            case BIRD:
                rotAdjust = 180.f;
                scl =
                    glm::scale(glm::mat4(1.f), glm::vec3(0.15f, 0.15f, 0.15f));
                break;
            case PYRAMID: {
                rotAdjust = 90.f;
                generateVertexAndNormalBuffersPyramid();
                break;
            }
            case SPARROW: {
                rotAdjust = 180.f;
                scl = glm::scale(glm::mat4(1.f),
                                 glm::vec3(0.25f, 0.25f, 0.25f));
                break;
            }
            default: {
                rotAdjust = 90.f;
                generateVertexAndNormalBuffersPyramid();
                break;
            }
        }
    }

    // todo: delete
    void rotateModel(float angle, glm::vec3 axis) {
        rot = glm::rotate(glm::mat4(1.f), angle, axis);
    }
    // todo: delete
    void addVertexAndNormal(glm::vec3 v, glm::vec3 n) {
        verts.push_back(v.x);
        verts.push_back(v.y);
        verts.push_back(v.z);
        norms.push_back(n.x);
        norms.push_back(n.y);
        norms.push_back(n.z);
    }
    // todo: delete
    void loadVertexAndNormalFromObj(std::string fn) {
        // read in file
        std::ifstream infile = std::ifstream(fn);
        std::string line;
        float a, b, c, d, e, f;
        while (getline(infile, line)) {
            // process pair (a,b)
            std::stringstream ss(line);
            std::string fl;
            std::vector<float> vns;
            while (getline(ss, fl, ' ')) {
                vns.push_back(std::stof(fl));
            }
            addVertexAndNormal(
                glm::vec3(vns[0], vns[1], vns[2]),
                glm::normalize(glm::vec3(vns[3], vns[4], vns[5])));
        }
        //vb = new VertexBufferC(verts, norms);
        //verts.clear();
        //norms.clear();
    }
    // todo: delete
    void generateVertexAndNormalBuffersPyramid() {
        // base
        glm::vec3 p0 = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 p1 = glm::vec3(0.f, 0.f, -1.f);
        glm::vec3 p2 = glm::vec3(0.f, 1.f, -1.f);
        glm::vec3 p3 = glm::vec3(0.f, 1.f, 0.f);
        // point
        glm::vec3 p4 = glm::vec3(2.f, 0.5f, -0.5f);

        // bottom face
        glm::vec3 normal = glm::cross(p2 - p0, p1 - p0);
        addVertexAndNormal(p0, normal);
        addVertexAndNormal(p1, normal);
        addVertexAndNormal(p2, normal);
        normal = glm::cross(p3 - p0, p2 - p0);
        addVertexAndNormal(p2, normal);
        addVertexAndNormal(p3, normal);
        addVertexAndNormal(p0, normal);

        // bottom tri
        normal = -glm::cross(p4 - p0, p1 - p0);
        addVertexAndNormal(p0, normal);
        addVertexAndNormal(p1, normal);
        addVertexAndNormal(p4, normal);

        // top tri
        normal = -glm::cross(p4 - p2, p3 - p2);
        addVertexAndNormal(p2, normal);
        addVertexAndNormal(p3, normal);
        addVertexAndNormal(p4, normal);

        // right tri
        normal = glm::cross(p4 - p0, p3 - p0);
        addVertexAndNormal(p0, normal);
        addVertexAndNormal(p3, normal);
        addVertexAndNormal(p4, normal);

        // left tri
        normal = -glm::cross(p4 - p1, p2 - p1);
        addVertexAndNormal(p1, normal);
        addVertexAndNormal(p2, normal);
        addVertexAndNormal(p4, normal);

        // vb = new VertexBufferC(verts, norms);
    /*  verts.clear();
        norms.clear();*/
    };
};
