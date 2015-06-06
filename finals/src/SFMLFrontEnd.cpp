#include "SFMLFrontEnd.hpp"

namespace calmare {

SFMLFrontEnd::SFMLFrontEnd() :
    window(sf::VideoMode(800, 600), "Calmare - Nokia 2015") {}

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

    //draw stuff
    window.clear(model.color);

    window.display();
}

}
