#pragma once

#include "VilagDef.h"
#include <random>
#include <iostream>
#include "Log.hpp"
#include "Resources.hpp"
#include "Position.hpp"

namespace calmare {

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
    std::size_t getNumberOfBuildings(Mezo m) const;
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
    std::vector<int> getUnitsProducingWare(Akcio akcio) const;

    void reAllocateWorkers(float food, float wood, float iron);

    bool isPieceTime() const;

    std::vector<Position> findBuildablePositions() const;
    bool isBuildablePosition(const Position& position) const;
    std::vector<Position> getBoundaryPositions() const;


    Position getPointTowardsMiddle() const;
    Position getPointTowardsSide1() const;
    Position getPointTowardsSide2() const;
};

}
