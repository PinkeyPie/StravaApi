#pragma once
#include "vector"
#include "GraphAlgorithms/vector2.h"
#include "GraphAlgorithms/edge.h"
#include "QMap"

class PointData;
struct Cluster;

struct Rect {
    QPair<double, double> upLeft;
    QPair<double, double> downRight;
};

using Vertex = Vector2<double>;

class Cluster : public Vector2<double> {
public:
    Cluster(int count, std::pair<double, double> center) : Vector2<double>(center.first, center.second, 0),
            pointsCount{count} {}
    int pointsCount;
};

class ClustersBuilder {
public:
    explicit ClustersBuilder(PointData& input);
    ~ClustersBuilder();
    void buildClusters();
    const char* getClusters();
    static std::pair<double, double> getCenter(std::vector<std::pair<double, double>> points);
    static std::pair<double, double> getCenter(std::vector<Vertex*> points);
    static std::pair<double, double> getCenter(std::vector<const Vertex*> points);
    std::vector<int> pointNumbers{};
private:
    void selectFromDatabase();
    PointData& pointData;
    double realUpLeftX{0.0};
    double realUpLeftY{0.0};
    double realDownRightX{0.0};
    double realDownRightY{0.0};
    std::vector<Vertex*> vertices{};
    std::vector<Cluster*> clusters{};
};