#pragma once

#include "VilagDef.h"
#include <iostream>
#include "Log.hpp"

namespace calmare {

class Agent {
public:
    TKoteg getOrders(TJatekos jatekos);

    void logMap(const TJatekos& jatekos, std::ostream& os = std::cerr);
    TJatekos jatekos;
private:

    enum Strategy {
        MakeWorkers
    } current_strategy = Strategy::MakeWorkers;

    void makeWorkersStrategy();
};

}
