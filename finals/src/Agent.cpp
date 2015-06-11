#include "Agent.hpp"
#include "Utasitas.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace calmare;

void eraseOrders() {
    std::cerr << "Erasing orders" << std::endl;
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

Position getLocationOfResourceNearBy(const TJatekos& jatekos, Mezo mezo,
                                     Position near) {
    const int maxX = jatekos.XMax;
    const int maxY = jatekos.YMax;
    std::vector<Position> positions;
    for(int x = 0; x < maxX ; ++x) {
        for(int y = 0; y < maxY; ++y) {
            if(jatekos.Vilag[y][x].Objektum == mezo) {
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
        std::cerr << "Didnt find any resource" << std::endl;
        return Position{near.x - 2, near.y - 2};
    }
    return *it;
}

void sendUnitTo(Position position, const TEgyseg& unit) {
    const auto x = position.x;
    const auto y = position.y;
    if(unit.AkcioKod == caNincs) {
        std::cerr<<"Unit " << unit.ID << " was idle, sending to "<<x << " "<< y<<std::endl;
        Utasit_Termel(unit.ID, x, y);
    } else {
        std::cerr<<"Unit " << unit.ID << " was already in movement"<<std::endl;

    }
}

TKoteg Agent::getOrders(TJatekos jatekos) {
    logMap(jatekos);
    eraseOrders();
    std::cerr << "Working with " << jatekos.EgySzam << " units" << std::endl;
    short myOnlySon = jatekos.Egysegek[0].ID;
    Position ofMyOnlySon = Position{jatekos.Egysegek[0].X, jatekos.Egysegek[0].Y};
    Position food = getLocationOfResourceNearBy(jatekos, cvKaja, ofMyOnlySon);
    std::cerr << "Found food at: " << food.x << " " << food.y << std::endl;
    sendUnitTo(food, jatekos.Egysegek[0]);
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
        std::cout << line << '\n';
    }
    std::cout << std::endl;
#endif
}
