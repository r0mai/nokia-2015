#include <iostream>

#include "VilagDef.h"
#include "Comm.h"
#include "Player.h"

using namespace std;

int main()
{
	initComm();

	TJatekos jatekos;
    // readData fuggvennyel beolvassuk a vilag allapotat (TJatekos)
	while(readData(jatekos)!=0) {		
        // A DemoPlayer fuggveny visszater egy TKoteg tipusu strukturaval.
        // Ebben vannak az utasitasok, amelyet a szervernek kuldunk.
        // Harom DemoPlayer-t definialtunk a Player.cpp-ben.
        writeData(DemoPlayer_2(jatekos));
	}

	return 0;
}