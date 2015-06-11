/*
 * Vilag deklaracio.
 */

#pragma once

//-------------------------------------------------------------------
// A kommunikaciohoz szukseges adatszerkezeteket definialjuk itt,
// es szamos olyan konstanst, illetve kezdoerteket, ami meggyorsithatja
// a programirast.
//
// TJatekos: Minden adatot tartalmaz, amit a vilagrol tudhatunk:
//   - a terkep felfedezett resze
//   - epuleteink, egysegeink, fejleszteseink allapota es a eroforrasaink
//   - mas jatekosok egysegei, amelyeket eppen latunk.
//
// A vilag legfontosabb tulajdonsagai ezekben a tombokben vannak:
//
// EgyTipusok  : egysegek tulajdonsagai
// EpTipusok   : epuletek tulajdonsagai
// FejlTipusok : fejlesztesek koltsegei es hatasa
//
//-------------------------------------------------------------------

//----------------------------------------------------------------

#define Verzio   0  // Az esetleges kompatibilitasi kerdesek miatt...
#define Revizio  1 

#define cSebzesSzorzo  0.9   // Ennyit kap, ha mast sebez.
#define cVesztSzorzo   0.3   // Ennyit veszit, ha ot sebzik.

//----------------------------------------------------------------

// Szelsoertekek a jatekban.
enum Limitek: int
{
    cMaxX =           100,    // A vilag merete
    cMaxY =           100,
    cMaxXY =          cMaxX * cMaxY,
    cIdoLimit =       50000,  // Ennyi ideig tart egy jatszma
    cJatekosMax =     4,      // Jatekosok maximalis szama egy jatszmaban.
	cKisLepSzam =     10,     // ennyit tellik az ido beavatkozasi lehetoseg nelkul
    cMaxEgyseg =      150,    // Legfeljebb ennyi egysegunk lehet.
    cMaxEpulet =      150,    // Ennel tobb epuletet nem epithet, fal is beleszamit.
    cMasEgyseg =      cJatekosMax * cMaxEgyseg,
    cMasEpulet =      cJatekosMax * cMaxEpulet,
    cMaxUtasitas =    100,    // Egyszerre csak ennyi utasitast adhat maximum.
    cEgysegSzintMax = 5,      // Egysegeink maximalis fejlettsegi szintje.
    cFejlSzintMax =   9,      // Fejlesztesek maximalis szintje. Nem mindent lehet 9-es szintre fejleszteni.
    cUtMax =          3,      // Egy mezon 1 jatekosnak csak max. 3 egysege tud utni
    cLoMax =          10      // es max 10 egysege tud loni.
};

//----------------------------------------------------------------

//-----------------------------------------------------------
// A vilagban levo negy alapveto nyersanyag kodja: crXXXX
// Mas konstansok is vannak, amelyek az eroforrasokkal vannak kapcsolatban.
// Pl. az aranybanya vagy az aranytermeles. Ezeknek altalaban
// ugyanaz a kodjuk, mint a resourcenak.
//-----------------------------------------------------------
enum Eroforras: int
{
    crKaja =  2,
    crFa =    3,
    crVas =   4,
    crArany = 5
};

//----------------------------------------------------------------

// Egysegek tipusai.
enum Egyseg: int
{
    ceParaszt = 1,
    ceKardos =  2,
    ceIjasz =   3,
    ceLovas =   4,
    cePuskas =  5 
};

//----------------------------------------------------------------

//-----------------------------------------------------------
// A vilag egy ketdimenzios tomb, a mezok lehetseges ertekei: cvXXXX
//-----------------------------------------------------------
enum Mezo: int
{
    cvMezo =             0,  // Altalanos ures mezo, atjarhato, 3*3-as kozepen lehet epiteni.
    cvKo =               1,  // Nem atjarhato. atloni lehet felette.
    cvKaja =        crKaja,  // Csak a parasztnak atjarhato. Max. 4 paraszt termel.
    cvFa =            crFa,  // Csak a parasztnak atjarhato. Max. 2 paraszt termel.
    cvVasBanya =     crVas,  // Csak a parasztnak atjarhato. Max. 4 paraszt termel.
    cvAranyBanya = crArany,  // Csak a parasztnak atjarhato. Max. 4 paraszt termel.
    cvFohaz =            6,  // Az epuletek nem atjarhatoak, le lehet rombolni oket. Parasztot lehet venni.
    cvLaktanya =         7,  // Kardost es puskast lehet venni.
    cvLoter =            8,  // Ijaszt lehet venni.
    cvIstallo =          9,  // Lovast lehet venni.
    cvAkademia =        10,  // Fejleszesek indithatok, tobbet is lehet epiteni, de akkor is csak 1 fejlesztes megy egyszere.
    cvKorhaz =          11,  // Gyogyitja a kozeleben allokat.
    cvTorony =          12,  // Csapatokat lehet ide tenni.
    cvFal =             13,  // Nincs semmi egyeb funkcioja, csak nem atjarhato.
    cvNemLatszik =      99   // Ez csak a jatekosok terkepen lehet, a globalison minden latszik.
};

