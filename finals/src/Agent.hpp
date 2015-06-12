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
        DefendBorders,
        AttackShit
    } current_strategy = Strategy::GetFood;

    // same format as how negyed returns
    int attackTarget = -1;

    // returns true, if strategy changes
    bool getFoodStrategy();
    bool getWoodStrategy();
    bool getIronStrategy();
    bool goForLoterStrategy();
    bool exploreBoundariesStrategy();
    bool defendBordersStrategy();
    bool attackShit();

    void logFeedback();

    Mezo buildingTypeForUnit(Egyseg e);
    Position getClosestBuildingSite() const;

    bool buildBuildingIfPossible(Mezo m, const Position& position);
    bool buildBuildingIfNotAlreadyPresent(Mezo m, const Position& position);
    bool makeUnitIfPossible(Egyseg e);
    void getStuff(Mezo mezo);

    bool researchBuildingDefence();
    bool researchCavalry();
    bool conductBasicResearchTillReachQuantity(short q);
    bool conductBasicResearchTillReachQuantityWithGold(short q);
    bool researchResearch();

    void handleFreeWorkers();

    void createWorkersForTargetCount(int targetCount);

    int getBuildingIndex(Mezo m) const;
    std::size_t getNumberOfBuildings(Mezo m) const;
    short getBuildingId(Mezo m);

    /* +--->x
     * |0 1
     * |2 3
     * y
     */

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
    std::vector<int> getArchers() const;
    std::size_t getNumberOfUnitsProducingWare(Akcio akcio) const;
    std::vector<int> getUnitsProducingWare(Akcio akcio) const;

    void reAllocateWorkers(float food, float wood, float iron, float gold);

    bool isPieceTime() const;

    void attackOpponentUnit(int ourIndex, int opponentIndex);
    void attackOpponentBuilding(int ourIndex, int opponentIndex);

    std::vector<Position> findBuildablePositions() const;
    bool isBuildablePosition(const Position& position) const;
    std::vector<Position> getBoundaryPositions() const;
    std::vector<Position> getBoundaryPositionsMezo() const;

    bool isPointInNegyed(const Position& p, int negyed);
    Position getPointInNegyed(int negyed);
    Position getDiscoveredPointTowards(const Position& to);

    Position getPointTowardsMiddle() const;
    Position getPointTowardsSide1() const;
    Position getPointTowardsSide2() const;

    int getOpponentUnitIndexNear(const Position& p, int radius);
    int getOpponentBuildingIndexNear(const Position& p, int radius);

    Position getPositionNearOpponentIfAny(const Position& p, int radius);
};

}
