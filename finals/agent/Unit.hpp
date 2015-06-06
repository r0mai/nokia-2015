#pragma once

#include "ContextFwd.hpp"
#include "Capabilities.hpp"

namespace calmare {

typedef int UnitId;

class Unit : public OwnedByContext {
	Capabilities currentCapabilities;
};

class Units {
	std::vector<Unit> units;
};

}
