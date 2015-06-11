#pragma once

#include "ContextFwd.hpp"
#include "Capabilities.hpp"
#include "Unit.hpp"

namespace calmare {

class Building : public OwnedByContext {
    CreationCapabilities currentCreationCapabilites;
    ImprovementCapabilities currentImprovementCapabilites;
    UnitKind buildingKind;

public:
    Building(Context* context) : OwnedByContext(context) {}
};

class Buildings {
    Context* context;
    std::vector<Building> buildings;

public:
    Buildings(Context* context) : context(context) {}
    const std::vector<Building>& getBuildings() const { return buildings; }
    friend struct Context;
};
}
