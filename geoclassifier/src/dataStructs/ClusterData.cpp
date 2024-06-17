//
// Created by leon on 26.04.24.
//

#include "ClusterData.h"
#include "QJsonValue"
#include "QJsonObject"
#include "QJsonArray"

ClusterData ClusterData::fromJson(const QJsonObject& json) {
    ClusterData result;

    if(const QJsonValue value = json["coords"]; value.isArray()) {
        const QJsonArray coords = value.toArray();
        result.coords.reserve(2);
        result.coords.append(coords[0].toDouble());
        result.coords.append(coords[1].toDouble());
    }
    if(const QJsonValue value = json["bsCount"]; value.isDouble()) {
        result.bsCount = value.toInt();
    }

    return result;
}

QJsonObject ClusterData::toJson() const {
    QJsonObject jsonObject;
    QJsonArray array;
    array.append(coords[0]);
    array.append(coords[1]);
    jsonObject["type"] = "cluster";
    jsonObject["coords"] = array;
    jsonObject["pointCount"] = bsCount;
    jsonObject["id"] = static_cast<int>(id);

    return jsonObject;
}