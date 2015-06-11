/*
 * Vilag definicio.
 */

#include "VilagDef.h"
#include <cmath>

//----------------------------------------------------------------

// Az egységek olvasható nevei. Csak a debuggolást segitik, nem kötelezõ használni.
const char* EgysegNevek[ ] = { "", "paraszt", "kardos", "ijasz", "lovas", "puskas" }; // ceParaszt .. cePuskas

const char EgysegNevBetu[ ] = { '\0', 'P', 'K', 'I', 'L', 'M' }; // ceParaszt .. cePuskas (puskás --> muskétás)

const char* FejlesztesNevek[ ] = { "",
    "Alapkutatas",
    "Kajatermeles fejlesztese",
    "Fatermeles fejlesztese",
    "Vastermeles fejlesztese",
    "Aranytermeles fejlesztese",
    "Kutatas gyorsitasa",
    "Epulet Vedelem",
    "Fal vedelem",
    "Torony",
    "Epulet Rombolas",
    "Latomezo",
    "Intelligencia",
    "Korhaz",
    "Populacio limit",
    "Paraszt",
    "Kardos",
    "Ijasz",
    "Lovas",
    "Puskas" }; // cfNincs .. cfPuskas

const float NyersiPont[ ] = { 0.0f, 0.0f, 0.4f, 0.6f, 0.8f, 1.0f }; // crKaja .. crArany Ennyi pontot kap 1 egyseg megtermelt nyersanyagert.

//----------------------------------------------------------------

// Egysegek, fejleszteseik es az epuletek tulajdonsagai ebben a vilagban.
TEgysegTip EgyTipusok[ cePuskas + 1 ];   // Egyseg::Paraszt..cePuskas
TEpuletTip EpTipusok[ cvFal + 1 ];       // cvFohaz..cvFal
TFejlTip FejlTipusok[ cfPuskas + 1 ];    // cfKaja..cfPuskas
int FejlSzorzo[ caArany + 1 ];           // Mennyi nyersiert kap egy KP-t, caKaja..caArany

//----------------------------------------------------------------

int roundFloatToInt( float aValue )
{
  return static_cast<int>(aValue < 0.0 ? ceil(aValue - 0.5) : floor(aValue + 0.5)); 
}

//----------------------------------------------------------------

