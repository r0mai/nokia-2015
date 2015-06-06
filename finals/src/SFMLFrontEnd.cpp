#include "SFMLFrontEnd.hpp"

namespace calmare {

SFMLFrontEnd::SFMLFrontEnd() :
    window(sf::VideoMode(800, 600), "Calmare - Nokia 2015") {}

void SFMLFrontEnd::run() {
    while (window.isOpen()) {
        sf::Event Event;
        while (window.pollEvent(Event)) {
            if (Event.type == sf::Event::Closed) {
                window.close();
            }
        }

        model.tick();

        window.clear(sf::Color::Black);

        //draw stuff
        window.clear(model.color);

        window.display();
    }
}

}
