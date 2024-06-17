#pragma once
#include "map"
#include "memory"
#include "vector"
#include "vector2.h"
#include "edge.h"

struct QuadNode {
public:
    QuadNode(std::pair<double, double>& upLeft, std::pair<double, double>& downRight);
    QuadNode(std::pair<double, double> upLeft, std::pair<double, double> downRight);
    QuadNode(){};
    void setVertexes(std::vector<Vector2<double>*>& value) {
        vertexes = value;
    }
    std::vector<Edge<double>> getEdges() {
        return edges;
    }
    void ExecuteSpanTreeBuild();
private:
    QuadNode* quadrants[4] = { nullptr, nullptr, nullptr, nullptr };
    std::pair<double, double> upLeft{};
    std::pair<double, double> downRight{};
    std::vector<Vector2<double>*> vertexes{};
    std::vector<Edge<double>> edges{};
    std::vector<Vector2<double>*> borderPoints{};
private:
    void JoinSubtrees();
    void SplitTree();
};

class QuadTree {
public:
    QuadTree(std::pair<double, double> upLeft, std::pair<double, double> downRight);
private:
    int clusterCount{};
    std::shared_ptr<QuadNode> rootNode{};
};