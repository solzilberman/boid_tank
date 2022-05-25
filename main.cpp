#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GL/glfw3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "STBI/stb_image_write.h"
#include <stdio.h>
#include <stdlib.h>
#include "include/helpers.h"
#include "include/shaders.h"
#include "include/vertexbuffer.h"
#include "include/camera.h"
#include "include/shapes.h"
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#include "include/boid.h"
#include "include/flock.h"
#include "engine.h"

// GLOBAL VARIABLES
namespace globals {
// graphics props
GLFWwindow *window;
glm::vec3 lastMousePos;
bool firstMouseClick = true;
int windowWidth = 1200;
int windowHeight = 800;
int guiWidth = 400;
float yaw = -90.f, pitch = -9.f;
float rotSpeed = 300.f;
bool arcballEnabled = false;
bool autoRotate = false;
float lastAutoRotate;
float walkSpeed = 3.f;

// ImGui props
ImGuiIO io;
int tab;
float previousTime;
int frameCount;
int renderCount;
int guiFrameRate;
float guiCPULoad;
bool recordEnabled = false;

// simulation props
bool simulationState = 0;
bool staticEmmiterEnabled = false;
bool shouldEmit = false;
std::string selectedObstacle = "0";
// flock props
EngineC *engine;
FlockC *flock;
int FLOCK_SIZE = 1000;

// misc (TODO: CLEANUP)
glm::vec3 emitterPos = glm::vec3(0.f, 10.f, 0.f);
float dt = .5f;
int lifetime = 1000;

}  // namespace globals

// gui needs access to globals
#include "gui.h"

static void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void window_size_callback(GLFWwindow *window, int width, int height) {
    globals::windowWidth = width - globals::guiWidth;
    globals::windowHeight = height;
    glViewport(globals::guiWidth, 0, (GLsizei)(globals::windowWidth),
               (GLsizei)(globals::windowHeight));
}

/* Saves current GL pixel buffer to png in (projDir)/model/ss[i].png */
void Snapshot() {
    // ref: https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int x = viewport[0];
    int y = viewport[1];
    int width = viewport[2];
    int height = viewport[3];
    char *data = (char *)malloc((size_t)(width * height * 4));  // 3 components (R, G, B)
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_flip_vertically_on_write(1);
    std::string suff = ".png";
    std::string fn = "logs/frames/ss" + std::to_string(globals::renderCount) + suff;
    int saved = stbi_write_png(fn.data(), width, height, 4, data, 0);
    free(data);
}

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else if (key == GLFW_KEY_N && action != GLFW_RELEASE) {
        globals::engine->stepSimulation();
    } else if (key == GLFW_KEY_P && action != GLFW_RELEASE) {
        globals::simulationState = !globals::simulationState;
        std::cout << "play/pause\n";
    }
    // move camera eye and target with a s w d keys
    else if (key == GLFW_KEY_W && action != GLFW_RELEASE) {
        glm::vec3 tr =
            -glm::normalize(globals::engine->camera->eye - globals::engine->camera->target) *
            globals::walkSpeed;
        tr.y = 0.f;
        globals::engine->camera->updateEye(globals::engine->camera->eye + tr);
        globals::engine->camera->updateTarget(globals::engine->camera->target + tr);
    } else if (key == GLFW_KEY_S && action != GLFW_RELEASE) {
        glm::vec3 tr =
            glm::normalize(globals::engine->camera->eye - globals::engine->camera->target) *
            globals::walkSpeed;
        tr.y = 0.f;
        globals::engine->camera->updateEye(globals::engine->camera->eye + tr);
        globals::engine->camera->updateTarget(globals::engine->camera->target + tr);
    } else if (key == GLFW_KEY_A && action != GLFW_RELEASE) {
        glm::vec3 tr =
            glm::normalize(globals::engine->camera->eye - globals::engine->camera->target);
        tr = glm::cross(tr, globals::engine->camera->up) * globals::walkSpeed;
        tr.y = 0.f;
        globals::engine->camera->updateEye(globals::engine->camera->eye + tr);
        globals::engine->camera->updateTarget(globals::engine->camera->target + tr);
    } else if (key == GLFW_KEY_D && action != GLFW_RELEASE) {
        glm::vec3 tr =
            glm::normalize(globals::engine->camera->eye - globals::engine->camera->target);
        tr = -glm::cross(tr, globals::engine->camera->up) * globals::walkSpeed;
        tr.y = 0.f;
        globals::engine->camera->updateEye(globals::engine->camera->eye + tr);
        globals::engine->camera->updateTarget(globals::engine->camera->target + tr);
    }
}

static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }
    globals::engine->camera->updateFovy(globals::engine->camera->fovy - yoffset * 10.f);
    std::cout << globals::engine->camera->fovy - yoffset * 10.f << std::endl;
    if (globals::engine->camera->fovy <= 5.f) globals::engine->camera->updateFovy(5.f);
    if (globals::engine->camera->fovy >= 65.f) globals::engine->camera->updateFovy(65.f);
}