//----------------------------------------------------------------

// Tokfold epitese.
enum TokFold: int
{
    cKajaKP = 175,    // Akkor tud uj tokfoldet letrehozni egy jatekos, ha legalabb cKajaKP KP-je van.
    cKajaFa = 150     // Ennyi faba kerul.
};

//----------------------------------------------------------------

// Torony tulajdonsagok.
enum Torony: int
{
    cToronyLo =  20,  // Torony sebzes. A tornyok nem fejlesztheto tulajdonsagai.
    cToronyVar = 20   // Torony sebesseg.
};

//----------------------------------------------------------------

//-----------------------------------------------------------
// Fejlesztes kodok: cfXXXX
// Az Utasit_Fejleszt, ill. Utasit_Kutat parancsokkal lehet fejleszteseket inditani.
// Az akademian lehet megnezni, hogy allnak a fejleszteseink.
// A rendszer mindig a kovetkezo szintet fejleszti. Tehat ha most mondjuk a
// Populacio limit 2 es szintu, akkor a 3 as szint fejleszteset tudja elinditani.
//-----------------------------------------------------------
enum Fejlesztes: int
{
    cfNincs =            0,  // Eppen semmilyen fejlesztes nincs.
    cfKutat =            1,  // Alapkutatas.
    cfKaja =        crKaja,  // Kajatermeles fejlesztese.
    cfFa =            crFa,  // Fatermeles fejlesztese.
    cfVas =          crVas,  // Vastermeles fejlesztese.
    cfArany =      crArany,  // Aranytermeles fejlesztese.
    cfKutGyors =         6,  // Kutatas gyorsitasa.
    cfEpVed =            7,  // Epulet vedelem, vedoertek novelese, amivel a sebzes csokken.
    cfFalVed =           8,  // Fal vedelem, vedoertek novelese, amivel a sebzes csokken.
    cfTorony =           9,  // Torony fejlesztese.
    cfEpRombol =        10,  // Epulet vedelem, tamdoertek novelese, amivel a sebzes no.
    cfLat =             11,  // Latomezo.
    cfIQ =              12,  // Intelligencia.
    cfKorhaz =          13,  // Korhaz.
    cfPopLimit =        14,  // Populacio limit.
    cfParaszt =         15,  // Parasztot fejleszti.
    cfKardos =          16,  // kardost fejleszti       cfKardos = ceKardos + cfParaszt - 1;
    cfIjasz =           17,  // Ijaszt fejleszti
    cfLovas =           18,  // Lovast fejleszti
    cfPuskas =          19   // Puskast fejleszti
};  

//----------------------------------------------------------------

//-----------------------------------------------------------
// A TEgyseg.AkcioKod lehetseges ertekei: caXXXX
// Az utasitasaink vagy az egyseg viselkedese alapjan allitja a rendszer.
// Megnezhetjuk hogy az egysegeink eppen mit csinalnak.
//-----------------------------------------------------------
enum Akcio: int
{
    caNincs =    0,  // Semmilyen akcio nincs folyamatban.
    caLep =      1,  // at akar lepni a szomszedos mezore, es ez AkcioVege ido mulva fog neki sikerulni.
    caKaja =     2,  // Kajat termel, AkciaVege ido mulva kapja meg.
    caFa =       3,  // Fat termel, AkciaVege ido mulva kapja meg.
    caVas =      4,  // Vasat termel, AkciaVege ido mulva kapja meg.
    caArany =    5,  // Aranyat termel, AkciaVege ido mulva kapja meg.
    caTamad =    6,  // Meg akar utni vagy loni valakit, ez AkcioVege ido mulva fog sikerulni.
                     // Akkor is, ha az kozben mar ellepett onnan. Persze ha meghal kozben, akkor nem.
    caVar =      7,  // Nem tudja azt csinalni amit szeretne, ezert var a sorara. A belso szervezes hasznalja.
    caEpulet = 100   // Ha epuletet tamad azt ugy jeloljuk, hogy a szinhez ennyit ad hozza a rendszer.
};

