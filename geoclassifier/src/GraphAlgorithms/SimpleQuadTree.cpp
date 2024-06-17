//
// Created by leon on 14.06.24.
//

#include "SimpleQuadTree.h"
#include "../ClustersBuilder.h"
#include "utils.h"
#include "deque"

void SimpleTreeNode::Split() {
    double width = (bounds.right - bounds.left) / 2;
    double height = (bounds.top - bounds.bottom) / 2;
    if(recursionLevel == maxRecursionLevel) {
        return;
    }
    quadrants = std::vector<SimpleTreeNode*>(4);
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++) {
            Bounds nodeBounds {
                bounds.left + width * j,
                bounds.left + width * (j + 1),
                bounds.top - height * i,
                bounds.top - height * (i + 1)
            };
            auto node = new SimpleTreeNode{nodeBounds};
            node->recursionLevel = recursionLevel + 1;
            node->maxRecursionLevel = maxRecursionLevel;
            quadrants[i * 2 + j] = node;
        }
    }

    std::mutex mutexes[4];

    parallel_for(size_t(0), vertexes.size(), [ref=this, &mutexes](size_t i) {
        for(int j = 0; j < 4; j++) {
            double x = ref->vertexes[i]->getX();
            double y = ref->vertexes[i]->getY();
            if(x > ref->quadrants[j]->bounds.left and x < ref->quadrants[j]->bounds.right
            and y > ref->quadrants[j]->bounds.bottom and y < ref->quadrants[j]->bounds.top) {
                std::lock_guard lock{mutexes[j]};
                ref->quadrants[j]->vertexes.push_back(ref->vertexes[i]);
            }
        }
    });
    for(auto& quadrant : quadrants) {
        if(quadrant->vertexes.empty()) {
            delete quadrant;
            quadrant = nullptr;
        }
    }

}

void SimpleTreeNode::Join() {
    if(quadrants.empty()) {
        auto center = ClustersBuilder::getCenter(vertexes);
        clusters.push_back(new Cluster{(int)vertexes.size(), std::make_pair(center.first, center.second)});
    } else {
        for (auto &quadrant: quadrants) {
            if(quadrant != nullptr) {
                clusters.reserve(clusters.size() + std::distance(quadrant->clusters.begin(), quadrant->clusters.end()));
                clusters.insert(clusters.end(), quadrant->clusters.begin(), quadrant->clusters.end());
            }
        }
        for (auto &quadrant: quadrants) {
            delete quadrant;
        }
    }
    vertexes.clear();
}

std::vector<Cluster*> SimpleQuadTree::getClusters() {
    auto queue = std::deque<SimpleTreeNode*>();
    auto stack = std::deque<SimpleTreeNode*>();
    auto root = new SimpleTreeNode{bounds};
    root->maxRecursionLevel = level;
    root->vertexes = points;
    queue.push_back(root);
    while (!queue.empty()) {
        auto node = queue.front();
        queue.pop_front();
        node->Split();
        for(auto& quadrant : node->quadrants) {
            if(quadrant != nullptr) {
                queue.push_back(quadrant);
            }
        }
        stack.push_back(node);
    }
    SimpleTreeNode* head = nullptr;
    while (!stack.empty()) {
        auto node = stack.back();
        stack.pop_back();
        node->Join();
        head = node;
    }
    auto clusters = std::vector<Cluster*>();
    clusters.reserve(clusters.size() + std::distance(head->clusters.begin(), head->clusters.end()));
    clusters.insert(clusters.end(), head->clusters.begin(), head->clusters.end());

    return clusters;
}