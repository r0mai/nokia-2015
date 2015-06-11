#pragma once

#include <vector>
#include <functional>
#include "Capabilities.hpp"
#include "Resources.hpp"

namespace calmare {

class Improvement : public Capability {
    int level = -1;

    std::function<Cost(&)(Context, int)> f;

public:
    Improvement(Context* context, std::function < Cost(&)(Context, int) > f) : Capability(context), f(f){};
    Improvement(Context* context, std::function < Cost(&)(Context) > f) : Capability(context), f(std::bind(f, std::placeholders::_1)){};

    Cost get() const
    {
        return f(this->getContext(), level + 1);
    }

    void made()
    {
        ++level;
    }

};

}
