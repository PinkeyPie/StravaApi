#include "QObject"

class ClustersData {
    Q_GADGET
public:
    ClustersData() = default;
    static ClustersData fromJson(const QJsonObject& json);
    QJsonObject toJson() const;
    void addPoint(QList<double> point) { points.push_back(point); }
    void addCluster(class ClusterData);
private:
    QList<QList<double>> points{};
    QList<ClusterData> clusters{};
};
