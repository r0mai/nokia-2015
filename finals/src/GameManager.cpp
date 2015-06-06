#include "GameManager.hpp"

namespace calmare {

GameManager::GameManager() :
    window(sf::VideoMode(800, 600), "Calmare - Nokia 2015") {}

void GameManager::run() {
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
