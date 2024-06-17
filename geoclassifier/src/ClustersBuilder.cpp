//
// Created by leon on 27.04.24.
//

#include "ClustersBuilder.h"
#include "dataStructs/PointData.h"
#include "cstdlib"
#include "GraphAlgorithms/delaunay.h"
#include "GraphAlgorithms/utils.h"
#include "GraphAlgorithms/SpanTreeBuilder.h"
#include "GraphAlgorithms/GraphDivider.h"
#include "dataStructs/ClusterData.h"
#include "dataStructs/ClustersData.h"
#include "QJsonObject"
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonObject"
#include "arpa/inet.h"
#include "algorithm"
#include "pqxx/pqxx"
#include "sstream"
#include "GraphAlgorithms/QuadTree.h"
#include "GraphAlgorithms/SimpleQuadTree.h"

static const unsigned widthCountBoxes = 80;
static const unsigned heightCountBoxes = 20;

const QMap<int, double> zoomMap {{1, 360.0}, {2, 30.0}, {3, 30.0}, {4, 30.0}, {5, 30.0},
                                 {6, 10.0}, {7, 6.0}, {8, 3.0}, {9, 1.5},
                                 {10, 0.5},{11, 0.1}, {12, 0.1},
                                 {13, 0.1}, {14, 0.05}, {15, 0.025}
                                // , {16, 2.0}, {17, 2.0}, {18, 1.0}, {20, 1.0}
                                };
const double mapLatLength = 180.0;
const double mapLonLength = 90.0;

void getLowerBound(double& iter, double end, double difference, double point) {
    double minDiff = end - iter;
    while(iter < 180.0) {
        double pointDiff = iter - point;
        if(minDiff > abs(pointDiff)) {
            minDiff = abs(pointDiff);
        }
        if(pointDiff > 0) {
            iter -= difference;
            break;
        }
        iter += difference;
    }
}

void getUpperBound(double& iter, double end, double difference, double point) {
    double minDiff = end - iter;
    while(iter < 180.0) {
        double pointDiff = iter - point;
        iter += difference;
        if(pointDiff < 0) {
            continue;
        } else if(minDiff < abs(pointDiff)) {
            minDiff = abs(pointDiff);
        } else {
            iter -= difference;
            break;
        }
    }
}

#define _round(number) std::ceil(roundNumber * number) / roundNumber

ClustersBuilder::ClustersBuilder(PointData &input) : pointData{input}, vertices(std::vector<Vertex*>()) {
    int roundNumber = 1;
    if(pointData.getZoomLevel() > 9 and pointData.getZoomLevel() < 14) {
        roundNumber = 10;
    } else if(pointData.getZoomLevel() == 14) {
        roundNumber = 100;
    } else if(pointData.getZoomLevel() == 15) {
        roundNumber = 1000;
    }
    if(zoomMap.contains(pointData.getZoomLevel())) {
        realUpLeftX = 0.0;
        double diff = zoomMap[pointData.getZoomLevel()];
        double iter = -180.0;
        getLowerBound(iter, 180.0, diff, pointData.getUpLeft().first);
        realUpLeftX = _round(iter);
        getUpperBound(iter, 180.0, diff, pointData.getDownRight().first);
        realDownRightX = _round(iter);
        iter = -90;
        diff /= 2;
        getUpperBound(iter, 90.0, diff, pointData.getDownRight().second);
        realDownRightY = _round(iter);
        getLowerBound(iter, 90.0, diff, pointData.getUpLeft().second);
        realUpLeftY = _round(iter);
    }
}

void ClustersBuilder::buildClusters() {
    selectFromDatabase();
    if(zoomMap[pointData.getZoomLevel()] == 0) {
        return;
    }
    auto clustersBoulder = SimpleQuadTree{Bounds{
        realUpLeftX,
        realDownRightX,
        realUpLeftY,
        realDownRightY
    }, 4};
    clustersBoulder.setPoints(vertices);
    clusters = clustersBoulder.getClusters();

//    auto treeObtain = QuadNode{std::make_pair(realUpLeftX, realUpLeftY),
//                               std::make_pair(realDownRightX, realDownRightY)};
//    treeObtain.setVertexes(vertices);
//    treeObtain.ExecuteSpanTreeBuild();
//    auto spanTree = treeObtain.getEdges();
//    parallel_for(size_t(0), vertices.size(),
//                 [ref=this](size_t i) {
//        ref->vertices[i]->setNum(i);
//    });
//
//    auto divider = GraphDivider{spanTree, vertices};
//    divider.buildClusters(100);
//    auto clusterPoints = divider.getClusters();
//    for(auto& cluster : clusterPoints) {
//        auto clusterCenter = getCenter(cluster);
//        clusters.push_back(Cluster{static_cast<int>(cluster.size()), clusterCenter});
//    }
}

