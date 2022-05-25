#pragma once
#include <map>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <time.h>
#include <glm/glm.hpp>

class UniformSubC {
   public:
    int NBINS = 0;
    int RANGE;
    float halfRange;
    float CELL_SIZE;
    int p0 = 6398804701549423;
    int p1 = 4189199475445163;
    int p2 = 5634001703766853;

    std::unordered_map<int, std::vector<int>> points;
    std::unordered_map<int, std::vector<int>> fastQ;
    std::vector<glm::vec3> nHood;

    UniformSubC(int n, int range, std::vector<glm::vec3> typeHood) {
        NBINS = n * 3;
        RANGE = range;
        halfRange = range / 2.f;
        for (int i = 0; i < NBINS; i++) {
            points[i] = {};
        }
        CELL_SIZE = (float)RANGE / (float)(n);
        nHood = typeHood;
        buildLookupTable();
    }

    void clearPoints() {
        points.clear();
        for (int i = 0; i < NBINS; i++) {
            points[i] = {};
        }
    }

    void buildLookupTable() {
        for (int i = 0; i < NBINS / 3; i++) {
            for (int j = 0; j < NBINS / 3; j++) {
                for (int k = 0; k < NBINS / 3; k++) {
                    int tKey = calcKey(glm::clamp(i, 0, NBINS - 1),
                                    glm::clamp(j, 0, NBINS - 1),
                                    glm::clamp(k, 0, NBINS - 1));
                    if (fastQ.find(tKey) == fastQ.end()) {
                        for (auto v : nHood) {
                            // calculates hash key of cell x,y,z
                            int q = calcKey(
                                glm::clamp(i + (int)v.x, 0, NBINS - 1),
                                glm::clamp(j + (int)v.y, 0, NBINS - 1),
                                glm::clamp(k + (int)v.z, 0, NBINS - 1));
                            fastQ[tKey].push_back(q);
                        }
                    }
                }
            }
        }
    }

    void populate(const std::vector<BoidC*> &boids) {
        for (int i = 0; i < boids.size(); i++) {
            insert(boids[i]->pos, i);
        }
    }

    std::vector<int> getCurrentVoxelXYZ(glm::vec3 p) {
        int x = floor((p.x + halfRange) / CELL_SIZE);
        int y = floor((p.y + halfRange) / CELL_SIZE);
        int z = floor((p.z + halfRange) / CELL_SIZE);
        return {x, y, z};
    }

    int getCurrentKeyFromVec3(glm::vec3 p) {
        int x = floor((p.x + halfRange) / CELL_SIZE);
        int y = floor((p.y + halfRange) / CELL_SIZE);
        int z = floor((p.z + halfRange) / CELL_SIZE);
        return ((p0 * x) ^ (p1 * y) ^ (p2 * z)) % NBINS;
    }

    int calcKey(int x, int y, int z) {
        return ((p0 * x) ^ (p1 * y) ^ (p2 * z)) % NBINS;
    }

    int calcBin(glm::vec3 p) {
        std::vector<int> xyz = getCurrentVoxelXYZ(p);
        return calcKey(xyz[0], xyz[1], xyz[2]);
    }

    // insertion
    void insert(glm::vec3 p, int index) {
        std::vector<int> xyz = getCurrentVoxelXYZ(p);
        int key = calcKey(xyz[0], xyz[1], xyz[2]);
        points[key].push_back(index);
    }

    // searching
    std::vector<int> searchByVec(glm::vec3 p) {
        std::vector<int> xyz = getCurrentVoxelXYZ(p);
        int key = calcKey(xyz[0], xyz[1], xyz[2]);
        return points[key];
    }

    std::vector<int> searchByUniKey(int x, int y, int z) {
        int key = calcKey(x, y, z);
        return points[key];
    }
};