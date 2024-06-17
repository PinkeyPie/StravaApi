#pragma once
#include "QObject"


class ClusterData {
    Q_GADGET
public:
    ClusterData() : bsCount(0), coords(QList<double>(2)) {};
    explicit ClusterData(double ltt, double lng, int bs) : coords(QList<double>{ltt, lng}), bsCount(bs) {}
    double getLat() const { return coords[0]; }
    void setLat(double value) { coords[0] = value; }
    double getLon() const { return coords[1]; }
    void setLon(double value) { coords[1] = value; }
    int getBsCount() { return bsCount; }
    void setBsCount(int value) { bsCount = value; }

    static ClusterData fromJson(const QJsonObject& json);
    QJsonObject toJson() const;
    long id = 0;
private:
    int bsCount{};
    QList<double> coords{};
};