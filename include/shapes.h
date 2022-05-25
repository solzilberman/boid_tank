#pragma once
#include <glm/glm.hpp>
#include <stdio.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <helpers.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const float cubeVerts[] = {
    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,
    1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f,
    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,
    -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f};

class MaterialC
{
public:
    glm::vec3 ka; // ambient
    glm::vec3 kd; // diffuse
    glm::vec3 ks; // specular
    float sh;     // shineness
    MaterialC(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, float sh)
        : ka(ka), kd(kd), ks(ks), sh(sh) {};
};
glm::vec3 ka = glm::vec3(.0f, .3f, .0f);
glm::vec3 kd = glm::vec3(0.f, .7f, 0.5f);
glm::vec3 ks = glm::vec3(.0f, .0f, .0f);

glm::vec3 tka = glm::vec3(1.f, 1.f, 1.f);
glm::vec3 tkd = glm::vec3(1.f, 1.f, 1.f);
glm::vec3 tks = glm::vec3(1.f, 1.f, 1.f);

glm::vec3 cka = glm::vec3(0.f, 0.f, 0.f);
glm::vec3 ckd = glm::vec3(0.f, 0.f, 0.f);
glm::vec3 cks = glm::vec3(0.f, 0.f, 0.f);

class TriangleC
{
public:
    glm::vec3 p0;
    glm::vec3 p1;
    glm::vec3 p2;
    glm::vec3 normal;
    TriangleC(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
    {
        this->p0 = p0;
        this->p1 = p1;
        this->p2 = p2;
        setNormal();
    }
    void setNormal()
    {
        this->normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));
    }
};

class BoundingSphereC
{
public:
    float radius;
    glm::vec3 center;
    BoundingSphereC(glm::vec3 c, float r)
    {
        center = c;
        radius = r;
    };
};

class ShapeC
{
public:
    VertexBufferC *vb;
    MaterialC *mat;
    BoundingSphereC *boundingSphere;
    std::vector<TriangleC *> triangles;
    std::vector<float> verts, norms;
    glm::mat4 model;
    glm::mat4 rot;
    glm::vec3 translate, scale;
    ShapeC()
    {
        model = glm::mat4(1.f);
        translate = glm::vec3(1.f);
        rot = glm::mat4(1.f);
        scale = glm::vec3(1.f);
        updateModel();
        mat = new MaterialC(ka, kd, ks, 100.f);
        float maxScale = std::max(scale.x, std::max(scale.y, scale.z));
        boundingSphere = new BoundingSphereC(translate, maxScale);
    };
    virtual void setTranslate(glm::vec3 tr)
    {
        translate = tr;
    }
    void scaleModel(glm::vec3 sc)
    {
        scale = sc;
    }
    void rotateModel(float angle, glm::vec3 ax)
    {
        rot = glm::rotate(glm::mat4(1), angle, ax);
    }
    void updateModel()
    {
        model = glm::mat4(1);
        model = glm::translate(model, translate);
        model = rot * model;
        model = glm::scale(model, scale);
        if (boundingSphere)
        {
            boundingSphere->center = translate;
        }
        // loop through triangles and update position
        for (auto t : triangles)
        {
            t->p0 = glm::vec3(model * glm::vec4(t->p0, 1));
            t->p1 = glm::vec3(model * glm::vec4(t->p1, 1));
            t->p2 = glm::vec3(model * glm::vec4(t->p2, 1));
            t->setNormal();
        }
    }

    void addVertexAndNormal(glm::vec3 v, glm::vec3 n)
    {
        verts.push_back(v.x);
        verts.push_back(v.y);
        verts.push_back(v.z);
        norms.push_back(n.x);
        norms.push_back(n.y);
        norms.push_back(n.z);
    }
    void addTriangle(TriangleC *triangle)
    {
        triangles.push_back(triangle);
        addVertexAndNormal(triangle->p0, triangle->normal);
        addVertexAndNormal(triangle->p1, triangle->normal);
        addVertexAndNormal(triangle->p2, triangle->normal);
        bindVertexBuffer();
    }
    void bindVertexBuffer()
    {
        if (vb != NULL)
        {
            delete vb;
        }
        vb = new VertexBufferC(verts, norms);
    }
    void setAmbient(glm::vec3 a)
    {
        mat->ka = a;
    }
    void setDiffuse(glm::vec3 d)
    {
        mat->kd = d;
    }
    void setSpecular(glm::vec3 s)
    {
        mat->ks = s;
    }
};

