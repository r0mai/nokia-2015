/*
* Utasitas definicio.
*/

#include <iostream>

#include "VilagDef.h"

using namespace std;

TKoteg koteg;
TUtasitas utasitas;

void UtasitasFel(TUtasitas u) {
    if (koteg.UtasitSzam < cMaxUtasitas)
        koteg.Utasitasok[koteg.UtasitSzam++] = u;
    else cout << "ERROR: tul sok utasitas" << endl;
}


// Epitkezes
//   tipus: Epulet tipusa, cvXXXX
//   x, y: Ide szeretne epiteni
// Lehetseges hibauzenetek
//   chEroforras Nincs eleg eroforras
//   chEpitLat Felderitetlen helyre akar epitkezni.
//   chEpitHely Az epitkezes kornyeken van valami zavaro objektum
//   chEpitFog All ott valaki
//   chKPHiany Nincs meg a szukseges KP
// Megjegyzes: 3*3as ures terulet kozepere lehet epitkezni.
// A falat kiveve, amit lehet ko vagy fa melle is epiteni.
void Utasit_Epit(int tipus, int x, int y)
{
    utasitas.Kod =cuEpit ;
    utasitas.Kieg = tipus; 
    utasitas.X = x;
    utasitas.Y = y;
    UtasitasFel(utasitas);
}

// Epulet javitasa
//   id: Epulet ID-je
// Lehetseges hibauzenetek
//   chNincsID Nincs ilyen ID-ju epulet
//   chEroforras Nincs eleg eroforras
// Megjegyzes: A javitashoz szukseges ido es nyersanyag aranyos a karral.
// Ha kozben tovabb romboljak, attol a javitas meg az eredeti tervnek megfelelo sebesseggel es
// mertekben folyik.
void Utasit_Javit(short id)
{
    utasitas.Kod = cuJavit;
    utasitas.ID = id;
    UtasitasFel(utasitas);
}

// Epulet bontasa
//   id: Epulet ID-je
// Lehetseges hibauzenetek
//   chNincsID Nincs ilyen ID-ju epulet.
// Megjegyzes: A bontashoz fele annyi ido kell, mint az epiteshez.
// Max. 150 epuletunk lehet. Ha nem tudjuk megvedeni, es lebontjuk, akkor az ellenfel nem kap
// pontot a rombolasert, es mi sem veszitunk.
void Utasit_Bont(short id)
{
    utasitas.Kod = cuBont;
    utasitas.ID = id;
    UtasitasFel(utasitas);
}

// Alapkutatas az akademian
//   tipus: Milyen nyersanyagot akar hasznalni. caXXXX
//   nyersanyag: Mennyi nyersanyagot hasznal fel.
// Lehetseges hibauzenetek
//   chNincsID Nincs akademia
//   chEpul Meg epul az epulet.
//   chFoglalt Megy valamilyen fejlesztes az akademian.
//   chEroforras Nincs eleg eroforras
// Megjegyzes: Csak egy mukodo akademiaja lehet minden birodalomnak. Ha azt leromboljak,
// akkor a folyamatban levo fejlesztes elveszik, de ami keszen van, az megmarad. 
// A nyersanyag tipusa az utasitas rekord ID mezojebe kerul, a mennyisege pedig az ID2-be.
void Utasit_Kutat(short tipus, short nyersanyag)
{
    utasitas.Kod = cuFejleszt;
    utasitas.Kieg = cfKutat; 
    utasitas.ID = tipus;
    utasitas.ID = nyersanyag;
    UtasitasFel(utasitas);
}

// Fejlesztesek az akademian
//   fejlKod: Fejlesztes kodja. cfXXXX
// Lehetseges hibauzenetek
//   chNincsID Nincs akademia
//   chEpul Meg epul az epulet.
//   chFoglalt Megy valamilyen fejlesztes az akademian.
//   chEroforras Nincs eleg eroforras
//   chInvalid Ervenytelen fejlesztes kod, vagy max. szinten van.
// Megjegyzes: Csak egy mukodo akademiaja lehet minden birodalomnak.
void Utasit_Fejleszt(int fejlKod)
{
    utasitas.Kod = cuFejleszt;
    utasitas.Kieg = fejlKod;    
    UtasitasFel(utasitas);
}

