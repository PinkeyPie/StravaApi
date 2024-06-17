//
// Created by leon on 24.04.24.
//

#include "edge.h"

template<typename T>
Edge<T>::Edge(VertexType &first, VertexType &second) : v(&first), w(&second) {}

template<typename T>
bool Edge<T>::operator==(const Edge<T> &edge) const {
    return (*(this->v) == *edge.v && *(this->w) == *edge.w) ||
           (*(this->v) == *edge.w && *(this->w) == *edge.v);
}

template<typename U>
std::ostream &operator<<(std::ostream &os, const Edge<U> &edge) {
    return os << "Edge" << *edge.v << " , " << *edge.w;
}

template<typename T>
T Edge<T>::getCost() const {
    return std::sqrt(std::pow(v->getX() - v->getY(), 2) + std::pow(w->getX() - w->getY(), 2));
}

template
class Edge<float>;

template
class Edge<double>;
