#pragma once
#include <utility>

#include "QObject"

class PointData {
    Q_GADGET
public:
    PointData() = default;
    PointData(double x1, double y1, double x2, double y2, int level) : upLeft({x1, y1}), downRight({x2, y2}),
    zoomLevel(level) {}
    QPair<double, double> getUpLeft() { return {upLeft[0], upLeft[1]}; }
    void setUpLeft(double x, double y) { upLeft[0] = x; upLeft[1] = y; }
    QPair<double, double> getDownRight() { return {downRight[0], downRight[1]}; }
    void setDownRight(double x, double y) { downRight[0] = x; downRight[1] = y; }
    int getZoomLevel() const { return zoomLevel; }
    void setZoomLevel(int value) { zoomLevel = value; }
    static PointData fromJson(const QJsonObject &json);
private:
    QList<double> upLeft{0.0,0.0};
    QList<double> downRight{0.0, 0.0};
    int zoomLevel{};
};