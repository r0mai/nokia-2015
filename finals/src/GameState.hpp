#pragma once

#include "ContextFwd.hpp"

namespace calmare {

class GameState : public OwnedByContext {
    unsigned ticks;
public:
    GameState(Context* context) : OwnedByContext(context), ticks(0) { }
    void advance();
    auto getNumberOfTicksElapsed() const { return ticks; }
};

}
