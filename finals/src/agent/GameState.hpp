#pragma once

#include "agent/ContextFwd.hpp"

namespace calmare {

class GameState : public OwnedByContext {
public:
    GameState(Context* context) : OwnedByContext(context) { }
};

}
