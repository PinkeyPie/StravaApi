//
// Created by leon on 24.04.24.
//
#include "delaunay.h"

template<typename T>
const std::vector<Triangle<T>> &Delaunay<T>::triangulate(std::vector<VertexType *> &vertices) {
    _vertices = vertices;

    T minX = vertices[0]->getX();
    T minY = vertices[0]->getY();
    T maxX = minX;
    T maxY = minY;

    for (std::size_t i = 0; i < vertices.size(); ++i) {
        if (vertices[i]->getX() < minX) {
            minX = vertices[i]->getX();
        }
        if (vertices[i]->getY() < minY) {
            minY = vertices[i]->getY();
        }
        if (vertices[i]->getX() > maxX) {
            maxX = vertices[i]->getX();
        }
        if (vertices[i]->getY() > maxY) {
            maxY = vertices[i]->getY();
        }
    }

    const T dx = maxX - minX;
    const T dy = maxY - minY;
    const T deltaMax = std::max(dx, dy);
    const T midX = (minX + maxX) / 2;
    const T midY = (minY + maxY) / 2;

    VertexType p1(midX - 20 * deltaMax, midY - deltaMax);
    VertexType p2(midX, midY + 20 * deltaMax);
    VertexType p3(midX + 20 * deltaMax, midY - deltaMax);

    _triangles.push_back(TriangleType(p1, p2, p3));

    for (auto p = std::begin(vertices); p != std::end(vertices); p++) {
        std::vector<EdgeType> polygon;
        for (auto &t: _triangles) {
            if (t.circumCircleContains(**p)) {
                t.setIsBad(true);
                polygon.push_back(Edge<T>{*t.getA(), *t.getB()});
                polygon.push_back(Edge<T>(*t.getB(), *t.getC()));
                polygon.push_back(Edge<T>(*t.getC(), *t.getA()));
            }
        }

        _triangles.erase(std::remove_if(std::begin(_triangles), std::end(_triangles), [](TriangleType &t) {
            return t.getIsBad();
        }), std::end(_triangles));

        for (auto edgeFirst = std::begin(polygon); edgeFirst != std::end(polygon); ++edgeFirst) {
            for (auto edgeSecond = edgeFirst + 1; edgeSecond != std::end(polygon); ++edgeSecond) {
                if (almost_equal(*edgeFirst, *edgeSecond)) {
                    edgeFirst->setIsBad(true);
                    edgeSecond->setIsBad(true);
                }
            }
        }

        polygon.erase(std::remove_if(polygon.begin(), polygon.end(), [](EdgeType &e) {
            return e.getIsBad();
        }), polygon.end());

        for (const auto edge: polygon) {
            _triangles.push_back(TriangleType{*edge.getV(), *edge.getW(), **p});
        }
    }

    _triangles.erase(std::remove_if(_triangles.begin(), _triangles.end(), [p1, p2, p3](TriangleType &t) {
        return t.containsVertex(p1) || t.containsVertex(p2) || t.containsVertex(p3);
    }), _triangles.end());

    auto qwe1 = _triangles.size();

    for (const auto t: _triangles) {
        _edges.push_back(Edge<T>{*t.getA(), *t.getB()});
        _edges.push_back(Edge<T>{*t.getB(), *t.getC()});
        _edges.push_back(Edge<T>{*t.getC(), *t.getA()});
    }

    return _triangles;
}

template
class Delaunay<float>;

template
class Delaunay<double>;
