#pragma once

#include <cstdlib>
#include "vector"
#include "vector2.h"

struct Bounds {
    double left = 0.0;
    double right = 0.0;
    double top = 0.0;
    double bottom = 0.0;
};

using Vertex = Vector2<double>;
struct Cluster;

class SimpleTreeNode {
public:
    SimpleTreeNode(Bounds b) : bounds(b) {};
private:
    friend class SimpleQuadTree;
    std::vector<Vertex*> vertexes{};
    std::vector<Cluster*> clusters{};
    Bounds bounds{};
    std::vector<SimpleTreeNode*> quadrants{};
    void Split();
    void Join();
    int recursionLevel{0};
    int maxRecursionLevel{0};
};

class SimpleQuadTree {
public:
    explicit SimpleQuadTree(Bounds b, int recursionLevel) : bounds(b), level(recursionLevel) {};
    std::vector<Cluster*> getClusters();
    void setPoints(std::vector<Vertex*>& value) {
        points = value;
    }
private:
    int level{};
    Bounds bounds{};
    std::vector<Vertex*> points{};
};