void egysegInit( )
{
  EgyTipusok[ ceParaszt ].Kaja = 50; EgyTipusok[ ceParaszt ].Fa = 0; EgyTipusok[ ceParaszt ].Vas = 0; EgyTipusok[ ceParaszt ].Arany = 0;
  EgyTipusok[ ceParaszt ].KepzesIdo = 150;
  EgyTipusok[ ceParaszt ].Elet[ 0 ] = 50; EgyTipusok[ ceParaszt ].Ut[ 0 ] = 5; EgyTipusok[ ceParaszt ].UtTav[ 0 ] = 1; EgyTipusok[ ceParaszt ].UtVar[ 0 ] = 20;
  EgyTipusok[ ceParaszt ].Elet[ 1 ] = 50; EgyTipusok[ ceParaszt ].Ut[ 1 ] = 5; EgyTipusok[ ceParaszt ].UtTav[ 1 ] = 1; EgyTipusok[ ceParaszt ].UtVar[ 1 ] = 20;
  EgyTipusok[ ceParaszt ].Elet[ 2 ] = 50; EgyTipusok[ ceParaszt ].Ut[ 2 ] = 5; EgyTipusok[ ceParaszt ].UtTav[ 2 ] = 1; EgyTipusok[ ceParaszt ].UtVar[ 2 ] = 19;
  EgyTipusok[ ceParaszt ].Elet[ 3 ] = 60; EgyTipusok[ ceParaszt ].Ut[ 3 ] = 6; EgyTipusok[ ceParaszt ].UtTav[ 3 ] = 1; EgyTipusok[ ceParaszt ].UtVar[ 3 ] = 18;
  EgyTipusok[ ceParaszt ].Elet[ 4 ] = 70; EgyTipusok[ ceParaszt ].Ut[ 4 ] = 7; EgyTipusok[ ceParaszt ].UtTav[ 4 ] = 1; EgyTipusok[ ceParaszt ].UtVar[ 4 ] = 17;
  EgyTipusok[ ceParaszt ].Elet[ 5 ] = 80; EgyTipusok[ ceParaszt ].Ut[ 5 ] = 8; EgyTipusok[ ceParaszt ].UtTav[ 5 ] = 2; EgyTipusok[ ceParaszt ].UtVar[ 5 ] = 16;
  EgyTipusok[ ceParaszt ].VedUt[ 0 ] = 0; EgyTipusok[ ceParaszt ].VedNyil[ 0 ] = 0; EgyTipusok[ ceParaszt ].VedLo[ 0 ] = 0; EgyTipusok[ ceParaszt ].Sebesseg[ 0 ] = 7;
  EgyTipusok[ ceParaszt ].VedUt[ 1 ] = 0; EgyTipusok[ ceParaszt ].VedNyil[ 1 ] = 0; EgyTipusok[ ceParaszt ].VedLo[ 1 ] = 0; EgyTipusok[ ceParaszt ].Sebesseg[ 1 ] = 8;
  EgyTipusok[ ceParaszt ].VedUt[ 2 ] = 1; EgyTipusok[ ceParaszt ].VedNyil[ 2 ] = 0; EgyTipusok[ ceParaszt ].VedLo[ 2 ] = 0; EgyTipusok[ ceParaszt ].Sebesseg[ 2 ] = 8;
  EgyTipusok[ ceParaszt ].VedUt[ 3 ] = 1; EgyTipusok[ ceParaszt ].VedNyil[ 3 ] = 0; EgyTipusok[ ceParaszt ].VedLo[ 3 ] = 0; EgyTipusok[ ceParaszt ].Sebesseg[ 3 ] = 8;
  EgyTipusok[ ceParaszt ].VedUt[ 4 ] = 1; EgyTipusok[ ceParaszt ].VedNyil[ 4 ] = 0; EgyTipusok[ ceParaszt ].VedLo[ 4 ] = 0; EgyTipusok[ ceParaszt ].Sebesseg[ 4 ] = 9;
  EgyTipusok[ ceParaszt ].VedUt[ 5 ] = 1; EgyTipusok[ ceParaszt ].VedNyil[ 5 ] = 0; EgyTipusok[ ceParaszt ].VedLo[ 5 ] = 0; EgyTipusok[ ceParaszt ].Sebesseg[ 5 ] = 9;

  EgyTipusok[ ceKardos ].Kaja = 40; EgyTipusok[ ceKardos ].Fa = 0; EgyTipusok[ ceKardos ].Vas = 20; EgyTipusok[ ceKardos ].Arany = 0;
  EgyTipusok[ ceKardos ].KepzesIdo = 200;
  EgyTipusok[ ceKardos ].Elet[ 0 ] = 150; EgyTipusok[ ceKardos ].Ut[ 0 ] = 10; EgyTipusok[ ceKardos ].UtTav[ 0 ] = 1; EgyTipusok[ ceKardos ].UtVar[ 0 ] = 15;
  EgyTipusok[ ceKardos ].Elet[ 1 ] = 150; EgyTipusok[ ceKardos ].Ut[ 1 ] = 11; EgyTipusok[ ceKardos ].UtTav[ 1 ] = 1; EgyTipusok[ ceKardos ].UtVar[ 1 ] = 15;
  EgyTipusok[ ceKardos ].Elet[ 2 ] = 160; EgyTipusok[ ceKardos ].Ut[ 2 ] = 11; EgyTipusok[ ceKardos ].UtTav[ 2 ] = 1; EgyTipusok[ ceKardos ].UtVar[ 2 ] = 14;
  EgyTipusok[ ceKardos ].Elet[ 3 ] = 170; EgyTipusok[ ceKardos ].Ut[ 3 ] = 12; EgyTipusok[ ceKardos ].UtTav[ 3 ] = 1; EgyTipusok[ ceKardos ].UtVar[ 3 ] = 13;
  EgyTipusok[ ceKardos ].Elet[ 4 ] = 180; EgyTipusok[ ceKardos ].Ut[ 4 ] = 13; EgyTipusok[ ceKardos ].UtTav[ 4 ] = 1; EgyTipusok[ ceKardos ].UtVar[ 4 ] = 12;
  EgyTipusok[ ceKardos ].Elet[ 5 ] = 200; EgyTipusok[ ceKardos ].Ut[ 5 ] = 15; EgyTipusok[ ceKardos ].UtTav[ 5 ] = 1; EgyTipusok[ ceKardos ].UtVar[ 5 ] = 10;
  EgyTipusok[ ceKardos ].VedUt[ 0 ] = 3; EgyTipusok[ ceKardos ].VedNyil[ 0 ] = 1; EgyTipusok[ ceKardos ].VedLo[ 0 ] = 0; EgyTipusok[ ceKardos ].Sebesseg[ 0 ] = 6;
  EgyTipusok[ ceKardos ].VedUt[ 1 ] = 3; EgyTipusok[ ceKardos ].VedNyil[ 1 ] = 1; EgyTipusok[ ceKardos ].VedLo[ 1 ] = 0; EgyTipusok[ ceKardos ].Sebesseg[ 1 ] = 6;
  EgyTipusok[ ceKardos ].VedUt[ 2 ] = 4; EgyTipusok[ ceKardos ].VedNyil[ 2 ] = 2; EgyTipusok[ ceKardos ].VedLo[ 2 ] = 0; EgyTipusok[ ceKardos ].Sebesseg[ 2 ] = 6;
  EgyTipusok[ ceKardos ].VedUt[ 3 ] = 4; EgyTipusok[ ceKardos ].VedNyil[ 3 ] = 2; EgyTipusok[ ceKardos ].VedLo[ 3 ] = 0; EgyTipusok[ ceKardos ].Sebesseg[ 3 ] = 7;
  EgyTipusok[ ceKardos ].VedUt[ 4 ] = 4; EgyTipusok[ ceKardos ].VedNyil[ 4 ] = 2; EgyTipusok[ ceKardos ].VedLo[ 4 ] = 1; EgyTipusok[ ceKardos ].Sebesseg[ 4 ] = 7;
  EgyTipusok[ ceKardos ].VedUt[ 5 ] = 4; EgyTipusok[ ceKardos ].VedNyil[ 5 ] = 2; EgyTipusok[ ceKardos ].VedLo[ 5 ] = 2; EgyTipusok[ ceKardos ].Sebesseg[ 5 ] = 8;

  EgyTipusok[ ceIjasz ].Kaja = 30; EgyTipusok[ ceIjasz ].Fa = 20; EgyTipusok[ ceIjasz ].Vas = 0; EgyTipusok[ ceIjasz ].Arany = 0;
  EgyTipusok[ ceIjasz ].KepzesIdo = 250;
  EgyTipusok[ ceIjasz ].Elet[ 0 ] = 60; EgyTipusok[ ceIjasz ].Ut[ 0 ] = 5; EgyTipusok[ ceIjasz ].UtTav[ 0 ] = 2; EgyTipusok[ ceIjasz ].UtVar[ 0 ] = 20;
  EgyTipusok[ ceIjasz ].Elet[ 1 ] = 60; EgyTipusok[ ceIjasz ].Ut[ 1 ] = 5; EgyTipusok[ ceIjasz ].UtTav[ 1 ] = 2; EgyTipusok[ ceIjasz ].UtVar[ 1 ] = 20;
  EgyTipusok[ ceIjasz ].Elet[ 2 ] = 60; EgyTipusok[ ceIjasz ].Ut[ 2 ] = 5; EgyTipusok[ ceIjasz ].UtTav[ 2 ] = 3; EgyTipusok[ ceIjasz ].UtVar[ 2 ] = 20;
  EgyTipusok[ ceIjasz ].Elet[ 3 ] = 60; EgyTipusok[ ceIjasz ].Ut[ 3 ] = 6; EgyTipusok[ ceIjasz ].UtTav[ 3 ] = 3; EgyTipusok[ ceIjasz ].UtVar[ 3 ] = 20;
  EgyTipusok[ ceIjasz ].Elet[ 4 ] = 70; EgyTipusok[ ceIjasz ].Ut[ 4 ] = 7; EgyTipusok[ ceIjasz ].UtTav[ 4 ] = 3; EgyTipusok[ ceIjasz ].UtVar[ 4 ] = 19;
  EgyTipusok[ ceIjasz ].Elet[ 5 ] = 80; EgyTipusok[ ceIjasz ].Ut[ 5 ] = 8; EgyTipusok[ ceIjasz ].UtTav[ 5 ] = 3; EgyTipusok[ ceIjasz ].UtVar[ 5 ] = 18;
  EgyTipusok[ ceIjasz ].VedUt[ 0 ] = 0; EgyTipusok[ ceIjasz ].VedNyil[ 0 ] = 0; EgyTipusok[ ceIjasz ].VedLo[ 0 ] = 0; EgyTipusok[ ceIjasz ].Sebesseg[ 0 ] = 4;
  EgyTipusok[ ceIjasz ].VedUt[ 1 ] = 0; EgyTipusok[ ceIjasz ].VedNyil[ 1 ] = 0; EgyTipusok[ ceIjasz ].VedLo[ 1 ] = 0; EgyTipusok[ ceIjasz ].Sebesseg[ 1 ] = 5;
  EgyTipusok[ ceIjasz ].VedUt[ 2 ] = 0; EgyTipusok[ ceIjasz ].VedNyil[ 2 ] = 0; EgyTipusok[ ceIjasz ].VedLo[ 2 ] = 0; EgyTipusok[ ceIjasz ].Sebesseg[ 2 ] = 5;
  EgyTipusok[ ceIjasz ].VedUt[ 3 ] = 0; EgyTipusok[ ceIjasz ].VedNyil[ 3 ] = 0; EgyTipusok[ ceIjasz ].VedLo[ 3 ] = 0; EgyTipusok[ ceIjasz ].Sebesseg[ 3 ] = 5;
  EgyTipusok[ ceIjasz ].VedUt[ 4 ] = 0; EgyTipusok[ ceIjasz ].VedNyil[ 4 ] = 1; EgyTipusok[ ceIjasz ].VedLo[ 4 ] = 0; EgyTipusok[ ceIjasz ].Sebesseg[ 4 ] = 6;
  EgyTipusok[ ceIjasz ].VedUt[ 5 ] = 1; EgyTipusok[ ceIjasz ].VedNyil[ 5 ] = 1; EgyTipusok[ ceIjasz ].VedLo[ 5 ] = 0; EgyTipusok[ ceIjasz ].Sebesseg[ 5 ] = 6;

  EgyTipusok[ ceLovas ].Kaja = 100; EgyTipusok[ ceLovas ].Fa = 0; EgyTipusok[ ceLovas ].Vas = 0; EgyTipusok[ ceLovas ].Arany = 15;
  EgyTipusok[ ceLovas ].KepzesIdo = 300;
  EgyTipusok[ ceLovas ].Elet[ 0 ] = 200; EgyTipusok[ ceLovas ].Ut[ 0 ] = 10; EgyTipusok[ ceLovas ].UtTav[ 0 ] = 1; EgyTipusok[ ceLovas ].UtVar[ 0 ] = 15;
  EgyTipusok[ ceLovas ].Elet[ 1 ] = 210; EgyTipusok[ ceLovas ].Ut[ 1 ] = 11; EgyTipusok[ ceLovas ].UtTav[ 1 ] = 1; EgyTipusok[ ceLovas ].UtVar[ 1 ] = 14;
  EgyTipusok[ ceLovas ].Elet[ 2 ] = 220; EgyTipusok[ ceLovas ].Ut[ 2 ] = 12; EgyTipusok[ ceLovas ].UtTav[ 2 ] = 1; EgyTipusok[ ceLovas ].UtVar[ 2 ] = 13;
  EgyTipusok[ ceLovas ].Elet[ 3 ] = 250; EgyTipusok[ ceLovas ].Ut[ 3 ] = 13; EgyTipusok[ ceLovas ].UtTav[ 3 ] = 1; EgyTipusok[ ceLovas ].UtVar[ 3 ] = 12;
  EgyTipusok[ ceLovas ].Elet[ 4 ] = 280; EgyTipusok[ ceLovas ].Ut[ 4 ] = 14; EgyTipusok[ ceLovas ].UtTav[ 4 ] = 1; EgyTipusok[ ceLovas ].UtVar[ 4 ] = 11;
  EgyTipusok[ ceLovas ].Elet[ 5 ] = 300; EgyTipusok[ ceLovas ].Ut[ 5 ] = 15; EgyTipusok[ ceLovas ].UtTav[ 5 ] = 1; EgyTipusok[ ceLovas ].UtVar[ 5 ] = 10;
  EgyTipusok[ ceLovas ].VedUt[ 0 ] = 1; EgyTipusok[ ceLovas ].VedNyil[ 0 ] = 2; EgyTipusok[ ceLovas ].VedLo[ 0 ] = 0; EgyTipusok[ ceLovas ].Sebesseg[ 0 ] = 10;
  EgyTipusok[ ceLovas ].VedUt[ 1 ] = 2; EgyTipusok[ ceLovas ].VedNyil[ 1 ] = 2; EgyTipusok[ ceLovas ].VedLo[ 1 ] = 0; EgyTipusok[ ceLovas ].Sebesseg[ 1 ] = 11;
  EgyTipusok[ ceLovas ].VedUt[ 2 ] = 2; EgyTipusok[ ceLovas ].VedNyil[ 2 ] = 3; EgyTipusok[ ceLovas ].VedLo[ 2 ] = 1; EgyTipusok[ ceLovas ].Sebesseg[ 2 ] = 11;
  EgyTipusok[ ceLovas ].VedUt[ 3 ] = 2; EgyTipusok[ ceLovas ].VedNyil[ 3 ] = 3; EgyTipusok[ ceLovas ].VedLo[ 3 ] = 2; EgyTipusok[ ceLovas ].Sebesseg[ 3 ] = 11;
  EgyTipusok[ ceLovas ].VedUt[ 4 ] = 2; EgyTipusok[ ceLovas ].VedNyil[ 4 ] = 4; EgyTipusok[ ceLovas ].VedLo[ 4 ] = 3; EgyTipusok[ ceLovas ].Sebesseg[ 4 ] = 12;
  EgyTipusok[ ceLovas ].VedUt[ 5 ] = 2; EgyTipusok[ ceLovas ].VedNyil[ 5 ] = 4; EgyTipusok[ ceLovas ].VedLo[ 5 ] = 4; EgyTipusok[ ceLovas ].Sebesseg[ 5 ] = 13;

  EgyTipusok[ cePuskas ].Kaja = 50; EgyTipusok[ cePuskas ].Fa = 0; EgyTipusok[ cePuskas ].Vas = 30; EgyTipusok[ cePuskas ].Arany = 0;
  EgyTipusok[ cePuskas ].KepzesIdo = 300;
  EgyTipusok[ cePuskas ].Elet[ 0 ] = 100; EgyTipusok[ cePuskas ].Ut[ 0 ] = 8; EgyTipusok[ cePuskas ].UtTav[ 0 ] = 2; EgyTipusok[ cePuskas ].UtVar[ 0 ] = 24;
  EgyTipusok[ cePuskas ].Elet[ 1 ] = 100; EgyTipusok[ cePuskas ].Ut[ 1 ] = 9; EgyTipusok[ cePuskas ].UtTav[ 1 ] = 3; EgyTipusok[ cePuskas ].UtVar[ 1 ] = 22;
  EgyTipusok[ cePuskas ].Elet[ 2 ] = 110; EgyTipusok[ cePuskas ].Ut[ 2 ] = 10; EgyTipusok[ cePuskas ].UtTav[ 2 ] = 3; EgyTipusok[ cePuskas ].UtVar[ 2 ] = 20;
  EgyTipusok[ cePuskas ].Elet[ 3 ] = 110; EgyTipusok[ cePuskas ].Ut[ 3 ] = 12; EgyTipusok[ cePuskas ].UtTav[ 3 ] = 4; EgyTipusok[ cePuskas ].UtVar[ 3 ] = 18;
  EgyTipusok[ cePuskas ].Elet[ 4 ] = 120; EgyTipusok[ cePuskas ].Ut[ 4 ] = 15; EgyTipusok[ cePuskas ].UtTav[ 4 ] = 4; EgyTipusok[ cePuskas ].UtVar[ 4 ] = 16;
  EgyTipusok[ cePuskas ].Elet[ 5 ] = 130; EgyTipusok[ cePuskas ].Ut[ 5 ] = 18; EgyTipusok[ cePuskas ].UtTav[ 5 ] = 5; EgyTipusok[ cePuskas ].UtVar[ 5 ] = 14;
  EgyTipusok[ cePuskas ].VedUt[ 0 ] = 0; EgyTipusok[ cePuskas ].VedNyil[ 0 ] = 1; EgyTipusok[ cePuskas ].VedLo[ 0 ] = 0; EgyTipusok[ cePuskas ].Sebesseg[ 0 ] = 5;
  EgyTipusok[ cePuskas ].VedUt[ 1 ] = 0; EgyTipusok[ cePuskas ].VedNyil[ 1 ] = 1; EgyTipusok[ cePuskas ].VedLo[ 1 ] = 0; EgyTipusok[ cePuskas ].Sebesseg[ 1 ] = 5;
  EgyTipusok[ cePuskas ].VedUt[ 2 ] = 1; EgyTipusok[ cePuskas ].VedNyil[ 2 ] = 1; EgyTipusok[ cePuskas ].VedLo[ 2 ] = 0; EgyTipusok[ cePuskas ].Sebesseg[ 2 ] = 6;
  EgyTipusok[ cePuskas ].VedUt[ 3 ] = 1; EgyTipusok[ cePuskas ].VedNyil[ 3 ] = 1; EgyTipusok[ cePuskas ].VedLo[ 3 ] = 0; EgyTipusok[ cePuskas ].Sebesseg[ 3 ] = 6;
  EgyTipusok[ cePuskas ].VedUt[ 4 ] = 1; EgyTipusok[ cePuskas ].VedNyil[ 4 ] = 1; EgyTipusok[ cePuskas ].VedLo[ 4 ] = 1; EgyTipusok[ cePuskas ].Sebesseg[ 4 ] = 6;
  EgyTipusok[ cePuskas ].VedUt[ 5 ] = 1; EgyTipusok[ cePuskas ].VedNyil[ 5 ] = 1; EgyTipusok[ cePuskas ].VedLo[ 5 ] = 1; EgyTipusok[ cePuskas ].Sebesseg[ 5 ] = 7;

  for ( int i = ceParaszt; i <= cePuskas; i++ ) {
    EgyTipusok[ i ].Koltseg = roundFloatToInt( EgyTipusok[ i ].Kaja  * NyersiPont[ crKaja ] +
                                              EgyTipusok[ i ].Fa    * NyersiPont[ crFa ] +
                                              EgyTipusok[ i ].Vas   * NyersiPont[ crVas ] +
                                              EgyTipusok[ i ].Arany * NyersiPont[ crArany ] );
  }

}

