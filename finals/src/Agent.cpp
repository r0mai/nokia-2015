#include "Agent.hpp"
#include "Utasitas.h"

using namespace calmare;

void eraseOrders() {
    koteg.UtasitSzam = 0;
}

void sendUnitTo(int x, int y, short unitId) {
    Utasit_Mozog(unitId, x, y);
}

TKoteg Agent::getOrders(TJatekos jatekos) {
    eraseOrders();
    short myOnlySon = jatekos.Egysegek[0].ID;
    sendUnitTo(52, 52, myOnlySon);
    return koteg;
}
