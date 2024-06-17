//
// Created by leon on 24.04.24.
//

#include "triangle.h"

template<typename T>
Triangle<T>::Triangle(VertexType &first, VertexType &second,
                      VertexType &third) : a(&first), b(&second), c(&third) {}

template<typename T>
bool Triangle<T>::containsVertex(const VertexType &vertex) const {
    return almost_equal(*a, vertex) || almost_equal(*b, vertex) || almost_equal(*c, vertex);
}

template<typename T>
bool Triangle<T>::circumCircleContains(const VertexType &vertex) const {
    const T ab = a->norm2();
    const T cd = b->norm2();
    const T ef = c->norm2();

    const T ax = a->getX();
    const T ay = a->getY();
    const T bx = b->getX();
    const T by = b->getY();
    const T cx = c->getX();
    const T cy = c->getY();

    const T circum_x =
            (ab * (cy - by) + cd * (ay - cy) + ef * (by - ay)) / (ax * (cy - by) + bx * (ay - cy) + cx * (by - ay));
    const T circum_y =
            (ab * (cx - bx) + cd * (ax - cx) + ef * (bx - ax)) / (ay * (cx - bx) + by * (ax - cx) + cy * (bx - ax));

    const VertexType circum(circum_x / 2, circum_y / 2);

    const T circum_radius = a->dist2(circum);
    const T dist = vertex.dist2(circum);
    return dist <= circum_radius;
}

template<typename T>
bool Triangle<T>::operator==(const Triangle <T> &other) const {
    return (*this->a == *other.a || *this->a == *other.b || *this->a == *other.c) &&
           (*this->b == *other.a || *this->b == *other.b || *this->b == *other.c) &&
           (*this->c == *other.a || *this->c == *other.b || *this->c == *other.c);
}

template<typename U>
std::ostream &operator<<(std::ostream &str, const Triangle <U> &triangle) {
    return str << "Triangle: " << "\n\t" <<
               *triangle.a << "\n\t" <<
               *triangle.b << "\n\t" <<
               *triangle.c << "\n";
}

template
class Triangle<float>;

template
class Triangle<double>;
