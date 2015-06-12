#include "Agent.hpp"
#include "Utasitas.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace calmare;

void eraseOrders() {
    log("Erasing orders");
    koteg.UtasitSzam = 0;
}

std::size_t distanceBetween(Position position1, Position position2) {
    return std::max(std::abs(position1.x - position2.x),
            std::abs(position1.y - position2.y));
}

std::size_t Agent::unitsOnCell(Position position) const {
    std::size_t count = 0;
    for(int i=0; i<jatekos.EgySzam; ++i) {
        const int x = jatekos.Egysegek[i].X;
        const int y = jatekos.Egysegek[i].Y;
        if(x == position.x && y == position.y) {
            ++count;
        }
    }
    return count;
}

bool Agent::isAvailableForMovement(Position cell) const {
    if (isValidPosition(cell)) {
        const auto worldCell = jatekos.Vilag[cell.y][cell.x].Objektum;
        if(worldCell == cvMezo || worldCell == cvKaja || worldCell == cvFa || worldCell == cvVasBanya || worldCell == cvAranyBanya) {
            return true;
        }
    }
    return false;
}

Position Agent::getExplorationPosition(Position near) const {
    auto poss = getBoundaryPositions();
    decltype(poss) nearposs;

    int dist = jatekos.XMax;

    for (unsigned i = 0; i < poss.size(); ++i) {
        int d2 = distanceBetween(near, poss[i]);
        if (d2 < dist && jatekos.Vilag[poss[i].y][poss[i].x].Objektum == cvMezo) {
            dist = d2;
            nearposs.clear();
            nearposs.push_back(poss[i]);
        }
        else if (d2 == dist && jatekos.Vilag[poss[i].y][poss[i].x].Objektum == cvMezo){
            nearposs.push_back(poss[i]);
        }
    }
    if (nearposs.size()) {
        std::uniform_int_distribution<int> uidd(0, nearposs.size() - 1);
        for (;;) {
            const int ind = uidd(gen);
            auto destination = nearposs[ind];
            return destination;
        }
    }
    return{ -1, -1 };
}

Position Agent::getLocationOfResourceNearBy(Mezo mezo, Position near) const {
    std::uniform_int_distribution<int> uid(-jatekos.Kepesseg.LatEgy / 1.414,
                                           jatekos.Kepesseg.LatEgy / 1.414);
    auto bound = getMainDiagonal();
    std::vector<Position> positions;
    for (int x = bound.first.x; x < bound.second.x; ++x) {
        for (int y = bound.first.y; y < bound.second.y; ++y) {
            if (jatekos.Vilag[y][x].Objektum == mezo &&
                unitsOnCell(Position{ x, y }) < (mezo == cvFa ? 2U : 4U) ) {
                positions.push_back(Position{x, y});
            }
        }
    }
    const auto it =
        std::min_element(positions.begin(), positions.end(),
                         [&near](const Position& p1, const Position& p2) {
            return distanceBetween(p1, near) < distanceBetween(p2, near);
        });
    if (it == positions.end()) {
        log("Didnt find any resource");
        return getExplorationPosition(near);
    }
    return *it;
}

void unitTo(Viselkedes v, Position position, const TEgyseg& unit) {
    const auto x = position.x;
    const auto y = position.y;
    switch (v)
    {
    case cviTermel : 
        if (unit.Viselkedes != cviTermel || unit.CelX != x || unit.CelY != y) {
            log("Unit %d was idle, sending (viselkedes %d) to %d, %d from %d, %d", int(unit.ID), int(v), x, y, unit.X, unit.Y);
            Utasit_Termel(unit.ID, x, y);
        }
        break;
    case cviMozog :
        if ((unit.Viselkedes != cviMozog || unit.CelX != x || unit.CelY != y) && (unit.X != x && unit.Y != y)) {
            log("Unit %d was idle, sending (viselkedes %d) to %d, %d from %d, %d", int(unit.ID), int(v), x, y, unit.X, unit.Y);
            Utasit_Mozog(unit.ID, x, y);
        }
        break;
    case cviJaror :
        if (unit.Viselkedes != cviJaror || unit.CelX != x || unit.CelY != y) {
            log("Unit %d was idle, sending (viselkedes %d) to %d, %d from %d, %d", int(unit.ID), int(v), x, y, unit.X, unit.Y);
            Utasit_Jaror(unit.ID, x, y);
        }
        break;
    default:
        assert(false);
    }
}

std::vector<int> Agent::getFreeArchers() const {
    std::vector<int> archers;
    for(int i=0; i<jatekos.EgySzam; ++i) {
        if(jatekos.Egysegek[i].Tipus == ceIjasz && jatekos.Egysegek[i].AkcioKod == caNincs) {
            archers.push_back(i);
        }
    }
    return archers;
}

std::vector<int> Agent::getArchers() const {
    std::vector<int> archers;
    for (int i=0; i<jatekos.EgySzam; ++i) {
        if (jatekos.Egysegek[i].Tipus == ceIjasz || jatekos.Egysegek[i].Tipus == ceLovas) {
            archers.push_back(i);
        }
    }
    return archers;
}

std::vector<int> Agent::getFreeWorkers() const {
    std::vector<int> workers;
    for (int i = 0; i < jatekos.EgySzam; ++i) {
        if (jatekos.Egysegek[i].Tipus == ceParaszt && jatekos.Egysegek[i].AkcioKod == caNincs) {
            workers.push_back(i);
        }
    }
    return workers;
}

