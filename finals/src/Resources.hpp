#pragma once

//#include "agent/ContextFwd.hpp"
#include <vector>
#include <valarray>
#include <cassert>
#include <cstdint>

namespace calmare {

    using Resource = int;
    /*class Resource : public OwnedByContext {
    public:
    Resource(Context* context) : OwnedByContext(context) { }
    };*/

    class Resources : public OwnedByContext {
        // food wood iron gold kp
        std::valarray<Resource> resources;
    public:
        enum class ResourceId : std::int8_t { NO = -1, food = 0, wood, iron, gold, kp, last };

        Resources(Context* context, std::initializer_list<Resource>&& values = { Resource(), Resource(), Resource(), Resource(), Resource() }) :
            OwnedByContext(context), resources(values) {
            assert(values.size() == static_cast<int>(ResourceId::last));
        }

        Resources& operator+=(const Resources& oth) {
            resources += oth.resources;
            return *this;
        }

        Resources& operator-=(const Resources& oth) {
            resources -= oth.resources;
            return *this;
        }

        const Resource& food() const { return resources[static_cast<int>(ResourceId::food)]; }
        const Resource& wood() const { return resources[static_cast<int>(ResourceId::wood)]; }
        const Resource& iron() const { return resources[static_cast<int>(ResourceId::iron)]; }
        const Resource& gold() const { return resources[static_cast<int>(ResourceId::gold)]; }
        const Resource& kp() const { return resources[static_cast<int>(ResourceId::kp)]; }



        bool is_good() const { return (resources >= 0).min(); }
        bool is_bad() const { return !is_good(); }
        operator bool() const { return is_good(); }


        // public friend operators

        friend Resources operator+(const Resources& lhs, const Resources& rhs)
        {
            Resources result(lhs);
            result += rhs;
            return result;
        }

        friend Resources operator-(const Resources& lhs, const Resources& rhs)
        {
            Resources result(lhs);
            result -= rhs;
            return result;
        }

        friend Resources operator+(Resources&& lhs, Resources&& rhs)
        {
            return std::move(lhs += rhs);
        }

        friend Resources operator-(Resources&& lhs, Resources&& rhs)
        {
            return std::move(lhs -= rhs);
        }

        // constructor static functions

        static Resources food(Context* context, Resource res) {
            return Resources(context, { res, Resource(), Resource(), Resource(), Resource() });
        }

        static Resources wood(Context* context, Resource res) {
            return Resources(context, { Resource(), res, Resource(), Resource(), Resource() });
        }

        static Resources iron(Context* context, Resource res) {
            return Resources(context, { Resource(), Resource(), res, Resource(), Resource() });
        }

        static Resources gold(Context* context, Resource res) {
            return Resources(context, { Resource(), Resource(), Resource(), res, Resource() });
        }

        static Resources kp(Context* context, Resource res) {
            return Resources(context, { Resource(), Resource(), Resource(), Resource(), res });
        }
    };

    class Cost : public Resources {
        Resource _time;

    public:
        Cost(Context* context,
            Resource time = Resource(),
            std::initializer_list<Resource>&& values = { Resource(), Resource(), Resource(), Resource(), Resource() }) :
            Resources(context, std::move(values)), _time(time) {}

        Cost(const Resources& resources,
            Resource time = Resource()) :
            Resources(resources), _time(time) {}

        const Resource& time() const { return _time; }


        // constructor static functions -

        // units costs

        static Cost Paraszt(Context* context){
            return Cost(Resources::food(context, 50), 150);
        }

        static Cost Kardos(Context* context){
            return Cost(Resources::food(context, 40) + Resources::iron(context, 20), 200);
        }

        static Cost Ijasz(Context* context){
            return Cost(Resources::food(context, 30) + Resources::wood(context, 20), 250);
        }

        static Cost Lovas(Context* context){
            return Cost(Resources::food(context, 100) + Resources::gold(context, 15), 300);
        }

        static Cost Puskas(Context* context){
            return Cost(Resources::food(context, 50) + Resources::iron(context, 30), 300);
        }


        // building costs

        static Cost Varoshaza(Context* context){
            return Cost(Resources::wood(context, 100), 100);
        }

        static Cost Laktanya(Context* context){
            return Cost(Resources::wood(context, 150), 200);
        }

