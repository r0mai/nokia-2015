#include "SFMLFrontEnd.hpp"

#include <cmath>
#include <iostream>

namespace calmare {

SFMLFrontEnd::SFMLFrontEnd() :
    window(sf::VideoMode(800, 800), "Calmare - Nokia 2015"),
    worldView(sf::FloatRect(0, 0, 1, 1))
{
    generateGrid();
}

void SFMLFrontEnd::generateGrid() {
    unsigned w = 10, h = 10;
    float gridThickness = 0.01;
    for (unsigned x = 0; x <= w; ++x) {
        sf::RectangleShape s{sf::Vector2f{gridThickness, 1}};
        s.setPosition(sf::Vector2f{float(x)/w - gridThickness/2, 0});
        s.setFillColor(sf::Color::White);
        gridShapes.push_back(s);
    }
    for (unsigned y = 0; y <= h; ++y) {
        sf::RectangleShape s{sf::Vector2f{1, gridThickness}};
        s.setPosition(sf::Vector2f{0, float(y)/h - gridThickness/2});
        s.setFillColor(sf::Color::White);
        gridShapes.push_back(s);
    }
}

void SFMLFrontEnd::run() {
    while (window.isOpen()) {
        handleEvents();
        tick();
        draw();
    }
}

void SFMLFrontEnd::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseWheelScrolled:
            {
                if (event.mouseWheelScroll.wheel != sf::Mouse::VerticalWheel) {
                    break;
                }
                const float zoomFactor = 0.9;
                const float delta = event.mouseWheelScroll.delta;
                if (delta < 0.f) {
                    worldView.zoom(std::pow(1/zoomFactor, -delta));
                } else if (delta > 0.f) {
                    worldView.zoom(std::pow(zoomFactor, delta));
                }
                break;
            }
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                    case sf::Keyboard::Q:
                        window.close();
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
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
    for (const auto& shape : gridShapes) {
        window.draw(shape);
    }
}

void SFMLFrontEnd::drawGUI() {

}

}