void Agent::reAllocateWorkers(float food, float wood, float iron, float gold) {
    const auto numberOfWorkers = getUnitCount(ceParaszt);
    const int neededForFood = numberOfWorkers * food;
    const int neededForWood = numberOfWorkers * wood;
    const int neededForIron = numberOfWorkers * iron;
    const int neededForGold = numberOfWorkers * gold;

    auto actualFood = getUnitsProducingWare(caKaja);
    auto actualWood = getUnitsProducingWare(caFa);
    auto actualIron = getUnitsProducingWare(caVas);
    auto actualGold = getUnitsProducingWare(caArany);

    const int foodDeficit = neededForFood - actualFood.size();
    const int woodDeficit = neededForWood - actualWood.size();
    const int ironDeficit = neededForIron - actualIron.size();
    const int goldDeficit = neededForGold - actualGold.size();

    std::vector<int> surplusWorkers = getFreeWorkers();

    for(int i=0; i<-foodDeficit;++i) {
        surplusWorkers.push_back(actualFood[i]);
    }

    for(int i=0; i<-woodDeficit;++i) {
        surplusWorkers.push_back(actualWood[i]);
    }

    for(int i=0; i<-ironDeficit;++i) {
        surplusWorkers.push_back(actualIron[i]);
    }

    for(int i=0; i<-goldDeficit;++i) {
        surplusWorkers.push_back(actualGold[i]);
    }

    // ------ //

    auto getPos = [&](int p) {
        Position pos{jatekos.Egysegek[p].X, jatekos.Egysegek[p].X};
        return pos;
    };

    for (int i = 0; i < foodDeficit && !surplusWorkers.empty(); ++i) {
        Position resource = getLocationOfResourceNearBy(
                cvKaja, getPos(surplusWorkers.back()));
        unitTo(cviTermel, resource, jatekos.Egysegek[surplusWorkers.back()]);
        surplusWorkers.pop_back();
    }

    for (int i = 0; i < woodDeficit && !surplusWorkers.empty(); ++i) {
        Position resource = getLocationOfResourceNearBy(
                cvFa, getPos(surplusWorkers.back()));
        unitTo(cviTermel, resource, jatekos.Egysegek[surplusWorkers.back()]);
        surplusWorkers.pop_back();
    }

    for (int i = 0; i < ironDeficit && !surplusWorkers.empty(); ++i) {
        Position resource = getLocationOfResourceNearBy(
                cvVasBanya, getPos(surplusWorkers.back()));
        unitTo(cviTermel, resource, jatekos.Egysegek[surplusWorkers.back()]);
        surplusWorkers.pop_back();
    }

    for (int i = 0; i < goldDeficit && !surplusWorkers.empty(); ++i) {
        Position resource = getLocationOfResourceNearBy(
                cvAranyBanya, getPos(surplusWorkers.back()));
        unitTo(cviTermel, resource, jatekos.Egysegek[surplusWorkers.back()]);
        surplusWorkers.pop_back();
    }
}

bool Agent::buildBuildingIfPossible(Mezo m, const Position& position) {
    auto our = Resources::fromJatekos(jatekos);
    auto cost = Cost::Building(m);

    if (!(our - cost)) {
        return false;
    }

    if (!isBuildablePosition(position)) {
        return false;
    }

    Utasit_Epit(m, position.x, position.y);
    jatekos.Eroforras.Kaja -= cost.food();
    jatekos.Eroforras.Fa -= cost.wood();
    jatekos.Eroforras.Vas -= cost.iron();
    jatekos.Eroforras.Arany -= cost.gold();
    return true;
}

bool Agent::buildBuildingIfNotAlreadyPresent(Mezo m, const Position& position) {
    if(getNumberOfBuildings(m) == 1) {
        return false;
    }
    return buildBuildingIfPossible(m, position);
}

bool Agent::makeUnitIfPossible(Egyseg e) {
    auto our = Resources::fromJatekos(jatekos);
    auto cost = Cost::Unit(e);

    if (!(our - cost)) {
        return false;
    }

    auto buildingType = buildingTypeForUnit(e);
    int epuletIndex = -1;
    for (int i = 0; i < jatekos.EpSzam; ++i) {
        if (jatekos.Epuletek[i].Tipus == buildingType &&
            jatekos.Epuletek[i].AkcioKod == caNincs)
        {
            epuletIndex = i;
            break;
        }
    }
    if (epuletIndex < 0) {
        return false;
    }

    if(jatekos.EgySzam == jatekos.Kepesseg.PopLimit) {
        return false;
    }

    Utasit_Kepez(e, jatekos.Epuletek[epuletIndex].ID);
    jatekos.Epuletek[epuletIndex].AkcioKod = cuKepzes;
    jatekos.Eroforras.Kaja -= cost.food();
    jatekos.Eroforras.Fa -= cost.wood();
    jatekos.Eroforras.Vas -= cost.iron();
    jatekos.Eroforras.Arany -= cost.gold();
    return true;
}

