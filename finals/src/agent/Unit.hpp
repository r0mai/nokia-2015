#pragma once

#include "agent/ContextFwd.hpp"
#include "agent/Capabilities.hpp"
#include "agent/Order.hpp"

namespace calmare {

typedef int UnitId;

class Unit : public OwnedByContext {
    Capabilities currentCapabilities;
public:

    Unit(Context* context) : OwnedByContext(context) { }
    Order getCurrentOrder() const {
        return "Up";
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
