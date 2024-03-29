#if 0
#pragma once

#include "Position.hpp"
#include "ContextFwd.hpp"
#include "Unit.hpp"

#include <vector>
#include <functional>
#include <algorithm>

namespace calmare {

class MapState : public OwnedByContext {
    std::vector<std::pair<std::reference_wrapper<const Unit>, Position> >
        positions;

    int width = 10;
    int height = 15;

    decltype(positions)::const_iterator findUnit(const Unit& unit) const {
        auto it = std::find_if(positions.begin(), positions.end(),
                [&unit](const decltype(positions)::value_type& p) {
                return &p.first.get() == &unit;
        });
        return it;
    }
    decltype(positions)::iterator addUnit(const Unit& unit) {
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
    unsigned distanceBetween(const Unit& a, const Unit& b) const;

    int getWidth() const;
    int getHeight() const;

    bool isVisibleFor(const Unit& from, const Unit& to) const;
};

}
#endif
