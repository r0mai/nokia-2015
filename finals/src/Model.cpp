#if 0
#include "Model.hpp"

namespace calmare {

void Model::tick() {

    context.executeCurrentOrders();

}

Context& Model::getContext() {
    return context;
}

const Context& Model::getContext() const {
    return context;
}

}
#endif
