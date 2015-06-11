#include "Agent.hpp"
#include "Utasitas.h"

#include <iostream>
#include <vector>
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
    eraseOrders();
    std::cerr << "Working with " << jatekos.EgySzam << " units" << std::endl;
    short myOnlySon = jatekos.Egysegek[0].ID;
    Position ofMyOnlySon = Position{jatekos.Egysegek[0].X, jatekos.Egysegek[0].Y};
    Position food = getLocationOfResourceNearBy(jatekos, cvKaja, ofMyOnlySon);
    std::cerr << "Found food at: " << food.x << " " << food.y << std::endl;
    sendUnitTo(food, jatekos.Egysegek[0]);
    return koteg;
}
