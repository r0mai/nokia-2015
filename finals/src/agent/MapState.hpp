#pragma once

#include "agent/ContextFwd.hpp"
#include "agent/Unit.hpp"

#include <vector>
#include <functional>
#include <algorithm>

namespace calmare {

struct Position {
    int x = 0;
    int y = 0;
};


class MapState : public OwnedByContext {
    std::vector<std::pair<std::reference_wrapper<const Unit>, Position> >
        positions;
    auto findUnit(const Unit& unit) const {
        const auto it = std::find_if(positions.begin(), positions.end(),
                [&unit](const decltype(positions)::value_type& p) {
                return &p.first.get() == &unit;
        });
        return it;
    }
    auto addUnit(const Unit& unit) {
        const auto& it = findUnit(unit);
        if(it == positions.end()) {
            positions.push_back(std::make_pair(std::cref(unit), Position{}));
        }
        return positions.begin()+positions.size()-1;
    }
public:
    MapState(Context* context) : OwnedByContext(context) { }
    void setUnitPosition(const Unit& unit, Position newPosition) {
        const auto it = addUnit(unit);
        auto& unitPair = *it;
        unitPair.second = newPosition;
    }
    Position getUnitPosition(const Unit& unit) const {
        const auto it = findUnit(unit);
        return it->second;
    }
    int distanceBetween(const Unit& a, const Unit& b) const;
};

}
