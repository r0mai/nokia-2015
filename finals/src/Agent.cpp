#include "Agent.hpp"
#include "Utasitas.h"

using namespace calmare;

void eraseOrders() {
    koteg.UtasitSzam = 0;
}

TKoteg Agent::getOrders(TJatekos jatekos) {
    eraseOrders();
    return koteg;
}