//----------------------------------------------------------------

void epuletInit( )
{
  for ( int i = cvFohaz; i <= cvFal; ++i )
  {
    EpTipusok[ i ].Elet = 1000;
    EpTipusok[ i ].Fa = 100;
    EpTipusok[ i ].EpitesIdo = 200;
  }

  EpTipusok[ cvFohaz ].EpitesIdo = 100;
  EpTipusok[ cvFohaz ].Egyseg[ 0 ] = ceParaszt;
  EpTipusok[ cvLaktanya ].Fa = 150;
  EpTipusok[ cvLaktanya ].Egyseg[ 0 ] = ceKardos;
  EpTipusok[ cvLaktanya ].Egyseg[ 1 ] = cePuskas;
  EpTipusok[ cvLoter ].Egyseg[ 0 ] = ceIjasz;
  EpTipusok[ cvIstallo ].Fa = 200;
  EpTipusok[ cvIstallo ].Egyseg[ 0 ] = ceLovas;
  EpTipusok[ cvIstallo ].EpitesIdo = 300;
  EpTipusok[ cvKorhaz ].Fa = 60;
  EpTipusok[ cvKorhaz ].Vas = 10;
  EpTipusok[ cvKorhaz ].Arany = 10;
  EpTipusok[ cvKorhaz ].EpitesIdo = 150;
  EpTipusok[ cvKorhaz ].KpLimit = 200;
  EpTipusok[ cvTorony ].Fa = 200;
  EpTipusok[ cvTorony ].Vas = 20;
  EpTipusok[ cvTorony ].EpitesIdo = 500;
  EpTipusok[ cvFal ].Fa = 40;
  EpTipusok[ cvFal ].Vas = 5;
  EpTipusok[ cvFal ].Elet = 500;
  EpTipusok[ cvFal ].EpitesIdo = 80;

  for ( int i = cvFohaz; i <= cvFal; ++i )
  {
    EpTipusok[ i ].Koltseg = roundFloatToInt( EpTipusok[ i ].Kaja  * NyersiPont[ crKaja ] +
                                            EpTipusok[ i ].Fa    * NyersiPont[ crFa ] +
                                            EpTipusok[ i ].Vas   * NyersiPont[ crVas ] +
                                            EpTipusok[ i ].Arany * NyersiPont[ crArany ] );
  }
}

//----------------------------------------------------------------

