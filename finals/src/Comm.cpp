/*
 * Kommunikacio.
 */

#if defined(__WIN32__) || defined(_WIN32_) || defined(_MSC_VER)

#include <iostream>
#include <windows.h>
#include "Comm.h"

//----------------------------------------------------------------

unsigned char readBuffer[ 65500 ];
unsigned char writeBuffer[ 4096 ];

int( _cdecl *init_proc )( );
int( _cdecl *read_data_proc )( void *p );
int( _cdecl *write_data_proc )( void *p );

//----------------------------------------------------------------

int loadFunctions( )
{
  HINSTANCE hInstLibrary = LoadLibrary( "comm.dll" );
  if ( hInstLibrary )
  {
    init_proc = ( int( _cdecl * )( ) )GetProcAddress( hInstLibrary, "init_proc" );
    read_data_proc = ( int( _cdecl * )( void *p ) )GetProcAddress( hInstLibrary, "read_data_proc" );
    write_data_proc = ( int( _cdecl * )( void *p ) )GetProcAddress( hInstLibrary, "write_data_proc" );

    if ( init_proc )
    {
      return init_proc( );
    }
  }

  return 1;
}

//----------------------------------------------------------------

void initComm( )
{
  // Kommunikacio inicializalasa.
  const int res = loadFunctions( );
  if ( res != 0 )
  {
    std::cout << "ERROR: communication initialization" << std::endl;
    exit( res );
  }

  // VilagDef inicializacio.
  egysegInit( );
  epuletInit( );
  fejlesztesInit( );
}

//----------------------------------------------------------------

bool readData( TJatekos &jatekos )
{
  const int res = read_data_proc( &readBuffer );
  std::memcpy( &jatekos, readBuffer, sizeof( jatekos ) );
  return res != 0;
}

//----------------------------------------------------------------

bool writeData( const TKoteg& koteg )
{
  std::memcpy( writeBuffer, &koteg, sizeof( koteg ) );
  const int res = write_data_proc( &writeBuffer );
  return res != 0;
}

#else

#include "Comm.h"

void initComm( ) { }
bool readData( TJatekos &jatekos ) { return true; }
bool writeData( const TKoteg &koteg ) { return true; }

#endif
