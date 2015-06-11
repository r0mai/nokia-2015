/*
* Jatekos definicio.
*/

#include <iostream>
#include <set>

#include "VilagDef.h"
#include "Utasitas.h"

using namespace std;

//----------------------------------------------------------------

// Bekeidoben termeszetesen csak a sajat terfelunkon mozoghatunk.
bool ideFelderithet(int x, int y, TJatekos &jatekos)
{
    return (x >= 0) && (y >= 0) && (x < cMaxX) && (y < cMaxY) &&
        (jatekos.Vilag[y][x].Objektum == cvMezo || jatekos.Vilag[y][x].Objektum == cvKaja || 
        jatekos.Vilag[y][x].Objektum == cvFa || jatekos.Vilag[y][x].Objektum == cvVasBanya || jatekos.Vilag[y][x].Objektum == cvAranyBanya);
}

// A koordinata tipusa cvMezo.
bool ideEpithet(int x, int y, TJatekos &jatekos)
{
    return (x >= 0) && (y >= 0) && (x < cMaxX) && (y < cMaxY) && (jatekos.Vilag[y][x].Objektum == cvMezo);
}

//----------------------------------------------------------------

// Ez a DemoPlayer nem csinal semmit, nem kuld semmilyen utasitast a szervernek.
TKoteg DemoPlayer_0(TJatekos jatekos) {
    // Nullazuk az utasitasszamot.
    koteg.UtasitSzam=0; 
    // Visszaterunk az utasitaskoteggel. (a koteg az Utasitas.h-ban van deklaralva
    return koteg; // a 'koteg' valtozo 
}

//----------------------------------------------------------------

// Ez a DemoPlayer az egyetlen parasztjaval random lepked a terkepen.
TKoteg DemoPlayer_1(TJatekos jatekos) {
    koteg.UtasitSzam=0;
    // Az egyetlen paraszttal random lepked.
    if (jatekos.EgySzam > 0)
    {
        if(jatekos.Egysegek[0].AkcioKod==caNincs) {
            do {
                int ujX = jatekos.Egysegek[0].X + (rand() % 11 - 5); 
                int ujY = jatekos.Egysegek[0].Y + (rand() % 11 - 5);
                if (ideFelderithet(ujX,ujY,jatekos)) {
                    // Bekeidoben termeszetesen csak a sajat terfelunkon mozoghatunk.
                    Utasit_Mozog(jatekos.Egysegek[0].ID, ujX, ujY);
                    break;
                }
            }
            while(true);
        }
    }
    return koteg;
}

//----------------------------------------------------------------

// Ez a DemoPlayer kezdetben parasztokat termel es random felderit a terkepen.
// Ha valamelyik paraszt talal tokfoldet, akkor ott elkezd kajat termelni 4 paraszttal.
// Majd tovabbi 2 paraszttal elkezd fat vagni, a tobbi paraszt pedig tovabbra is random felderit.
// 5000 ido utan megprobalunk loteret epiteni (ha van ra nyersanyag), es parasztok helyett ijaszokat termelunk a tovabbiakban.
// A parasztok termeszetesen tovabbra is random lepkednek a terkepen.

int Uzemmod = 1;         // Ezzel jelezzuk, hogy mit akarunk eppen csinalni (ez egyfajta kodolasi strategia, sok egyeb jobb strategiat is ki lehet talalni).
// 1: Parasztokat termelunk es elkuldjuk oket random felderiteni.
// 2: Ha talalunk a terkepen tokfoldet, akkor az elso 4 parasztot odaelkuldjuk termelni.
// 3: A kovetkezo 2 parasztot elkuldjuk fat vagni, ha talaltunk a terkepen fat, ezen feluli parasztok pedig random felderitenek.
// 4: Ha 5000 idonel jarunk, akkor megprobalunk loteret epiteni.
// 5: Ha mar van loterunk, akkor folyamatosan ijaszt gyartunk. Parasztot mar nem.

