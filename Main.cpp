//========================================================//
// LAIVANUPOTUSPELI - Emil Pirinen - kev�t 2017			  //
// Ohjelmointikielet, jatko - harjoitusty�				  //
//========================================================//

#include "Upotus.h"
#include <iostream>
using namespace std;

// Variables
bool laivatSyotetty = false;

int main() {
	// --- //
	char menu;
	printMenu();
	menu = menuChoice();
	if (menu == 'l' || menu == 'L') {
		return 0; // Exit the program
	}
	printEmptyArea();
	askDefaultShips();
	// --- //
	system("pause");
	return 0;
}