//----------------------------------------------------------------

//-----------------------------------------------------------
// A TEgyseg.Viselkedes lehetseges ertekei: cviXXXX
// Az utasitasaink alapjan allitja a rendszer.
// Megnezhetjuk hogy az egysegeink hogyan viselkednek.
// A katona nem tud termelni.
// A paraszt nem allithato agresziv modokra. (cviAllVed vagy cviJaror nem lehet.)
// Egy ellefel egyseget parasztal is meg lehet tamadni.
//-----------------------------------------------------------
enum Viselkedes: int
{
    cviNincs =  0,  // Egyaltalan semmit nem csinal.
    cviAllVed = 1,  // Egyhelyben all, ha tud utni vagy loni, akkor megteszi.
    cviTamad =  2,  // Egy adott egyseget akar megtamadni. Ha kivul van a hatotavolsagan, akkor nem csinal semmit.  Param: Kieg: szin, ID2
    cviJaror =  3,  // Ket pont kozott oda vissza jarkal, de ha meglat egy ellenfelet azt megtamadja.
                    // Tamadasra is hasznalhato. Ha epitkezes miatt a jarorozesre hasznalt ut jarhatatlan lesz, akkor megall, es cviAllVed modba kerul.
    cviMozog =  4,  // Megy valahova, meg akkor is ha meglovik. Param: X, Y
    cviTermel = 5   // Odamegy egy banyahoz es elkezd termelni. Param: X, Y
};

//----------------------------------------------------------------

//-----------------------------------------------------------
// A TUtasitas.Kod lehetseges ertekei: cuXXXX
// Nem kell kozvetlenul hasznalni oket. Az utasitasok, pl. Utasit_Epit
// ezeket beallitjak, csak akkor van a kodokra szukseg,
// ha a korabbi utasitasainkat meg akarjuk nezni a TKoteg tombben.
//-----------------------------------------------------------
enum Utasitas: int
{
    cuEpit =      1,  // Epitkezes inditasa
    cuJavit =     2,  // Epulet javitasa
    cuBont =      3,  // Elkezdunk lebontani egy epuletet
    cuFejleszt =  4,  // Fejlesztest inditunk.
    cuKepzes =    5,  // Egy epuletben valamilyen egyseget elkezdunk kepezni.
    cuParancs =   6,  // Egy egysegnek uj parancsot adunk.
    cuPrefer =    7,  // Meg tudja valtoztani az utessorrendet, ha van hozza IQ-ja az egysegnek.
    cuElkuld =    8,  // Egy egyseget elkuldunk a seregunkbol. Pl. tul sok paraszt van, meg akarunk szabadulni toluk.
    cuGyulekezo = 9   // Az epuletben kepzett egysegek hol gyulekezzenek.
};

//----------------------------------------------------------------

//-----------------------------------------------------------
// A TUtasitas.Result lehetseges ertekei: chXXXX
// Ha nem fogadja el az utasitast a rendszer, akkor a Result ad informaciot az
// elutasitas okarol. Mivel csak a kovetkezo korben kapjuk meg, igy ha ez alapjan
// akarunk dolgozni, akkor kesleltetett lesz a rendszerunk.
// Az javasoljuk, hogy minnel kevesebb hibas utasitast adjatok ki.
// Bar pontlevonas nem jar a felesleges es hibas utasitasok kuldeseert,
// de nehezebben atlathato lesz a rendszer.
//-----------------------------------------------------------
enum Valasz: int
{
    chOk =         0,  // Az utasitast elfogadta a rendszer
    chEroforras =  1,  // Nincs eleg eroforras az utasitas vegrehajtasahoz.
    chEpitHely =   2,  // Nem indithato epitkezes a megadott helyre. (Rossz koordinata.)
    chEpitLat =    3,  // Nem lathato helyre akar epitkezni.
    chEpitFog =    4,  // Ott all valaki, ahova epiteni akar.
    chEpitTulSok = 5,  // Tul sok epuletet akar epiteni.
    chNincsID =    6,  // A hivatkozott ID-ju egyseg nem letezik.
    chNemKepez =   7,  // Nem kepzeheto a kert egyseg az adott epuletben.
    chEpul =       8,  // Nem kepzeheto egyseg az adott epuletben, mert meg epul.
    chFoglalt =    9,  // Nem indithato a kepzes, mert tele van a varolista.
    chKPHiany =   10,  // Nincs eleg KP.
    chNemIndit =  11,  // Nem indithato fejlesztes az adott epuletben.
    chMozogLat =  12,  // Nem lathato koordinatat adott meg a mozgasnal.
    chMozogHely = 13,  // ervenytelen helyre akar menni.
    chIQNuku =    14,  // Nem eleg intelligens az egyseg a parancs megertesehez.
	chFelesleges= 15,  // Olyasmit ker, ami mar amugy is ugy van.
	chBeke =      16,  // Beekeido alatt ki akar lepni
    chInvalid =   17   // Invalid parameter.
};

