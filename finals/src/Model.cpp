#include "Model.hpp"

namespace calmare {

void Model::tick() {

    context.executeCurrentOrders();

}

const Context& Model::getContext() const {
    return context;
}

}
