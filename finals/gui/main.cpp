#include "SFMLFrontEnd.hpp"

int main() {
    calmare::Model model;
    calmare::Unit& unit = model.getContext().addUnit();
    calmare::SFMLFrontEnd gm(model);
    gm.run();
}
