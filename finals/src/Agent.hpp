#pragma once

#include "VilagDef.h"
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

class Agent {
public:
    TKoteg getOrders(TJatekos jatekos);

    void logMap(const TJatekos& jatekos);
    TJatekos jatekos;
private:
    enum class Strategy {
        GetFood,
        GetWood,
        GetIron,
        GoForLoter,
        DefendBorders
    } current_strategy = Strategy::GetFood;

    // returns true, if strategy changes
    bool getFoodStrategy();
    bool getWoodStrategy();
    bool getIronStrategy();
    bool goForLoterStrategy();
    bool defendBordersStrategy();

    void logFeedback();

    Mezo buildingTypeForUnit(Egyseg e);

    bool buildBuildingIfPossible(Mezo m, const Position& position);
    bool makeUnitIfPossible(Egyseg e);
    void getStuff(Mezo mezo);

    int getBuildingIndex(Mezo m) const;
    short getBuildingId(Mezo m);
    int negyed() const;
    int getUnitCount(Egyseg e);

    bool isAvailableForMovement(Position cell) const;
    bool isValidPosition(Position p) const; // benne vagyunk e a mezobe

    std::pair<Position, Position> getMainDiagonal() const;

    std::size_t unitsOnCell(Position position) const;
    Position getLocationOfResourceNearBy(Mezo mezo, Position near) const;
    std::vector<int> getFreeWorkers() const;
    std::size_t getNumberOfUnitsProducingWare(Akcio akcio) const;

    bool isPieceTime() const;

    std::vector<Position> findBuildablePositions() const;
    bool isBuildablePosition(const Position& position) const;
    std::vector<Position> getBoundaryPositions() const;
};

}
