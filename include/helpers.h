#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <glm/glm.hpp>
#include <Windows.h>

// MACROS
#define XAXIS glm::vec3(1.f, 0.f, 0.f)
#define YAXIS glm::vec3(0.f, 1.f, 0.f)
#define ZAXIS glm::vec3(0.f, 0.f, 1.f)
#define EPS 1e-2

#ifndef tstr(x)
#define tstr(x) glm::to_string(x)
#endif

// GENERAL HELPERS
bool aboutEq(float a, float b, float eps) { return fabs(a - b) < eps; };

// BOID HELPERS
std::string MODELNAME = "sparrow";
enum boidType {
    FISH,
    DOLPHIN,
    PEPE,
    BIRD,
    CHICKEN,
    PYRAMID,
    PILL,
    PRED,
    SPARROW,
    CRANE
};
int numTypes = CRANE + 1;

// RANDOM NUM HELPERS
float gauss(float x, float m, float s) {
    // ref:
    // https://stackoverflow.com/questions/10847007/using-the-gaussian-probability-density-function-in-c
    static const float inv_sqrt_2pi = 0.39894;
    float a = (x - m) / s;

    return inv_sqrt_2pi / s * std::exp(-0.5f * a * a);
}

// rand float 0.0 too 1.0 inclusive
float randFloat() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
};

// rand float 0.0 too x
float randFloat(float x) {
    return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / x));
};

// rand float a too b
float randFloat(float a, float b) {
    return a + static_cast<float>(rand()) /
                   (static_cast<float>(RAND_MAX / (b - a)));
};

// FLOAT COMP HELPERS
bool gt(float a, float b, float epsilon) {
    return (a - b) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
};

bool lt(float a, float b, float epsilon) {
    return (b - a) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
};



// LOGGING HELPERS
// REF:
// https://stackoverflow.com/questions/23143693/retrieving-cpu-load-percent-total-in-windows-with-c
static float calculateCPUsage(unsigned long long idleTicks,
                              unsigned long long totalTicks) {
    static unsigned long long _previousTotalTicks = 0;
    static unsigned long long _previousIdleTicks = 0;

    unsigned long long totalTicksSinceLastTime =
        totalTicks - _previousTotalTicks;
    unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;

    float ret =
        1.0f - ((totalTicksSinceLastTime > 0)
                    ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime
                    : 0);

    _previousTotalTicks = totalTicks;
    _previousIdleTicks = idleTicks;
    return ret;
};

static unsigned long long ftimeToI64(const FILETIME &ft) {
    return (((unsigned long long)(ft.dwHighDateTime)) << 32) |
           ((unsigned long long)ft.dwLowDateTime);
};

float getCPULoad() {
    FILETIME idleTime, kernelTime, userTime;
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        return calculateCPUsage(ftimeToI64(idleTime),
                                ftimeToI64(kernelTime) + ftimeToI64(userTime));
    }
    return -1.f;
};

void writeToFile(std::string fn, std::string contents, bool append = false) {
    std::ofstream myfile;
    if (append)
        myfile.open(fn, std::ios_base::app);
    else
        myfile.open(fn);
    myfile << contents;
    myfile.close();
}
