#include "agent/Context.hpp"

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
}
