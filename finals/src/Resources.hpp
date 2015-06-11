#pragma once

#include <vector>
#include <valarray>
#include <cassert>
#include <cstdint>

#include "VilagDef.h"

namespace calmare {

    using Resource = int;

    class Resources {
        // food wood iron gold kp
        std::valarray<Resource> resources;
    public:
        enum class ResourceId : std::int8_t { NO = -1, food = 0, wood, iron, gold, kp, last };

        static Resources fromJatekos(const TJatekos& jatekos) {
            return Resources({
                jatekos.Eroforras.Kaja,
                jatekos.Eroforras.Fa,
                jatekos.Eroforras.Vas,
                jatekos.Eroforras.Arany,
                jatekos.Kepesseg.KP
            });
        }

        Resources(std::initializer_list<Resource>&& values = { Resource(), Resource(), Resource(), Resource(), Resource() }) :
            resources(values) {
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

        static Resources food(Resource res) {
            return Resources({ res, Resource(), Resource(), Resource(), Resource() });
        }

        static Resources wood(Resource res) {
            return Resources({ Resource(), res, Resource(), Resource(), Resource() });
        }

        static Resources iron(Resource res) {
            return Resources({ Resource(), Resource(), res, Resource(), Resource() });
        }

        static Resources gold(Resource res) {
            return Resources({ Resource(), Resource(), Resource(), res, Resource() });
        }

        static Resources kp(Resource res) {
            return Resources({ Resource(), Resource(), Resource(), Resource(), res });
        }
    };

    class Cost : public Resources {
        Resource _time;

    public:
        Cost(
            Resource time = Resource(),
            std::initializer_list<Resource>&& values = { Resource(), Resource(), Resource(), Resource(), Resource() }) :
            Resources(std::move(values)), _time(time) {}

        Cost(const Resources& resources,
            Resource time = Resource()) :
            Resources(resources), _time(time) {}

        const Resource& time() const { return _time; }


        // constructor static functions -

        // units costs

        static Cost Paraszt(){
            return Cost(Resources::food(50), 150);
        }

        static Cost Kardos(){
            return Cost(Resources::food(40) + Resources::iron(20), 200);
        }

        static Cost Ijasz() {
            return Cost(Resources::food(30) + Resources::wood(20), 250);
        }

        static Cost Lovas(){
            return Cost(Resources::food(100) + Resources::gold(15), 300);
        }

        static Cost Puskas(){
            return Cost(Resources::food(50) + Resources::iron(30), 300);
        }

        static Cost Unit(Egyseg egyseg) {
            switch (egyseg) {
                case ceParaszt:
                    return Paraszt();
                case ceKardos:
                    return Kardos();
                case ceIjasz:
                    return Ijasz();
                case ceLovas:
                    return Lovas();
                case cePuskas:
                    return Puskas();
                default:
                    return Cost(-1);
            }
        }


        // building costs

        static Cost Varoshaza(){
            return Cost(Resources::wood(100), 100);
        }

        static Cost Laktanya(){
            return Cost(Resources::wood(150), 200);
        }

        static Cost Loter(){
            return Cost(Resources::wood(100), 200);
        }

        static Cost Istallo(){
            return Cost(Resources::wood(200) +  /* LOVAS MIATT */ Resources::kp(40), 300);
        }

        static Cost Akademia(){
            return Cost(Resources::wood(100), 200);
        }

        static Cost Korhaz(){
            return Cost(Resources::wood(80) + Resources::iron(10) + Resources::gold(10) + /* GYOGYITAS MIATT */  Resources::kp(200), 150);
        }

        static Cost Torony(){
            return Cost(Resources::wood(200) + Resources::iron(20), 500);
        }

        static Cost Fal(){
            return Cost(Resources::wood(40) + Resources::iron(5), 80);
        }


        // improvements
        // soldiers

