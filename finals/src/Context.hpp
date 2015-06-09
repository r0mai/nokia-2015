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
    friend class OwnedByContext;

    const GameState& getGameState() const { return gameState; }
    const MapState& getMapState() const { return mapState; }
    const Resources& getResources() const { return resources; }
    const Units& getUnits() const { return units; }
    GameState& getGameState() { return gameState; }
    MapState& getMapState() { return mapState; }
    Resources& getResources() { return resources; }
    Units& getUnits() { return units; }

    Unit& addUnit();
};

}
