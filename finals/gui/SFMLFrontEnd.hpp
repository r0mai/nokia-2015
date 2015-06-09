#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Model.hpp"

namespace calmare {

class SFMLFrontEnd {
public:
    SFMLFrontEnd(Model& model);

    void run();

private:
    void generateGrid();

    void handleEvents();
    void handleMouseWheelScrollEvent(
        const sf::Event::MouseWheelScrollEvent& event);
    void handleKeyPressedEvent(const sf::Event::KeyEvent& event);
    void handleMouseMovedEvent(const sf::Event::MouseMoveEvent& event);

    void tick();
    void draw();

    void drawWorld();
    void drawGUI();

    Model& model;

    sf::RenderWindow window;
    sf::View worldView;

    std::vector<sf::RectangleShape> gridShapes;

    // Gesture related stuff
    bool dragInProgress = false;
    sf::Vector2f dragStart;
};

}