class CubeC : public ShapeC
{
public:
    CubeC(glm::vec3 tr, glm::vec3 sc)
    {
        float maxScale = std::max(sc.x, std::max(sc.y, sc.z)) * 1.5f;
        boundingSphere = new BoundingSphereC(tr, maxScale);
        model = glm::scale(glm::translate(glm::mat4(1.f), tr), sc);
        translate = tr;
        scale = sc;
        generateVertexAndNormalBuffersCube();
        // updateModel();
    };

    CubeC(glm::vec3 tr, glm::vec3 sc, glm::mat4 r)
    {
        float maxScale = std::max(sc.x, std::max(sc.y, sc.z)) * 1.5f;
        boundingSphere = new BoundingSphereC(tr, maxScale);
        mat = new MaterialC(glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), 100.f);
        model = glm::mat4(1.f);
        translate = tr;
        rot = r;
        scale = sc;
        generateVertexAndNormalBuffersCube();
        updateModel();
    };

    
    void generateVertexAndNormalBuffersCube() {
        float radius = 1.f;
        glm::vec3 v;
        float slices = 20, stacks = 20;
        GLfloat deltaTheta = 2 * M_PI / (GLfloat)slices;
        GLfloat deltaPhi = M_PI / (GLfloat)stacks;
        GLfloat phi, theta;
        for (GLint i = 0; i < stacks; i++) {
            phi = i * deltaPhi;
            for (GLint j = 0; j < slices; j++) {
                theta = j * deltaTheta;
                // the first triangle
                v = glm::vec3(radius * cos(theta) * sin(phi), radius * sin(theta) * sin(phi),
                              radius * cos(phi));
                addVertexAndNormal(v, glm::normalize(v));
                v = glm::vec3(radius * cos(theta + deltaTheta) * sin(phi),
                              radius * sin(theta + deltaTheta) * sin(phi), radius * cos(phi));
                addVertexAndNormal(v, glm::normalize(v));
                v = glm::vec3(radius * cos(theta) * sin(phi + deltaPhi),
                              radius * sin(theta) * sin(phi + deltaPhi),
                              radius * cos(phi + deltaPhi));
                addVertexAndNormal(v, glm::normalize(v));
                // the second triangle
                v = glm::vec3(radius * cos(theta + deltaTheta) * sin(phi),
                              radius * sin(theta + deltaTheta) * sin(phi), radius * cos(phi));
                addVertexAndNormal(v, glm::normalize(v));
                v = glm::vec3(radius * cos(theta) * sin(phi + deltaPhi),
                              radius * sin(theta) * sin(phi + deltaPhi),
                              radius * cos(phi + deltaPhi));
                addVertexAndNormal(v, glm::normalize(v));
                v = glm::vec3(radius * cos(theta + deltaTheta) * sin(phi + deltaPhi),
                              radius * sin(theta + deltaTheta) * sin(phi + deltaPhi),
                              radius * cos(phi + deltaPhi));
                addVertexAndNormal(v, glm::normalize(v));
            }
        }
        vb = new VertexBufferC(verts, norms);
    };
};

class QuadC : public ShapeC
{
public:
    glm::vec3 p0;
    glm::vec3 p1;
    glm::vec3 p2;
    glm::vec3 p3;
    glm::vec3 normal;
    QuadC(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
    {
        this->p0 = p0;
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
        setNormal();
        addTriangle(new TriangleC(p0, p1, p2));
        addTriangle(new TriangleC(p0, p2, p3));
        addVertexAndNormal(this->p0, normal);
        addVertexAndNormal(this->p1, normal);
        addVertexAndNormal(this->p2, normal);
        addVertexAndNormal(this->p3, normal);
    }
    void setNormal()
    {
        this->normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));
    }
};

