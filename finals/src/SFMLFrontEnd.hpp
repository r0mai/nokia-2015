#pragma once

#include <SFML/Graphics.hpp>

#include "Model.hpp"

namespace calmare {

class SFMLFrontEnd {
public:
    SFMLFrontEnd();

    void run();

    void handleEvents();
    void tick();
    void draw();
private:
    Model model;
    sf::RenderWindow window;
};

}
