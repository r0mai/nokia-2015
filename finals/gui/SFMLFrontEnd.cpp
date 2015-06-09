#include "SFMLFrontEnd.hpp"

#include <cmath>
#include <iostream>
#include <algorithm>

namespace calmare {

SFMLFrontEnd::SFMLFrontEnd(Model& model) :
    model(model),
    window(sf::VideoMode(800, 800), "Calmare - Nokia 2015")
{
    int w = model.getContext().mapState.getWidth();
    int h = model.getContext().mapState.getHeight();

    int max = std::max(w, h);
    worldView.reset(sf::FloatRect(0, 0, max, max));

    generateGrid();
}

void SFMLFrontEnd::generateGrid() {
    int w = model.getContext().mapState.getWidth();
    int h = model.getContext().mapState.getHeight();

    float gridThickness = 0.01 * w;

    for (int x = 0; x <= w; ++x) {
        sf::RectangleShape s{sf::Vector2f{gridThickness, 1.f * h}};
        s.setPosition(sf::Vector2f{float(x) - gridThickness/2, 0});
        s.setFillColor(sf::Color::White);
        gridShapes.push_back(s);
    }
    for (int y = 0; y <= h; ++y) {
        sf::RectangleShape s{sf::Vector2f{1.f * w, gridThickness}};
        s.setPosition(sf::Vector2f{0, float(y) - gridThickness/2});
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
                handleMouseWheelScrollEvent(event.mouseWheelScroll);
                break;
            case sf::Event::KeyPressed:
                handleKeyPressedEvent(event.key);
                break;
            case sf::Event::MouseMoved:
                handleMouseMovedEvent(event.mouseMove);
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    dragStart = window.mapPixelToCoords(sf::Vector2i(
                        event.mouseButton.x,
                        event.mouseButton.y
                    ), worldView);
                    dragInProgress = true;
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    dragInProgress = false;
                }
                break;
            default:
                break;
        }
    }
}

void SFMLFrontEnd::handleMouseWheelScrollEvent(
    const sf::Event::MouseWheelScrollEvent& event)
{
    if (event.wheel != sf::Mouse::VerticalWheel) {
        return;
    }
    const float zoomFactor = 0.9;
    const float delta = event.delta;
    if (delta < 0.f) {
        worldView.zoom(std::pow(1/zoomFactor, -delta));
    } else if (delta > 0.f) {
        worldView.zoom(std::pow(zoomFactor, delta));
    }
}

void SFMLFrontEnd::handleKeyPressedEvent(const sf::Event::KeyEvent& event) {
    switch (event.code) {
        case sf::Keyboard::Q:
            window.close();
            break;
        default:
            break;
    }
}

void SFMLFrontEnd::handleMouseMovedEvent(
    const sf::Event::MouseMoveEvent& event)
{
    // Dragging
    if (!dragInProgress) {
        return;
    }
    sf::Vector2f current = window.mapPixelToCoords(sf::Vector2i(
        event.x,
        event.y
    ), worldView);

    worldView.move(dragStart - current);
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
