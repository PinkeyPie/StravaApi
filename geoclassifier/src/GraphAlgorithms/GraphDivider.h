#include "vector"
#include "edge.h"

class GraphDivider {
public:
    GraphDivider(std::vector<Edge<double>>& e,
                 std::vector<Vector2<double>*>& p);
    void buildClusters(int clusterCount);
    std::vector<std::vector<Vector2<double>*>> getClusters();
private:
    std::vector<Vector2<double>*>& points;
    std::vector<std::vector<Vector2<double>*>> clusters{};
    std::vector<Edge<double>> &edges;
    int pointsLength;
    std::vector<int> parent{};
    std::vector<int> rank{};
    void makeSet(int v);
    int findSet(int v);
    void unionSets(int a, int b);
};