//-----------------------------------------------------------
// A statisztikak mutatjak, hogy mit ert el eddig a birodalmunk.
// Hasznalatuk opcionalis. Eleg, ha mindig azt teszuk, ami az adott helyzetben
// a legjobb. Akkor nem kell tudnunk, hogy pontosan hol tartunk. :)
//-----------------------------------------------------------
enum Pontok: int
{
    stScore =       0,    // 0. score  (termeles + 0.9 * sebzes - 0.3 * vesztesseg)
    stTermel =      1,    // 1. termeles score
    stSebez =       2,    // 2. katonasag, sebzes
    stVeszt =       3,    // 3. katonasag, vesztesseg
    stPop =         4,    // 4. populacio
    stParaszt =     5,    // 5. parasztok szama
    stFejleszt =    6,    // 6. fejlesztesre koltott nyersanyag
    cStOszlopMax =  stFejleszt,
    cStLep =        100,  // 100-ankent teszi el az ertekeket
    cStSorMax =     cIdoLimit / cStLep
};

//----------------------------------------------------------------

// A preferenciahoz hasznalhato kodok. Csak akkor ertik meg az egysegeink
// az utasitast, ha fejlesztettuk az intelligenciat.
enum Sorrend: int
{
    cpEPUL = 0,  // 0: Epulet, Paraszt, Utos, Lovos sorrend.
    cpELUP = 1,  // 1: Epulet, Lovos, Utos, Paraszt sorrend.
    cpLUPE = 2   // 2: Lovos, Utos, Paraszt, Epulet  sorrend.
};

//----------------------------------------------------------------

#pragma pack( push, 1 )
//-----------------------------------------------------------
// EgysegTip: Az egysegek altalanos tulajdonsagai.
// A vilagdefinicio resze.
//-----------------------------------------------------------
struct TEgysegTip
{
    int Kaja;       // bekerulesi koltseg, nyersanyagok
    int Fa;
    int Vas;
    int Arany;
    int Koltseg;    // osszesitett bekerulesi koltseg, sebzesszamolashoz
    int KepzesIdo;  // Kepzesido, ennyi ido alatt keszul el az egyseg.

	// Fejlesztheto tulajdonsagok. A fejlesztes koltsegei es feltetelei a FejlTip-ban vannak.
    int Elet[ cEgysegSzintMax + 1 ];   
    int Ut[ cEgysegSzintMax + 1 ];       // Mekkorat ut.
    int UtTav[ cEgysegSzintMax + 1 ];    // Milyen messzire. Ha 1-nel tavolabb, akkor az loves.
    int UtVar[ cEgysegSzintMax + 1 ];    // Mennyi ido alatt tud utni.
    int VedUt[ cEgysegSzintMax + 1 ];    // Vedekezo ertek utes ellen.
    int VedNyil[ cEgysegSzintMax + 1 ];  // Vedekezo ertek nyil ellen.
    int VedLo[ cEgysegSzintMax + 1 ];    // Vedekezo ertek loves ellen.
    int Sebesseg[ cEgysegSzintMax + 1 ]; // Milyen gyorsan tud a terkepen mozogni.

    TEgysegTip( ): Kaja( 0 ), Fa( 0 ), Vas( 0 ), Arany( 0 ), Koltseg( 0 ), KepzesIdo( 0 ) { }
};

//-----------------------------------------------------------
// EpuletTip: Az epuletek altalanos tulajdonsagai.
// A vilagdefinicio resze.
//
// Az epuletek tulajdonsagai az altalanos fejlesztesekkel javithatok.
// Pl. cfEpVed fejlesztes minden epulet vedoerteket noveli,
// kiveve a falat, amire a cfFalVed vonatkozik.
//-----------------------------------------------------------
struct TEpuletTip
{
    int Kaja;         // bekerulesi koltseg, nyersanyagok
    int Fa;
    int Vas;
    int Arany;
    int Koltseg;      // osszesitett bekerulesi koltseg, sebzesszamolashoz
    int EpitesIdo;    // Epitesi ido, ennyi ido alatt epul fel.
    int Elet;
    int KpLimit;      // Ennyi kutatas pont (KP) kell az epites elinditasahoz.
    int Egyseg[ 2 ];  // Ezt lehet kepezni az epuletben, ceXXXX.
	                  // Ha ketto felet lehet egy helyen, akkor a masodik is ertekes

