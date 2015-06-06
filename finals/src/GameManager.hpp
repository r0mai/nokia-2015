#pragma once

#include <SFML/Graphics.hpp>

namespace calmare {

class GameManager {
public:
    GameManager();

    void run();
private:
    sf::RenderWindow window;
};

}
