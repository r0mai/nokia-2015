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

    Mezo buildingTypeForUnit(Egyseg e);

    bool makeUnitIfPossible(Egyseg e);
    void getStuff(Mezo mezo);

    bool makeWorkerIfPossible();
    int getFoHazEpuletId();
    short getFoHazId();
    short getBuildingId(Mezo m);
    int negyed();
    int getUnitCount(Egyseg e);
};

}
