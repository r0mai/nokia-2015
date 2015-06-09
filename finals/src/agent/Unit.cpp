#include "agent/Unit.hpp"
#include "agent/Context.hpp"

using namespace calmare::agent;

void Unit::executeOrder(const Order& order) {
    if(order == "Move") {
        const auto& gameState = getContext().getGameState();
        auto currentPosition = getContext().getMapState().getUnitPosition(*this);
        int xDelta = 2*(gameState.getNumberOfTicksElapsed() % 2)-1;
        currentPosition.x += xDelta;

        auto& mapState = getContext().getMapState();
        mapState.setUnitPosition(*this, currentPosition);
    }
}
