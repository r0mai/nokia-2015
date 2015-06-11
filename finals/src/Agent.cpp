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

struct Position {
    int x = 0;
    int y = 0;
    Position() = default;
    Position(int x, int y) : x(x), y(y) { }
};

std::size_t distanceBetween(Position position1, Position position2) {
    return std::max(std::abs(position1.x - position2.x),
            std::abs(position1.y - position2.y));
}

std::size_t unitsOnCell(const TJatekos& jatekos, Position position) {
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

Position getLocationOfResourceNearBy(const TJatekos& jatekos, Mezo mezo,
                                     Position near) {
    const int maxX = jatekos.XMax;
    const int maxY = jatekos.YMax;
    std::vector<Position> positions;
    for(int x = 0; x < maxX ; ++x) {
        for(int y = 0; y < maxY; ++y) {
            if (jatekos.Vilag[y][x].Objektum == mezo &&
                unitsOnCell(jatekos, Position{x, y}) < 4) {
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
        return Position{near.x - 2, near.y - 2};
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

std::vector<int> getFreeWorkers(const TJatekos& jatekos) {
    std::vector<int> workers;
    for (int i = 0; i < jatekos.EgySzam; ++i) {
        if (jatekos.Egysegek[i].AkcioKod == caNincs) {
            workers.push_back(i);
        }
    }
    return workers;
}

bool Agent::makeWorkerIfPossible() {
    if (jatekos.Eroforras.Kaja < 50) {
        return false;
    }
    auto fohazid = getFoHazId();
    if (getFoHazId() < 0) {
        return false;
    }
    Utasit_Kepez(ceParaszt, fohazid);
    jatekos.Eroforras.Kaja -= 50;
    return true;
}

short Agent::getFoHazId() {
    for (int i = 0; i < jatekos.EpSzam; ++i) {
        if (jatekos.Epuletek[i].Tipus == cvFohaz) {
            return jatekos.Epuletek[i].ID;
        }
    }
    return -1;
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

bool Agent::makeWorkersStrategy() {
    if (getUnitCount(ceParaszt) > 6) {
        current_strategy = Strategy::GoForLoter;
        return true;
    }

    for (const auto& freeWorker : getFreeWorkers(jatekos)) {
        short myOnlySon = jatekos.Egysegek[freeWorker].ID;
        Position ofMyOnlySon = Position{jatekos.Egysegek[freeWorker].X,
                                        jatekos.Egysegek[freeWorker].Y};
        Position food =
            getLocationOfResourceNearBy(jatekos, cvKaja, ofMyOnlySon);
        std::cerr << "Found food at: " << food.x << " " << food.y << std::endl;
        sendUnitTo(food, jatekos.Egysegek[freeWorker]);
    }

    while (makeWorkerIfPossible()) {}

    return false;
}

bool Agent::goForLoterStrategy() {
    return false;
}

TKoteg Agent::getOrders(TJatekos jatekos) {
    eraseOrders();
    this->jatekos = jatekos;

    bool strategy_changes = false;
    do {
        switch (current_strategy) {
            case Strategy::MakeWorkers:
                strategy_changes = makeWorkersStrategy();
                break;
            case Strategy::GoForLoter:
                strategy_changes = goForLoterStrategy();
                break;
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
void Agent::logMap(const TJatekos& jatekos, std::ostream& os) {
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
#endif
}
