//
// Created by leon on 26.04.24.
//

#include "ClustersData.h"
#include "QJsonValue"
#include "QJsonObject"
#include "QJsonArray"
#include "PointData.h"
#include "ClusterData.h"


ClustersData ClustersData::fromJson(const QJsonObject &json) {
    ClustersData clustersData;

    if(const QJsonValue value = json["points"]; value.isArray()) {
        const QJsonArray points = value.toArray();
        clustersData.points.reserve(points.size());
        for(const QJsonValue& point : points) {
            if(point.isArray()) {
                const QJsonArray pointCoords = point.toArray();
                auto coords = QList<double>(2);
                coords[0] = pointCoords[0].toDouble();
                coords[1] = pointCoords[1].toDouble();
                clustersData.points.append(coords);
            }
        }
    }

    if(const QJsonValue value = json["clusters"]; value.isArray()) {
        const QJsonArray clusters = value.toArray();
        clustersData.clusters.reserve(clusters.size());
        for(const QJsonValue& cluster : clusters) {
            clustersData.clusters.append(ClusterData::fromJson(cluster.toObject()));
        }
    }

    return clustersData;
}

void ClustersData::addCluster(ClusterData cluster) {
    clusters.push_back(cluster);
}

QJsonObject ClustersData::toJson() const {
    QJsonObject jsonObject;
    QJsonArray pointsArray;
    for(const auto& point : points) {
        QJsonArray coordsArray;
        coordsArray.append(point[0]);
        coordsArray.append(point[1]);
        pointsArray.append(coordsArray);
    }
    jsonObject["points"] = pointsArray;
    QJsonArray clustersArray;
    for(const auto& cluster : clusters) {
        clustersArray.append(cluster.toJson());
    }
    jsonObject["clusters"] = clustersArray;
    return jsonObject;
}