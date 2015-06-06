#include "SFMLFrontEnd.hpp"

namespace calmare {

SFMLFrontEnd::SFMLFrontEnd() :
    window(sf::VideoMode(800, 600), "Calmare - Nokia 2015"),
    worldView(sf::FloatRect(0, 0, 1000, 1000))
{}

void SFMLFrontEnd::run() {
    while (window.isOpen()) {
        handleEvents();
        tick();
        draw();
    }
}

void SFMLFrontEnd::handleEvents() {
    sf::Event Event;
    while (window.pollEvent(Event)) {
        if (Event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void SFMLFrontEnd::tick() {
    model.tick();
}

void SFMLFrontEnd::draw() {
    window.clear(sf::Color::Black);

    window.setView(worldView);
    drawWorld();

    window.setView(window.getDefaultView());
    drawGUI();

    window.display();
}

void SFMLFrontEnd::drawWorld() {
    sf::CircleShape shape(50);
    shape.setFillColor(sf::Color(100, 250, 50));

    window.draw(shape);
}

void SFMLFrontEnd::drawGUI() {

}

}
