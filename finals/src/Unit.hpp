#pragma once

#include "ContextFwd.hpp"
#include "Capabilities.hpp"
#include "Order.hpp"

namespace calmare {

typedef int UnitId;

class Unit : public OwnedByContext {
    Capabilities currentCapabilities;
public:

    Unit(Context* context) : OwnedByContext(context) { }
    Order getCurrentOrder() const {
        return "Move";
    }

    void executeOrder(const Order& order);

};

class Units {
    Context* context;
    std::vector<Unit> units;

public:
    Units(Context* context) : context(context) { }
    friend class Context;
};

}
