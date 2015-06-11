#if 0
#include "just/test.hpp"
#include "Context.hpp"
#include "MapState.hpp"

using namespace calmare;

JUST_TEST_CASE(MapState_UnitShouldBeVisibleToItsself) {

	Context context;

	auto& unit = context.addUnit();

	JUST_ASSERT_EQUAL(context.mapState.isVisibleFor(unit, unit), true);

}

JUST_TEST_CASE(MapState_UnitShouldNotBeVisibleToFarAwayUnit) {

	Context context;

	auto& unit1 = context.addUnit();
	auto& unit2 = context.addUnit();

	Position pos1;
	Position pos2;
	pos2.x = 10;
	pos2.y = 10;

	context.mapState.setUnitPosition(unit1, pos1);
	context.mapState.setUnitPosition(unit2, pos2);

	JUST_ASSERT_EQUAL(context.mapState.isVisibleFor(unit1, unit2), false);

}
#endif
