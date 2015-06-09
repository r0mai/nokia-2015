#pragma once

#include <vector>
#include <utility>
#include "Capabilities.hpp"
#include "GameState.hpp"
#include "MapState.hpp"
#include "Order.hpp"
#include "Resources.hpp"
#include "Unit.hpp"

namespace calmare {

struct Context {

    GameState gameState{this};
    MapState mapState{this};
    Resources resources{this};
    Units units{this};

    std::vector<std::pair<Unit&, Order>> currentOrders() const;
    void executeCurrentOrders();
    Unit& addUnit();
};

}