        static Cost Paraszt_F(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::food(100) + Resources::wood(50), 80);
            case 2: return Cost(Resources::food(200) + Resources::iron(50) + Resources::kp(100), 80);
            case 3: return Cost(Resources::food(300) + Resources::gold(50) + Resources::kp(200), 80);
            case 4: return Cost(Resources::food(400) + Resources::iron(50) + Resources::gold(50) + Resources::kp(300), 80);
            case 5: return Cost(Resources::food(500) + Resources::iron(100) + Resources::gold(100) + Resources::kp(500), 80);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Kardos_F(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::food(50) + Resources::iron(50) + Resources::kp(50), 90);
            case 2: return Cost(Resources::food(100) + Resources::iron(100) + Resources::kp(150), 90);
            case 3: return Cost(Resources::food(150) + Resources::iron(200) + Resources::kp(250), 90);
            case 4: return Cost(Resources::food(200) + Resources::iron(300) + Resources::kp(350), 90);
            case 5: return Cost(Resources::food(300) + Resources::iron(500) + Resources::kp(450), 90);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Ijasz_F(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::food(100) + Resources::wood(50) + Resources::kp(80), 90);
            case 2: return Cost(Resources::food(150) + Resources::wood(100) + Resources::kp(180), 90);
            case 3: return Cost(Resources::food(200) + Resources::wood(200) + Resources::gold(20) + Resources::kp(280), 90);
            case 4: return Cost(Resources::food(250) + Resources::wood(300) + Resources::gold(50) + Resources::kp(380), 90);
            case 5: return Cost(Resources::food(300) + Resources::wood(400) + Resources::gold(100) + Resources::kp(480), 90);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Lovas_F(int level){
            switch (level)
            {
            case 0: return Cost(Resources::kp(40));
            case 1: return Cost(Resources::food(150) + Resources::iron(50) + Resources::kp(80), 90);
            case 2: return Cost(Resources::food(200) + Resources::gold(50) + Resources::kp(160), 90);
            case 3: return Cost(Resources::food(250) + Resources::iron(50) + Resources::gold(50) + Resources::kp(230), 90);
            case 4: return Cost(Resources::food(300) + Resources::iron(100) + Resources::gold(150) + Resources::kp(300), 90);
            case 5: return Cost(Resources::food(500) + Resources::iron(200) + Resources::gold(300) + Resources::kp(380), 90);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Puskas_F(int level){
            switch (level)
            {
            case 0: return Cost(Resources::kp(110));
            case 1: return Cost(Resources::food(150) + Resources::iron(50) + Resources::kp(170), 90);
            case 2: return Cost(Resources::food(200) + Resources::gold(50) + Resources::kp(280), 90);
            case 3: return Cost(Resources::food(250) + Resources::iron(50) + Resources::gold(50) + Resources::kp(400), 90);
            case 4: return Cost(Resources::food(300) + Resources::iron(100) + Resources::gold(150) + Resources::kp(550), 90);
            case 5: return Cost(Resources::food(500) + Resources::iron(200) + Resources::gold(300) + Resources::kp(730), 90);
            default:
                // ??
                return Cost(-1);
            }
        }

        // academic

        // KP fejlesztes!!!!!!

        static Cost Kutatas_Gyorsitas(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::wood(200), 100);
            case 2: return Cost(Resources::wood(300) + Resources::iron(100) + Resources::gold(50), 100);
            case 3: return Cost(Resources::wood(500) + Resources::iron(200) + Resources::gold(100), 100);
            case 4: return Cost(Resources::wood(1000) + Resources::iron(300) + Resources::gold(150), 100);
            case 5: return Cost(Resources::wood(1500) + Resources::iron(400) + Resources::gold(200), 100);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Epulet_Vedelem(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::wood(100) + Resources::iron(100) + Resources::kp(20), 200);
            case 2: return Cost(Resources::wood(200) + Resources::iron(200) + Resources::kp(100), 350);
            case 3: return Cost(Resources::wood(600) + Resources::iron(400) + Resources::kp(500), 500);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Fal_Vedelem(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::wood(50) + Resources::iron(50) + Resources::kp(100), 100);
            case 2: return Cost(Resources::wood(100) + Resources::iron(100) + Resources::kp(250), 150);
            case 3: return Cost(Resources::wood(150) + Resources::iron(150) + Resources::kp(400), 200);
            case 4: return Cost(Resources::wood(200) + Resources::iron(200) + Resources::kp(550), 350);
            case 5: return Cost(Resources::wood(250) + Resources::iron(250) + Resources::kp(700), 350);
            case 6: return Cost(Resources::wood(300) + Resources::iron(300) + Resources::kp(850), 500);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Torony_Vedelem(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::wood(200) + Resources::iron(50) + Resources::kp(140), 500);
            case 2: return Cost(Resources::wood(400) + Resources::iron(100) + Resources::kp(300), 500);
            case 3: return Cost(Resources::wood(600) + Resources::iron(150) + Resources::kp(650), 500);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Epulet_Rombolas(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::wood(200) + Resources::iron(200) + Resources::gold(200) + Resources::kp(200), 300);
            case 2: return Cost(Resources::wood(250) + Resources::iron(250) + Resources::gold(250) + Resources::kp(350), 400);
            case 3: return Cost(Resources::wood(300) + Resources::iron(300) + Resources::gold(300) + Resources::kp(500), 500);
            case 4: return Cost(Resources::wood(350) + Resources::iron(350) + Resources::gold(350) + Resources::kp(700), 600);
            case 5: return Cost(Resources::wood(400) + Resources::iron(400) + Resources::gold(400) + Resources::kp(800), 700);
            case 6: return Cost(Resources::wood(450) + Resources::iron(450) + Resources::gold(450) + Resources::kp(900), 800);
            case 7: return Cost(Resources::wood(500) + Resources::iron(500) + Resources::gold(500) + Resources::kp(1000), 900);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Latotavolsag(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::food(100) + Resources::wood(100) + Resources::iron(100) + Resources::gold(100) + Resources::kp(50), 50);
            case 2: return Cost(Resources::food(200) + Resources::wood(200) + Resources::iron(200) + Resources::gold(200) + Resources::kp(200), 100);
            case 3: return Cost(Resources::food(500) + Resources::wood(500) + Resources::iron(500) + Resources::gold(500) + Resources::kp(600), 200);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Inteligencia(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::food(100) + Resources::wood(100) + Resources::iron(100) + Resources::gold(100) + Resources::kp(250), 400);
            case 2: return Cost(Resources::food(100) + Resources::wood(100) + Resources::iron(100) + Resources::gold(100) + Resources::kp(420), 400);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Korhaz_F(int level){
            switch (level)
            {
            case 0: return Cost(Resources::kp(200));
            case 1: return Cost(Resources::food(50) + Resources::wood(50) + Resources::iron(100) + Resources::gold(100) + Resources::kp(350), 300);
            case 2: return Cost(Resources::food(100) + Resources::wood(100) + Resources::iron(150) + Resources::gold(150) + Resources::kp(400), 400);
            case 3: return Cost(Resources::food(150) + Resources::wood(150) + Resources::iron(200) + Resources::gold(200) + Resources::kp(500), 500);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Populacio(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::food(100) + Resources::kp(100), 50);
            case 2: return Cost(Resources::food(200) + Resources::iron(20) + Resources::gold(30) + Resources::kp(200), 50);
            case 3: return Cost(Resources::food(300) + Resources::iron(40) + Resources::gold(60) + Resources::kp(300), 100);
            case 4: return Cost(Resources::food(400) + Resources::iron(80) + Resources::gold(100) + Resources::kp(400), 160);
            case 5: return Cost(Resources::food(500) + Resources::iron(100) + Resources::gold(120) + Resources::kp(500), 220);
            case 6: return Cost(Resources::food(500) + Resources::iron(120) + Resources::gold(140) + Resources::kp(600), 370);
            case 7: return Cost(Resources::food(500) + Resources::iron(140) + Resources::gold(160) + Resources::kp(700), 480);
            case 8: return Cost(Resources::food(500) + Resources::iron(160) + Resources::gold(180) + Resources::kp(800), 580);
            case 9: return Cost(Resources::food(500) + Resources::iron(180) + Resources::gold(200) + Resources::kp(900), 700);
            default:
                // ??
                return Cost(-1);
            }
        }


        // termeles_fejlesztes

        static Cost Kaja_Termeles(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::wood(50) + Resources::iron(10) + Resources::gold(10) + Resources::kp(40), 150);
            case 2: return Cost(Resources::food(50) + Resources::wood(100) + Resources::iron(20) + Resources::gold(20) + Resources::kp(80), 150);
            case 3: return Cost(Resources::food(100) + Resources::wood(150) + Resources::iron(40) + Resources::gold(40) + Resources::kp(140), 150);
            case 4: return Cost(Resources::food(100) + Resources::wood(200) + Resources::iron(60) + Resources::gold(60) + Resources::kp(200), 150);
            case 5: return Cost(Resources::food(100) + Resources::wood(200) + Resources::iron(80) + Resources::gold(80) + Resources::kp(320), 150);
            case 6: return Cost(Resources::food(100) + Resources::wood(200) + Resources::iron(80) + Resources::gold(100) + Resources::kp(480), 150);
            case 7: return Cost(Resources::food(100) + Resources::wood(200) + Resources::iron(80) + Resources::gold(150) + Resources::kp(620), 150);
            case 8: return Cost(Resources::food(100) + Resources::wood(200) + Resources::iron(80) + Resources::gold(200) + Resources::kp(850), 150);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Fa_Termeles(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::food(50) + Resources::kp(60), 180);
            case 2: return Cost(Resources::food(100) + Resources::wood(50) + Resources::iron(20) + Resources::gold(20) + Resources::kp(120), 180);
            case 3: return Cost(Resources::food(100) + Resources::wood(100) + Resources::iron(40) + Resources::gold(40) + Resources::kp(200), 180);
            case 4: return Cost(Resources::food(100) + Resources::wood(100) + Resources::iron(60) + Resources::gold(60) + Resources::kp(280), 180);
            case 5: return Cost(Resources::food(100) + Resources::wood(100) + Resources::iron(80) + Resources::gold(80) + Resources::kp(360), 180);
            case 6: return Cost(Resources::food(100) + Resources::wood(100) + Resources::iron(80) + Resources::gold(100) + Resources::kp(440), 180);
            case 7: return Cost(Resources::food(100) + Resources::wood(100) + Resources::iron(80) + Resources::gold(150) + Resources::kp(520), 180);
            case 8: return Cost(Resources::food(100) + Resources::wood(100) + Resources::iron(80) + Resources::gold(200) + Resources::kp(600), 180);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Vas_Termeles(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::food(50) + Resources::wood(50) + Resources::kp(100), 200);
            case 2: return Cost(Resources::food(100) + Resources::wood(100) + Resources::iron(20) + Resources::gold(20) + Resources::kp(220), 200);
            case 3: return Cost(Resources::food(150) + Resources::wood(150) + Resources::iron(40) + Resources::gold(40) + Resources::kp(350), 200);
            case 4: return Cost(Resources::food(200) + Resources::wood(200) + Resources::iron(40) + Resources::gold(80) + Resources::kp(480), 200);
            case 5: return Cost(Resources::food(250) + Resources::wood(250) + Resources::iron(40) + Resources::gold(120) + Resources::kp(600), 200);
            case 6: return Cost(Resources::food(250) + Resources::wood(250) + Resources::iron(40) + Resources::gold(160) + Resources::kp(720), 200);
            case 7: return Cost(Resources::food(250) + Resources::wood(250) + Resources::iron(40) + Resources::gold(200) + Resources::kp(840), 200);
            case 8: return Cost(Resources::food(250) + Resources::wood(250) + Resources::iron(40) + Resources::gold(300) + Resources::kp(1000), 200);
            default:
                // ??
                return Cost(-1);
            }
        }

