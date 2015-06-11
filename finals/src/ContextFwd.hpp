#if 0
#pragma once

namespace calmare {

struct Context;

class OwnedByContext {
    Context* context;
public:
    OwnedByContext(Context* context) : context(context) { }
    Context& getContext() const {
        return *context;
    }
};

}
#endif