    TEpuletTip( ): Kaja( 0 ), Fa( 0 ), Vas( 0 ), Arany( 0 ), Koltseg( 0 ), EpitesIdo( 0 ), Elet( 0 ), KpLimit( 0 ) { Egyseg[ 0 ] = 0; Egyseg[ 1 ] = 0; }
};

//-----------------------------------------------------------
// FejlTip: A fejlesztesek feltetelei, koltsegei es hatasuk.
// A vilagdefinicio resze.
//
// A fejlesztesek inditasa elott elegans dolog elenorizni, hogy
// van-e eleg nyersanyagunk, illetve a KP limitet. Meg elegansabb, ha
// a strategiank resze, hogy mikor mit akarunk fejleszteni, es sporolunk
// is ra, ha szukseges.
//
// Az egysegek fejlesztheto tulajdonsagai kulon adatszerkezetben vannak. (EgysegTip)
// A tobbi, egyszerubb fejlesztes hatasa a FejltTip.Param-ban van, de ezeket jobban
// meg lehet erteni a leirasbol. Minden fejlesztes eseten egyedi jelentese van a parametereknek.
//
//-----------------------------------------------------------
struct TFejlTip
{
    int SzMax;                            // Hany szintje van ennek a fejlesztesnek.
    int Kaja[ cFejlSzintMax + 1 ];        // Bekerulesi koltseg.
    int Fa[ cFejlSzintMax + 1 ];
    int Vas[ cFejlSzintMax + 1 ];
    int Arany[ cFejlSzintMax + 1 ];
    int FejlIdo[ cFejlSzintMax + 1 ];     // A fejleszteshez szukseges ido
    int KpLimit[ cFejlSzintMax + 1 ];     // csak akkor tudjuk elinditani a fejlesztest, ha legalabb ennyi KP-nk van. 
    int Param[ cFejlSzintMax + 1 ][ 4 ];  // Mi a fejlesztes hatasa, egyedi jelentese van.

    TFejlTip( ): SzMax( 0 ) { }
};

//----------------------------------------------------------------

// A vilag egy ketdimenzios tomb, amely mezokbol all.
//-----------------------------------------------------------
// A vilag terkepe.
// A epuletek kozvetlenul latszanak a terkepen. Az egysegek nem,
// azok X, Y koordinatajat ismerjuk, es nekunk kell odakepzelni oket.
// A debugger termeszetesen megteszi ezt helyettunk.
//-----------------------------------------------------------
struct TMezo
{
    unsigned char Objektum;  // cvXXXX
    unsigned char Szin;      // Jatekos szine, ha epulet van ott

    TMezo( ): Objektum( 0 ), Szin( 0 ) { }
};

//----------------------------------------------------------------
// Egysegeink (parasztok es katonak) adatszerkezete.
//
// Mielott kiadunk egy utasitast erdemes megnezni, hogy mit csinal eppen az egysegunk.
// Legfontosabb a Viselkedes, AkcioKod es AkcioVar.
// Nem minden allapotban van az osszes valtozo kihasznalva.
//
// Ajanlott sajat adatszerkezetet is letrehozni, hiszem a jo strategiak
// kialakitasahoz tobb informacio tarolasara lehet szukseg.
// Celszeru a sajat adatokat ID alapjan parositani a szerverben tarolt adatszerkezettel, mert
// az egyedi azonosito egy jatekoson belul. Ha meghal egy egyseg nem lesz kiosztva ujra a kodja.
//-----------------------------------------------------------
struct TEgyseg
{
    short ID;                  // Egyedi azonosito.
    unsigned char X;
    unsigned char Y;
    unsigned char Tipus;       // ceXXXX
    short Elet;                // Mennyi az aktualis elete. Ha 0 lesz, akkor meghal.
    short EloSebzes;           // Ennyit fog csokkenni az elet, ha bejonnek ra az utese,
                               // amelyek elkezdodtek. Csak tajekoztato jellegu adat !
                               // Ha pl. meghal, aki utne minket, akkor nem tud sebezni. Ha pedig
                               // eppen befejezodik egy fejlesztes, akkor megvaltozik a sebzes merteke.
    unsigned char Viselkedes;  // default viselkedes, cviXXXX
    unsigned char Prefer;      // Preferencia, hogy kit szeret utni. cpXXXX
    unsigned char AkcioKod;    // Ezt csinalja eppen az egysegunk, caXXXX
    unsigned char AkcioVege;   // Mikor lesz keszen azzal, amit csinal.
    short KitUt;               // Egyseg ID-je, akit utni akar
    unsigned char KitSzin;     // Milyen szinu egyseget ut.  epulet eseten 100 + szin
    unsigned char GyogyVege;   // Gyogyulas az egyeb tevekenyseg kozben is mehet,
    unsigned char GyogyElet;   // GyogyVege ido mulva megkapja a GyogyElet eletpontot.
    unsigned char CelX;        // Mozgas vagy termeles eseten a celmezo.
    unsigned char CelY;        // Lehet tavoli mezo is.
    unsigned char ElsoX;       // Mozgas soran ide fog kozvetlenul lepni
    unsigned char ElsoY;
    unsigned char RajtX;       // Innen indult, ha jarorozik,
    unsigned char RajtY;       // akkor vissza is fog jonni.

