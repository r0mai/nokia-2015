#pragma once

#include "agent/ContextFwd.hpp"
#include <vector>

namespace calmare {

class Resource : public OwnedByContext {
public:
    Resource(Context* context) : OwnedByContext(context) { }
};

class Resources : public OwnedByContext {
public:
    Resources(Context* context) : OwnedByContext(context) { }
};

}