bool Agent::researchBuildingDefence() {
    const int currentLevel = jatekos.Kepesseg.Szintek[cfEpVed];
    log("currentLevel for building defence is %d", currentLevel);
    auto our = Resources::fromJatekos(jatekos);
    auto cost = Cost::Epulet_Vedelem(currentLevel + 1);

    if (!(our - cost)) {
        return false;
    }

    if (jatekos.Epuletek[getBuildingIndex(cvAkademia)].AkcioKod != caNincs) {
        log("Cannot research at this time, research is already under way");
        return false;
    }

    Utasit_Fejleszt(cfEpVed);
    jatekos.Epuletek[getBuildingIndex(cvAkademia)].AkcioKod = caVar;
    jatekos.Eroforras.Kaja -= cost.food();
    jatekos.Eroforras.Fa -= cost.wood();
    jatekos.Eroforras.Vas -= cost.iron();
    jatekos.Eroforras.Arany -= cost.gold();
    return true;
}

bool Agent::researchArchery() {
    const int currentLevel = jatekos.Kepesseg.Szintek[cfIjasz];
    log("currentLevel for archery is %d", currentLevel);
    auto our = Resources::fromJatekos(jatekos);
    auto cost = Cost::Ijasz_F(currentLevel + 1);

    if (!(our - cost)) {
        return false;
    }

    if (jatekos.Epuletek[getBuildingIndex(cvAkademia)].AkcioKod != caNincs) {
        log("Cannot research at this time, research is already under way");
        return false;
    }

    Utasit_Fejleszt(cfIjasz);
    jatekos.Epuletek[getBuildingIndex(cvAkademia)].AkcioKod = caVar;
    jatekos.Eroforras.Kaja -= cost.food();
    jatekos.Eroforras.Fa -= cost.wood();
    jatekos.Eroforras.Vas -= cost.iron();
    jatekos.Eroforras.Arany -= cost.gold();
    return true;
}

bool Agent::researchCavalry() {
    const int currentLevel = jatekos.Kepesseg.Szintek[cfLovas];
    log("currentLevel for cavalry is %d", currentLevel);
    auto our = Resources::fromJatekos(jatekos);
    auto cost = Cost::Lovas_F(currentLevel + 1);

    if (!(our - cost)) {
        return false;
    }

    if (jatekos.Epuletek[getBuildingIndex(cvAkademia)].AkcioKod != caNincs) {
        log("Cannot research at this time, research is already under way");
        return false;
    }

    Utasit_Fejleszt(cfLovas);
    jatekos.Epuletek[getBuildingIndex(cvAkademia)].AkcioKod = caVar;
    jatekos.Eroforras.Kaja -= cost.food();
    jatekos.Eroforras.Fa -= cost.wood();
    jatekos.Eroforras.Vas -= cost.iron();
    jatekos.Eroforras.Arany -= cost.gold();
    return true;
}

bool Agent::conductBasicResearchTillReachQuantity(short q) {
    if (q <= jatekos.Kepesseg.KP) {
        log("We already have enough KP, not creating any more");
        return false;
    }

    auto our = Resources::fromJatekos(jatekos);
    auto cost = Cost(Resources::iron(10), 400);

    if (!(our - cost)) {
        log("We don't have enough resources to create a KP");
        return false;
    }

    if (jatekos.Epuletek[getBuildingIndex(cvAkademia)].AkcioKod != caNincs) {
        log("Cannot research at this time, research is already under way");
        return false;
    }

    Utasit_Kutat(caVas, 10);
    jatekos.Epuletek[getBuildingIndex(cvAkademia)].AkcioKod = caVar;
    log("Ordered research");
    jatekos.Eroforras.Vas -= cost.iron();
    return true;
}

bool Agent::conductBasicResearchTillReachQuantityWithGold(short q) {
    if (q <= jatekos.Kepesseg.KP) {
        log("We already have enough KP, not creating any more");
        return false;
    }

    auto our = Resources::fromJatekos(jatekos);
    auto cost = Cost(Resources::gold(4), 400);

    if (!(our - cost)) {
        log("We don't have enough resources to create a KP");
        return false;
    }

    if (jatekos.Epuletek[getBuildingIndex(cvAkademia)].AkcioKod != caNincs) {
        log("Cannot research at this time, research is already under way");
        return false;
    }

    Utasit_Kutat(caArany, 4);
    log("Ordered research");
    jatekos.Epuletek[getBuildingIndex(cvAkademia)].AkcioKod = caVar;
    jatekos.Eroforras.Arany -= cost.gold();
    return true;
}

bool Agent::researchResearch() {
    const int currentLevel = jatekos.Kepesseg.Szintek[cfKutGyors];
    log("currentLevel for research is %d", currentLevel);
    auto our = Resources::fromJatekos(jatekos);
    auto cost = Cost::Kutatas_Gyorsitas(currentLevel + 1);

    if (!(our - cost)) {
        return false;
    }

    if (jatekos.Epuletek[getBuildingIndex(cvAkademia)].AkcioKod != caNincs) {
        log("Cannot research at this time, research is already under way");
        return false;
    }

    Utasit_Fejleszt(cfKutGyors);
    jatekos.Epuletek[getBuildingIndex(cvAkademia)].AkcioKod = caVar;
    jatekos.Eroforras.Kaja -= cost.food();
    jatekos.Eroforras.Fa -= cost.wood();
    jatekos.Eroforras.Vas -= cost.iron();
    jatekos.Eroforras.Arany -= cost.gold();
    return true;
}

