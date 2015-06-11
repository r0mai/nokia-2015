#pragma once

#include "ContextFwd.hpp"
#include "Capabilities.hpp"
#include "Unit.hpp"

namespace calmare {

class Building : public OwnedByContext {
    Capabilities currentCapabilites;
    UnitKind buildingKind;

public:
    Building(Context* context) : OwnedByContext(context) {}
};

class Buildings {
    Context* context;
    std::vector<Building> buildings;

public:
    const std::vector<Building>& getBuildings() const { return buildings; }
    friend struct Context;
};
}
