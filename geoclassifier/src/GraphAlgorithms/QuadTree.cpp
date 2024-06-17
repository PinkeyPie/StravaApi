//
// Created by leon on 25.04.24.
//

#include <algorithm>
#include "QuadTree.h"
#include "utils.h"
#include "delaunay.h"
#include "SpanTreeBuilder.h"

QuadNode::QuadNode(std::pair<double, double>& uL, std::pair<double, double>& dR) : upLeft(uL), downRight(dR) {}

QuadNode::QuadNode(std::pair<double, double> uL, std::pair<double, double> dR) :
upLeft(std::move(uL)), downRight(std::move(dR)) {}

void QuadNode::ExecuteSpanTreeBuild() {
    if(vertexes.size() > 1000) {
        SplitTree();
        parallel_for(0, 4, [ref=this](int i){
            if(ref->quadrants[i] != nullptr) {
                ref->quadrants[i]->ExecuteSpanTreeBuild();
            }
        }, 10);
        JoinSubtrees();
    } else if(vertexes.size() > 2){
        parallel_for(size_t(0), vertexes.size(), [ref=this](size_t i) {
            ref->vertexes[i]->setNum(i);
        });
        auto triangulationBuilder = Delaunay<double>();
        triangulationBuilder.triangulate(vertexes);
        auto triangulationEdges = triangulationBuilder.getEdges();
        if(!triangulationEdges.empty()) {
            qsort_async(triangulationEdges, static_cast<int>(triangulationEdges.size()),
                        [](Edge<double> first, Edge<double> second) {
                            return first.getCost() - second.getCost();
                        });
            auto spanTreeBuilder = SpanTreeBuilder(triangulationEdges, static_cast<int>(vertexes.size()));
            spanTreeBuilder.buildST();
            auto spanTree = spanTreeBuilder.getSpanningTree();
            for(auto& edge : spanTree) {
                edges.push_back(edge);
            }
        }
    }
}

void QuadNode::SplitTree() {
    double width = (downRight.first - upLeft.first) / 2;
    double height = (upLeft.second - downRight.second) / 2;

    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++) {
            auto node = new QuadNode{};
            node->upLeft.first = upLeft.first + width * j;
            node->downRight.first = upLeft.first + width * (j + 1);
            node->upLeft.second = upLeft.second - height * i;
            node->downRight.second = upLeft.second - height * (i + 1);
            quadrants[i * 2 + j] = node;
        }
    }

    std::mutex mutexes[4];
    parallel_for(size_t(0), vertexes.size(), [ref=this, &mutexes](size_t i) {
        for(int j = 0; j < 4; j++) {
            double x = ref->vertexes[i]->getX();
            double y = ref->vertexes[i]->getY();
            if(x > ref->quadrants[j]->upLeft.first and x < ref->quadrants[j]->downRight.first
            and y > ref->quadrants[j]->downRight.second and y < ref->quadrants[j]->upLeft.second) {
                std::lock_guard lock{mutexes[j]};
                ref->quadrants[j]->vertexes.push_back(ref->vertexes[i]);
            }
        }
    });
    for(int i = 0; i < 4; i++) {
        if(quadrants[i]->vertexes.size() == 0) {
            delete quadrants[i];
        }
    }
}

void QuadNode::JoinSubtrees() {
    // Todo: maybe there are more complicated and smart way to unite them
    for(auto& quadrant : quadrants) {
        edges.reserve(edges.size() + std::distance(quadrant->edges.begin(), quadrant->edges.end()));
        edges.insert(edges.end(), quadrant->edges.begin(), quadrant->edges.end());
    }
    std::vector<Edge<double>> minEdges{};
    for(int i = 0; i < 4; i++) {
        QuadNode* from = quadrants[i];
        if(from == nullptr) {
            continue;
        }
        for(int j = 0; j < 4 and j != i; j++) {
            QuadNode* to = quadrants[j];
            if(to == nullptr) {
                continue;
            }
            auto xComparer = [](Vector2<double>* first, Vector2<double>* second) {
                return first->getX() - second->getX();
            };
            auto yComparer = [](Vector2<double>* first, Vector2<double>* second) {
                return first->getY() - second->getY();
            };
            qsort_async(from->vertexes, from->vertexes.size(), xComparer);
            qsort_async(to->vertexes, to->vertexes.size(), xComparer);
            auto firstCandidate = Edge<double>{*from->vertexes[0], *to->vertexes[0]};
            qsort_async(from->vertexes, from->vertexes.size(), yComparer);
            qsort_async(to->vertexes, to->vertexes.size(), yComparer);
            auto secondCandidate = Edge<double>{*from->vertexes[0], *to->vertexes[0]};
            if(firstCandidate.getCost() > secondCandidate.getCost()) {
                secondCandidate.getW()->setNum(i);
                secondCandidate.getV()->setNum(j);
                minEdges.push_back(secondCandidate);
            } else {
                firstCandidate.getW()->setNum(i);
                firstCandidate.getV()->setNum(j);
                minEdges.push_back(firstCandidate);
            }
        }
    }
    qsort_async(minEdges, minEdges.size(), [](Edge<double> first, Edge<double> second) {
        return first.getCost() - second.getCost();
    });
    int added[] = {0, 0, 0, 0};
    for(auto edge : minEdges) {
        if(added[edge.getV()->getNum()] == 0 or added[edge.getW()->getNum()] == 0) {
            edges.push_back(edge);
            added[edge.getW()->getNum()] = 1;
            added[edge.getV()->getNum()] = 1;
        }
    }
    for(auto & quadrant : quadrants) {
        delete quadrant;
    }
}

QuadTree::QuadTree(std::pair<double, double> upLeft, std::pair<double, double> downRight) {
    rootNode = std::make_shared<QuadNode>();
}