class CageC : public ShapeC
{
public:
    glm::vec3 p0 = glm::vec3(1.f, 1.f, 1.f);
    glm::vec3 p1 = glm::vec3(1.f, 0.f, 1.f);
    glm::vec3 p2 = glm::vec3(1.f, 0.f, 0.f);
    glm::vec3 p3 = glm::vec3(1.f, 1.f, 0.f);
    glm::vec3 p4 = glm::vec3(0.f, 1.f, 1.f);
    glm::vec3 p5 = glm::vec3(0.f, 0.f, 1.f);
    glm::vec3 p6 = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 p7 = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 normal = glm::vec3(1.f);
    CageC(float s)
    {
        // back face
        addVertexAndNormal(this->p0, normal);
        addVertexAndNormal(this->p1, normal);
        addVertexAndNormal(this->p1, normal);
        addVertexAndNormal(this->p2, normal);
        addVertexAndNormal(this->p2, normal);
        addVertexAndNormal(this->p3, normal);
        addVertexAndNormal(this->p3, normal);
        addVertexAndNormal(this->p0, normal);

        // front face
        addVertexAndNormal(this->p4, normal);
        addVertexAndNormal(this->p5, normal);
        addVertexAndNormal(this->p5, normal);
        addVertexAndNormal(this->p6, normal);
        addVertexAndNormal(this->p6, normal);
        addVertexAndNormal(this->p7, normal);
        addVertexAndNormal(this->p7, normal);
        addVertexAndNormal(this->p4, normal);

        // left face
        addVertexAndNormal(this->p1, normal);
        addVertexAndNormal(this->p5, normal);
        addVertexAndNormal(this->p5, normal);
        addVertexAndNormal(this->p6, normal);
        addVertexAndNormal(this->p6, normal);
        addVertexAndNormal(this->p2, normal);
        addVertexAndNormal(this->p2, normal);
        addVertexAndNormal(this->p1, normal);

        // right face
        addVertexAndNormal(this->p4, normal);
        addVertexAndNormal(this->p0, normal);
        addVertexAndNormal(this->p0, normal);
        addVertexAndNormal(this->p3, normal);
        addVertexAndNormal(this->p3, normal);
        addVertexAndNormal(this->p7, normal);
        addVertexAndNormal(this->p7, normal);
        addVertexAndNormal(this->p4, normal);

        // top face
        addVertexAndNormal(this->p4, normal);
        addVertexAndNormal(this->p5, normal);
        addVertexAndNormal(this->p5, normal);
        addVertexAndNormal(this->p1, normal);
        addVertexAndNormal(this->p1, normal);
        addVertexAndNormal(this->p0, normal);
        addVertexAndNormal(this->p0, normal);
        addVertexAndNormal(this->p4, normal);

        // bottom face
        addVertexAndNormal(this->p7, normal);
        addVertexAndNormal(this->p6, normal);
        addVertexAndNormal(this->p6, normal);
        addVertexAndNormal(this->p2, normal);
        addVertexAndNormal(this->p2, normal);
        addVertexAndNormal(this->p3, normal);
        addVertexAndNormal(this->p3, normal);
        addVertexAndNormal(this->p7, normal);

        float sf = s;
        scale = glm::vec3(sf, sf, sf);
        translate = glm::vec3(-.5f*sf, -.5f*sf, -.5f*sf);
        updateModel();
        bindVertexBuffer();
        mat = new MaterialC(cka, ckd, cks, 0.f);
    }
};

class TerrainC : public ShapeC
{
public:
    int dim = 150;
    int cellSize = 200;
    ShaderC *shader;
    std::vector<glm::vec2> grad;
    std::vector<float> seed;
    float tscl = 1.f;

