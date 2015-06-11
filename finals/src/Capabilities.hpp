#pragma once

#include "ContextFwd.hpp"
#include "Position.hpp"

#include <vector>
#include <unordered_map>

namespace calmare {

enum class UnitType;

class CreationCapability : public OwnedByContext {

public:
    CreationCapability(Context* context) : OwnedByContext(context) {}
};

class ImprovementCapability : public OwnedByContext {

public:
    ImprovementCapability(Context* context) : OwnedByContext(context) {}
};

class MoveCapability : public OwnedByContext {
    Position goal;
};

using CreationCapabilities = std::vector<CreationCapability>;
using ImprovementCapabilities = std::vector<ImprovementCapability>;

}