    TEgyseg( ): ID( 0 ), X( 0 ), Y( 0 ), Tipus( 0 ), Elet( 0 ), EloSebzes( 0 ), Viselkedes( 0 ), Prefer( 0 ), AkcioKod( 0 ), AkcioVege( 0 ), KitUt( 0 ), KitSzin( 0 ), GyogyVege( 0 ), GyogyElet( 0 ), CelX( 0 ), CelY( 0 ), ElsoX( 0 ), ElsoY( 0 ), RajtX( 0 ), RajtY( 0 ) { }
};

//----------------------------------------------------------------
// Mas jatekosok egysegei, akiket lat a terkepen. Ezekrol kevesebbet tudunk, mint a sajatunkrol.
struct TMasEgyseg
{
    short ID;             // Az egyseg ID-je.
    unsigned char X;
    unsigned char Y;
    unsigned char Szin;   // Jatekos szine.
    unsigned char Tipus;  // ceXXXX
    short Elet;           // mennyi az aktualis elete

    TMasEgyseg( ): ID( 0 ), X( 0 ), Y( 0 ), Szin( 0 ), Tipus( 0 ), Elet( 0 ) { }
};

//----------------------------------------------------------------

struct TEgyHely
{
    short Next;
    unsigned char SSzam;

    TEgyHely( ): Next( 0 ), SSzam( 0 ) { }
};

//----------------------------------------------------------------
// Epuleteink adatszerkezete.
//
// Az epuletek viselkedese nincs, akcio vagy sajat magara vonatkozik,
// mint az epites vagy a bontas, vagy arra, hogy mit csinalnak eppen az epuletben.
// Ez altalaban valamilyen egyseg kepzese, vagy az akademian egy fejlesztes.
// Akkor is ki lehet adni meg egy Utasit_Kepez utasitast, ha mar kepez valamilyen
// egyseget az epulet. Ilyenkor varolistara kerul, de rogton levonja a rendszer a
// nyersanyagot az utasitas elfogadasakor. A Varolistan csak egy egyseg lehet.
//----------------------------------------------------------------
struct TEpulet
{
    short ID;               // Az epulet ID-je.
    unsigned char X;
    unsigned char Y;
    unsigned char Tipus;    // cvXXXX-bol ami lehet epulet
    short Elet;             // mennyi az aktualis elete
    unsigned char GyulX;    // ide gyulekeznek a kepzett egysegek
    unsigned char GyulY;
    unsigned char AkcioKod; // Mit csinal az epulet. Kepzes eseten cuKepez.
    short AkcioKieg;        // Akciora vonatkozo parameter, pl. milyen egyseget kepez eppen, mit kutat
    short AkcioVege;        // Mikor lesz keszen azzal, amit csinal.
    unsigned char VaroKod;  // Ezt fogja elkezdeni, ha keszen van azzal, amit csinal
    unsigned char VaroKieg;

    TEpulet( ): ID( 0 ), X( 0 ), Y( 0 ), Tipus( 0 ), Elet( 0 ), GyulX( 0 ), GyulY( 0 ), AkcioKod( 0 ), AkcioKieg( 0 ), AkcioVege( 0 ), VaroKod( 0 ), VaroKieg( 0 ) { }
};

