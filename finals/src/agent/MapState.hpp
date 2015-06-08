#pragma once

#include "agent/ContextFwd.hpp"

namespace calmare {

class MapState : public OwnedByContext {
public:
    MapState(Context* context) : OwnedByContext(context) { }
};

}
