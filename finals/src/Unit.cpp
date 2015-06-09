#include "Unit.hpp"
#include "Context.hpp"

using namespace calmare;

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
