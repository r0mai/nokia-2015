#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Model.hpp"

namespace calmare {

class SFMLFrontEnd {
public:
    SFMLFrontEnd();

    void run();

private:
    void generateGrid();

    void handleEvents();
    void tick();
    void draw();

    void drawWorld();
    void drawGUI();

    Model model;

    sf::RenderWindow window;
    sf::View worldView;

    std::vector<sf::RectangleShape> gridShapes;
};

}
