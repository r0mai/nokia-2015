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
    std::uniform_int_distribution<int> uidd(0, nearposs.size() - 1);
    for (;;) {
        const int ind = uidd(gen);
        auto destination = nearposs[ind];
        return destination;
    }
}

Position Agent::getLocationOfResourceNearBy(Mezo mezo, Position near) const {
    std::uniform_int_distribution<int> uid(-jatekos.Kepesseg.LatEgy / 1.414,
                                           jatekos.Kepesseg.LatEgy / 1.414);
    const int maxX = jatekos.XMax;
    const int maxY = jatekos.YMax;
    std::vector<Position> positions;
    for(int x = 0; x < maxX ; ++x) {
        for(int y = 0; y < maxY; ++y) {
            if (jatekos.Vilag[y][x].Objektum == mezo &&
                unitsOnCell(Position{x, y}) < 4) {
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

    }
    return *it;
}

void sendUnitTo(Position position, const TEgyseg& unit) {
    const auto x = position.x;
    const auto y = position.y;
    if(unit.AkcioKod == caNincs) {
        log("Unit %d was idle, sending to %d, %d", int(unit.ID), x, y);
        Utasit_Termel(unit.ID, x, y);
    } else {
        log("Unit %d was already in movement", int(unit.ID));
    }
}

void moveUnitTo(Position position, const TEgyseg& unit) {
    const auto x = position.x;
    const auto y = position.y;
    if(unit.AkcioKod == caNincs) {
        log("Unit %d was idle, sending to %d, %d", int(unit.ID), x, y);
        Utasit_Mozog(unit.ID, x, y);
    } else {
        log("Unit %d was already in movement", int(unit.ID));
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

std::vector<int> Agent::getFreeWorkers() const {
    std::vector<int> workers;
    for (int i = 0; i < jatekos.EgySzam; ++i) {
        if (jatekos.Egysegek[i].Tipus == ceParaszt && jatekos.Egysegek[i].AkcioKod == caNincs) {
            workers.push_back(i);
        }
    }
    return workers;
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
    Utasit_Kepez(e, jatekos.Epuletek[epuletIndex].ID);
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

short Agent::getBuildingId(Mezo m) {
    for (int i = 0; i < jatekos.EpSzam; ++i) {
        if (jatekos.Epuletek[i].Tipus == m) {
            return jatekos.Epuletek[i].ID;
        }
    }
    return -1;
}

int Agent::negyed() const {
    int fohazEpuletIndex = getBuildingIndex(cvFohaz);
    if (fohazEpuletIndex != -1) {
        return (jatekos.Epuletek[fohazEpuletIndex].X > jatekos.XMax / 2) | ((jatekos.Epuletek[fohazEpuletIndex].Y > jatekos.YMax / 2) << 1);
    }
    else {
        return -1;
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

void Agent::getStuff(Mezo mezo) {

    for (const auto& freeWorker : getFreeWorkers()) {
        Position ofMyOnlySon = Position{jatekos.Egysegek[freeWorker].X,
                                        jatekos.Egysegek[freeWorker].Y};
        Position food = getLocationOfResourceNearBy(mezo, ofMyOnlySon);
        std::cerr << "Found food at: " << food.x << " " << food.y << std::endl;
        sendUnitTo(food, jatekos.Egysegek[freeWorker]);
    }

    while (makeUnitIfPossible(ceParaszt)) {}
}

bool Agent::getFoodStrategy() {
    if (getNumberOfUnitsProducingWare(caKaja) >= 6) {
        current_strategy = Strategy::GetWood;
        return true;
    }
    log("getFood");
    getStuff(cvKaja);
    return false;
}

bool Agent::getWoodStrategy() {
    if (getNumberOfUnitsProducingWare(caFa) >= 6) {
        current_strategy = Strategy::GetIron;
        return true;
    }
    log("getwood");
    getStuff(cvFa);
    return false;
}

bool Agent::getIronStrategy() {
    if (getNumberOfUnitsProducingWare(caVas) >= 4) {
        current_strategy = Strategy::GoForLoter;
        return true;
    }
    log("getiron");
    getStuff(cvVasBanya);
    return false;
}

bool Agent::goForLoterStrategy() {
    log("loter");
    if(getBuildingIndex(cvLoter) != -1) {
        current_strategy = Strategy::DefendBorders;
        return true;
    }

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
        buildBuildingIfPossible(cvLoter, buildingSite);
    }
    return false;
}

bool Agent::defendBordersStrategy() {
    log("defendBorders");

    for (const auto& freeArcher : getFreeArchers()) {
        Position ofMyOnlySon = Position{jatekos.Egysegek[freeArcher].X,
                                        jatekos.Egysegek[freeArcher].Y};
        moveUnitTo(getExplorationPosition(ofMyOnlySon), jatekos.Egysegek[freeArcher]);
    }

    while (makeUnitIfPossible(ceIjasz)) {
    }

    return false;
}

void Agent::logFeedback() {
#ifndef CALMARE_DISABLE_LOG
    for (int i = 0; i < jatekos.UtasitSzam; ++i) {
        auto errorCode = jatekos.Utasitasok[i].Result;
        if (errorCode != chOk) {
            log("Warning: Got error code %c", errorCode);
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
            case Strategy::DefendBorders:
                strategy_changes = defendBordersStrategy();
            default:
                strategy_changes = false;
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
        case 0: return{ { 0, 0 }, { jatekos.XMax / 2, jatekos.YMax / 2 } };
        case 1: return{ { jatekos.XMax / 2, 0 }, { jatekos.XMax, jatekos.YMax / 2 } };
        case 2: return{ { 0, jatekos.YMax / 2 }, { jatekos.XMax / 2, jatekos.YMax } };
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
                    jatekos.Vilag[j][i - 1].Objektum == cvNemLatszik || jatekos.Vilag[j][i - 1].Objektum == cvNemLatszik) {
                    res.push_back({ i, j });
                }
            }
        }
    }
    return res;
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
        case 1: return Position{xMax/2 + 1, yMax/2 - 1};
        case 2: return Position{xMax/2 - 1, yMax/2 + 1};
        case 3: return Position{xMax/2 + 1, yMax/2 + 1};
    }
    assert(false);
}
