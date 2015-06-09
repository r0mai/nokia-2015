#include "MapState.hpp"

#include <algorithm>
#include <cassert>
#include <cstdlib>

using namespace calmare;

int MapState::distanceBetween(const Unit& a, const Unit& b) const {
    const auto aIt = findUnit(a);
    const auto bIt = findUnit(b);
    if(aIt == positions.end() || bIt == positions.end()) {
        assert(!"Can only get position between units contained on the map");
    }
    const auto aPos = aIt->second;
    const auto bPos = bIt->second;

    const auto dX = std::abs(aPos.x - bPos.x);
    const auto dY = std::abs(aPos.y - bPos.y);

    // Use Chebyshev distance
    // http://en.wikipedia.org/wiki/Chebyshev_distance

    return std::max(dX, dY);
}