// Kepzes, parasztot vagy katonat gyart
//   tipus: Egyseg tipusa, ceXXXX
//   id: Epulet ID-je
// Lehetseges hibauzenetek
//   chNincsID Nincs ilyen ID-ju epulet.
//   chEpul Meg epul az epulet.
//   chFoglalt Varolistan is van mar egyseg. Az eppen kepzett egysegen kivul meg egy kepzesere adhatunk ki parancsot.
//   chNemKepez Az adott epuletben nem kepezheto ilyen egyseg
//   chEroforras Nincs eleg eroforras
//   chKPHiany Nincs meg a szukseges KP
void Utasit_Kepez(int tipus, short id)
{
    utasitas.Kod = cuKepzes;
    utasitas.Kieg = tipus; 
    utasitas.ID = id;
    UtasitasFel(utasitas);
}

// Parancs: Mozog 
//   id: Egyseg ID-je.
//   x, y: Cel koordinatai
// Lehetseges hibauzenetek
//   chNincsID Nincs ilyen ID-ju egyseg.
//   chMozogHely Ki akar menni a vilagbol.
//   chMozogLat Nem lathato a cel.
// A parancs hatasara az egyseg viselkedese cviMozog lesz. 
// Odamegy a megadott koordinatara, ha felderitett a cel es van oda vezeto ut. Ha odaert, akkor
// Katona cviAllVed modba valt, Paraszt cviNincs modba, azaz lazsalni kezd. :) Ha menet kozben
// elzarodik az ut, akkor az egyseg hibauzenet nelkul megall.
void Utasit_Mozog(short id, int x, int y)
{
    utasitas.Kod = cuParancs;
    utasitas.Kieg = cviMozog; 
    utasitas.X = x;
    utasitas.Y = y;
    utasitas.ID = id;
    UtasitasFel(utasitas);
}

// Parancs: Termel
//   id: Egyseg ID-je.
//   x, y: Cel koordinatai
// Lehetseges hibauzenetek
//   chNincsID Nincs ilyen ID-ju egyseg.
//   chMozogHely Ki akar menni a vilagbol.
//   chMozogLat Nem lathato a cel.
//   chInvalid Csak paraszttal lehet termelni. 
// A parancs hatasara az egyseg viselkedese cviTermel lesz.
// Odamegy a megadott koordinatara, ha felderitett a cel es van oda vezeto ut. Ha odaer,
// automatikusan elkezd termelni. Ha a cel nem banya, fa vagy tokfold, akkor megszunik a
// cviTermel mod.
void Utasit_Termel(short id, int x, int y)
{
    utasitas.Kod = cuParancs;
    utasitas.Kieg = cviTermel; 
    utasitas.X = x;
    utasitas.Y = y;
    utasitas.ID = id;
    UtasitasFel(utasitas);
}

// Parancs: Jarorozik
//   id: Egyseg ID-je.
//   x, y: Cel koordinatai
// Lehetseges hibauzenetek
//   chNincsID Nincs ilyen ID-ju egyseg.
//   chMozogHely Ki akar menni a vilagbol.
//   chMozogLat Nem lathato a cel.
//   chInvalid Csak katonaval lehet jarorozni. 
// A parancs hatasara az egyseg viselkedese cviJaror lesz.
// Odamegy a megadott koordinatara, ha felderitett a cel es van oda vezeto ut. Ha odaer, akkor
// visszaindul oda, ahol a parancsot kapta. Ha kozben ellenfelet lat, akkor megtamadja. Jarorozes
// kozben a caLep es caTamas akciokat magatol inditja a rendszer. Ha szeretne utni valakit, de nem
// tud, mert akadalyozzak, akkor caVar Akciot indit, ami egyideig tart. Azaz ott marad, es nem
// csinal semmit, de a kovetkezo idopillanatban ujra fogja ertekelni, mit tegyen.
void Utasit_Jaror(short id, int x, int y)
{
    utasitas.Kod = cuParancs;
    utasitas.Kieg = cviJaror; 
    utasitas.X = x;
    utasitas.Y = y;
    utasitas.ID = id;
    UtasitasFel(utasitas);
}

