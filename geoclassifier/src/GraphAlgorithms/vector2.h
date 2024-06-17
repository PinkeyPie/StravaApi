#pragma once

#include "numeric.h"
#include "iostream"
#include "cmath"
#include "type_traits"


template<typename T>
class Vector2 {
public:
    using Type = T;

    Vector2() = default;

    Vector2(const Vector2<T> &other) = default;

    Vector2(Vector2<T> &&other) = default;

    Vector2(const T vx, const T vy, unsigned int n = 0);

    T dist2(const Vector2<T> &other) const;

    T dist(const Vector2<T> &other) const;

    T norm2() const;

    Vector2 &operator=(const Vector2<T> &) = default;

    Vector2 &operator=(Vector2<T> &&) = default;

    bool operator==(const Vector2<T> &other) const;

    template<class U>
    friend std::ostream &operator<<(std::ostream &str, const Vector2<U> &vector);

    T getX() const { return x; }

    T getY() const { return y; }

    unsigned int getNum() const { return num; }

    void setX(T value) { x = value; }

    void setY(T value) { y = value; }

    void setNum(unsigned int val) { num = val; }

private:
    T x;
    T y;
    unsigned int num;
};

template<typename T>
bool almost_equal(const Vector2<T> &first, const Vector2<T> &second) {
    return almost_equal(first.getX(), second.getX()) && almost_equal(first.getY(), second.getY());
}