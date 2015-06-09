#include "MapState.hpp"

#include <algorithm>
#include <cassert>
#include <cstdlib>

using namespace calmare;

int MapState::distanceBetween(const Unit& a, const Unit& b) const {
    const auto aIt = findUnit(a);
    const auto bIt = findUnit(b);

    assert(aIt != positions.end() && bIt != positions.end() &&
        "Can only get position between units contained on the map");

    const auto aPos = aIt->second;
    const auto bPos = bIt->second;

    const auto dX = std::abs(aPos.x - bPos.x);
    const auto dY = std::abs(aPos.y - bPos.y);

    // Use Chebyshev distance
    // http://en.wikipedia.org/wiki/Chebyshev_distance

    return std::max(dX, dY);
}

int MapState::getWidth() const {
    return width;
}

int MapState::getHeight() const {
    return height;
}

bool MapState::isVisibleFor(const Unit& from, const Unit& to) const {
    const auto fromIt = findUnit(to);
    const auto toIt = findUnit(to);

    assert(fromIt != positions.end() && toIt != positions.end() &&
        "Can only get position between units contained on the map");

    const auto visibilityRadius = from.getVisibilityRadius();

    return visibilityRadius >= distanceBetween(from, to);
}
