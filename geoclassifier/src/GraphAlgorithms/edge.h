#pragma once

#include "vector2.h"

template<typename T>
class Edge {
    using Type = T;
    using VertexType = Vector2<Type>;
public:
    Edge() = default;

    Edge(const Edge &) = default;

    Edge(Edge &&) = default;

    Edge(VertexType &first, VertexType &second);

    Edge &operator=(const Edge &) = default;

    Edge &operator=(Edge &&) = default;

    bool operator==(const Edge &edge) const;

    template<typename U>
    friend std::ostream &operator<<(std::ostream &os, const Edge<U> &edge);

    VertexType *getV() const { return v; }

    VertexType *getW() const { return w; }

    T getCost() const;

    bool getIsBad() const { return isBad; }

    void setV(VertexType *val) { v = val; }

    void setW(VertexType *val) { w = val; }

    void setIsBad(bool val) { isBad = val; }

private:
    VertexType *v;
    VertexType *w;
    bool isBad = false;
};

template<typename T>
bool almost_equal(const Edge<T> &first, const Edge<T> &second) {
    return (almost_equal(*first.getV(), *second.getV()) && almost_equal(*first.getW(), *second.getW()) ||
            almost_equal(*first.getV(), *second.getW()) && almost_equal(*first.getW(), *second.getV()));
}