bool vanTokfold = false; // Ha talalunk a terkepen tokfoldet akkor ezzel jelezzuk.
int tokfoldX, tokfoldY;
bool vanFa = false;      // Ha talalunk a terkepen fat akkor ezzel jelezzuk.
int faX, faY;
bool vanLoter = false;   // Ha van loter akkor ezzel jelezzuk.

set<int> tokfoldParasztok;
set<int> faParasztok;

TKoteg DemoPlayer_2(TJatekos jatekos) {
    koteg.UtasitSzam=0;

    // Paraszt termeles a fohazban.
    if (jatekos.EpSzam > 0 && Uzemmod < 4)
    {
        // Melyik fohazban tudunk parasztot termelni:
        int fohazIndex = -1;
        for (int i = 0; i < jatekos.EpSzam; i++) {
            if (jatekos.Epuletek[i].Tipus == cvFohaz && jatekos.Epuletek[i].AkcioKod == caNincs) { fohazIndex = i; break; }
        }
        // Ha van eleg nyersanyag akkor parasztot termelunk (paraszthoz csak kaja kell).
        if (fohazIndex >= 0 && jatekos.Eroforras.Kaja >= EgyTipusok[ceParaszt].Kaja)
            Utasit_Kepez(ceParaszt, jatekos.Epuletek[fohazIndex].ID);
    }

    // Megnezzuk, hogy kell-e uzemmodot valtani.
    if (!vanTokfold)
    {
        for (int x = 0; x < cMaxX; x++)
            for (int y = 0; y < cMaxY; y++)
                if (jatekos.Vilag[y][x].Objektum == cvKaja)
                {
                    vanTokfold = true;
                    tokfoldX = x;
                    tokfoldY = y;
                    Uzemmod = 2;
                }
    }

    if (!vanFa)
    {
        for (int x = 0; x < cMaxX; x++)
            for (int y = 0; y < cMaxY; y++)
                if (jatekos.Vilag[y][x].Objektum == cvFa)
                {
                    vanFa = true;
                    faX = x;
                    faY = y;
                }
    }

    if (tokfoldParasztok.size() == 4 && vanFa) Uzemmod = 3;

    // 5000 ido utan megprobalunk loteret epiteni.
    if (jatekos.Ido == 5000) Uzemmod = 4;

    // Ha mar megepult a loter, akkor nyilvan ket darab epuletunk van, de azert leellenorizzuk, hogy tenyleg van-e loter.
    if (jatekos.EpSzam >= 2)
    {
        for (int i = 0; i < jatekos.EpSzam; i++) {
            if (jatekos.Epuletek[i].Tipus == cvLoter) { vanLoter = true; break; }
        }
        if (vanLoter) Uzemmod = 5;
    }

    // Ha van tokfold akkor legalabb 4 parasztot odakuldunk termelni.
    if (Uzemmod == 2)
    {
        for (int i = 0; i < jatekos.EgySzam; i++)
        {
            if (jatekos.Egysegek[i].Tipus == ceParaszt && jatekos.Egysegek[i].AkcioKod == caNincs && tokfoldParasztok.size() < 4 && 
                !(tokfoldParasztok.count(jatekos.Egysegek[i].ID)==1 || faParasztok.count(jatekos.Egysegek[i].ID)==1))
            {
                Utasit_Termel(jatekos.Egysegek[i].ID, tokfoldX, tokfoldY);
                tokfoldParasztok.insert(jatekos.Egysegek[i].ID);
            }
        }
    }

    // Ebben az uzemmodban legalabb 2 parasztot elkuldunk fat vagni.
    if (Uzemmod == 3)
    {
        for (int i = 0; i < jatekos.EgySzam; i++)
        {
            if (jatekos.Egysegek[i].Tipus == ceParaszt && jatekos.Egysegek[i].AkcioKod == caNincs && faParasztok.size() < 2 && 
                !(tokfoldParasztok.count(jatekos.Egysegek[i].ID)==1 || faParasztok.count(jatekos.Egysegek[i].ID)==1))
            {
                Utasit_Termel(jatekos.Egysegek[i].ID, faX, faY);
                faParasztok.insert(jatekos.Egysegek[i].ID);
            }
        }
    }

    if (Uzemmod == 4)
    {
        if (jatekos.EpSzam > 0)
        {
            // Az elso epuletunk kozeleben, amely egy fohaz, elkezdunk loteret epiteni.
            if (jatekos.Epuletek[0].Tipus == cvFohaz)
            {
                bool talalt = false;
                for (int x = jatekos.Epuletek[0].X - 10; x < jatekos.Epuletek[0].Y + 10; x++)
                {
                    for (int y = jatekos.Epuletek[0].Y - 10; y < jatekos.Epuletek[0].Y + 10; y++)
                    {
                        // Keresunk egy 3x3-as reszt a terkepen, amely mindegyike cvMezo tipusu.
                        if (ideEpithet(x - 1, y + 1, jatekos) && ideEpithet(x, y + 1, jatekos) && ideEpithet(x + 1, y + 1, jatekos) &&
                            ideEpithet(x - 1, y, jatekos) && ideEpithet(x, y, jatekos) && ideEpithet(x + 1, y, jatekos) &&
                            ideEpithet(x - 1, y - 1, jatekos) && ideEpithet(x, y - 1, jatekos) && ideEpithet(x + 1, y - 1, jatekos))
                        {
                            Utasit_Epit(cvLoter, x, y);
                            talalt = true;
                            break;
                        }
                    }
                    if (talalt) break;
                }
            }
        }
    }

    // Termelunk ijaszt
    if (Uzemmod == 5)
    {
        // Melyik loteren tudunk ijaszt termelni:
        int loterIndex = -1;
        for (int i = 0; i < jatekos.EpSzam; i++)
            if (jatekos.Epuletek[i].Tipus == cvLoter && jatekos.Epuletek[i].AkcioKod == caNincs) { loterIndex = i; break; }
        // Ha van eleg nyersanyag akkor ijaszt termelunk (ijaszhoz kaja es fa kell).
        if (loterIndex >= 0 && jatekos.Eroforras.Kaja >= EgyTipusok[ceIjasz].Kaja && jatekos.Eroforras.Fa >= EgyTipusok[ceIjasz].Fa)
            Utasit_Kepez(ceIjasz, jatekos.Epuletek[loterIndex].ID);
    }

    // A termelo parasztokat leellenorizzuk, hogy tenyleg termelnek-e. Ha nem akkor kuldjuk oket termelni.
    for (int i = 0; i < jatekos.EgySzam; i++)
    {
        if (jatekos.Egysegek[i].Viselkedes != cviTermel && tokfoldParasztok.count(jatekos.Egysegek[i].ID)==1)
            Utasit_Termel(jatekos.Egysegek[i].ID, tokfoldX, tokfoldY);
        if (jatekos.Egysegek[i].Viselkedes != cviTermel && faParasztok.count(jatekos.Egysegek[i].ID)==1)
            Utasit_Termel(jatekos.Egysegek[i].ID, faX, faY);
    }

    // Azokat a parasztokat amelyek nem termelnek fat vagy kajat es eppen nem csinalnak semmit, azokat elkuldjuk random felderiteni.
    for (int i = 0; i < jatekos.EgySzam; i++)
    {
        if (jatekos.Egysegek[i].Tipus == ceParaszt && jatekos.Egysegek[i].AkcioKod == caNincs && !(tokfoldParasztok.count(jatekos.Egysegek[i].ID)==1 || faParasztok.count(jatekos.Egysegek[i].ID)==1))
        {
            do
            {
                // Keresunk egy random pontot ahova elsetalhat.
                int ujX = jatekos.Egysegek[i].X + (rand() % 11 - 5); 
                int ujY = jatekos.Egysegek[i].Y + (rand() % 11 - 5);
                if (!(ujX == jatekos.Egysegek[i].X && ujY == jatekos.Egysegek[i].Y))
                    if (ideFelderithet(ujX,ujY,jatekos)) {
                        // Bekeidoben termeszetesen csak a sajat terfelunkon mozoghatunk.
                        Utasit_Mozog(jatekos.Egysegek[i].ID, ujX, ujY);
                        break;
                    }
            }
            while (true);
        }
    }

    return koteg;
}