static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            globals::lastMousePos = glm::vec3(x, y, -1);
            globals::arcballEnabled = true;
        } else if (action == GLFW_RELEASE)
            globals::arcballEnabled = false;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            globals::shouldEmit = true;
            globals::flock->emitBoidOnClick(x, y);
        } else
            globals::shouldEmit = false;
    }
}

static void updateCamera() {
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }
    if (globals::shouldEmit) {
        double x, y;
        glfwGetCursorPos(globals::window, &x, &y);
        globals::flock->emitBoidOnClick(x, y);
    }
    if (!globals::arcballEnabled) return;
    float yclamp = 90.f;
    double cx = 0.f, cy = 0.f;
    glfwGetCursorPos(globals::window, &cx, &cy);
    if (globals::firstMouseClick) {
        globals::lastMousePos.x = cx;
        globals::lastMousePos.y = cy;
        globals::firstMouseClick = false;
    }
    double dx = cx - globals::lastMousePos.x;
    double dy = globals::lastMousePos.y - cy;
    float sens = .1f;
    dx *= sens;
    dy *= sens;
    glm::vec3 tmpEye = glm::rotateY(globals::engine->camera->eye, (float)-dx);
    globals::engine->camera->updateEye(tmpEye);
    //// camera pan functionality
    globals::lastMousePos = glm::vec3(cx, cy, -1);
}

void autoPanCamera() {
    float dx = 1.f;
    float mult = .01f;
    glm::vec3 tmpEye = glm::rotateY(globals::engine->camera->eye, dx * mult * 10.f);
    globals::engine->camera->updateEye(tmpEye);
    globals::engine->camera->updateTarget(glm::vec3(0.f, 0.f, 0.0f));
    globals::yaw += mult * dx;
    globals::lastAutoRotate = dx;
}

void autoForwardCamera() {
    glm::vec3 tmpEye = globals::engine->camera->eye + glm::vec3(0.f, 0.f, -1.f);
    globals::engine->camera->updateEye(tmpEye);
    glm::vec3 tmpTarget = globals::engine->camera->target;
    tmpTarget.y = tmpEye.y;
    globals::engine->camera->updateTarget(tmpTarget);
}

void initGLFW() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    globals::window = glfwCreateWindow(globals::windowWidth + globals::guiWidth,
                                       globals::windowHeight, "Boids", NULL, NULL);
    if (!globals::window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(globals::window, keyCallback);
    // set mouse callback glfw
    glfwSetMouseButtonCallback(globals::window, mouseButtonCallback);
    glfwSetScrollCallback(globals::window, scrollCallback);
    glfwSetWindowSizeCallback(globals::window, window_size_callback);
    glfwMakeContextCurrent(globals::window);
    glfwSwapInterval(1);
}

void initGLEW() {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << glewGetErrorString(err) << std::endl;
        exit(1);
    }
    if (!GLEW_VERSION_3_3) exit(1);
    glClearColor(0.1f, 0.6f, 0.8f, 1);  // clear color buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glViewport(globals::guiWidth, 0, (GLsizei)(globals::windowWidth),
               (GLsizei)(globals::windowHeight));
}

void updateProfilingProps() {
    float currTime = glfwGetTime();
    globals::frameCount++;
    if (currTime - globals::previousTime >= 1.f) {
        globals::guiFrameRate = globals::frameCount;
        globals::guiCPULoad = getCPULoad();
        globals::frameCount = 0.f;
        globals::previousTime = currTime;
    }
}

int main(void) {
    initGLFW();
    initGLEW();
    // init flock and engine
    globals::flock = new FlockC(globals::FLOCK_SIZE, globals::windowWidth, globals::windowHeight,
                                globals::lifetime, globals::dt, globals::window);
    globals::engine = new EngineC(globals::windowWidth, globals::windowHeight, globals::flock);
    initImgui();
    // display loop
    while (!glfwWindowShouldClose(globals::window)) {
        // glReadPixels(0, 0, 100, 100, GL_RGBA, GL_UNSIGNED_BYTE, data);
        if (globals::recordEnabled) {
            Snapshot();
            globals::renderCount++;
            if (globals::renderCount > 1200) exit(0);
        }
        updateProfilingProps();
        updateCamera();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render gui
        renderImguiMenu();

        if (globals::autoRotate) {
            autoForwardCamera();
        }

        // render scene
        if (globals::simulationState) {
            globals::engine->stepSimulation();
        }
        globals::engine->renderScene();  // render all particles

        glfwPollEvents();
        glfwSwapBuffers(globals::window);
    }
    closeImgui();
    glfwDestroyWindow(globals::window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