//----------------------------------------------------------------
// Mas jatekosok epuletei. Ezekrol kevesebbet tudunk, mint a sajatunkrol.
struct TMasEpulet
{
    short ID;             // Az epulet ID-je.
    unsigned char X;
    unsigned char Y;
    unsigned char Tipus;  // cvXXXX-bol ami lehet epulet
    short Elet;           // mennyi az aktualis elete

    TMasEpulet( ): ID( 0 ), X( 0 ), Y( 0 ), Tipus( 0 ), Elet( 0 ) { }
};

//----------------------------------------------------------------

struct TEsemeny
{
    int Sebzett;  // integer, sebzett egyseg ID je.
    int Elet;     // Maradek elet a sebzes utan.

    TEsemeny( ): Sebzett( 0 ), Elet( 0 ) { }
};

//-----------------------------------------------------------
// Az Utasitasaink adatszerkezete.
//
// A birodalmunkat utasitasok kiadasaval iranyithatjuk. Pl. Utasit_Epit
// vagy Utasit_Mozog. Nem kell kozvetlenul irni az Utasitas rekordot.
//
// A Jatekos.Utasitasok adatszerkezetben megkapjuk az elozo korben feldolgozott
// utasitasokat. Ha elfogadta az utasitast a rendszer, akkor a Result-ben 0 lesz,
// kulonben hibakod. Ha a korabban kiadott utasitasainkat ertelmezni akarjuk,
// akkor lehet szuksegunk az Utasitas rekordra.
//
//-----------------------------------------------------------
struct TUtasitas
{
    unsigned char Kod;     // caXXXX
    unsigned char Kieg;    // kiegeszito informacio.
    short ID;              // Ennek az egysegnek adunk utasitas.;   AlapKutatasnal a nyersanyag tipusa
    short ID2;             // Ha megegy egyseg szerepel, pl. ezt tamadjuk; AlapKutatasnal a nyersanyag mennyisege
    unsigned char X;       // ha koordinata is tartozik az utasitashoz.; cviTamad-nal a szin
    unsigned char Y;
    unsigned char Result;  // Ezt visszajelzesre hasznaljuk. chXXXX

    TUtasitas( ): Kod( 0 ), Kieg( 0 ), ID( 0 ), ID2( 0 ), X( 0 ), Y( 0 ), Result( 0 ) { }
};

//----------------------------------------------------------------
// A negy nyersanyag, ami ebben a vilagban letezik.
struct TEroforras
{
    int Kaja;
    int Fa;
    int Vas;
    int Arany;

    TEroforras( ): Kaja( 0 ), Fa( 0 ), Vas( 0 ), Arany( 0 ) { }
};

//----------------------------------------------------------------

struct TKepesseg
{
    short KP;                                // Ennyi kutatasi pontja van a jatekosnak.
    unsigned char Szintek[ cfPuskas + 1 ];   // A fejlesztesek szintje.  Elso ket ertek nem hasznalt.
    float KutGyors;           // ertek
    unsigned char EpVedUt;    // epulet vedelem, vedoertek novelese, amivel a sebzes csokken
    unsigned char EpVedLo;    // epulet vedelem, vedoertek novelese, amivel a sebzes csokken
    unsigned char FalVedUt;   // Fal vedelem, vedoertek novelese, amivel a sebzes csokken
    unsigned char FalVedLo;   // Fal vedelem, vedoertek novelese, amivel a sebzes csokken
    unsigned char ToronyTav;  // milyen messze lo a torony
    unsigned char EpRomUt;    // uto
    unsigned char EpRomLo;    // Lovo egysegek sebzesbonusza epulet ellen.
    unsigned char LatEgy;     // Egyseg latotavolsaga, mezo
    unsigned char LatEp;      // epulet latotavolsaga, mezo
    unsigned char LatFal;     // fal specialis, az nem lat olyan messze.
    unsigned char IQ;         // Intelligencia
    unsigned char KorhazTav;  // Korhaz hatotavolsag
    unsigned char KorhazVar;  // Korhaz varakozas gyogyitas elott
    unsigned char PopLimit;   // Populacio limit
    unsigned char Kaja;       // Kajatermeles, varakozas, mielott megkap 1 egyseg nyersit
    unsigned char Fa;         // Favagas varakozasi ideje
    unsigned char Vas;        // Vastermeles feljesztese
    unsigned char Arany;      // Aranytermeles feljesztese

    TKepesseg( ): KP( 0 ), KutGyors( 0 ), EpVedUt( 0 ), EpVedLo( 0 ), FalVedUt( 0 ), FalVedLo( 0 ), ToronyTav( 0 ), EpRomUt( 0 ), EpRomLo( 0 ), LatEgy( 0 ), LatEp( 0 ), LatFal( 0 ), IQ( 0 ), KorhazTav( 0 ), KorhazVar( 0 ), PopLimit( 0 ), Kaja( 0 ), Fa( 0 ), Vas( 0 ), Arany( 0 ) {}
};

