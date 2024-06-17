//
// Created by leon on 24.04.24.
//

#include "vector2.h"


template<typename T>
Vector2<T>::Vector2(const T vx, const T vy, unsigned int n) : x(vx), y(vy), num(n) {}

template<typename T>
T Vector2<T>::dist2(const Vector2<T> &other) const {
    const T dx = x - other.x;
    const T dy = y - other.y;
    return dx * dx + dy * dy;
}

template<>
float Vector2<float>::dist(const Vector2<float> &other) const {
    return hypotf(x - other.x, y - other.y);
}

template<>
double Vector2<double>::dist(const Vector2<double> &other) const {
    return hypot(x - other.x, y - other.y);
}


template<typename T>
T Vector2<T>::norm2() const {
    return x * x + y * y;
}

template<typename T>
bool Vector2<T>::operator==(const Vector2<T> &other) const {
    return (this->x == other.x) && (this->y == other.y);
}

template<typename U>
std::ostream &operator<<(std::ostream &str, const Vector2<U> &vector) {
    return str << "Point x:" << vector.x << " y: " << vector.y;
}

template
class Vector2<float>;

template
class Vector2<double>;
