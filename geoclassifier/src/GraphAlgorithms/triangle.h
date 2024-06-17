#pragma once

#include "numeric.h"
#include "vector2.h"
#include "edge.h"


template<typename T>
class Triangle {
    using Type = T;
    using VertexType = Vector2<Type>;
    using EdgeType = Edge<T>;
public:
    Triangle() = default;

    Triangle(const Triangle &) = default;

    Triangle(Triangle &&) = default;

    Triangle(VertexType &first, VertexType &second, VertexType &third);

    bool containsVertex(const VertexType &vertex) const;

    bool circumCircleContains(const VertexType &vertex) const;

    Triangle &operator=(const Triangle &) = default;

    Triangle &operator=(Triangle &&) = default;

    bool operator==(const Triangle &other) const;

    VertexType *getA() const { return a; }

    VertexType *getB() const { return b; }

    VertexType *getC() const { return c; }

    bool getIsBad() const { return isBad; }

    void setA(VertexType &val) { a = &val; }

    void setB(VertexType &val) { b = &val; }

    void setC(VertexType &val) { c = &val; }

    void setIsBad(bool val) { isBad = val; }

    template<class U>
    friend std::ostream &operator<<(std::ostream &os, const Triangle<U> &triangle);

private:
    VertexType *a;
    VertexType *b;
    VertexType *c;
    bool isBad = false;

    static_assert(std::is_floating_point<Triangle<T>::Type>::value, "Type must be floating point");
};

template<typename T>
bool almost_equal(const Triangle<T> &first, const Triangle<T> &second) {
    return (almost_equal(*first.getA(), *second.getA()) || almost_equal(*first.getA(), *second.getB()) ||
            almost_equal(*first.getA(), *second.getC())) &&
           (almost_equal(*first.getB(), *second.getA()) || almost_equal(*first.getB(), *second.getB()) ||
            almost_equal(*first.getB(), *second.getC())) &&
           (almost_equal(*first.getC(), *second.getA()) || almost_equal(*first.getC(), *second.getB()) ||
            almost_equal(*first.getC(), *second.getC()));
}