        static Cost Loter(Context* context){
            return Cost(Resources::wood(context, 100), 200);
        }

        static Cost Istallo(Context* context){
            return Cost(Resources::wood(context, 200) +  /* LOVAS MIATT */ Resources::kp(context, 40), 300);
        }

        static Cost Akademia(Context* context){
            return Cost(Resources::wood(context, 100), 200);
        }

        static Cost Korhaz(Context* context){
            return Cost(Resources::wood(context, 80) + Resources::iron(context, 10) + Resources::gold(context, 10) + /* GYOGYITAS MIATT */  Resources::kp(context, 200), 150);
        }

        static Cost Torony(Context* context){
            return Cost(Resources::wood(context, 200) + Resources::iron(context, 20), 500);
        }

        static Cost Fal(Context* context){
            return Cost(Resources::wood(context, 40) + Resources::iron(context, 5), 80);
        }


        // improvements
        // soldiers

        static Cost Paraszt_F(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::food(context, 100) + Resources::wood(context, 50), 80);
            case 2: return Cost(Resources::food(context, 200) + Resources::iron(context, 50) + Resources::kp(context, 100), 80);
            case 3: return Cost(Resources::food(context, 300) + Resources::gold(context, 50) + Resources::kp(context, 200), 80);
            case 4: return Cost(Resources::food(context, 400) + Resources::iron(context, 50) + Resources::gold(context, 50) + Resources::kp(context, 300), 80);
            case 5: return Cost(Resources::food(context, 500) + Resources::iron(context, 100) + Resources::gold(context, 100) + Resources::kp(context, 500), 80);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Kardos_F(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::food(context, 50) + Resources::iron(context, 50) + Resources::kp(context, 50), 90);
            case 2: return Cost(Resources::food(context, 100) + Resources::iron(context, 100) + Resources::kp(context, 150), 90);
            case 3: return Cost(Resources::food(context, 150) + Resources::iron(context, 200) + Resources::kp(context, 250), 90);
            case 4: return Cost(Resources::food(context, 200) + Resources::iron(context, 300) + Resources::kp(context, 350), 90);
            case 5: return Cost(Resources::food(context, 300) + Resources::iron(context, 500) + Resources::kp(context, 450), 90);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Ijasz_F(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::food(context, 100) + Resources::wood(context, 50) + Resources::kp(context, 80), 90);
            case 2: return Cost(Resources::food(context, 150) + Resources::wood(context, 100) + Resources::kp(context, 180), 90);
            case 3: return Cost(Resources::food(context, 200) + Resources::wood(context, 200) + Resources::gold(context, 20) + Resources::kp(context, 280), 90);
            case 4: return Cost(Resources::food(context, 250) + Resources::wood(context, 300) + Resources::gold(context, 50) + Resources::kp(context, 380), 90);
            case 5: return Cost(Resources::food(context, 300) + Resources::wood(context, 400) + Resources::gold(context, 100) + Resources::kp(context, 480), 90);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Lovas_F(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(Resources::kp(context, 40));
            case 1: return Cost(Resources::food(context, 150) + Resources::iron(context, 50) + Resources::kp(context, 80), 90);
            case 2: return Cost(Resources::food(context, 200) + Resources::gold(context, 50) + Resources::kp(context, 160), 90);
            case 3: return Cost(Resources::food(context, 250) + Resources::iron(context, 50) + Resources::gold(context, 50) + Resources::kp(context, 230), 90);
            case 4: return Cost(Resources::food(context, 300) + Resources::iron(context, 100) + Resources::gold(context, 150) + Resources::kp(context, 300), 90);
            case 5: return Cost(Resources::food(context, 500) + Resources::iron(context, 200) + Resources::gold(context, 300) + Resources::kp(context, 380), 90);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Puskas_F(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(Resources::kp(context, 110));
            case 1: return Cost(Resources::food(context, 150) + Resources::iron(context, 50) + Resources::kp(context, 170), 90);
            case 2: return Cost(Resources::food(context, 200) + Resources::gold(context, 50) + Resources::kp(context, 280), 90);
            case 3: return Cost(Resources::food(context, 250) + Resources::iron(context, 50) + Resources::gold(context, 50) + Resources::kp(context, 400), 90);
            case 4: return Cost(Resources::food(context, 300) + Resources::iron(context, 100) + Resources::gold(context, 150) + Resources::kp(context, 550), 90);
            case 5: return Cost(Resources::food(context, 500) + Resources::iron(context, 200) + Resources::gold(context, 300) + Resources::kp(context, 730), 90);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        // academic

        // KP fejlesztes!!!!!!

        static Cost Kutatas_Gyorsitas(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::wood(context, 200), 100);
            case 2: return Cost(Resources::wood(context, 300) + Resources::iron(context, 100) + Resources::gold(context, 50), 100);
            case 3: return Cost(Resources::wood(context, 500) + Resources::iron(context, 200) + Resources::gold(context, 100), 100);
            case 4: return Cost(Resources::wood(context, 1000) + Resources::iron(context, 300) + Resources::gold(context, 150), 100);
            case 5: return Cost(Resources::wood(context, 1500) + Resources::iron(context, 400) + Resources::gold(context, 200), 100);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Epulet_Vedelem(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::wood(context, 100) + Resources::iron(context, 100) + Resources::kp(context, 20), 200);
            case 2: return Cost(Resources::wood(context, 200) + Resources::iron(context, 200) + Resources::kp(context, 100), 350);
            case 3: return Cost(Resources::wood(context, 600) + Resources::iron(context, 400) + Resources::kp(context, 500), 500);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Fal_Vedelem(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::wood(context, 50) + Resources::iron(context, 50) + Resources::kp(context, 100), 100);
            case 2: return Cost(Resources::wood(context, 100) + Resources::iron(context, 100) + Resources::kp(context, 250), 150);
            case 3: return Cost(Resources::wood(context, 150) + Resources::iron(context, 150) + Resources::kp(context, 400), 200);
            case 4: return Cost(Resources::wood(context, 200) + Resources::iron(context, 200) + Resources::kp(context, 550), 350);
            case 5: return Cost(Resources::wood(context, 250) + Resources::iron(context, 250) + Resources::kp(context, 700), 350);
            case 6: return Cost(Resources::wood(context, 300) + Resources::iron(context, 300) + Resources::kp(context, 850), 500);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Torony_Vedelem(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::wood(context, 200) + Resources::iron(context, 50) + Resources::kp(context, 140), 500);
            case 2: return Cost(Resources::wood(context, 400) + Resources::iron(context, 100) + Resources::kp(context, 300), 500);
            case 3: return Cost(Resources::wood(context, 600) + Resources::iron(context, 150) + Resources::kp(context, 650), 500);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Epulet_Rombolas(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::wood(context, 200) + Resources::iron(context, 200) + Resources::gold(context, 200) + Resources::kp(context, 200), 300);
            case 2: return Cost(Resources::wood(context, 250) + Resources::iron(context, 250) + Resources::gold(context, 250) + Resources::kp(context, 350), 400);
            case 3: return Cost(Resources::wood(context, 300) + Resources::iron(context, 300) + Resources::gold(context, 300) + Resources::kp(context, 500), 500);
            case 4: return Cost(Resources::wood(context, 350) + Resources::iron(context, 350) + Resources::gold(context, 350) + Resources::kp(context, 700), 600);
            case 5: return Cost(Resources::wood(context, 400) + Resources::iron(context, 400) + Resources::gold(context, 400) + Resources::kp(context, 800), 700);
            case 6: return Cost(Resources::wood(context, 450) + Resources::iron(context, 450) + Resources::gold(context, 450) + Resources::kp(context, 900), 800);
            case 7: return Cost(Resources::wood(context, 500) + Resources::iron(context, 500) + Resources::gold(context, 500) + Resources::kp(context, 1000), 900);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Latotavolsag(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::food(context, 100) + Resources::wood(context, 100) + Resources::iron(context, 100) + Resources::gold(context, 100) + Resources::kp(context, 50), 50);
            case 2: return Cost(Resources::food(context, 200) + Resources::wood(context, 200) + Resources::iron(context, 200) + Resources::gold(context, 200) + Resources::kp(context, 200), 100);
            case 3: return Cost(Resources::food(context, 500) + Resources::wood(context, 500) + Resources::iron(context, 500) + Resources::gold(context, 500) + Resources::kp(context, 600), 200);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Inteligencia(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::food(context, 100) + Resources::wood(context, 100) + Resources::iron(context, 100) + Resources::gold(context, 100) + Resources::kp(context, 250), 400);
            case 2: return Cost(Resources::food(context, 100) + Resources::wood(context, 100) + Resources::iron(context, 100) + Resources::gold(context, 100) + Resources::kp(context, 420), 400);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Korhaz_F(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(Resources::kp(context, 200));
            case 1: return Cost(Resources::food(context, 50) + Resources::wood(context, 50) + Resources::iron(context, 100) + Resources::gold(context, 100) + Resources::kp(context, 350), 300);
            case 2: return Cost(Resources::food(context, 100) + Resources::wood(context, 100) + Resources::iron(context, 150) + Resources::gold(context, 150) + Resources::kp(context, 400), 400);
            case 3: return Cost(Resources::food(context, 150) + Resources::wood(context, 150) + Resources::iron(context, 200) + Resources::gold(context, 200) + Resources::kp(context, 500), 500);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Populacio(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::food(context, 100) + Resources::kp(context, 100), 50);
            case 2: return Cost(Resources::food(context, 200) + Resources::iron(context, 20) + Resources::gold(context, 30) + Resources::kp(context, 200), 50);
            case 3: return Cost(Resources::food(context, 300) + Resources::iron(context, 40) + Resources::gold(context, 60) + Resources::kp(context, 300), 100);
            case 4: return Cost(Resources::food(context, 400) + Resources::iron(context, 80) + Resources::gold(context, 100) + Resources::kp(context, 400), 160);
            case 5: return Cost(Resources::food(context, 500) + Resources::iron(context, 100) + Resources::gold(context, 120) + Resources::kp(context, 500), 220);
            case 6: return Cost(Resources::food(context, 500) + Resources::iron(context, 120) + Resources::gold(context, 140) + Resources::kp(context, 600), 370);
            case 7: return Cost(Resources::food(context, 500) + Resources::iron(context, 140) + Resources::gold(context, 160) + Resources::kp(context, 700), 480);
            case 8: return Cost(Resources::food(context, 500) + Resources::iron(context, 160) + Resources::gold(context, 180) + Resources::kp(context, 800), 580);
            case 9: return Cost(Resources::food(context, 500) + Resources::iron(context, 180) + Resources::gold(context, 200) + Resources::kp(context, 900), 700);
            default:
                // ??
                return Cost(context, -1);
            }
        }


        // termeles_fejlesztes

        static Cost Kaja_Termeles(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::wood(context, 50) + Resources::iron(context, 10) + Resources::gold(context, 10) + Resources::kp(context, 40), 150);
            case 2: return Cost(Resources::food(context, 50) + Resources::wood(context, 100) + Resources::iron(context, 20) + Resources::gold(context, 20) + Resources::kp(context, 80), 150);
            case 3: return Cost(Resources::food(context, 100) + Resources::wood(context, 150) + Resources::iron(context, 40) + Resources::gold(context, 40) + Resources::kp(context, 140), 150);
            case 4: return Cost(Resources::food(context, 100) + Resources::wood(context, 200) + Resources::iron(context, 60) + Resources::gold(context, 60) + Resources::kp(context, 200), 150);
            case 5: return Cost(Resources::food(context, 100) + Resources::wood(context, 200) + Resources::iron(context, 80) + Resources::gold(context, 80) + Resources::kp(context, 320), 150);
            case 6: return Cost(Resources::food(context, 100) + Resources::wood(context, 200) + Resources::iron(context, 80) + Resources::gold(context, 100) + Resources::kp(context, 480), 150);
            case 7: return Cost(Resources::food(context, 100) + Resources::wood(context, 200) + Resources::iron(context, 80) + Resources::gold(context, 150) + Resources::kp(context, 620), 150);
            case 8: return Cost(Resources::food(context, 100) + Resources::wood(context, 200) + Resources::iron(context, 80) + Resources::gold(context, 200) + Resources::kp(context, 850), 150);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Fa_Termeles(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::food(context, 50) + Resources::kp(context, 60), 180);
            case 2: return Cost(Resources::food(context, 100) + Resources::wood(context, 50) + Resources::iron(context, 20) + Resources::gold(context, 20) + Resources::kp(context, 120), 180);
            case 3: return Cost(Resources::food(context, 100) + Resources::wood(context, 100) + Resources::iron(context, 40) + Resources::gold(context, 40) + Resources::kp(context, 200), 180);
            case 4: return Cost(Resources::food(context, 100) + Resources::wood(context, 100) + Resources::iron(context, 60) + Resources::gold(context, 60) + Resources::kp(context, 280), 180);
            case 5: return Cost(Resources::food(context, 100) + Resources::wood(context, 100) + Resources::iron(context, 80) + Resources::gold(context, 80) + Resources::kp(context, 360), 180);
            case 6: return Cost(Resources::food(context, 100) + Resources::wood(context, 100) + Resources::iron(context, 80) + Resources::gold(context, 100) + Resources::kp(context, 440), 180);
            case 7: return Cost(Resources::food(context, 100) + Resources::wood(context, 100) + Resources::iron(context, 80) + Resources::gold(context, 150) + Resources::kp(context, 520), 180);
            case 8: return Cost(Resources::food(context, 100) + Resources::wood(context, 100) + Resources::iron(context, 80) + Resources::gold(context, 200) + Resources::kp(context, 600), 180);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Vas_Termeles(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::food(context, 50) + Resources::wood(context, 50) + Resources::kp(context, 100), 200);
            case 2: return Cost(Resources::food(context, 100) + Resources::wood(context, 100) + Resources::iron(context, 20) + Resources::gold(context, 20) + Resources::kp(context, 220), 200);
            case 3: return Cost(Resources::food(context, 150) + Resources::wood(context, 150) + Resources::iron(context, 40) + Resources::gold(context, 40) + Resources::kp(context, 350), 200);
            case 4: return Cost(Resources::food(context, 200) + Resources::wood(context, 200) + Resources::iron(context, 40) + Resources::gold(context, 80) + Resources::kp(context, 480), 200);
            case 5: return Cost(Resources::food(context, 250) + Resources::wood(context, 250) + Resources::iron(context, 40) + Resources::gold(context, 120) + Resources::kp(context, 600), 200);
            case 6: return Cost(Resources::food(context, 250) + Resources::wood(context, 250) + Resources::iron(context, 40) + Resources::gold(context, 160) + Resources::kp(context, 720), 200);
            case 7: return Cost(Resources::food(context, 250) + Resources::wood(context, 250) + Resources::iron(context, 40) + Resources::gold(context, 200) + Resources::kp(context, 840), 200);
            case 8: return Cost(Resources::food(context, 250) + Resources::wood(context, 250) + Resources::iron(context, 40) + Resources::gold(context, 300) + Resources::kp(context, 1000), 200);
            default:
                // ??
                return Cost(context, -1);
            }
        }

