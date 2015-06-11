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
        MakeWorkers,
        GoForLoter
    } current_strategy = Strategy::MakeWorkers;

    // returns true, if strategy changes
    bool makeWorkersStrategy();
    bool goForLoterStrategy();

    Mezo buildingTypeForUnit(Egyseg e);

    bool makeUnitIfPossible(Egyseg e);
    short getFoHazId();
    short getBuildingId(Mezo m);
    int getUnitCount(Egyseg e);
};

}