Mezo Agent::buildingTypeForUnit(Egyseg e) {
    switch (e) {
        case ceParaszt:
            return cvFohaz;
        case ceKardos: case cePuskas:
            return cvLaktanya;
        case ceIjasz:
            return cvLoter;
        case ceLovas:
            return cvIstallo;
    }
    return Mezo(-1);
}

int Agent::getBuildingIndex(Mezo m) const {
    for (int i = 0; i < jatekos.EpSzam; ++i) {
        if (jatekos.Epuletek[i].Tipus == m) {
            return i;
        }
    }
    return -1;
}

std::size_t Agent::getNumberOfBuildings(Mezo m) const {
    std::size_t count = 0;
    for(int i=0; i<jatekos.EpSzam; ++i) {
        if(jatekos.Epuletek[i].Tipus == m) {
            ++count;
        }
    }
    return count;
}

short Agent::getBuildingId(Mezo m) {
    for (int i = 0; i < jatekos.EpSzam; ++i) {
        if (jatekos.Epuletek[i].Tipus == m) {
            return jatekos.Epuletek[i].ID;
        }
    }
    return -1;
}

Position Agent::getClosestBuildingSite() const {
    const auto buildingSites = findBuildablePositions();
    if (!buildingSites.empty()) {
        const auto baseIndex = getBuildingIndex(cvFohaz);
        const auto base = jatekos.Epuletek[baseIndex];
        const auto basePos = Position{base.X, base.Y};
        const auto buildingSite =
                *std::min_element(buildingSites.begin(), buildingSites.end(),
                                  [&basePos](const Position& p1,
                                             const Position& p2) {
                    return distanceBetween(p1, basePos) <
                           distanceBetween(p2, basePos);
                });
        return buildingSite;
    } else {
        return {};
    }
}

int Agent::negyed() const {
    int fohazEpuletIndex = getBuildingIndex(cvFohaz);
    if (fohazEpuletIndex != -1) {
        return (jatekos.Epuletek[fohazEpuletIndex].X > jatekos.XMax / 2) | ((jatekos.Epuletek[fohazEpuletIndex].Y > jatekos.YMax / 2) << 1);
    }
    else {
        return 0;
    }
}

int Agent::getUnitCount(Egyseg e) {
    int c = 0;
    for (int i = 0; i < jatekos.EgySzam; ++i) {
        if (jatekos.Egysegek[i].Tipus == e) {
            ++c;
        }
    }
    return c;
}

std::size_t Agent::getNumberOfUnitsProducingWare(Akcio akcio) const {
    std::size_t count = 0;
    for (int i = 0; i < jatekos.EgySzam; ++i) {
        const auto unit = jatekos.Egysegek[i];
        if (unit.AkcioKod == akcio) {
            ++count;
        }
    }
    return count;
}

std::vector<int> Agent::getUnitsProducingWare(Akcio akcio) const {
    std::vector<int> units;
    for (int i = 0; i < jatekos.EgySzam; ++i) {
        const auto unit = jatekos.Egysegek[i];
        if (unit.AkcioKod == akcio) {
            units.push_back(i);
        } else if ( unit.Viselkedes == cviTermel ) {
            const auto mezo = jatekos.Vilag[unit.CelY][unit.CelX];
            if(mezo.Objektum == static_cast<Mezo>(akcio)) {
                units.push_back(i);
            }
        }
    }
    return units;
}

void Agent::getStuff(Mezo mezo) {

    for (const auto& freeWorker : getFreeWorkers()) {
        Position ofMyOnlySon = Position{jatekos.Egysegek[freeWorker].X,
                                        jatekos.Egysegek[freeWorker].Y};
        Position food = getLocationOfResourceNearBy(mezo, ofMyOnlySon);
        std::cerr << "Found food at: " << food.x << " " << food.y << std::endl;
        unitTo(cviTermel, food, jatekos.Egysegek[freeWorker]);
    }

    while (makeUnitIfPossible(ceParaszt)) {}
}

void Agent::handleFreeWorkers() {
    for (const auto& freeWorker : getFreeWorkers()) {
        Position ofMyOnlySon = Position{jatekos.Egysegek[freeWorker].X,
                                        jatekos.Egysegek[freeWorker].Y};
        Position gold = getLocationOfResourceNearBy(cvAranyBanya, ofMyOnlySon);
        std::cerr << "Found gold at: " << gold.x << " " << gold.y << std::endl;
        unitTo(cviTermel, gold, jatekos.Egysegek[freeWorker]);
    }
}

void Agent::createWorkersForTargetCount(int targetCount) {
    int currentCount = getUnitCount(ceParaszt);

    for (int i = currentCount; i <= targetCount; ++i) {
        if (!makeUnitIfPossible(ceParaszt)) {
            break;
        }
    }
}

bool Agent::areControlPointsVisible() const {
    const auto middle = getPointTowardsMiddle();
    const auto side1 = getPointTowardsSide1();
    const auto side2 = getPointTowardsSide2();

    if (jatekos.Vilag[middle.y][middle.x].Objektum == cvNemLatszik) {
        return false;
    }
    if (jatekos.Vilag[side1.y][side1.x].Objektum == cvNemLatszik) {
        return false;
    }
    if (jatekos.Vilag[side2.y][side2.x].Objektum == cvNemLatszik) {
        return false;
    }
    return true;
}

