#include "Model.hpp"

namespace calmare {

void Model::tick() {

    context.executeCurrentOrders();

}

}
