#include "Agent.hpp"
#include "Utasitas.h"

#include <iostream>

using namespace calmare;

void eraseOrders() {
    std::cerr << "Erasing orders" << std::endl;
    koteg.UtasitSzam = 0;
}

void sendUnitTo(int x, int y, const TEgyseg& unit) {
    if(unit.AkcioKod == caNincs) {
        std::cerr<<"Unit " << unit.ID << " was idle, sending to "<<x << " "<< y<<std::endl;
        Utasit_Mozog(unit.ID, x, y);
    } else {
        std::cerr<<"Unit " << unit.ID << " was already in movement"<<std::endl;

    }
}

TKoteg Agent::getOrders(TJatekos jatekos) {
    eraseOrders();
    std::cerr << "Working with " << jatekos.EgySzam << " units" << std::endl;
    short myOnlySon = jatekos.Egysegek[0].ID;
    sendUnitTo(52, 52, jatekos.Egysegek[0]);
    return koteg;
}