bool Agent::getFoodStrategy() {
    log("getFood");
    if (getNumberOfUnitsProducingWare(caKaja) >= 8) {
        current_strategy = Strategy::GetWood;
        return true;
    }
    getStuff(cvKaja);
    return false;
}

bool Agent::getWoodStrategy() {
    log("getwood");
    if (getNumberOfUnitsProducingWare(caFa) >= 10) {
        current_strategy = Strategy::GetIron;
        return true;
    }
    getStuff(cvFa);
    return false;
}

bool Agent::getIronStrategy() {
    log("getiron");
    if (getNumberOfUnitsProducingWare(caVas) >= 4) {
        current_strategy = Strategy::GoForLoter;
        return true;
    }
    getStuff(cvVasBanya);
    return false;
}

bool Agent::goForLoterStrategy() {
    log("loter");
    if(getNumberOfBuildings(cvLoter) >= 2 && getNumberOfBuildings(cvAkademia)) {
        current_strategy = Strategy::ExploreBoundaries;
        return true;
    }

    createWorkersForTargetCount(30);
    reAllocateWorkers(0.3, 0.6, 0.1, 0.0);

    const auto buildingSite = getClosestBuildingSite();
    buildBuildingIfNotAlreadyPresent(cvAkademia, buildingSite);
    buildBuildingIfPossible(cvLoter, buildingSite);

    return false;
}

bool Agent::exploreBoundariesStrategy() {
    log("exploreBoundaries");
    bool seeAllTheSafePlaces = false;
    for (const auto& freeArcher : getFreeArchers()) {
        Position ofMyOnlySon = Position{jatekos.Egysegek[freeArcher].X,
                                        jatekos.Egysegek[freeArcher].Y};
        auto nextPos = getExplorationPosition(ofMyOnlySon);
        if (nextPos.x != -1) {
            unitTo(cviMozog, nextPos, jatekos.Egysegek[freeArcher]);
        }
        else {
            seeAllTheSafePlaces = true;
            auto pos = getLocationOfResourceNearBy(cvMezo, rand() % 2 ? getPointTowardsSide1() : getPointTowardsSide2());

            unitTo(cviMozog, pos, jatekos.Egysegek[freeArcher]);
        }

    }
    if (seeAllTheSafePlaces) {
        current_strategy = Strategy::DefendBorders;
        return true;
    }

    while (makeUnitIfPossible(ceLovas)) { }
    while (makeUnitIfPossible(ceIjasz)) {
    }
    const auto buildingSite = getClosestBuildingSite();
    while(buildBuildingIfNotAlreadyPresent(cvIstallo, buildingSite)) { }
    while(researchResearch()) { }
    while(conductBasicResearchTillReachQuantity(80)) { }

    return false;
}

bool Agent::defendBordersStrategy() {
    log("defendBorders");

    if (jatekos.Ido > jatekos.BekeIdo &&
        jatekos.EgySzam >= jatekos.Kepesseg.PopLimit - 5)
    {
        current_strategy = Strategy::AttackShit;
        return true;
    }

    // jarorozes!!
    for (const auto& freeArcher : getFreeArchers()) {
        Position ofMyOnlySon = Position{ jatekos.Egysegek[freeArcher].X,
            jatekos.Egysegek[freeArcher].Y };
        int distanceFromSide1 = distanceBetween(ofMyOnlySon, getPointTowardsSide1());
        int distanceFromSide2 = distanceBetween(ofMyOnlySon, getPointTowardsSide2());
        if (distanceFromSide1 > distanceFromSide2) {
            auto pos = getLocationOfResourceNearBy(cvMezo, getPointTowardsSide1());

            unitTo(cviJaror, pos, jatekos.Egysegek[freeArcher]);
        }
        else {
            auto pos = getLocationOfResourceNearBy(cvMezo, getPointTowardsSide2());
            unitTo(cviJaror, pos, jatekos.Egysegek[freeArcher]);
        }
    }
    reAllocateWorkers(0.2, 0.65, 0.15, 0.0);

    // Find tower locations closest to borders
    auto positions = findBuildablePositions();

    auto diagonal = std::make_pair(getPointTowardsSide1(), getPointTowardsSide2());

    auto distanceFromDiagonal = [&](const Position& p) {
        auto v = diagonal.first;
        auto w = diagonal.second;
        // Return minimum distance between line segment vw and point p
        const double l2 = lengthSquared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
        // Consider the line extending the segment, parameterized as v + t (w - v).
        // We find projection of point p onto the line.
        // It falls where t = [(p-v) . (w-v)] / |w-v|^2
        const double t = dot(p - v, w - v) / l2;
        if (t < 0.0) return length(p, v);       // Beyond the 'v' end of the segment
        else if (t > 1.0) return length(p, w);  // Beyond the 'w' end of the segment
        const Positionf projection = Positionf(v) + t * Positionf(w - v);
        return length(Positionf(p), projection);
    };

    std::sort(positions.begin(), positions.end(),
        [&](const Position& p1, const Position& p2) {
            return distanceFromDiagonal(p1) < distanceFromDiagonal(p2);
        }
    );

    log("diagonal for towers (%d, %d), (%d, %d)",
        diagonal.first.x,
        diagonal.first.y,
        diagonal.second.x,
        diagonal.second.y);

    for (const Position& p : positions) {
        if (distanceFromDiagonal(p) > 8.) {
            break;
        }

        if (!buildBuildingIfPossible(cvTorony, p)) {
            break;
        }
    }

    const auto buildingSite = getClosestBuildingSite();

    while (makeUnitIfPossible(ceLovas)) { }
    while (makeUnitIfPossible(ceIjasz)) {}
    while(researchBuildingDefence()) { }
    while(researchArchery()) { }
    while(researchCavalry()) { }
    while(buildBuildingIfNotAlreadyPresent(cvIstallo, buildingSite)) { }
    while(conductBasicResearchTillReachQuantity(80)) { }

    return false;
}

