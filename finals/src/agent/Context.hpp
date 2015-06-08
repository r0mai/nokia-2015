#pragma once

#include <vector>
#include <utility>
#include "agent/Capabilities.hpp"
#include "agent/GameState.hpp"
#include "agent/MapState.hpp"
#include "agent/Order.hpp"
#include "agent/Resources.hpp"
#include "agent/Unit.hpp"

namespace calmare {

class Context {

    GameState gameState;
    MapState mapState;
    Resources resources;
    Units units;

public:
    Context() : gameState(this), mapState(this), resources(this), units(this) {
    }
    std::vector<std::pair<Unit&, Order> > currentOrders() const;
    void executeCurrentOrders();
};

}