        static Cost Arany_Termeles(int level){
            switch (level)
            {
            case 0: return Cost();
            case 1: return Cost(Resources::food(100) + Resources::wood(100) + Resources::kp(120), 250);
            case 2: return Cost(Resources::food(150) + Resources::wood(150) + Resources::iron(30) + Resources::gold(20) + Resources::kp(220), 250);
            case 3: return Cost(Resources::food(200) + Resources::wood(200) + Resources::iron(60) + Resources::gold(40) + Resources::kp(320), 250);
            case 4: return Cost(Resources::food(250) + Resources::wood(250) + Resources::iron(100) + Resources::gold(50) + Resources::kp(420), 250);
            case 5: return Cost(Resources::food(300) + Resources::wood(300) + Resources::iron(150) + Resources::gold(50) + Resources::kp(520), 250);
            case 6: return Cost(Resources::food(300) + Resources::wood(350) + Resources::iron(200) + Resources::gold(50) + Resources::kp(620), 250);
            case 7: return Cost(Resources::food(300) + Resources::wood(400) + Resources::iron(250) + Resources::gold(50) + Resources::kp(720), 250);
            case 8: return Cost(Resources::food(300) + Resources::wood(500) + Resources::iron(300) + Resources::gold(50) + Resources::kp(820), 250);
            default:
                // ??
                return Cost(-1);
            }
        }
    };


}
