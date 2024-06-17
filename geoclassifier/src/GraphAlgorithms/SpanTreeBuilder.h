#pragma once
#include "edge.h"
#include "vector"
#include "map"

class SpanTreeBuilder {
public:
    SpanTreeBuilder(std::vector<Edge<double>>& e, int pointsCount);
    void buildST();
    std::vector<Edge<double>> getSpanningTree();
private:
    std::vector<Edge<double>> edges;
    std::vector<Edge<double>> result{};
    std::vector<int> parent, rank;
    void make_set(int v);
    int find_set(int v);
    void union_sets(int a, int b);
};