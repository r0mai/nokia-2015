#pragma once

#include "ContextFwd.hpp"
#include "Capabilities.hpp"
#include "Order.hpp"

namespace calmare {

typedef int UnitId;

enum class UnitType {
    Paraszt = 1,
    Kardos,
    Ijasz,
    Lovas,
    Puskas,

    Varoshaza,
    Laktanya,
    Loter,
    Istallo,
    Akademia,
    Korhaz,
    Torony,
    Fal,

    Ko,
    Fa,
    Banya,
    Tokfold,

    Ismeretlen
};



enum class UnitKind {
    friendly = 1,
    enemy,
    invisible
};

class Unit : public OwnedByContext {
    Capabilities currentCapabilities;
    UnitType unitType = UnitType::Egyeb;
    UnitKind unitKind = UnitKind::friendly;
    UnitId unitId;
    unsigned visibilityRadius = 2;
public:

    Unit(Context* context) : OwnedByContext(context) { }
    Order getCurrentOrder() const {
        return "Move";
    }

    void executeOrder(const Order& order);

    UnitId getId() const { return unitId; }
    unsigned getVisibilityRadius() const { return visibilityRadius; }

    template<UnitKind kind> bool is() { return unitKind == kind; }

    UnitType getUnitType() const { return unitType; }
    UnitKind getUnitKind() const { return unitKind; }
};

class Units {
    Context* context;
    std::vector<Unit> units;

public:
    Units(Context* context) : context(context) { }

    const std::vector<Unit>& getUnits() const { return units; }

    friend struct Context;
};

}