    TerrainC()
    {
        shader = new ShaderC("shaders/terrain/terr.vert", "shaders/terrain/terr.frag");
        srand(17);
        // populate seed arr
        for (int i = 0; i < dim * dim * dim; i++)
        {
            glm::vec2 tmp((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
            grad.push_back(glm::normalize(tmp));
            seed.push_back((float)rand() / (float)RAND_MAX);
        }

        for (int z = 0; z < dim; z++)
        {
            for (int x = 0; x < dim; x++)
            {
                if (x == dim || z == dim)
                    continue;

                glm::vec3 base;

                // a
                glm::vec3 tmp;
                tmp.x = x * cellSize;
                tmp.z = z * cellSize;
                tmp.y = tscl * perlin(x, z);

                // b
                glm::vec3 tmp1;
                tmp1.x = x * cellSize;
                tmp1.z = (z + 1) * cellSize;
                tmp1.y = tscl * perlin(x, z + 1);

                //// c
                glm::vec3 tmp2;
                tmp2.x = (x + 1) * cellSize;
                tmp2.z = (z)*cellSize;
                tmp2.y = tscl * perlin(x + 1, z);

                glm::vec3 tmp3;
                tmp3.x = (x + 1) * cellSize;
                tmp3.z = (z + 1) * cellSize;
                tmp3.y = tscl * perlin(x + 1, z + 1);

                glm::vec3 norm1 =
                  glm::cross(glm::normalize(tmp2 - tmp), glm::normalize(tmp2 - tmp1));

                addVertexAndNormal(tmp, -norm1);
                addVertexAndNormal(tmp1, -norm1);
                addVertexAndNormal(tmp2, -norm1);
                addVertexAndNormal(tmp3, -norm1);
            }
        }
        float sf = -dim * cellSize * .5f;
        // model = glm::scale(model, glm::vec3(1,1,1));
        model = glm::translate(model, glm::vec3(sf, -350.f, sf));
        bindVertexBuffer();
        mat = new MaterialC(tka, tkd, tks, 100.f);
    }

    float fade(float t, float q)
    {
        return (6 * (t * t) - 15 * (t * t * t * t) + 10 * (t * t * t)) *
               (6 * (q * q) - 15 * (q * q * q * q) + 10 * (q * q * q));
    }

    float lerp(float t, float a, float b)
    {
        return a + t * (b - a);
    }

    float noise(float x, float y)
    {
        // set vals
        int x0 = (int)floor(x);
        int y0 = (int)floor(y);
        int x1 = (x0 + 1) % dim;
        int y1 = (y0 + 1) % dim;
        x -= x0;
        y -= y0;
        // vec from corners -> x,y
        glm::vec2 v00 = glm::vec2(x, y) - glm::vec2(x0, y0);
        glm::vec2 v10 = glm::vec2(x, y) - glm::vec2(x1, y0);
        glm::vec2 v01 = glm::vec2(x, y) - glm::vec2(x0, y1);
        glm::vec2 v11 = glm::vec2(x, y) - glm::vec2(x1, y1);

        float g00 = seed[y0 * dim + x0] * glm::dot(v00, grad[y0 * dim + x0]);
        float g10 = seed[y0 * dim + x1] * glm::dot(v10, grad[y0 * dim + x1]);
        float g01 = seed[y1 * dim + x0] * glm::dot(v01, grad[y1 * dim + x0]);
        float g11 = seed[y1 * dim + x1] * glm::dot(v10, grad[y1 * dim + x1]);

        float phi =
          fade(1 - x, 1 - y) * g00 + fade(x, 1 - y) * g10 + fade(1 - x, y) * g01 + fade(x, y) * g11;
        return phi;
    }

    float perlin(float x, float y)
    {
        float sm = 0;
        float p = .1f;
        float sl = 1.3f;
        for (int i = 0; i < 3; i++)
        {
            float val = pow(p, i) * noise(pow(sl, i) * x, pow(sl, i) * y);
            sm += val;
        }
        return -sm;
    }
};

class CubeInstC : public ShapeC
{
public:
    VertexBufferInstancedC *vbi;
    ShaderC *shaderI = new ShaderC("shaders/inst.vert", "shaders/inst.frag");

    CubeInstC(glm::vec3 tr, glm::vec3 sc)
    {
        float maxScale = std::max(sc.x, std::max(sc.y, sc.z)) * 1.5f;
        boundingSphere = new BoundingSphereC(tr, maxScale);
        model = glm::scale(glm::translate(glm::mat4(1.f), tr), sc);
        translate = tr;
        scale = sc;
        generateVertexAndNormalBuffersCube();
        // updateModel();
    };

    CubeInstC(glm::vec3 tr, glm::vec3 sc, glm::mat4 r)
    {
        float maxScale = std::max(sc.x, std::max(sc.y, sc.z)) * 1.5f;
        boundingSphere = new BoundingSphereC(tr, maxScale);
        mat = new MaterialC(ka, kd, ks, 100.f);
        model = glm::mat4(1.f);
        translate = tr;
        rot = r;
        scale = sc;
        generateVertexAndNormalBuffersCube();
        updateModel();
    };

    void generateVertexAndNormalBuffersCube()
    {
        for (int i = 0; i < 12; i++)
        {
            int ind = i * 9;
            std::vector<glm::vec3> tmpVerts;
            for (int j = 0; j < 3; j++)
            {
                glm::vec3 v = glm::vec3(
                  cubeVerts[ind + j * 3], cubeVerts[ind + j * 3 + 1], cubeVerts[ind + j * 3 + 2]);
                tmpVerts.push_back(v);
            }
            TriangleC *t = new TriangleC(tmpVerts[0], tmpVerts[1], tmpVerts[2]);
            triangles.push_back(t);
            addVertexAndNormal(t->p0, t->normal);
            addVertexAndNormal(t->p1, t->normal);
            addVertexAndNormal(t->p2, t->normal);
        }
    }
};