void fejlesztesInit( )
{
  FejlSzorzo[ caKaja ] = 20;
  FejlSzorzo[ caFa ] = 20;
  FejlSzorzo[ caVas ] = 10;
  FejlSzorzo[ caArany ] = 4;

  // Kutatas gyorsitasa, a szint az egyesek helyen van,
  FejlTipusok[ cfKutGyors ].SzMax = 5;
  FejlTipusok[ cfKutGyors ].Param[ 0 ][ 1 ] = 100;
  FejlTipusok[ cfKutGyors ].FejlIdo[ 1 ] = 100; FejlTipusok[ cfKutGyors ].Fa[ 1 ] = 200; FejlTipusok[ cfKutGyors ].Param[ 1 ][ 1 ] = 90;
  FejlTipusok[ cfKutGyors ].FejlIdo[ 2 ] = 100; FejlTipusok[ cfKutGyors ].Fa[ 2 ] = 300; FejlTipusok[ cfKutGyors ].Vas[ 2 ] = 100; FejlTipusok[ cfKutGyors ].Arany[ 2 ] = 50; FejlTipusok[ cfKutGyors ].Param[ 2 ][ 1 ] = 80;
  FejlTipusok[ cfKutGyors ].FejlIdo[ 3 ] = 100; FejlTipusok[ cfKutGyors ].Fa[ 3 ] = 500; FejlTipusok[ cfKutGyors ].Vas[ 3 ] = 200; FejlTipusok[ cfKutGyors ].Arany[ 3 ] = 100; FejlTipusok[ cfKutGyors ].Param[ 3 ][ 1 ] = 70;
  FejlTipusok[ cfKutGyors ].FejlIdo[ 4 ] = 100; FejlTipusok[ cfKutGyors ].Fa[ 4 ] = 1000; FejlTipusok[ cfKutGyors ].Vas[ 4 ] = 300; FejlTipusok[ cfKutGyors ].Arany[ 4 ] = 150; FejlTipusok[ cfKutGyors ].Param[ 4 ][ 1 ] = 60;
  FejlTipusok[ cfKutGyors ].FejlIdo[ 5 ] = 100; FejlTipusok[ cfKutGyors ].Fa[ 5 ] = 1500; FejlTipusok[ cfKutGyors ].Vas[ 5 ] = 400; FejlTipusok[ cfKutGyors ].Arany[ 5 ] = 200; FejlTipusok[ cfKutGyors ].Param[ 5 ][ 1 ] = 50;

  // Epulet vedelem, vedoertek novelese, amivel a sebzes csokken
  FejlTipusok[ cfEpVed ].SzMax = 3;
  FejlTipusok[ cfEpVed ].Param[ 0 ][ 1 ] = 0; FejlTipusok[ cfEpVed ].Param[ 0 ][ 2 ] = 1;
  FejlTipusok[ cfEpVed ].KpLimit[ 1 ] = 20; FejlTipusok[ cfEpVed ].FejlIdo[ 1 ] = 200; FejlTipusok[ cfEpVed ].Fa[ 1 ] = 100; FejlTipusok[ cfEpVed ].Vas[ 1 ] = 100; FejlTipusok[ cfEpVed ].Param[ 1 ][ 1 ] = 1; FejlTipusok[ cfEpVed ].Param[ 1 ][ 2 ] = 2;
  FejlTipusok[ cfEpVed ].KpLimit[ 2 ] = 150; FejlTipusok[ cfEpVed ].FejlIdo[ 2 ] = 350; FejlTipusok[ cfEpVed ].Fa[ 2 ] = 200; FejlTipusok[ cfEpVed ].Vas[ 2 ] = 200; FejlTipusok[ cfEpVed ].Param[ 2 ][ 1 ] = 2; FejlTipusok[ cfEpVed ].Param[ 2 ][ 2 ] = 3;
  FejlTipusok[ cfEpVed ].KpLimit[ 3 ] = 500; FejlTipusok[ cfEpVed ].FejlIdo[ 3 ] = 500; FejlTipusok[ cfEpVed ].Fa[ 3 ] = 600; FejlTipusok[ cfEpVed ].Vas[ 3 ] = 400; FejlTipusok[ cfEpVed ].Param[ 3 ][ 1 ] = 3; FejlTipusok[ cfEpVed ].Param[ 3 ][ 2 ] = 4;

  // Fal vedelem, vedoertek novelese, amivel a sebzes csokken
  FejlTipusok[ cfFalVed ].SzMax = 6;
  FejlTipusok[ cfFalVed ].Param[ 0 ][ 1 ] = 0; FejlTipusok[ cfFalVed ].Param[ 0 ][ 2 ] = 1;
  FejlTipusok[ cfFalVed ].KpLimit[ 1 ] = 100; FejlTipusok[ cfFalVed ].FejlIdo[ 1 ] = 100; FejlTipusok[ cfFalVed ].Fa[ 1 ] = 50; FejlTipusok[ cfFalVed ].Vas[ 1 ] = 50; FejlTipusok[ cfFalVed ].Param[ 1 ][ 1 ] = 1; FejlTipusok[ cfFalVed ].Param[ 1 ][ 2 ] = 2;
  FejlTipusok[ cfFalVed ].KpLimit[ 2 ] = 250; FejlTipusok[ cfFalVed ].FejlIdo[ 2 ] = 150; FejlTipusok[ cfFalVed ].Fa[ 2 ] = 100; FejlTipusok[ cfFalVed ].Vas[ 2 ] = 100; FejlTipusok[ cfFalVed ].Param[ 2 ][ 1 ] = 2; FejlTipusok[ cfFalVed ].Param[ 2 ][ 2 ] = 3;
  FejlTipusok[ cfFalVed ].KpLimit[ 3 ] = 400; FejlTipusok[ cfFalVed ].FejlIdo[ 3 ] = 200; FejlTipusok[ cfFalVed ].Fa[ 3 ] = 150; FejlTipusok[ cfFalVed ].Vas[ 3 ] = 150; FejlTipusok[ cfFalVed ].Param[ 3 ][ 1 ] = 3; FejlTipusok[ cfFalVed ].Param[ 3 ][ 2 ] = 4;
  FejlTipusok[ cfFalVed ].KpLimit[ 4 ] = 550; FejlTipusok[ cfFalVed ].FejlIdo[ 4 ] = 250; FejlTipusok[ cfFalVed ].Fa[ 4 ] = 200; FejlTipusok[ cfFalVed ].Vas[ 4 ] = 200; FejlTipusok[ cfFalVed ].Param[ 4 ][ 1 ] = 4; FejlTipusok[ cfFalVed ].Param[ 4 ][ 2 ] = 5;
  FejlTipusok[ cfFalVed ].KpLimit[ 5 ] = 700; FejlTipusok[ cfFalVed ].FejlIdo[ 5 ] = 350; FejlTipusok[ cfFalVed ].Fa[ 5 ] = 250; FejlTipusok[ cfFalVed ].Vas[ 5 ] = 250; FejlTipusok[ cfFalVed ].Param[ 5 ][ 1 ] = 5; FejlTipusok[ cfFalVed ].Param[ 5 ][ 2 ] = 6;
  FejlTipusok[ cfFalVed ].KpLimit[ 6 ] = 850; FejlTipusok[ cfFalVed ].FejlIdo[ 6 ] = 500; FejlTipusok[ cfFalVed ].Fa[ 6 ] = 300; FejlTipusok[ cfFalVed ].Vas[ 6 ] = 300; FejlTipusok[ cfFalVed ].Param[ 6 ][ 1 ] = 6; FejlTipusok[ cfFalVed ].Param[ 6 ][ 2 ] = 8;

  // Torony, elete fejlodik es vegul meszebb is lo
  FejlTipusok[ cfTorony ].SzMax = 3;
  FejlTipusok[ cfTorony ].Param[ 0 ][ 1 ] = 1000; FejlTipusok[ cfTorony ].Param[ 0 ][ 2 ] = 3;
  FejlTipusok[ cfTorony ].KpLimit[ 1 ] = 140; FejlTipusok[ cfTorony ].FejlIdo[ 1 ] = 500; FejlTipusok[ cfTorony ].Fa[ 1 ] = 200; FejlTipusok[ cfTorony ].Vas[ 1 ] = 50; FejlTipusok[ cfTorony ].Param[ 1 ][ 1 ] = 1100; FejlTipusok[ cfTorony ].Param[ 1 ][ 2 ] = 3;
  FejlTipusok[ cfTorony ].KpLimit[ 2 ] = 300; FejlTipusok[ cfTorony ].FejlIdo[ 2 ] = 500; FejlTipusok[ cfTorony ].Fa[ 2 ] = 400; FejlTipusok[ cfTorony ].Vas[ 2 ] = 100; FejlTipusok[ cfTorony ].Param[ 2 ][ 1 ] = 1200; FejlTipusok[ cfTorony ].Param[ 2 ][ 2 ] = 3;
  FejlTipusok[ cfTorony ].KpLimit[ 3 ] = 650; FejlTipusok[ cfTorony ].FejlIdo[ 3 ] = 500; FejlTipusok[ cfTorony ].Fa[ 3 ] = 600; FejlTipusok[ cfTorony ].Vas[ 3 ] = 200; FejlTipusok[ cfTorony ].Param[ 3 ][ 1 ] = 1300; FejlTipusok[ cfTorony ].Param[ 3 ][ 2 ] = 4;

  // Epulet vedelem, tamdoertek novelese, amivel a sebzes no
  FejlTipusok[ cfEpRombol ].SzMax = 7;
  FejlTipusok[ cfEpRombol ].Param[ 0 ][ 1 ] = 0; FejlTipusok[ cfEpRombol ].Param[ 0 ][ 2 ] = 0;
  FejlTipusok[ cfEpRombol ].KpLimit[ 1 ] = 200; FejlTipusok[ cfEpRombol ].FejlIdo[ 1 ] = 300; FejlTipusok[ cfEpRombol ].Fa[ 1 ] = 200; FejlTipusok[ cfEpRombol ].Vas[ 1 ] = 200; FejlTipusok[ cfEpRombol ].Arany[ 1 ] = 200; FejlTipusok[ cfEpRombol ].Param[ 1 ][ 1 ] = 1; FejlTipusok[ cfEpRombol ].Param[ 1 ][ 2 ] = 0;
  FejlTipusok[ cfEpRombol ].KpLimit[ 2 ] = 350; FejlTipusok[ cfEpRombol ].FejlIdo[ 2 ] = 400; FejlTipusok[ cfEpRombol ].Fa[ 2 ] = 250; FejlTipusok[ cfEpRombol ].Vas[ 2 ] = 250; FejlTipusok[ cfEpRombol ].Arany[ 2 ] = 250; FejlTipusok[ cfEpRombol ].Param[ 2 ][ 1 ] = 2; FejlTipusok[ cfEpRombol ].Param[ 2 ][ 2 ] = 0;
  FejlTipusok[ cfEpRombol ].KpLimit[ 3 ] = 500; FejlTipusok[ cfEpRombol ].FejlIdo[ 3 ] = 500; FejlTipusok[ cfEpRombol ].Fa[ 3 ] = 300; FejlTipusok[ cfEpRombol ].Vas[ 3 ] = 300; FejlTipusok[ cfEpRombol ].Arany[ 3 ] = 300; FejlTipusok[ cfEpRombol ].Param[ 3 ][ 1 ] = 3; FejlTipusok[ cfEpRombol ].Param[ 3 ][ 2 ] = 1;
  FejlTipusok[ cfEpRombol ].KpLimit[ 4 ] = 700; FejlTipusok[ cfEpRombol ].FejlIdo[ 4 ] = 600; FejlTipusok[ cfEpRombol ].Fa[ 4 ] = 350; FejlTipusok[ cfEpRombol ].Vas[ 4 ] = 350; FejlTipusok[ cfEpRombol ].Arany[ 4 ] = 350; FejlTipusok[ cfEpRombol ].Param[ 4 ][ 1 ] = 4; FejlTipusok[ cfEpRombol ].Param[ 4 ][ 2 ] = 2;
  FejlTipusok[ cfEpRombol ].KpLimit[ 5 ] = 800; FejlTipusok[ cfEpRombol ].FejlIdo[ 5 ] = 700; FejlTipusok[ cfEpRombol ].Fa[ 5 ] = 400; FejlTipusok[ cfEpRombol ].Vas[ 5 ] = 400; FejlTipusok[ cfEpRombol ].Arany[ 5 ] = 400; FejlTipusok[ cfEpRombol ].Param[ 5 ][ 1 ] = 6; FejlTipusok[ cfEpRombol ].Param[ 5 ][ 2 ] = 3;
  FejlTipusok[ cfEpRombol ].KpLimit[ 6 ] = 900; FejlTipusok[ cfEpRombol ].FejlIdo[ 6 ] = 800; FejlTipusok[ cfEpRombol ].Fa[ 6 ] = 450; FejlTipusok[ cfEpRombol ].Vas[ 6 ] = 450; FejlTipusok[ cfEpRombol ].Arany[ 6 ] = 450; FejlTipusok[ cfEpRombol ].Param[ 6 ][ 1 ] = 8; FejlTipusok[ cfEpRombol ].Param[ 6 ][ 2 ] = 4;
  FejlTipusok[ cfEpRombol ].KpLimit[ 7 ] = 1000; FejlTipusok[ cfEpRombol ].FejlIdo[ 7 ] = 900; FejlTipusok[ cfEpRombol ].Fa[ 7 ] = 500; FejlTipusok[ cfEpRombol ].Vas[ 7 ] = 500; FejlTipusok[ cfEpRombol ].Arany[ 7 ] = 500; FejlTipusok[ cfEpRombol ].Param[ 7 ][ 1 ] = 11; FejlTipusok[ cfEpRombol ].Param[ 7 ][ 2 ] = 5;

  // Latomezo
  FejlTipusok[ cfLat ].SzMax = 3;
  FejlTipusok[ cfLat ].KpLimit[ 1 ] = 50; FejlTipusok[ cfLat ].FejlIdo[ 1 ] = 50; FejlTipusok[ cfLat ].Kaja[ 1 ] = 100; FejlTipusok[ cfLat ].Fa[ 1 ] = 100; FejlTipusok[ cfLat ].Vas[ 1 ] = 100; FejlTipusok[ cfLat ].Arany[ 1 ] = 100;
  FejlTipusok[ cfLat ].KpLimit[ 2 ] = 200; FejlTipusok[ cfLat ].FejlIdo[ 2 ] = 100; FejlTipusok[ cfLat ].Kaja[ 2 ] = 200; FejlTipusok[ cfLat ].Fa[ 2 ] = 200; FejlTipusok[ cfLat ].Vas[ 2 ] = 200; FejlTipusok[ cfLat ].Arany[ 2 ] = 200;
  FejlTipusok[ cfLat ].KpLimit[ 3 ] = 600; FejlTipusok[ cfLat ].FejlIdo[ 3 ] = 200; FejlTipusok[ cfLat ].Kaja[ 3 ] = 500; FejlTipusok[ cfLat ].Fa[ 3 ] = 500; FejlTipusok[ cfLat ].Vas[ 3 ] = 500; FejlTipusok[ cfLat ].Arany[ 3 ] = 500;
  FejlTipusok[ cfLat ].Param[ 0 ][ 1 ] = 2; FejlTipusok[ cfLat ].Param[ 0 ][ 2 ] = 3; FejlTipusok[ cfLat ].Param[ 0 ][ 3 ] = 1;
  FejlTipusok[ cfLat ].Param[ 1 ][ 1 ] = 3; FejlTipusok[ cfLat ].Param[ 1 ][ 2 ] = 4; FejlTipusok[ cfLat ].Param[ 1 ][ 3 ] = 1;
  FejlTipusok[ cfLat ].Param[ 2 ][ 1 ] = 4; FejlTipusok[ cfLat ].Param[ 2 ][ 2 ] = 5; FejlTipusok[ cfLat ].Param[ 2 ][ 3 ] = 1;
  FejlTipusok[ cfLat ].Param[ 3 ][ 1 ] = 5; FejlTipusok[ cfLat ].Param[ 3 ][ 2 ] = 6; FejlTipusok[ cfLat ].Param[ 3 ][ 3 ] = 2;

  // Intelligencia
  FejlTipusok[ cfIQ ].SzMax = 2;
  FejlTipusok[ cfIQ ].KpLimit[ 1 ] = 250; FejlTipusok[ cfIQ ].FejlIdo[ 1 ] = 400; FejlTipusok[ cfIQ ].Kaja[ 1 ] = 100; FejlTipusok[ cfIQ ].Fa[ 1 ] = 100; FejlTipusok[ cfIQ ].Vas[ 1 ] = 100; FejlTipusok[ cfIQ ].Arany[ 1 ] = 100;
  FejlTipusok[ cfIQ ].KpLimit[ 2 ] = 420; FejlTipusok[ cfIQ ].FejlIdo[ 2 ] = 400; FejlTipusok[ cfIQ ].Kaja[ 2 ] = 100; FejlTipusok[ cfIQ ].Fa[ 2 ] = 100; FejlTipusok[ cfIQ ].Vas[ 2 ] = 100; FejlTipusok[ cfIQ ].Arany[ 2 ] = 100;

  // Korhaz
  FejlTipusok[ cfKorhaz ].SzMax = 3;
  FejlTipusok[ cfKorhaz ].KpLimit[ 0 ] = 200; FejlTipusok[ cfKorhaz ].Param[ 0 ][ 1 ] = 2; FejlTipusok[ cfKorhaz ].Param[ 0 ][ 2 ] = 25;
  FejlTipusok[ cfKorhaz ].KpLimit[ 1 ] = 350; FejlTipusok[ cfKorhaz ].FejlIdo[ 1 ] = 300; FejlTipusok[ cfKorhaz ].Kaja[ 1 ] = 50; FejlTipusok[ cfKorhaz ].Fa[ 1 ] = 50; FejlTipusok[ cfKorhaz ].Vas[ 1 ] = 100; FejlTipusok[ cfKorhaz ].Arany[ 1 ] = 100; FejlTipusok[ cfKorhaz ].Param[ 1 ][ 1 ] = 3; FejlTipusok[ cfKorhaz ].Param[ 1 ][ 2 ] = 20;
  FejlTipusok[ cfKorhaz ].KpLimit[ 2 ] = 500; FejlTipusok[ cfKorhaz ].FejlIdo[ 2 ] = 400; FejlTipusok[ cfKorhaz ].Kaja[ 2 ] = 100; FejlTipusok[ cfKorhaz ].Fa[ 2 ] = 100; FejlTipusok[ cfKorhaz ].Vas[ 2 ] = 150; FejlTipusok[ cfKorhaz ].Arany[ 2 ] = 150; FejlTipusok[ cfKorhaz ].Param[ 2 ][ 1 ] = 4; FejlTipusok[ cfKorhaz ].Param[ 2 ][ 2 ] = 15;
  FejlTipusok[ cfKorhaz ].KpLimit[ 3 ] = 700; FejlTipusok[ cfKorhaz ].FejlIdo[ 3 ] = 500; FejlTipusok[ cfKorhaz ].Kaja[ 3 ] = 150; FejlTipusok[ cfKorhaz ].Fa[ 3 ] = 150; FejlTipusok[ cfKorhaz ].Vas[ 3 ] = 200; FejlTipusok[ cfKorhaz ].Arany[ 3 ] = 200; FejlTipusok[ cfKorhaz ].Param[ 3 ][ 1 ] = 4; FejlTipusok[ cfKorhaz ].Param[ 3 ][ 2 ] = 7;

  // Populacio limit
  FejlTipusok[ cfPopLimit ].SzMax = 9;
  FejlTipusok[ cfPopLimit ].Param[ 0 ][ 1 ] = 100;
  FejlTipusok[ cfPopLimit ].KpLimit[ 1 ] = 100; FejlTipusok[ cfPopLimit ].FejlIdo[ 1 ] = 50; FejlTipusok[ cfPopLimit ].Kaja[ 1 ] = 100; FejlTipusok[ cfPopLimit ].Param[ 1 ][ 1 ] = 105;
  FejlTipusok[ cfPopLimit ].KpLimit[ 2 ] = 200; FejlTipusok[ cfPopLimit ].FejlIdo[ 2 ] = 100; FejlTipusok[ cfPopLimit ].Kaja[ 2 ] = 200; FejlTipusok[ cfPopLimit ].Vas[ 2 ] = 20; FejlTipusok[ cfPopLimit ].Arany[ 2 ] = 30; FejlTipusok[ cfPopLimit ].Param[ 2 ][ 1 ] = 110;
  FejlTipusok[ cfPopLimit ].KpLimit[ 3 ] = 300; FejlTipusok[ cfPopLimit ].FejlIdo[ 3 ] = 160; FejlTipusok[ cfPopLimit ].Kaja[ 3 ] = 300; FejlTipusok[ cfPopLimit ].Vas[ 3 ] = 40; FejlTipusok[ cfPopLimit ].Arany[ 3 ] = 60; FejlTipusok[ cfPopLimit ].Param[ 3 ][ 1 ] = 115;
  FejlTipusok[ cfPopLimit ].KpLimit[ 4 ] = 400; FejlTipusok[ cfPopLimit ].FejlIdo[ 4 ] = 220; FejlTipusok[ cfPopLimit ].Kaja[ 4 ] = 400; FejlTipusok[ cfPopLimit ].Vas[ 4 ] = 80; FejlTipusok[ cfPopLimit ].Arany[ 4 ] = 100; FejlTipusok[ cfPopLimit ].Param[ 4 ][ 1 ] = 120;
  FejlTipusok[ cfPopLimit ].KpLimit[ 5 ] = 500; FejlTipusok[ cfPopLimit ].FejlIdo[ 5 ] = 290; FejlTipusok[ cfPopLimit ].Kaja[ 5 ] = 500; FejlTipusok[ cfPopLimit ].Vas[ 5 ] = 100; FejlTipusok[ cfPopLimit ].Arany[ 5 ] = 120; FejlTipusok[ cfPopLimit ].Param[ 5 ][ 1 ] = 125;
  FejlTipusok[ cfPopLimit ].KpLimit[ 6 ] = 600; FejlTipusok[ cfPopLimit ].FejlIdo[ 6 ] = 370; FejlTipusok[ cfPopLimit ].Kaja[ 6 ] = 500; FejlTipusok[ cfPopLimit ].Vas[ 6 ] = 120; FejlTipusok[ cfPopLimit ].Arany[ 6 ] = 140; FejlTipusok[ cfPopLimit ].Param[ 6 ][ 1 ] = 130;
  FejlTipusok[ cfPopLimit ].KpLimit[ 7 ] = 700; FejlTipusok[ cfPopLimit ].FejlIdo[ 7 ] = 480; FejlTipusok[ cfPopLimit ].Kaja[ 7 ] = 500; FejlTipusok[ cfPopLimit ].Vas[ 7 ] = 140; FejlTipusok[ cfPopLimit ].Arany[ 7 ] = 160; FejlTipusok[ cfPopLimit ].Param[ 7 ][ 1 ] = 135;
  FejlTipusok[ cfPopLimit ].KpLimit[ 8 ] = 800; FejlTipusok[ cfPopLimit ].FejlIdo[ 8 ] = 580; FejlTipusok[ cfPopLimit ].Kaja[ 8 ] = 500; FejlTipusok[ cfPopLimit ].Vas[ 8 ] = 160; FejlTipusok[ cfPopLimit ].Arany[ 8 ] = 180; FejlTipusok[ cfPopLimit ].Param[ 8 ][ 1 ] = 142;
  FejlTipusok[ cfPopLimit ].KpLimit[ 9 ] = 900; FejlTipusok[ cfPopLimit ].FejlIdo[ 9 ] = 700; FejlTipusok[ cfPopLimit ].Kaja[ 9 ] = 500; FejlTipusok[ cfPopLimit ].Vas[ 9 ] = 180; FejlTipusok[ cfPopLimit ].Arany[ 9 ] = 200; FejlTipusok[ cfPopLimit ].Param[ 9 ][ 1 ] = 150;

  // Kajatermeles fejlesztese
  FejlTipusok[ cfKaja ].SzMax = 8;
  FejlTipusok[ cfKaja ].Param[ 0 ][ 1 ] = 26;
  FejlTipusok[ cfKaja ].KpLimit[ 1 ] = 40; FejlTipusok[ cfKaja ].FejlIdo[ 1 ] = 150; FejlTipusok[ cfKaja ].Kaja[ 1 ] = 0; FejlTipusok[ cfKaja ].Fa[ 1 ] = 50; FejlTipusok[ cfKaja ].Vas[ 1 ] = 10; FejlTipusok[ cfKaja ].Arany[ 1 ] = 10; FejlTipusok[ cfKaja ].Param[ 1 ][ 1 ] = 24;
  FejlTipusok[ cfKaja ].KpLimit[ 2 ] = 80; FejlTipusok[ cfKaja ].FejlIdo[ 2 ] = 150; FejlTipusok[ cfKaja ].Kaja[ 2 ] = 50; FejlTipusok[ cfKaja ].Fa[ 2 ] = 100; FejlTipusok[ cfKaja ].Vas[ 2 ] = 20; FejlTipusok[ cfKaja ].Arany[ 2 ] = 20; FejlTipusok[ cfKaja ].Param[ 2 ][ 1 ] = 22;
  FejlTipusok[ cfKaja ].KpLimit[ 3 ] = 140; FejlTipusok[ cfKaja ].FejlIdo[ 3 ] = 150; FejlTipusok[ cfKaja ].Kaja[ 3 ] = 100; FejlTipusok[ cfKaja ].Fa[ 3 ] = 150; FejlTipusok[ cfKaja ].Vas[ 3 ] = 40; FejlTipusok[ cfKaja ].Arany[ 3 ] = 40; FejlTipusok[ cfKaja ].Param[ 3 ][ 1 ] = 20;
  FejlTipusok[ cfKaja ].KpLimit[ 4 ] = 200; FejlTipusok[ cfKaja ].FejlIdo[ 4 ] = 150; FejlTipusok[ cfKaja ].Kaja[ 4 ] = 100; FejlTipusok[ cfKaja ].Fa[ 4 ] = 200; FejlTipusok[ cfKaja ].Vas[ 4 ] = 60; FejlTipusok[ cfKaja ].Arany[ 4 ] = 60; FejlTipusok[ cfKaja ].Param[ 4 ][ 1 ] = 18;
  FejlTipusok[ cfKaja ].KpLimit[ 5 ] = 320; FejlTipusok[ cfKaja ].FejlIdo[ 5 ] = 150; FejlTipusok[ cfKaja ].Kaja[ 5 ] = 100; FejlTipusok[ cfKaja ].Fa[ 5 ] = 200; FejlTipusok[ cfKaja ].Vas[ 5 ] = 80; FejlTipusok[ cfKaja ].Arany[ 5 ] = 80; FejlTipusok[ cfKaja ].Param[ 5 ][ 1 ] = 16;
  FejlTipusok[ cfKaja ].KpLimit[ 6 ] = 480; FejlTipusok[ cfKaja ].FejlIdo[ 6 ] = 150; FejlTipusok[ cfKaja ].Kaja[ 6 ] = 100; FejlTipusok[ cfKaja ].Fa[ 6 ] = 200; FejlTipusok[ cfKaja ].Vas[ 6 ] = 80; FejlTipusok[ cfKaja ].Arany[ 6 ] = 100; FejlTipusok[ cfKaja ].Param[ 6 ][ 1 ] = 14;
  FejlTipusok[ cfKaja ].KpLimit[ 7 ] = 620; FejlTipusok[ cfKaja ].FejlIdo[ 7 ] = 150; FejlTipusok[ cfKaja ].Kaja[ 7 ] = 100; FejlTipusok[ cfKaja ].Fa[ 7 ] = 200; FejlTipusok[ cfKaja ].Vas[ 7 ] = 80; FejlTipusok[ cfKaja ].Arany[ 7 ] = 150; FejlTipusok[ cfKaja ].Param[ 7 ][ 1 ] = 12;
  FejlTipusok[ cfKaja ].KpLimit[ 8 ] = 850; FejlTipusok[ cfKaja ].FejlIdo[ 8 ] = 150; FejlTipusok[ cfKaja ].Kaja[ 8 ] = 100; FejlTipusok[ cfKaja ].Fa[ 8 ] = 200; FejlTipusok[ cfKaja ].Vas[ 8 ] = 80; FejlTipusok[ cfKaja ].Arany[ 8 ] = 200; FejlTipusok[ cfKaja ].Param[ 8 ][ 1 ] = 10;

  // Fatermeles fejlesztese
  FejlTipusok[ cfFa ].SzMax = 8;
  FejlTipusok[ cfFa ].Param[ 0 ][ 1 ] = 40;
  FejlTipusok[ cfFa ].KpLimit[ 1 ] = 60; FejlTipusok[ cfFa ].FejlIdo[ 1 ] = 180; FejlTipusok[ cfFa ].Kaja[ 1 ] = 50; FejlTipusok[ cfFa ].Fa[ 1 ] = 0; FejlTipusok[ cfFa ].Vas[ 1 ] = 0; FejlTipusok[ cfFa ].Arany[ 1 ] = 0; FejlTipusok[ cfFa ].Param[ 1 ][ 1 ] = 38;
  FejlTipusok[ cfFa ].KpLimit[ 2 ] = 120; FejlTipusok[ cfFa ].FejlIdo[ 2 ] = 180; FejlTipusok[ cfFa ].Kaja[ 2 ] = 100; FejlTipusok[ cfFa ].Fa[ 2 ] = 50; FejlTipusok[ cfFa ].Vas[ 2 ] = 20; FejlTipusok[ cfFa ].Arany[ 2 ] = 20; FejlTipusok[ cfFa ].Param[ 2 ][ 1 ] = 36;
  FejlTipusok[ cfFa ].KpLimit[ 3 ] = 200; FejlTipusok[ cfFa ].FejlIdo[ 3 ] = 180; FejlTipusok[ cfFa ].Kaja[ 3 ] = 100; FejlTipusok[ cfFa ].Fa[ 3 ] = 100; FejlTipusok[ cfFa ].Vas[ 3 ] = 40; FejlTipusok[ cfFa ].Arany[ 3 ] = 40; FejlTipusok[ cfFa ].Param[ 3 ][ 1 ] = 34;
  FejlTipusok[ cfFa ].KpLimit[ 4 ] = 280; FejlTipusok[ cfFa ].FejlIdo[ 4 ] = 180; FejlTipusok[ cfFa ].Kaja[ 4 ] = 100; FejlTipusok[ cfFa ].Fa[ 4 ] = 100; FejlTipusok[ cfFa ].Vas[ 4 ] = 60; FejlTipusok[ cfFa ].Arany[ 4 ] = 60; FejlTipusok[ cfFa ].Param[ 4 ][ 1 ] = 32;
  FejlTipusok[ cfFa ].KpLimit[ 5 ] = 360; FejlTipusok[ cfFa ].FejlIdo[ 5 ] = 180; FejlTipusok[ cfFa ].Kaja[ 5 ] = 100; FejlTipusok[ cfFa ].Fa[ 5 ] = 100; FejlTipusok[ cfFa ].Vas[ 5 ] = 80; FejlTipusok[ cfFa ].Arany[ 5 ] = 80; FejlTipusok[ cfFa ].Param[ 5 ][ 1 ] = 30;
  FejlTipusok[ cfFa ].KpLimit[ 6 ] = 440; FejlTipusok[ cfFa ].FejlIdo[ 6 ] = 180; FejlTipusok[ cfFa ].Kaja[ 6 ] = 100; FejlTipusok[ cfFa ].Fa[ 6 ] = 100; FejlTipusok[ cfFa ].Vas[ 6 ] = 80; FejlTipusok[ cfFa ].Arany[ 6 ] = 100; FejlTipusok[ cfFa ].Param[ 6 ][ 1 ] = 28;
  FejlTipusok[ cfFa ].KpLimit[ 7 ] = 520; FejlTipusok[ cfFa ].FejlIdo[ 7 ] = 180; FejlTipusok[ cfFa ].Kaja[ 7 ] = 100; FejlTipusok[ cfFa ].Fa[ 7 ] = 100; FejlTipusok[ cfFa ].Vas[ 7 ] = 80; FejlTipusok[ cfFa ].Arany[ 7 ] = 150; FejlTipusok[ cfFa ].Param[ 7 ][ 1 ] = 25;
  FejlTipusok[ cfFa ].KpLimit[ 8 ] = 600; FejlTipusok[ cfFa ].FejlIdo[ 8 ] = 180; FejlTipusok[ cfFa ].Kaja[ 8 ] = 100; FejlTipusok[ cfFa ].Fa[ 8 ] = 100; FejlTipusok[ cfFa ].Vas[ 8 ] = 80; FejlTipusok[ cfFa ].Arany[ 8 ] = 200; FejlTipusok[ cfFa ].Param[ 8 ][ 1 ] = 20;

  // Vastermeles fejlesztese
  FejlTipusok[ cfVas ].SzMax = 8;
  FejlTipusok[ cfVas ].Param[ 0 ][ 1 ] = 50;
  FejlTipusok[ cfVas ].KpLimit[ 1 ] = 100; FejlTipusok[ cfVas ].FejlIdo[ 1 ] = 200; FejlTipusok[ cfVas ].Kaja[ 1 ] = 50; FejlTipusok[ cfVas ].Fa[ 1 ] = 50; FejlTipusok[ cfVas ].Vas[ 1 ] = 0; FejlTipusok[ cfVas ].Arany[ 1 ] = 0; FejlTipusok[ cfVas ].Param[ 1 ][ 1 ] = 47;
  FejlTipusok[ cfVas ].KpLimit[ 2 ] = 220; FejlTipusok[ cfVas ].FejlIdo[ 2 ] = 200; FejlTipusok[ cfVas ].Kaja[ 2 ] = 100; FejlTipusok[ cfVas ].Fa[ 2 ] = 100; FejlTipusok[ cfVas ].Vas[ 2 ] = 20; FejlTipusok[ cfVas ].Arany[ 2 ] = 20; FejlTipusok[ cfVas ].Param[ 2 ][ 1 ] = 44;
  FejlTipusok[ cfVas ].KpLimit[ 3 ] = 350; FejlTipusok[ cfVas ].FejlIdo[ 3 ] = 200; FejlTipusok[ cfVas ].Kaja[ 3 ] = 150; FejlTipusok[ cfVas ].Fa[ 3 ] = 150; FejlTipusok[ cfVas ].Vas[ 3 ] = 40; FejlTipusok[ cfVas ].Arany[ 3 ] = 40; FejlTipusok[ cfVas ].Param[ 3 ][ 1 ] = 41;
  FejlTipusok[ cfVas ].KpLimit[ 4 ] = 480; FejlTipusok[ cfVas ].FejlIdo[ 4 ] = 200; FejlTipusok[ cfVas ].Kaja[ 4 ] = 200; FejlTipusok[ cfVas ].Fa[ 4 ] = 200; FejlTipusok[ cfVas ].Vas[ 4 ] = 40; FejlTipusok[ cfVas ].Arany[ 4 ] = 80; FejlTipusok[ cfVas ].Param[ 4 ][ 1 ] = 38;
  FejlTipusok[ cfVas ].KpLimit[ 5 ] = 600; FejlTipusok[ cfVas ].FejlIdo[ 5 ] = 200; FejlTipusok[ cfVas ].Kaja[ 5 ] = 250; FejlTipusok[ cfVas ].Fa[ 5 ] = 250; FejlTipusok[ cfVas ].Vas[ 5 ] = 40; FejlTipusok[ cfVas ].Arany[ 5 ] = 120; FejlTipusok[ cfVas ].Param[ 5 ][ 1 ] = 35;
  FejlTipusok[ cfVas ].KpLimit[ 6 ] = 720; FejlTipusok[ cfVas ].FejlIdo[ 6 ] = 200; FejlTipusok[ cfVas ].Kaja[ 6 ] = 250; FejlTipusok[ cfVas ].Fa[ 6 ] = 250; FejlTipusok[ cfVas ].Vas[ 6 ] = 40; FejlTipusok[ cfVas ].Arany[ 6 ] = 160; FejlTipusok[ cfVas ].Param[ 6 ][ 1 ] = 32;
  FejlTipusok[ cfVas ].KpLimit[ 7 ] = 850; FejlTipusok[ cfVas ].FejlIdo[ 7 ] = 200; FejlTipusok[ cfVas ].Kaja[ 7 ] = 250; FejlTipusok[ cfVas ].Fa[ 7 ] = 250; FejlTipusok[ cfVas ].Vas[ 7 ] = 40; FejlTipusok[ cfVas ].Arany[ 7 ] = 200; FejlTipusok[ cfVas ].Param[ 7 ][ 1 ] = 29;
  FejlTipusok[ cfVas ].KpLimit[ 8 ] = 1000; FejlTipusok[ cfVas ].FejlIdo[ 8 ] = 200; FejlTipusok[ cfVas ].Kaja[ 8 ] = 250; FejlTipusok[ cfVas ].Fa[ 8 ] = 250; FejlTipusok[ cfVas ].Vas[ 8 ] = 40; FejlTipusok[ cfVas ].Arany[ 8 ] = 300; FejlTipusok[ cfVas ].Param[ 8 ][ 1 ] = 25;

  // Aranytermeles fejlesztese
  FejlTipusok[ cfArany ].SzMax = 8;
  FejlTipusok[ cfArany ].Param[ 0 ][ 1 ] = 60;
  FejlTipusok[ cfArany ].KpLimit[ 1 ] = 120; FejlTipusok[ cfArany ].FejlIdo[ 1 ] = 250; FejlTipusok[ cfArany ].Kaja[ 1 ] = 100; FejlTipusok[ cfArany ].Fa[ 1 ] = 100; FejlTipusok[ cfArany ].Vas[ 1 ] = 0; FejlTipusok[ cfArany ].Arany[ 1 ] = 0; FejlTipusok[ cfArany ].Param[ 1 ][ 1 ] = 57;
  FejlTipusok[ cfArany ].KpLimit[ 2 ] = 220; FejlTipusok[ cfArany ].FejlIdo[ 2 ] = 250; FejlTipusok[ cfArany ].Kaja[ 2 ] = 150; FejlTipusok[ cfArany ].Fa[ 2 ] = 150; FejlTipusok[ cfArany ].Vas[ 2 ] = 30; FejlTipusok[ cfArany ].Arany[ 2 ] = 20; FejlTipusok[ cfArany ].Param[ 2 ][ 1 ] = 54;
  FejlTipusok[ cfArany ].KpLimit[ 3 ] = 320; FejlTipusok[ cfArany ].FejlIdo[ 3 ] = 250; FejlTipusok[ cfArany ].Kaja[ 3 ] = 200; FejlTipusok[ cfArany ].Fa[ 3 ] = 200; FejlTipusok[ cfArany ].Vas[ 3 ] = 60; FejlTipusok[ cfArany ].Arany[ 3 ] = 40; FejlTipusok[ cfArany ].Param[ 3 ][ 1 ] = 51;
  FejlTipusok[ cfArany ].KpLimit[ 4 ] = 420; FejlTipusok[ cfArany ].FejlIdo[ 4 ] = 250; FejlTipusok[ cfArany ].Kaja[ 4 ] = 250; FejlTipusok[ cfArany ].Fa[ 4 ] = 250; FejlTipusok[ cfArany ].Vas[ 4 ] = 100; FejlTipusok[ cfArany ].Arany[ 4 ] = 50; FejlTipusok[ cfArany ].Param[ 4 ][ 1 ] = 48;
  FejlTipusok[ cfArany ].KpLimit[ 5 ] = 520; FejlTipusok[ cfArany ].FejlIdo[ 5 ] = 250; FejlTipusok[ cfArany ].Kaja[ 5 ] = 300; FejlTipusok[ cfArany ].Fa[ 5 ] = 300; FejlTipusok[ cfArany ].Vas[ 5 ] = 150; FejlTipusok[ cfArany ].Arany[ 5 ] = 50; FejlTipusok[ cfArany ].Param[ 5 ][ 1 ] = 45;
  FejlTipusok[ cfArany ].KpLimit[ 6 ] = 620; FejlTipusok[ cfArany ].FejlIdo[ 6 ] = 250; FejlTipusok[ cfArany ].Kaja[ 6 ] = 300; FejlTipusok[ cfArany ].Fa[ 6 ] = 350; FejlTipusok[ cfArany ].Vas[ 6 ] = 200; FejlTipusok[ cfArany ].Arany[ 6 ] = 50; FejlTipusok[ cfArany ].Param[ 6 ][ 1 ] = 40;
  FejlTipusok[ cfArany ].KpLimit[ 7 ] = 720; FejlTipusok[ cfArany ].FejlIdo[ 7 ] = 250; FejlTipusok[ cfArany ].Kaja[ 7 ] = 300; FejlTipusok[ cfArany ].Fa[ 7 ] = 400; FejlTipusok[ cfArany ].Vas[ 7 ] = 250; FejlTipusok[ cfArany ].Arany[ 7 ] = 50; FejlTipusok[ cfArany ].Param[ 7 ][ 1 ] = 35;
  FejlTipusok[ cfArany ].KpLimit[ 8 ] = 820; FejlTipusok[ cfArany ].FejlIdo[ 8 ] = 250; FejlTipusok[ cfArany ].Kaja[ 8 ] = 300; FejlTipusok[ cfArany ].Fa[ 8 ] = 500; FejlTipusok[ cfArany ].Vas[ 8 ] = 300; FejlTipusok[ cfArany ].Arany[ 8 ] = 50; FejlTipusok[ cfArany ].Param[ 8 ][ 1 ] = 30;

  // Parasztok fejlesztese, a kepessegek az EgyTipusok[]-ban vannak leirva
  FejlTipusok[ cfParaszt ].SzMax = 5;
  FejlTipusok[ cfParaszt ].KpLimit[ 0 ] = 0; FejlTipusok[ cfParaszt ].FejlIdo[ 0 ] = 0; FejlTipusok[ cfParaszt ].Kaja[ 0 ] = 0; FejlTipusok[ cfParaszt ].Fa[ 0 ] = 0; FejlTipusok[ cfParaszt ].Vas[ 0 ] = 0; FejlTipusok[ cfParaszt ].Arany[ 0 ] = 0;
  FejlTipusok[ cfParaszt ].KpLimit[ 1 ] = 0; FejlTipusok[ cfParaszt ].FejlIdo[ 1 ] = 90; FejlTipusok[ cfParaszt ].Kaja[ 1 ] = 100; FejlTipusok[ cfParaszt ].Fa[ 1 ] = 50; FejlTipusok[ cfParaszt ].Vas[ 1 ] = 0; FejlTipusok[ cfParaszt ].Arany[ 1 ] = 0;
  FejlTipusok[ cfParaszt ].KpLimit[ 2 ] = 100; FejlTipusok[ cfParaszt ].FejlIdo[ 2 ] = 90; FejlTipusok[ cfParaszt ].Kaja[ 2 ] = 200; FejlTipusok[ cfParaszt ].Fa[ 2 ] = 0; FejlTipusok[ cfParaszt ].Vas[ 2 ] = 50; FejlTipusok[ cfParaszt ].Arany[ 2 ] = 0;
  FejlTipusok[ cfParaszt ].KpLimit[ 3 ] = 200; FejlTipusok[ cfParaszt ].FejlIdo[ 3 ] = 90; FejlTipusok[ cfParaszt ].Kaja[ 3 ] = 300; FejlTipusok[ cfParaszt ].Fa[ 3 ] = 0; FejlTipusok[ cfParaszt ].Vas[ 3 ] = 0; FejlTipusok[ cfParaszt ].Arany[ 3 ] = 50;
  FejlTipusok[ cfParaszt ].KpLimit[ 4 ] = 300; FejlTipusok[ cfParaszt ].FejlIdo[ 4 ] = 90; FejlTipusok[ cfParaszt ].Kaja[ 4 ] = 400; FejlTipusok[ cfParaszt ].Fa[ 4 ] = 0; FejlTipusok[ cfParaszt ].Vas[ 4 ] = 50; FejlTipusok[ cfParaszt ].Arany[ 4 ] = 50;
  FejlTipusok[ cfParaszt ].KpLimit[ 5 ] = 500; FejlTipusok[ cfParaszt ].FejlIdo[ 5 ] = 90; FejlTipusok[ cfParaszt ].Kaja[ 5 ] = 500; FejlTipusok[ cfParaszt ].Fa[ 5 ] = 0; FejlTipusok[ cfParaszt ].Vas[ 5 ] = 100; FejlTipusok[ cfParaszt ].Arany[ 5 ] = 100;

  // Kardosok fejlesztese
  FejlTipusok[ cfKardos ].SzMax = 5;
  FejlTipusok[ cfKardos ].KpLimit[ 0 ] = 0; FejlTipusok[ cfKardos ].FejlIdo[ 0 ] = 0; FejlTipusok[ cfKardos ].Kaja[ 0 ] = 0; FejlTipusok[ cfKardos ].Fa[ 0 ] = 0; FejlTipusok[ cfKardos ].Vas[ 0 ] = 0; FejlTipusok[ cfKardos ].Arany[ 0 ] = 0;
  FejlTipusok[ cfKardos ].KpLimit[ 1 ] = 50; FejlTipusok[ cfKardos ].FejlIdo[ 1 ] = 90; FejlTipusok[ cfKardos ].Kaja[ 1 ] = 50; FejlTipusok[ cfKardos ].Fa[ 1 ] = 0; FejlTipusok[ cfKardos ].Vas[ 1 ] = 50; FejlTipusok[ cfKardos ].Arany[ 1 ] = 0;
  FejlTipusok[ cfKardos ].KpLimit[ 2 ] = 150; FejlTipusok[ cfKardos ].FejlIdo[ 2 ] = 90; FejlTipusok[ cfKardos ].Kaja[ 2 ] = 100; FejlTipusok[ cfKardos ].Fa[ 2 ] = 0; FejlTipusok[ cfKardos ].Vas[ 2 ] = 100; FejlTipusok[ cfKardos ].Arany[ 2 ] = 0;
  FejlTipusok[ cfKardos ].KpLimit[ 3 ] = 250; FejlTipusok[ cfKardos ].FejlIdo[ 3 ] = 90; FejlTipusok[ cfKardos ].Kaja[ 3 ] = 150; FejlTipusok[ cfKardos ].Fa[ 3 ] = 0; FejlTipusok[ cfKardos ].Vas[ 3 ] = 200; FejlTipusok[ cfKardos ].Arany[ 3 ] = 0;
  FejlTipusok[ cfKardos ].KpLimit[ 4 ] = 350; FejlTipusok[ cfKardos ].FejlIdo[ 4 ] = 90; FejlTipusok[ cfKardos ].Kaja[ 4 ] = 200; FejlTipusok[ cfKardos ].Fa[ 4 ] = 0; FejlTipusok[ cfKardos ].Vas[ 4 ] = 300; FejlTipusok[ cfKardos ].Arany[ 4 ] = 0;
  FejlTipusok[ cfKardos ].KpLimit[ 5 ] = 450; FejlTipusok[ cfKardos ].FejlIdo[ 5 ] = 90; FejlTipusok[ cfKardos ].Kaja[ 5 ] = 300; FejlTipusok[ cfKardos ].Fa[ 5 ] = 0; FejlTipusok[ cfKardos ].Vas[ 5 ] = 500; FejlTipusok[ cfKardos ].Arany[ 5 ] = 0;

  // Ijaszok fejlesztese
  FejlTipusok[ cfIjasz ].SzMax = 5;
  FejlTipusok[ cfIjasz ].KpLimit[ 0 ] = 0; FejlTipusok[ cfIjasz ].FejlIdo[ 0 ] = 0; FejlTipusok[ cfIjasz ].Kaja[ 0 ] = 0; FejlTipusok[ cfIjasz ].Fa[ 0 ] = 0; FejlTipusok[ cfIjasz ].Vas[ 0 ] = 0; FejlTipusok[ cfIjasz ].Arany[ 0 ] = 0;
  FejlTipusok[ cfIjasz ].KpLimit[ 1 ] = 50; FejlTipusok[ cfIjasz ].FejlIdo[ 1 ] = 90; FejlTipusok[ cfIjasz ].Kaja[ 1 ] = 100; FejlTipusok[ cfIjasz ].Fa[ 1 ] = 50; FejlTipusok[ cfIjasz ].Vas[ 1 ] = 0; FejlTipusok[ cfIjasz ].Arany[ 1 ] = 0;
  FejlTipusok[ cfIjasz ].KpLimit[ 2 ] = 150; FejlTipusok[ cfIjasz ].FejlIdo[ 2 ] = 90; FejlTipusok[ cfIjasz ].Kaja[ 2 ] = 150; FejlTipusok[ cfIjasz ].Fa[ 2 ] = 100; FejlTipusok[ cfIjasz ].Vas[ 2 ] = 0; FejlTipusok[ cfIjasz ].Arany[ 2 ] = 0;
  FejlTipusok[ cfIjasz ].KpLimit[ 3 ] = 250; FejlTipusok[ cfIjasz ].FejlIdo[ 3 ] = 90; FejlTipusok[ cfIjasz ].Kaja[ 3 ] = 200; FejlTipusok[ cfIjasz ].Fa[ 3 ] = 200; FejlTipusok[ cfIjasz ].Vas[ 3 ] = 0; FejlTipusok[ cfIjasz ].Arany[ 3 ] = 20;
  FejlTipusok[ cfIjasz ].KpLimit[ 4 ] = 350; FejlTipusok[ cfIjasz ].FejlIdo[ 4 ] = 90; FejlTipusok[ cfIjasz ].Kaja[ 4 ] = 250; FejlTipusok[ cfIjasz ].Fa[ 4 ] = 300; FejlTipusok[ cfIjasz ].Vas[ 4 ] = 0; FejlTipusok[ cfIjasz ].Arany[ 4 ] = 50;
  FejlTipusok[ cfIjasz ].KpLimit[ 5 ] = 450; FejlTipusok[ cfIjasz ].FejlIdo[ 5 ] = 90; FejlTipusok[ cfIjasz ].Kaja[ 5 ] = 300; FejlTipusok[ cfIjasz ].Fa[ 5 ] = 400; FejlTipusok[ cfIjasz ].Vas[ 5 ] = 0; FejlTipusok[ cfIjasz ].Arany[ 5 ] = 100;

  // Lovasok fejlesztese
  FejlTipusok[ cfLovas ].SzMax = 5;
  FejlTipusok[ cfLovas ].KpLimit[ 0 ] = 40; FejlTipusok[ cfLovas ].FejlIdo[ 0 ] = 0; FejlTipusok[ cfLovas ].Kaja[ 0 ] = 0; FejlTipusok[ cfLovas ].Fa[ 0 ] = 0; FejlTipusok[ cfLovas ].Vas[ 0 ] = 0; FejlTipusok[ cfLovas ].Arany[ 0 ] = 0;
  FejlTipusok[ cfLovas ].KpLimit[ 1 ] = 80; FejlTipusok[ cfLovas ].FejlIdo[ 1 ] = 90; FejlTipusok[ cfLovas ].Kaja[ 1 ] = 150; FejlTipusok[ cfLovas ].Fa[ 1 ] = 0; FejlTipusok[ cfLovas ].Vas[ 1 ] = 50; FejlTipusok[ cfLovas ].Arany[ 1 ] = 0;
  FejlTipusok[ cfLovas ].KpLimit[ 2 ] = 160; FejlTipusok[ cfLovas ].FejlIdo[ 2 ] = 90; FejlTipusok[ cfLovas ].Kaja[ 2 ] = 200; FejlTipusok[ cfLovas ].Fa[ 2 ] = 0; FejlTipusok[ cfLovas ].Vas[ 2 ] = 0; FejlTipusok[ cfLovas ].Arany[ 2 ] = 50;
  FejlTipusok[ cfLovas ].KpLimit[ 3 ] = 230; FejlTipusok[ cfLovas ].FejlIdo[ 3 ] = 90; FejlTipusok[ cfLovas ].Kaja[ 3 ] = 250; FejlTipusok[ cfLovas ].Fa[ 3 ] = 0; FejlTipusok[ cfLovas ].Vas[ 3 ] = 50; FejlTipusok[ cfLovas ].Arany[ 3 ] = 50;
  FejlTipusok[ cfLovas ].KpLimit[ 4 ] = 300; FejlTipusok[ cfLovas ].FejlIdo[ 4 ] = 90; FejlTipusok[ cfLovas ].Kaja[ 4 ] = 300; FejlTipusok[ cfLovas ].Fa[ 4 ] = 0; FejlTipusok[ cfLovas ].Vas[ 4 ] = 100; FejlTipusok[ cfLovas ].Arany[ 4 ] = 150;
  FejlTipusok[ cfLovas ].KpLimit[ 5 ] = 380; FejlTipusok[ cfLovas ].FejlIdo[ 5 ] = 90; FejlTipusok[ cfLovas ].Kaja[ 5 ] = 500; FejlTipusok[ cfLovas ].Fa[ 5 ] = 0; FejlTipusok[ cfLovas ].Vas[ 5 ] = 200; FejlTipusok[ cfLovas ].Arany[ 5 ] = 300;

  // Puskasok fejlesztese
  FejlTipusok[ cfPuskas ].SzMax = 5;
  FejlTipusok[ cfPuskas ].KpLimit[ 0 ] = 110; FejlTipusok[ cfPuskas ].FejlIdo[ 0 ] = 0; FejlTipusok[ cfPuskas ].Kaja[ 0 ] = 0; FejlTipusok[ cfPuskas ].Fa[ 0 ] = 0; FejlTipusok[ cfPuskas ].Vas[ 0 ] = 0; FejlTipusok[ cfPuskas ].Arany[ 0 ] = 0;
  FejlTipusok[ cfPuskas ].KpLimit[ 1 ] = 170; FejlTipusok[ cfPuskas ].FejlIdo[ 1 ] = 90; FejlTipusok[ cfPuskas ].Kaja[ 1 ] = 150; FejlTipusok[ cfPuskas ].Fa[ 1 ] = 0; FejlTipusok[ cfPuskas ].Vas[ 1 ] = 50; FejlTipusok[ cfPuskas ].Arany[ 1 ] = 0;
  FejlTipusok[ cfPuskas ].KpLimit[ 2 ] = 280; FejlTipusok[ cfPuskas ].FejlIdo[ 2 ] = 90; FejlTipusok[ cfPuskas ].Kaja[ 2 ] = 200; FejlTipusok[ cfPuskas ].Fa[ 2 ] = 0; FejlTipusok[ cfPuskas ].Vas[ 2 ] = 0; FejlTipusok[ cfPuskas ].Arany[ 2 ] = 50;
  FejlTipusok[ cfPuskas ].KpLimit[ 3 ] = 400; FejlTipusok[ cfPuskas ].FejlIdo[ 3 ] = 90; FejlTipusok[ cfPuskas ].Kaja[ 3 ] = 250; FejlTipusok[ cfPuskas ].Fa[ 3 ] = 0; FejlTipusok[ cfPuskas ].Vas[ 3 ] = 50; FejlTipusok[ cfPuskas ].Arany[ 3 ] = 50;
  FejlTipusok[ cfPuskas ].KpLimit[ 4 ] = 550; FejlTipusok[ cfPuskas ].FejlIdo[ 4 ] = 90; FejlTipusok[ cfPuskas ].Kaja[ 4 ] = 300; FejlTipusok[ cfPuskas ].Fa[ 4 ] = 0; FejlTipusok[ cfPuskas ].Vas[ 4 ] = 100; FejlTipusok[ cfPuskas ].Arany[ 4 ] = 150;
  FejlTipusok[ cfPuskas ].KpLimit[ 5 ] = 730; FejlTipusok[ cfPuskas ].FejlIdo[ 5 ] = 90; FejlTipusok[ cfPuskas ].Kaja[ 5 ] = 500; FejlTipusok[ cfPuskas ].Fa[ 5 ] = 0; FejlTipusok[ cfPuskas ].Vas[ 5 ] = 200; FejlTipusok[ cfPuskas ].Arany[ 5 ] = 300;
}
