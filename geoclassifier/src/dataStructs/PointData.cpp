//
// Created by leon on 26.04.24.
//

#include <QJsonValue>
#include "QJsonObject"
#include "PointData.h"
#include "QJsonArray"

PointData PointData::fromJson(const QJsonObject &json) {
    PointData result;

    if(const QJsonValue value = json["upLeft"]; value.isArray()) {
        const QJsonArray coords = value.toArray();
        result.upLeft[0] = coords[0].toDouble();
        result.upLeft[1] = coords[1].toDouble();
    }
    if(const QJsonValue value = json["downRight"]; value.isArray()) {
        auto coords = value.toArray();
        result.downRight[0] = coords[0].toDouble();
        result.downRight[1] = coords[1].toDouble();
    }
    if(const QJsonValue v = json["zoomLevel"]; v.isDouble()) {
        result.zoomLevel = v.toInt();
    }

    return result;
}