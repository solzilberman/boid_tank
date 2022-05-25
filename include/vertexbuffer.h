#include <stdio.h>
#include <vector>

/* ===============
   VertexBuffer Manager Class
   ===============*/
class VertexBufferC {
   public:
    GLuint vao;
    GLuint vboHandles[2];
    GLuint sizeV;  // size of vertex array
    GLuint sizeN;  // size of normal array

    VertexBufferC(std::vector<GLfloat> verts, std::vector<GLfloat> norms);
    ~VertexBufferC(){};
    void use();

   protected:
    GLuint buffer;  // vertex buffer
};

inline size_t sum(std::vector<int> partition) {
    size_t ret = 0;
    for (auto i : partition) {
        ret += i;
    };
    return ret;
}

VertexBufferC::VertexBufferC(std::vector<GLfloat> verts,
                             std::vector<GLfloat> norms) {
    sizeV = verts.size();
    sizeN = norms.size();

    // get the vertex array handle and bind it
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // the vertex array will have two vbos, vertices and normals
    glGenBuffers(2, vboHandles);
    GLuint verticesID = vboHandles[0];
    GLuint normalsID = vboHandles[1];

    // send vertices
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, sizeV * sizeof(GLfloat), verts.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    verts.clear();  // no need for the vertex data, it is on the GPU now

    // send normals
    glBindBuffer(GL_ARRAY_BUFFER, normalsID);
    glBufferData(GL_ARRAY_BUFFER, sizeN * sizeof(GLfloat), norms.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    norms.clear();  // no need for the normal data, it is on the GPU now
}

void VertexBufferC::use() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(0);
}

class VertexBufferInstancedC {
   public:
    int vboCount = 8;
    GLuint vao;
    GLuint vboHandles[8];
    GLuint sizeV;  // size of vertex array
    GLuint sizeN;  // size of normal array
    int maxSize = 100000;
    GLuint buffer = 0;
    VertexBufferInstancedC(std::vector<float> verts, std::vector<float> norms) {
        sizeV = verts.size();
        sizeN = norms.size();

        // get the vertex array handle and bind it
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // the vertex array will have two vbos, vertices and normals
        glGenBuffers(vboCount, vboHandles);
        GLuint verticesID = vboHandles[0];
        GLuint normalsID = vboHandles[1];
        GLuint r0 = vboHandles[2];
        GLuint r1 = vboHandles[3];
        GLuint r2 = vboHandles[4];
        GLuint r3 = vboHandles[5];

        // send vertices
        glBindBuffer(GL_ARRAY_BUFFER, verticesID);
        glBufferData(GL_ARRAY_BUFFER, sizeV * sizeof(GLfloat), verts.data(),
                     GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        // send normals
        glBindBuffer(GL_ARRAY_BUFFER, normalsID);
        glBufferData(GL_ARRAY_BUFFER, sizeN * sizeof(GLfloat), norms.data(),
                     GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        for (int i = 2; i < 6; i++) {
            glBindBuffer(GL_ARRAY_BUFFER, vboHandles[i]);
            glBufferData(GL_ARRAY_BUFFER, maxSize * sizeof(GLfloat) * 4, NULL,
                         GL_STREAM_DRAW);
            glVertexAttribPointer((GLuint)i, 4, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(i);
            glVertexAttribDivisor(i, 1);
        }
    }

    void free() {
        glDeleteBuffers(1, &vao);
        glDeleteVertexArrays(1, &vao);
    }

    void use() {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
    }

    void loadData(GLint vboID, std::vector<float> data) {
        glBindBuffer(GL_ARRAY_BUFFER, vboHandles[vboID]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * data.size(),
                        data.data());
    }
};