#pragma once

#include "agent/Context.hpp"

namespace calmare {

class Model {
    agent::Context context;
public:
    void tick();
};

}
