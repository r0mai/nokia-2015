#pragma once

#include "agent/Context.hpp"

namespace calmare {

class Model {
    Context context;
public:
    void tick();
};

}
