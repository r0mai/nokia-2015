#if 0
#pragma once

#include "Context.hpp"

namespace calmare {

class Model {
    Context context;
public:
    void tick();
    Context& getContext();
    const Context& getContext() const;
};

}
#endif
