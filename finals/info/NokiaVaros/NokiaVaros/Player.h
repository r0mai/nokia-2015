/*
 * Jatekos deklaracio.
 */

#pragma once

#include "VilagDef.h"

//----------------------------------------------------------------

// Ez a DemoPlayer nem csinal semmit, nem kuld semmilyen utasitast a szervernek.
TKoteg DemoPlayer_0(TJatekos jatekos);

// Ez a DemoPlayer az egyetlen parasztjaval random lepked a terkepen.
TKoteg DemoPlayer_1(TJatekos jatekos);

// Ez a DemoPlayer kezdetben parasztokat termel es random felderit a terkepen.
// Ha valamelyik paraszt talal tokfoldet, akkor ott elkezd kajat termelni 4 paraszttal.
// Majd tovabbi 2 paraszttal elkezd fat vagni, a tobbi paraszt pedig tovabbra is random felderit.
// 5000 ido utan megprobalunk loteret epiteni (ha van ra nyersanyag), es parasztok helyett ijaszokat termelunk a tovabbiakban.
// A parasztok termeszetesen tovabbra is random lepkednek a terkepen.
TKoteg DemoPlayer_2(TJatekos jatekos);