bool Agent::attackShit() {
    log("attackShit");

    if (jatekos.EgySzam <= jatekos.Kepesseg.PopLimit - 30) {
        current_strategy = Strategy::DefendBorders;
        return true;
    }

    reAllocateWorkers(0.1, 0.4, 0.3, 0.2);

    if (attackTarget < 0) {
        // decide where to attack
        attackTarget = (negyed() + 1) % 4; // TODO lets discuss this
        attackStartedOn = jatekos.Ido;
    }

    auto targetPosition = getPointInNegyed(attackTarget);
    log("targetPosition: (%d, %d), negyed %d",
        targetPosition.x, targetPosition.y, attackTarget);

    bool goNext = false;
    for (int index : getArchers()) {
        const auto& archer = jatekos.Egysegek[index];
        if (lengthSquared(
                Position{archer.X, archer.Y},
                targetPosition) <= 25)
        {
            goNext = true;
        }

        Position nearOpponent = getPositionNearOpponentIfAny(
            {archer.X, archer.Y}, 2);

        if (nearOpponent.x >= 0) {
            //unitTo(
             //   cviJaror,
             //   nearOpponent,
             //   archer);
            unitTo(cviMozog,
                {archer.X, archer.Y},
                archer);
        } else {
            if (archer.Viselkedes != cviMozog) {
                unitTo(
                    cviMozog,
                    getDiscoveredPointTowards(targetPosition),
                    archer);
            }
        }
    }

    if (goNext || jatekos.Ido - attackStartedOn > 10000) {
        attackTarget++;
        attackTarget %= 4;
        attackStartedOn = jatekos.Ido;
    }

    const auto buildingSite = getClosestBuildingSite();

    while (makeUnitIfPossible(ceLovas)) { }
    while(researchBuildingDefence()) { }
    while(researchArchery()) { }
    while(researchCavalry()) { }
    while(buildBuildingIfNotAlreadyPresent(cvIstallo, buildingSite)) { }
    while (conductBasicResearchTillReachQuantityWithGold(80)) { }

    return false;
}

void Agent::attackOpponentUnit(int ourIndex, int opponentIndex) {
    log("Tamad unit id(%d) -/-> id(%d)",
        jatekos.Egysegek[ourIndex].ID,
        jatekos.MasEgysegek[opponentIndex].ID);
    Utasit_Tamad(
        jatekos.Egysegek[ourIndex].ID,
        jatekos.MasEgysegek[opponentIndex].ID,
        jatekos.MasEgysegek[opponentIndex].Szin);
}

void Agent::attackOpponentBuilding(int ourIndex, int opponentIndex) {
    log("Tamad building id(%d) -/-> id(%d)",
        jatekos.Egysegek[ourIndex].ID,
        jatekos.MasEpuletek[opponentIndex].ID);
    Utasit_Tamad(
        jatekos.Egysegek[ourIndex].ID,
        jatekos.MasEpuletek[opponentIndex].ID,
        jatekos.Vilag[jatekos.MasEpuletek[opponentIndex].Y][jatekos.MasEpuletek[opponentIndex].X].Szin);
}

int Agent::getOpponentUnitIndexNear(const Position& p, int radius) {
    for (int i = 0; i < jatekos.MasEgySzam; ++i) {
        if (lengthSquared(
            p,
            Position{jatekos.MasEgysegek[i].X, jatekos.MasEgysegek[i].Y}) <=
            radius*radius)
        {
            return i;
        }
    }
    return -1;
}

int Agent::getOpponentBuildingIndexNear(const Position& p, int radius) {
    int w = jatekos.XMax;
    int h = jatekos.YMax;

    for (int i = 0; i < jatekos.MasEpSzam; ++i) {
        if (lengthSquared(
            p,
            Position{jatekos.MasEpuletek[i].X, jatekos.MasEpuletek[i].Y}) <=
            radius*radius)
        {
            return i;
        }
    }
    return -1;
}

Position Agent::getPositionNearOpponentIfAny(const Position& p, int radius) {
    int opUIdx = getOpponentUnitIndexNear(p, radius);
    if (opUIdx >= 0) {
        return getLocationOfResourceNearBy(cvMezo,
                {jatekos.MasEgysegek[opUIdx].X, jatekos.MasEgysegek[opUIdx].Y});
    }
    int opBIdx = getOpponentBuildingIndexNear(p, radius);
    if (opBIdx >= 0) {
        return getLocationOfResourceNearBy(cvMezo,
                {jatekos.MasEpuletek[opBIdx].X, jatekos.MasEpuletek[opBIdx].Y});
    }
    return Position{-1, -1};
}

