#if 0
#pragma once

#include <vector>
#include <utility>
#include "Capabilities.hpp"
#include "GameState.hpp"
#include "MapState.hpp"
#include "Order.hpp"
#include "Resources.hpp"
#include "Unit.hpp"
#include "Building.hpp"

namespace calmare {

struct Context {

    GameState gameState{this};
    MapState mapState{this};
    Resources resources{this};
    Units units{this};
    Buildings buildings{this};

    std::vector<std::pair<Unit&, Order>> currentOrders() const;
    void executeCurrentOrders();
    Unit& addUnit();
};

}
#endif