        static Cost Arany_Termeles(Context* context, int level){
            switch (level)
            {
            case 0: return Cost(context);
            case 1: return Cost(Resources::food(context, 100) + Resources::wood(context, 100) + Resources::kp(context, 120), 250);
            case 2: return Cost(Resources::food(context, 150) + Resources::wood(context, 150) + Resources::iron(context, 30) + Resources::gold(context, 20) + Resources::kp(context, 220), 250);
            case 3: return Cost(Resources::food(context, 200) + Resources::wood(context, 200) + Resources::iron(context, 60) + Resources::gold(context, 40) + Resources::kp(context, 320), 250);
            case 4: return Cost(Resources::food(context, 250) + Resources::wood(context, 250) + Resources::iron(context, 100) + Resources::gold(context, 50) + Resources::kp(context, 420), 250);
            case 5: return Cost(Resources::food(context, 300) + Resources::wood(context, 300) + Resources::iron(context, 150) + Resources::gold(context, 50) + Resources::kp(context, 520), 250);
            case 6: return Cost(Resources::food(context, 300) + Resources::wood(context, 350) + Resources::iron(context, 200) + Resources::gold(context, 50) + Resources::kp(context, 620), 250);
            case 7: return Cost(Resources::food(context, 300) + Resources::wood(context, 400) + Resources::iron(context, 250) + Resources::gold(context, 50) + Resources::kp(context, 720), 250);
            case 8: return Cost(Resources::food(context, 300) + Resources::wood(context, 500) + Resources::iron(context, 300) + Resources::gold(context, 50) + Resources::kp(context, 820), 250);
            default:
                // ??
                return Cost(context, -1);
            }
        }
    };


}
