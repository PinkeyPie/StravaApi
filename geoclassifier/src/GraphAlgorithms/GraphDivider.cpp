//
// Created by leon on 27.04.24.
//

#include "GraphDivider.h"
#include "utils.h"
#include "algorithm"
#include "iostream"
#include "map"
#include "queue"

GraphDivider::GraphDivider(std::vector<Edge<double>>& e,
                           std::vector<Vector2<double>*>& p)
                           : edges(e), points(p), pointsLength((int)points.size()),
parent(std::vector<int>(pointsLength)), rank(std::vector<int>(pointsLength)) {
    for(int i = 0; i < pointsLength; i++) {
        makeSet(i);
    }
}

void GraphDivider::buildClusters(int clusterCount) {
    clusterCount--;
    for (int i = 0; i < clusterCount; i++) {
        if (edges.empty()) {
            break;
        }
        edges.pop_back();
    }
    if(edges.empty()) {
        return;
    }
    qsort_async(edges, static_cast<int>(edges.size()),
                [](Edge<double> first, Edge<double> second) {
        return first.getV() - second.getV();
    });
    for(const auto & edge : edges) {
        if(findSet(edge.getV()->getNum()) != findSet(edge.getW()->getNum())) {
            unionSets(edge.getV()->getNum(), edge.getW()->getNum());
        }
    }
    std::map<int, int> map{};
    int k = 0;
    for(int i = 0; i < parent.size(); i++) {
        if(!map.contains(findSet(i))) {
            map[findSet(i)] = k;
            k++;
            clusters.emplace_back(std::vector<Vector2<double>*>());
        }
        clusters[map[findSet(i)]].push_back(points[i]);
    }
}

void GraphDivider::makeSet(int v) {
    parent[v] = v;
    rank[v] = 0;
}

int GraphDivider::findSet(int v) {
    if(v == parent[v]) {
        return v;
    }
    return parent[v] = findSet(parent[v]);
}

void GraphDivider::unionSets(int a, int b) {
    a = findSet(a);
    b = findSet(b);
    if(a != b) {
        if(rank[a] < rank[b]) {
            std::swap(a, b);
        }
        parent[b] = a;
        if(rank[a] == rank[b]) {
            rank[a]++;
        }
    }
}

std::vector<std::vector<Vector2<double>*>> GraphDivider::getClusters() {
    return clusters;
}