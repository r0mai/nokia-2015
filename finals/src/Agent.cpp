#include "Agent.hpp"
#include "Utasitas.h"

#include <iostream>
#include <vector>
#include <string>

using namespace calmare;

void eraseOrders() {
    std::cerr << "Erasing orders" << std::endl;
    koteg.UtasitSzam = 0;
}

void sendUnitTo(int x, int y, const TEgyseg& unit) {
    if(unit.AkcioKod == caNincs) {
        std::cerr<<"Unit " << unit.ID << " was idle, sending to "<<x << " "<< y<<std::endl;
        Utasit_Mozog(unit.ID, x, y);
    } else {
        std::cerr<<"Unit " << unit.ID << " was already in movement"<<std::endl;

    }
}

TKoteg Agent::getOrders(TJatekos jatekos) {
    eraseOrders();
    std::cerr << "Working with " << jatekos.EgySzam << " units" << std::endl;
    short myOnlySon = jatekos.Egysegek[0].ID;
    sendUnitTo(96, 97, jatekos.Egysegek[0]);
    return koteg;
}

char unitToChar(int type) {
    switch (type) {
        case ceKardos: return 'k';
        case ceParaszt: return 'p';
        case ceIjasz: return 'i';
        case ceLovas: return 'l';
        case cePuskas: return 'r'; // as in rifle
    }
    return '\\';
}

char fieldToChar(int type) {
    switch (type) {
        case cvMezo: return ' ';
        case cvKo: return 'o';
        case cvKaja: return 'K';
        case cvFa: return 'F';
        case cvVasBanya: return 'V';
        case cvAranyBanya: return 'G';
        case cvFohaz: return 'H';
        case cvLaktanya: return 'L';
        case cvLoter: return 'T';
        case cvIstallo: return 'I';
        case cvAkademia: return 'A';
        case cvKorhaz: return 'K';
        case cvTorony: return 'R';
        case cvFal: return 'W';
        case cvNemLatszik: return '?';
    }
    return '\\';
}
void logMap(const TJatekos& jatekos, std::ostream& os) {
    std::vector<std::string> lines(jatekos.YMax, std::string(jatekos.XMax, ' '));
    for (int y = 0; y < jatekos.YMax; ++y) {
        for (int x = 0; x < jatekos.XMax; ++x) {
            char ch = fieldToChar(jatekos.Vilag[y][x].Objektum);
            lines[y][x] = ch;
        }
    }
    for (int i = 0; i < jatekos.EgySzam; ++i) {
        int x = jatekos.Egysegek[i].X;
        int y = jatekos.Egysegek[i].Y;
        char ch = unitToChar(jatekos.Egysegek[i].Tipus);
        lines[y][x] = ch;
    }
    for (const auto& line : lines) {
        std::cout << line << '\n';
    }
    std::cout << std::endl;
}
