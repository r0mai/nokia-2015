/*
 * Utasitas deklaracio.
 */

#pragma once

#include "VilagDef.h"

//----------------------------------------------------------------

extern TKoteg koteg; // ebbe gyujtjuk az utasitasokat


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
void Utasit_Epit(int tipus, int x, int y);

// Epulet javitasa
//   id: Epulet ID-je
// Lehetseges hibauzenetek
//   chNincsID Nincs ilyen ID-ju epulet
//   chEroforras Nincs eleg eroforras
// Megjegyzes: A javitashoz szukseges ido es nyersanyag aranyos a karral.
// Ha kozben tovabb romboljak, attol a javitas meg az eredeti tervnek megfelelo sebesseggel es
// mertekben folyik.
void Utasit_Javit(short id);

// Epulet bontasa
//   id: Epulet ID-je
// Lehetseges hibauzenetek
//   chNincsID Nincs ilyen ID-ju epulet.
// Megjegyzes: A bontashoz fele annyi ido kell, mint az epiteshez.
// Max. 150 epuletunk lehet. Ha nem tudjuk megvedeni, es lebontjuk, akkor az ellenfel nem kap
// pontot a rombolasert, es mi sem veszitunk.
void Utasit_Bont(short id);

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
void Utasit_Kutat(short tipus, short nyersanyag);

// Fejlesztesek az akademian
//   fejlKod: Fejlesztes kodja. cfXXXX
// Lehetseges hibauzenetek
//   chNincsID Nincs akademia
//   chEpul Meg epul az epulet.
//   chFoglalt Megy valamilyen fejlesztes az akademian.
//   chEroforras Nincs eleg eroforras
//   chInvalid Ervenytelen fejlesztes kod, vagy max. szinten van.
// Megjegyzes: Csak egy mukodo akademiaja lehet minden birodalomnak.
void Utasit_Fejleszt(int fejlKod);

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
void Utasit_Kepez(int tipus, short id);

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
void Utasit_Mozog(short id, int x, int y);

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
void Utasit_Termel(short id, int x, int y);

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
void Utasit_Jaror(short id, int x, int y);

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
void Utasit_Tamad(short id, short ellenfelID, int ellenfelSzin);

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
void Utasit_Prefer(short id, int prefKod);

// Elkuld egy egyseget
//   id: Egyseg ID-je.
// Lehetseges hibauzenetek
//   chNincsID Nincs ilyen ID-ju egyseg.
// Ha pl. tulsagosan sok parasztunk van, es nem tudunk toluk katonat kepezni, akkor
// megszabadulhatunk toluk.
void Utasit_Elkuld(short id);

// Gyulekezo pont egy epulethez 
//   id: Epulet ID-je.
//   x, y: koordinata
// Lehetseges hibauzenetek
//   chNincsID Nincs ilyen ID-ju epulet.
// Ha elkeszul egy egyseg az epuletben, akkor kilep, es automatikusan elindul a gyulekezo pont fele.
// Ha nem tud odamenni, akkor hibauzenet nelkul megall.
void Utasit_Gyulekezo(short id, int x, int y);

