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
namespace agent {

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
}
