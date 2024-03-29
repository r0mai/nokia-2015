#if 0
#include "Context.hpp"

using namespace calmare;

void Context::executeCurrentOrders() {
    gameState.advance();
    const auto& orders = currentOrders();
    for(const auto& pair: orders) {
        auto& unit = pair.first;
        const auto& order = pair.second;

        unit.executeOrder(order);
    }
}

std::vector<std::pair<Unit&, Order> > Context::currentOrders() const {
    std::vector<std::pair<Unit&, Order> > orders;
    for(auto& unit: units.units) {
        std::pair<Unit&, Order> pair(const_cast<Unit&>(unit), unit.getCurrentOrder());
        orders.push_back(std::move(pair));
    }
    return orders;
}

Unit& Context::addUnit() {
    units.units.push_back(Unit(this));
    Unit& unit = units.units.back();
    mapState.setUnitPosition(unit, {});
    return unit;
}
#endif
