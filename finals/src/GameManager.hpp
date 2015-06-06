#pragma once

#include <SFML/Graphics.hpp>

#include "Model.hpp"

namespace calmare {

class GameManager {
public:
    GameManager();

    void run();
private:
    Model model;
    sf::RenderWindow window;
};

}
