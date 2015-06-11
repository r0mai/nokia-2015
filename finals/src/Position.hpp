#pragma once

namespace calmare {

struct Position {
    int x = 0;
    int y = 0;
    Position() = default;
    Position(int x, int y) : x(x), y(y) { }
};

struct Positionf {
    double x = 0.;
    double y = 0.;
    Positionf() = default;
    Positionf(Position p) : x(p.x), y(p.y) {}
    Positionf(double x, double y) : x(x), y(y) {}
};

inline
int lengthSquared(const Position& p1, const Position& p2) {
    auto dx = p1.x - p2.x;
    auto dy = p1.y - p2.y;
    return dx*dx + dy*dy;
}

inline
double lengthSquared(const Positionf& p1, const Positionf& p2) {
    auto dx = p1.x - p2.x;
    auto dy = p1.y - p2.y;
    return dx*dx + dy*dy;
}

inline
double length(const Position& p1, const Position& p2) {
    return std::sqrt(double(lengthSquared(p1, p2)));
}

inline
double length(const Positionf& p1, const Positionf& p2) {
    return std::sqrt(double(lengthSquared(p1, p2)));
}

inline
int dot(const Position& p1, const Position& p2) {
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
