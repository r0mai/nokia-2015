#pragma once

#include <cmath>

namespace calmare {

template<class T>
struct PositionBase {
    T x = 0;
    T y = 0;
    PositionBase() = default;
    PositionBase(T x, T y) : x(x), y(y) { }

    template<class U>
    PositionBase(const PositionBase<U>& p) : x(p.x), y(p.y) {}
};

using Position = PositionBase<int>;
using Positionf = PositionBase<double>;


template<class T>
T lengthSquared(const PositionBase<T>& p1, const PositionBase<T>& p2) {
    auto dx = p1.x - p2.x;
    auto dy = p1.y - p2.y;
    return dx*dx + dy*dy;
}

template<class T>
double length(const PositionBase<T>& p1, const PositionBase<T>& p2) {
    return std::sqrt(double(lengthSquared(p1, p2)));
}

template<class T>
T dot(const PositionBase<T>& p1, const PositionBase<T>& p2) {
    return p1.x*p2.x + p1.y*p2.y;
}

inline
Position operator-(const Position& p1, const Position& p2) {
    return Position{p1.x - p2.x, p1.y - p2.y};
}

inline
Position operator+(const Position& p1, const Position& p2) {
    return Position{p1.x + p2.x, p1.y + p2.y};
}

inline
Positionf operator+(const Position& p1, const Positionf& p2) {
    return Positionf{p1.x + p2.x, p1.y + p2.y};
}

inline
Positionf operator*(double f, const Position& p1) {
    return Positionf{p1.x * f, p1.y * f};
}

}