void Agent::logFeedback() {
#ifndef CALMARE_DISABLE_LOG
    for (int i = 0; i < jatekos.UtasitSzam; ++i) {
        auto utasitas = jatekos.Utasitasok[i];
        if (utasitas.Result != chOk) {
            log("Warning: Got error code %d, command(kod=%d, kieg=%d, ID=%d, ID2=%d, X=%d, Y=%d)",
                int(utasitas.Result),
                int(utasitas.Kod),
                int(utasitas.Kieg),
                int(utasitas.ID),
                int(utasitas.ID2),
                int(utasitas.X),
                int(utasitas.Y));

            if (int(utasitas.Result) == 16)
            {
                Position complained = { int(utasitas.X), int(utasitas.Y) };
                if (isValidPosition(complained)) {
                    log("system complained about pos we think is good");
                }
                else {
                    log("we also thought it was invalid");
                }
            }
        }
    }
#endif
}

TKoteg Agent::getOrders(TJatekos jatekos) {
    logFeedback();
    eraseOrders();
    this->jatekos = jatekos;

    bool strategy_changes = false;
    do {
        switch (current_strategy) {
            case Strategy::GetFood:
                strategy_changes = getFoodStrategy();
                break;
            case Strategy::GetWood:
                strategy_changes = getWoodStrategy();
                break;
            case Strategy::GetIron:
                strategy_changes = getIronStrategy();
                break;
            case Strategy::GoForLoter:
                strategy_changes = goForLoterStrategy();
                break;
            case Strategy::ExploreBoundaries:
                strategy_changes = exploreBoundariesStrategy();
                break;
            case Strategy::DefendBorders:
                strategy_changes = defendBordersStrategy();
                break;
            case Strategy::AttackShit:
                strategy_changes = attackShit();
                break;
            default:
                strategy_changes = false;
                break;
        }
    } while (strategy_changes);

    return koteg;
}

char unitToChar(int type) {
    switch (type) {
        case ceKardos: return 'k';
        case ceParaszt: return 'p';
        case ceIjasz: return 'i';
        case ceLovas: return 'l';
        case cePuskas: return 'r'; // as in rifle
    }
    return '\\';
}

char fieldToChar(int type) {
    switch (type) {
        case cvMezo: return ' ';
        case cvKo: return 'o';
        case cvKaja: return 'K';
        case cvFa: return 'F';
        case cvVasBanya: return 'V';
        case cvAranyBanya: return 'G';
        case cvFohaz: return 'H';
        case cvLaktanya: return 'L';
        case cvLoter: return 'T';
        case cvIstallo: return 'I';
        case cvAkademia: return 'A';
        case cvKorhaz: return 'K';
        case cvTorony: return 'R';
        case cvFal: return 'W';
        case cvNemLatszik: return '?';
    }
    return '\\';
}
void Agent::logMap(const TJatekos& jatekos) {
#ifndef CALMARE_DISABLE_LOG
    std::vector<std::string> lines(jatekos.YMax, std::string(jatekos.XMax, ' '));
    for (int y = 0; y < jatekos.YMax; ++y) {
        for (int x = 0; x < jatekos.XMax; ++x) {
            char ch = fieldToChar(jatekos.Vilag[y][x].Objektum);
            lines[y][x] = ch;
        }
    }
    for (int i = 0; i < jatekos.EgySzam; ++i) {
        int x = jatekos.Egysegek[i].X;
        int y = jatekos.Egysegek[i].Y;
        char ch = unitToChar(jatekos.Egysegek[i].Tipus);
        lines[y][x] = ch;
    }
    for (const auto& line : lines) {
        log(line);
    }
#else
    (void)(jatekos);
#endif
}

bool Agent::isValidPosition(Position p) const {
    auto c = getMainDiagonal();
    return c.first.x <= p.x && p.x < c.second.x && c.first.y <= p.y && p.y < c.second.y;
}
std::pair<Position, Position> Agent::getMainDiagonal() const {
    if (isPieceTime())
    {
        switch (negyed())
        {
        case 0: return{ { 0, 0 }, { jatekos.XMax / 2 - 1, jatekos.YMax / 2 - 1 } };
        case 1: return{ { jatekos.XMax / 2, 0 }, { jatekos.XMax, jatekos.YMax / 2 - 1 } };
        case 2: return{ { 0, jatekos.YMax / 2 }, { jatekos.XMax / 2 - 1, jatekos.YMax } };
        case 3: return{ { jatekos.XMax / 2, jatekos.YMax / 2 }, { jatekos.XMax, jatekos.YMax } };
        }
    }
    return{ { 0, 0 }, { jatekos.XMax, jatekos.YMax } };
}

bool Agent::isPieceTime() const {
    return jatekos.Ido <= jatekos.BekeIdo;
}

