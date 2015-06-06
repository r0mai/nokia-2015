#pragma once

#include <vector>
#include <utility>
#include "Capabilities.hpp"
#include "GameState.hpp"
#include "MapState.hpp"
#include "Order.hpp"
#include "Resources.hpp"
#include "Unit.hpp"

namespace calmare {

class Context {

	GameState gameState;
	MapState mapState;
	Resources resources;
	Units units;

public:
	std::vector<std::pair<Unit, Order> > currentOrders() const;

};

}
