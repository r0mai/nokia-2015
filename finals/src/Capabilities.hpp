#pragma once

#include <vector>
#include "ContextFwd.hpp"
#include <unordered_map>

namespace calmare {

enum class UnitType;

class Capability : public OwnedByContext {

public:
    Capability(Context* context) : OwnedByContext(context) {}
};

using Capabilities = std::vector < Capability > ;
}
