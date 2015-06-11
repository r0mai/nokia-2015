#include "Agent.hpp"
#include "Utasitas.h"

void eraseOrders() {
    koteg.UtasitSzam = 0;
}

TKoteg getOrders(TJatekos jatekos) {
    eraseOrders();
    return koteg;
}
