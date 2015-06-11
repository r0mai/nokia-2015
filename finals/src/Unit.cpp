#if 0
#include "Unit.hpp"
#include "Context.hpp"

using namespace calmare;

void Unit::executeOrder(const Order& order) {
    if(order == "Move") {
        const auto& gameState = getContext().gameState;
        auto& mapState = getContext().mapState;
        auto currentPosition = mapState.getUnitPosition(*this);
        int xDelta = 2*(gameState.getNumberOfTicksElapsed() % 2)-1;
        currentPosition.x += xDelta;

        mapState.setUnitPosition(*this, currentPosition);
    }
}
#endif
