/*
 * Kommunikacio.
 */

#pragma once

#include "VilagDef.h"

//----------------------------------------------------------------

void initComm( );                       // Kommunikacio inicializalasa.
bool readData( TJatekos &jatekos );     // Vilag allapotanak olvasasa.
bool writeData( const TKoteg &koteg );  // Parancsok elkuldese.
