#pragma once

#include "VilagDef.h"
#include <random>
#include <iostream>
#include "Log.hpp"
#include "Resources.hpp"

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
    auto dy = p2.y - p2.y;
    return dx*dx + dy*dy;
}

inline
double lengthSquared(const Positionf& p1, const Positionf& p2) {
    auto dx = p1.x - p2.x;
    auto dy = p2.y - p2.y;
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

class Agent {
public:
    TKoteg getOrders(TJatekos jatekos);

    void logMap(const TJatekos& jatekos);
    TJatekos jatekos;
    Agent() : gen(rnd()) {
    }

private:
    std::random_device rnd;
    mutable std::mt19937 gen;
    enum class Strategy {
        GetFood,
        GetWood,
        GetIron,
        GoForLoter,
        ExploreBoundaries,
        DefendBorders
    } current_strategy = Strategy::GetFood;

    // returns true, if strategy changes
    bool getFoodStrategy();
    bool getWoodStrategy();
    bool getIronStrategy();
    bool goForLoterStrategy();
    bool exploreBoundariesStrategy();
    bool defendBordersStrategy();

    void logFeedback();

    Mezo buildingTypeForUnit(Egyseg e);

    bool buildBuildingIfPossible(Mezo m, const Position& position);
    bool makeUnitIfPossible(Egyseg e);
    void getStuff(Mezo mezo);

    void handleFreeWorkers();

    int getBuildingIndex(Mezo m) const;
    short getBuildingId(Mezo m);
    int negyed() const;
    int getUnitCount(Egyseg e);

    bool isAvailableForMovement(Position cell) const;
    bool isValidPosition(Position p) const; // benne vagyunk e a mezobe
    bool areControlPointsVisible() const;

    std::pair<Position, Position> getMainDiagonal() const;

    std::size_t unitsOnCell(Position position) const;
    Position getExplorationPosition(Position near) const;
    Position getLocationOfResourceNearBy(Mezo mezo, Position near) const;
    std::vector<int> getFreeWorkers() const;
    std::vector<int> getFreeArchers() const;
    std::size_t getNumberOfUnitsProducingWare(Akcio akcio) const;

    bool isPieceTime() const;

    std::vector<Position> findBuildablePositions() const;
    bool isBuildablePosition(const Position& position) const;
    std::vector<Position> getBoundaryPositions() const;


    Position getPointTowardsMiddle() const;
    Position getPointTowardsSide1() const;
    Position getPointTowardsSide2() const;
};

}
