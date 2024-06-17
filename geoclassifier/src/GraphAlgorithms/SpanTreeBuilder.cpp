//
// Created by leon on 27.04.24.
//
#include "SpanTreeBuilder.h"

SpanTreeBuilder::SpanTreeBuilder(std::vector<Edge<double>> &e, int pointsCount) : edges(e),
parent(std::vector<int>(pointsCount)), rank(std::vector<int>(pointsCount)) {
    for (int i = 0; i < pointsCount; i++) {
        make_set(i);
    }
}

void SpanTreeBuilder::buildST() {
    float cost = 0;
    for (int i = 0; i < edges.size(); i++) {
        if (find_set(edges[i].getV()->getNum()) != find_set(edges[i].getW()->getNum())) {
            cost += edges[i].getCost();
            result.emplace_back(edges[i]);
            union_sets(edges[i].getV()->getNum(), edges[i].getW()->getNum());
        }
    }
}

std::vector<Edge<double>> SpanTreeBuilder::getSpanningTree() {
    return result;
}

void SpanTreeBuilder::make_set(int v) {
    parent[v] = v;
    rank[v] = 0;
}

int SpanTreeBuilder::find_set(int v) {
    if (v == parent[v]) {
        return v;
    }
    return parent[v] = find_set(parent[v]);
}

void SpanTreeBuilder::union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (rank[a] < rank[b]) {
            std::swap(a, b);
        }
        parent[b] = a;
        if (rank[a] == rank[b]) {
            rank[a]++;
        }
    }
}