std::pair<double, double> ClustersBuilder::getCenter(std::vector<std::pair<double, double>> points) {
    if(points.size() == 1) {
        return {points[0].first, points[0].second};
    } else if(points.empty()) {
        return {0.0, 0.0};
    }

    auto center = std::make_pair(0.0, 0.0);
    for(auto & point : points) {
        center.first += point.first;
        center.second += point.second;
    }
    center.first /= static_cast<double>(points.size());
    center.second /= static_cast<double>(points.size());
    return center;
}

const char *ClustersBuilder::getClusters() {
    auto jsonArray = QJsonArray{};
    auto pointAdditionalInfo = QJsonObject{};
    for(auto& cluster : clusters) {
        if(cluster->pointsCount == 1) {
            auto point = QList<double>(2);
            point[0] = cluster->getX();
            point[1] = cluster->getY();
            auto pointObject = QJsonObject{};
            auto coordsArray = QJsonArray{};
            coordsArray.append(cluster->getX());
            coordsArray.append(cluster->getY());
            pointObject["coords"] = coordsArray;
            pointObject["type"] = "point";
            jsonArray.append(pointObject);
        }
        else {
            auto clusterData = ClusterData();
            clusterData.setBsCount(cluster->pointsCount);
            clusterData.setLat(cluster->getX());
            clusterData.setLon(cluster->getY());
            jsonArray.append(clusterData.toJson());
        }
    }
    int iter = 0;
    auto pointsData = QJsonObject{};

    auto pointCoords = QJsonArray{};
    auto document = QJsonDocument{jsonArray};
    auto strJson = document.toJson(QJsonDocument::Compact);
    char* retData = new char[strJson.size() + 1];
    retData[0] = '\0';
    strcat(retData, strJson.constData());
    return retData;
}

void ClustersBuilder::selectFromDatabase() {
    auto stringQuery = QString{
            "select st_x(coordinates) as x, st_y(coordinates) as y from select_points(%1, %2, %3, %4)"};
    stringQuery = stringQuery.arg(QString::number(realUpLeftX), QString::number(realDownRightY),
                                  QString::number(realDownRightX), QString::number(realUpLeftY));
    pqxx::connection conn{"dbname=strava user=postgres password=postgres host=localhost"};
    pqxx::work txn{conn};

    auto result = txn.exec(stringQuery.toStdString());
    if(!result.empty()) {
        for(auto i = 0; i < result.size(); i++) {
            auto x = result[i][0].as<double>();
            auto y = result[i][1].as<double>();
            vertices.push_back(new Vector2<double>(x, y, i));
        }
    }
}

ClustersBuilder::~ClustersBuilder() {
    for(auto vertex : vertices) {
        delete vertex;
    }
    vertices.clear();
}

std::pair<double, double> ClustersBuilder::getCenter(std::vector<Vertex*> points) {
    if(points.size() == 1) {
        return {points[0]->getX(), points[0]->getY()};
    } else if(points.empty()) {
        return {0.0, 0.0};
    }

    auto center = std::make_pair(0.0, 0.0);
    for(auto & point : points) {
        center.first += point->getX();
        center.second += point->getY();
    }
    center.first /= static_cast<double>(points.size());
    center.second /= static_cast<double>(points.size());
    return center;
}

std::pair<double, double> ClustersBuilder::getCenter(std::vector<const Vertex*> points) {
    if(points.size() == 1) {
        return {points[0]->getX(), points[0]->getY()};
    } else if(points.empty()) {
        return {0.0, 0.0};
    }

    auto center = std::make_pair(0.0, 0.0);
    for(auto & point : points) {
        center.first += point->getX();
        center.second += point->getY();
    }
    center.first /= static_cast<double>(points.size());
    center.second /= static_cast<double>(points.size());
    return center;
}

