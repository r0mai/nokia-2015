#include <iostream>

#include "VilagDef.h"
#include "Comm.h"
#include "Player.h"
#include "Agent.hpp"

using namespace std;

int run() {
    initComm();

    calmare::Agent agent;
    TJatekos jatekos;
    while (readData(jatekos) != 0) {
	TKoteg response = agent.getOrders(jatekos);
	writeData(response);
    }

    return 0;
}