//-----------------------------------------------------------
// TJatekos: Minden adatot tartalmaz, amit a vilagrol tudhatunk
//
// Ezeket az adatokat csak megnezni lehet.
// Beavatkozni utasitasok kiadasaval tudunk.
//-----------------------------------------------------------
struct TJatekos
{
    // Sajat birodalom leirasa.
    float Stat[ cStOszlopMax + 1 ];  // Statisztikak: Score es reszstatisztikak , stXXXX
    TEroforras Eroforras;            // A nyersanyagaink.
    TKepesseg Kepesseg;              // Kepessegek, fejlesztessel javithatok.
    int EpSzam;                      // Epuletek szama.
    TEpulet Epuletek[ cMaxEpulet ];  // Epuletek adatai
    int EgySzam;                     // Egysegekek szama, azaz populacio.
    TEgyseg Egysegek[ cMaxEgyseg ];  // Egysegek adatai

    // Vilag allapota, amit latunk belole.
    int Ido;       // Ennyi ido telt el.
    int BekeIdo;   // Bekeido.
    int IdoLimit;  // Ekkor er veget a jatek.
    int XMax;      // terkep merete
    int YMax;
    TMezo Vilag[ cMaxY ][ cMaxX ];         // A vilagnak az a resze, amit lat a jatekos.
    int MasEgySzam;                        // Mas jatekosok lathato egysegeinek szama.
    TMasEgyseg MasEgysegek[ cMasEgyseg ];  // Mas jatekosok egysegei, amit lat a terkepen.
    int MasEpSzam;                         // Mas jatekosok lathato epuleteinek szama.
    TMasEpulet MasEpuletek[ cMasEpulet ];  // Mas jatekosok epuletei, amit lat a terkepen.

    // Visszajelzes az utolso kotegben feldolgozott utasitasokrol.
    int UtasitSzam;                        // utoljara ennyi utasitast kapott
    TUtasitas Utasitasok[ cMaxUtasitas ];

    TJatekos( ): EpSzam( 0 ), EgySzam( 0 ), Ido( 0 ), BekeIdo( 0 ), IdoLimit( 0 ), XMax( 0 ), YMax( 0 ), MasEgySzam( 0 ), MasEpSzam( 0 ), UtasitSzam( 0 ) {}
};

//----------------------------------------------------------------

struct TJatKieg
{
    int LastEpID;                             // Legnagyobb kiadott epulet ID.
    int LastEgyID;                            // Legnagyobb kiadott egyseg ID.
    unsigned char Latszik[ cMaxY ][ cMaxX ];  // 0: ismeretlen, 1: felderitett, 2: most is latja

    TJatKieg( ): LastEpID( 0 ), LastEgyID( 0 ) { }
};

//----------------------------------------------------------------
// Egy korben kiadott utasitasok.
struct TKoteg
{
    int UtasitSzam;
    TUtasitas Utasitasok[ cMaxUtasitas ];

    TKoteg( ): UtasitSzam( 0 ) { }
};

#pragma pack( pop )

//----------------------------------------------------------------

// Az egysegek olvashato nevei. Csak a debuggolast segitik, nem kotelezo hasznalni.
extern const char* EgysegNevek[ ];      // ceParaszt .. cePuskas
extern const char EgysegNevBetu[ ];     // ceParaszt .. cePuskas
// A fejlesztesek olvashato nevei. Csak a debuggolast segitik, nem kotelezo hasznalni.
extern const char* FejlesztesNevek[ ];  // cfNincs .. cfPuskas
extern const float NyersiPont[ ];       // crKaja .. crArany Ennyi pontot kap 1 egyseg megtermelt nyersanyagert.

// Egysegek, fejleszteseik es az epuletek tulajdonsagai ebben a vilagban.
extern TEgysegTip EgyTipusok[ ];        // ceParaszt..cePuskas
extern TEpuletTip EpTipusok[ ];         // cvFohaz..cvFal
extern TFejlTip FejlTipusok[ ];         // cfKaja..cfPuskas
extern int FejlSzorzo[ ];               // Mennyi nyersiert kap egy KP-t, caKaja..caArany

//----------------------------------------------------------------

void egysegInit( );
void epuletInit( );
void fejlesztesInit( );

//----------------------------------------------------------------
