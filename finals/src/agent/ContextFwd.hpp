#pragma once

namespace calmare {
namespace agent {

class Context;

class OwnedByContext {
    Context* context;
public:
    OwnedByContext(Context* context) : context(context) { }
    Context& getContext() const {
        return *context;
    }
};

}
}