std::vector<Position> Agent::getBoundaryPositions() const {
    std::vector<Position> res;

    auto c = getMainDiagonal();
    for (int i = c.first.x + 1; i < c.second.x - 1; ++i) {
        for (int j = c.first.y + 1; j < c.second.y - 1; ++j) {
            if (jatekos.Vilag[j][i].Objektum != cvNemLatszik) {
                if (jatekos.Vilag[j - 1][i].Objektum == cvNemLatszik || jatekos.Vilag[j + 1][i].Objektum == cvNemLatszik ||
                    jatekos.Vilag[j][i - 1].Objektum == cvNemLatszik || jatekos.Vilag[j][i + 1].Objektum == cvNemLatszik ||
                    jatekos.Vilag[j - 1][i - 1].Objektum == cvNemLatszik || jatekos.Vilag[j + 1][i + 1].Objektum == cvNemLatszik ||
                    jatekos.Vilag[j + 1][i - 1].Objektum == cvNemLatszik || jatekos.Vilag[j - 1][i + 1].Objektum == cvNemLatszik) {
                    res.push_back({ i, j });
                }
            }
        }
    }
    return res;
}

std::vector<Position> Agent::getBoundaryPositionsMezo() const {
    auto positions = getBoundaryPositions();
    std::vector<Position> result;
    for (const Position& p : positions) {
        if (jatekos.Vilag[p.y][p.x].Objektum == cvMezo) {
            result.push_back(p);
        }
    }
    return result;
}

bool Agent::isPointInNegyed(const Position& p, int negyed) {
    switch (negyed) {
        case 0: return p.x < jatekos.XMax / 2 && p.y < jatekos.XMax / 2;
        case 1: return p.x > jatekos.XMax / 2 && p.y < jatekos.XMax / 2;
        case 2: return p.x < jatekos.XMax / 2 && p.y > jatekos.XMax / 2;
        case 3: return p.x > jatekos.XMax / 2 && p.y > jatekos.XMax / 2;
    }
    return false;
}

Position Agent::getPointInNegyed(int negyed) {
    switch (negyed) {
        case 0: return Position{2, 2};
        case 1: return Position{97, 2};
        case 2: return Position{2, 97};
        case 3: return Position{97, 97};
    }
    return Position{0, 0};
}

Position Agent::getDiscoveredPointTowards(const Position& to) {
    // TODO what if to is discovered, but is not a mezo?
    if (jatekos.Vilag[to.y][to.x].Objektum == cvMezo) {
        return to;
    }
    if (jatekos.Vilag[to.y][to.x].Objektum != cvNemLatszik) {
        return getLocationOfResourceNearBy(cvMezo, to);
    }

    auto positions = getBoundaryPositionsMezo();
    if (positions.empty()) {
        return getLocationOfResourceNearBy(cvMezo, to);
    }
    std::sort(positions.begin(), positions.end(),
        [&](const Position& p1, const Position& p2) {
            return lengthSquared(p1, to) < lengthSquared(p2, to);
        }
    );

    return positions[rand() % std::min<int>(5, positions.size())];
}

std::vector<Position> Agent::findBuildablePositions() const {
    std::vector<Position> result;
    for (int x = 0; x < jatekos.XMax; ++x) {
        for (int y = 0; y < jatekos.YMax; ++y) {
            if (jatekos.Vilag[y][x].Objektum != cvMezo) {
                continue;
            }
            if (isBuildablePosition(Position{x, y})) {
                result.push_back(Position{x, y});
            }
        }
    }
    return result;
}

bool Agent::isBuildablePosition(const Position& position) const {
    if (position.x <= 0 || position.y <= 0 ||
        position.x >= jatekos.XMax-1 || position.y >= jatekos.YMax-1)
    {
        return false;
    }

    for (int x = position.x - 1; x <= position.x + 1; ++x) {
        for (int y = position.y - 1; y <= position.y + 1; ++y) {
            if (jatekos.Vilag[y][x].Objektum != cvMezo) {
                return false;
            }
        }
    }

    for (int i = 0; i < jatekos.EgySzam; ++i) {
        if (jatekos.Egysegek[i].X == position.x &&
            jatekos.Egysegek[i].Y == position.y)
        {
            return false;
        }
    }

    for (int i = 0; i < jatekos.MasEgySzam; ++i) {
        if (jatekos.MasEgysegek[i].X == position.x &&
            jatekos.MasEgysegek[i].Y == position.y)
        {
            return false;
        }
    }

    return true;
}

Position Agent::getPointTowardsMiddle() const {
    auto xMax = jatekos.XMax;
    auto yMax = jatekos.YMax;
    switch (negyed()) {
        case 0: return Position{xMax/2 - 1, yMax/2 - 1};
        case 1: return Position{xMax/2, yMax/2 - 1};
        case 2: return Position{xMax/2 - 1, yMax/2};
        case 3: return Position{xMax/2, yMax/2};
    }
    assert(false);
    return Position{};
}

Position Agent::getPointTowardsSide1() const {
    auto xMax = jatekos.XMax;
    auto yMax = jatekos.YMax;
    switch (negyed()) {
        case 0: return Position{xMax/2 - 1, 0};
        case 1: return Position{xMax - 1, yMax/2 - 1};
        case 2: return Position{0, yMax/2};
        case 3: return Position{xMax - 1, yMax/2};
    }
    assert(false);
    return Position{};
}

Position Agent::getPointTowardsSide2() const {
    auto xMax = jatekos.XMax;
    auto yMax = jatekos.YMax;
    switch (negyed()) {
        case 0: return Position{0, yMax/2 - 1};
        case 1: return Position{xMax/2, 0};
        case 2: return Position{xMax/2 - 1, yMax - 1};
        case 3: return Position{xMax/2, yMax - 1};
    }
    assert(false);
    return Position{};
}
