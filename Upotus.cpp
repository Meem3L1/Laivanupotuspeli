#include "Upotus.h"
#include <iostream>
using namespace std;
// --- //
void clearInput() { // Clears the input from any not-valid data
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}
// --- //
void printMenu() { // Print out the main menu
	cout << "Laivanupotuspeli" << endl;
	cout << "================" << endl;
	cout << endl;
	cout << "Valinnat:" << endl;
	cout << "1) Syota laivat" << endl;
	cout << "2) Pelaa" << endl;
	cout << "3) Arvo laivojen sijainnit" << endl;
	cout << "L) Lopeta" << endl;
	cout << endl;
}
// -- //
char menuChoice() { // Asks the user to input his/her menu choice
	char choice = '0';
	do {
		cout << "Valintasi: ";
		cin >> choice;
		clearInput();
		if (choice != '1' && choice != '2' && choice != '3' && choice != 'L' && choice != 'l') {
			cout << "Virhe! ";
		}
	} while (choice != '1' && choice != '2' && choice != '3' && choice != 'L' && choice != 'l');
	cout << endl;
	return choice;
}
// --- //
void printEmptyArea() { // Prints out the empty game area
	cout << "     1 2 3 4 5 6 7    " << endl;
	cout << "   -----------------  " << endl;
	cout << " A |               | A" << endl;
	cout << " B |               | B" << endl;
	cout << " C |               | C" << endl;
	cout << " D |               | D" << endl;
	cout << " E |               | E" << endl;
	cout << " F |               | F" << endl;
	cout << " G |               | G" << endl;
	cout << "   -----------------  " << endl;
	cout << "     1 2 3 4 5 6 7    " << endl;
	cout << endl;
}
// --- //
void askDefaultShips() {
	char s5, s4, s3, s2;
	cout << "Anna viiden pituisen laivan alkupiste: ";
	s5 = shipDirection();
	cout << endl;
	cout << "Anna neljan pituisen laivan alkupiste: ";
	s4 = shipDirection();
	cout << endl;
	cout << "Anna kolmen pituisen laivan alkupiste: ";
	s3 = shipDirection();
	cout << endl;
	cout << "Anna kahden pituisen laivan alkupiste: ";
	s2 = shipDirection();
	cout << endl;

}
// --- //
char shipDirection() {
	char x = 'x';
	do {
		cout << endl << "Anna suunta ( p(ohjoinen) / i(ta) / e(tela) / l(ansi) ): ";
		cin >> x;
		clearInput();
		if (x != 'p' && x != 'i' && x != 'e' && x != 'l') {
			cout << "Virhe! ";
		}
	} while (x != 'p' && x != 'i' && x != 'e' && x != 'l');
	return x;
}