#pragma once

#include <SFML/Graphics.hpp>

namespace calmare {

class Model {
public:
    void tick();

    sf::Color color = sf::Color::Black;
};

}
