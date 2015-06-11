#pragma once

#include "VilagDef.h"
#include <iostream>
#include "Log.hpp"
#include "Resources.hpp"

namespace calmare {

class Agent {
public:
    TKoteg getOrders(TJatekos jatekos);

    void logMap(const TJatekos& jatekos, std::ostream& os = std::cerr);
    TJatekos jatekos;
private:
    enum class Strategy {
        GetFood,
        GetWood,
        GetIron,
        GoForLoter
    } current_strategy = Strategy::GetFood;

    // returns true, if strategy changes
    bool getFoodStrategy();
    bool getWoodStrategy();
    bool getIronStrategy();
    bool goForLoterStrategy();

    void getStuff(Mezo mezo);

    bool makeWorkerIfPossible();
    short getFoHazId();
    int getUnitCount(Egyseg e);
};

}