// Parancs: Tamad
//   id: Egyseg ID-je.
//   ellenfelID: Ezt akarjuk tamadni
//   ellenfelSzin: Szin, melyik jatekos ID2 egyseget akarjuk tamadni.
// Lehetseges hibauzenetek
//   chNincsID Nincs ilyen ID-ju egyseg.
// A parancs hatasara az egyseg viselkedese cviTamad lesz.
// Ha utestavolsagon belul van a celpont, akkor automatikusan indit caTamad akciokat, mig
// valamelyik egyseg meghal vagy a celpont kimegy a hatotavolsagon kivulre. Utana cviAllVed
// modba kerulnek a katonak, a parasztok pedig cviNincs-be.
// A kozvetlen tamadoparancsot csak indokol esetben erdemes kiadni. cviJaror vagy cviAllVed
// viselkedes eseten automatikusan keres a rendszer celpontot.
// A szin az Utasitas rekord X mezojebe kerul.
void Utasit_Tamad(short id, short ellenfelID, int ellenfelSzin)
{
    utasitas.Kod = cuParancs;
    utasitas.Kieg = cviTamad; 
    utasitas.X = ellenfelSzin;
    utasitas.ID = id;
    utasitas.ID2 = ellenfelID;
    UtasitasFel(utasitas);
}

// Utasit_Prefer Preferencia valtoztatas, kit szeret utni egy egyseg
//   id: Egyseg ID-je.
//   prefKod: Preferencia kod
// Lehetseges hibauzenetek
//   chNincsID Nincs ilyen ID-ju egyseg.
//   chIQNuku Nem eleg intelligens az egyseg a parancs megertesehez
//   chInvalid Ervenytelen preferencia kod
// A preferencia koddal azt adja meg, hogy kit usson meg egy egyseg, ha tobbfele ellenfelet is tudna.
// Default: Epulet, Paraszt, Utos, Lovos sorrendet jelent. Utos egyseg a Kardos es a Lovas, Lovos
// egyseg Ijasz vagy Puskas, ezen belul mar nem lehet tovabbi preferenciat megadni.
void Utasit_Prefer(short id, int prefKod)
{
    utasitas.Kod = cuPrefer;
    utasitas.Kieg = prefKod; 
    utasitas.ID = id;
    UtasitasFel(utasitas);
}

// Elkuld egy egyseget
//   id: Egyseg ID-je.
// Lehetseges hibauzenetek
//   chNincsID Nincs ilyen ID-ju egyseg.
// Ha pl. tulsagosan sok parasztunk van, es nem tudunk toluk katonat kepezni, akkor
// megszabadulhatunk toluk.
void Utasit_Elkuld(short id)
{
    utasitas.Kod = cuElkuld;
    utasitas.ID = id;
    UtasitasFel(utasitas);
}

// Gyulekezo pont egy epulethez 
//   id: Epulet ID-je.
//   x, y: koordinata
// Lehetseges hibauzenetek
//   chNincsID Nincs ilyen ID-ju epulet.
// Ha elkeszul egy egyseg az epuletben, akkor kilep, es automatikusan elindul a gyulekezo pont fele.
// Ha nem tud odamenni, akkor hibauzenet nelkul megall.
void Utasit_Gyulekezo(short id, int x, int y)
{
    utasitas.Kod = cuGyulekezo;
    utasitas.X = x;
    utasitas.Y = y;
    utasitas.ID = id;
    UtasitasFel(utasitas);
}
