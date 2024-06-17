#include "library.h"

#include <iostream>
#include "ClustersBuilder.h"
#include "QJsonObject"
#include "QJsonDocument"
#include "dataStructs/PointData.h"
#include "QCoreApplication"

const char* GetClassifiedPointsCh(const char* pointsData) {
    QByteArray rawData{pointsData};
    QJsonDocument document = QJsonDocument::fromJson(rawData);
    if(!document.isNull()) {
        if(document.isObject()) {
            auto object = document.object();
            auto points = PointData::fromJson(object);
            auto clustersBuilder = ClustersBuilder(points);
            clustersBuilder.buildClusters();
            auto clusters = clustersBuilder.getClusters();
            return clusters;
        }
        return "{\"status\": \"error\"}";
    }
    return "{\"status\": \"error\"}";
}

void FreeMemory(const char* ptr) {
    delete[] ptr;
}