#pragma once

#include "Context.hpp"

namespace calmare {

class Model {
    Context context;
public:
    void tick();
};

}
