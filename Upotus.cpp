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
	cout << endl << "Laivanupotuspeli" << endl;
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
void gameLoop() { // Handles the game
	char choice = '0';
	bool keep_playing = true;
	char shots[MAX_RIVI_PITUUS][MAX_RIVI_PITUUS] = { " " }; // Ammukset / osumat
	char ships[MAX_RIVI_PITUUS][MAX_RIVI_PITUUS] = { 0 }; // Laivat
	do {
		printMenu();
		do {
			choice = '0';
			cout << "Valintasi: ";
			cin >> choice;
			clearInput();
			if (choice != '1' && choice != '2' && choice != '3' && choice != 'L' && choice != 'l') {
				cout << "Virhe! ";
			}
		} while (choice != '1' && choice != '2' && choice != '3' && choice != 'L' && choice != 'l');
		cout << endl;
		// ===
		//
		// ===
		if (choice == '1') {
			// Syötä laivat
			bool laivat_ok = false;
			askDefaultShips(ships);
		}
		else if (choice == '2') {
			// Pelaa
			bool jatka_ampumista = true;
			do {
				printGameStatus(shots, MAX_RIVI_PITUUS, MAX_RIVI_PITUUS);
				shoot(shots, MAX_RIVI_PITUUS, jatka_ampumista);
			} while (jatka_ampumista);
		}
		else if (choice == '3') {
			// Arvo laivojen sijainnit
		}
		else {
			keep_playing = false; // exit the game
		}
	} while (keep_playing);
}
// --- //
void askDefaultShips(char ships[][MAX_RIVI_PITUUS]) {
	Ship laivat[4];
	short shipIndex = 0;
	char coords[3]; // +1
	char *shipNames[4] = { "kahden", "kolmen", "neljan", "viiden" };
	char shipChar = '5';

	for (int i = 5; i > 1; i--) {
		do {
			cout << "Anna " << shipNames[i - 2] << " pituisen laivan alkupiste: ";
			cin >> coords;
			clearInput();
			charcharToCoord(coords[0], laivat[shipIndex].y);
			charnumToCoord(coords[1], laivat[shipIndex].x);
			if (laivat[shipIndex].x < 0 || laivat[shipIndex].x >= MAX_RIVI_PITUUS || laivat[shipIndex].y < 0 || laivat[shipIndex].y >= MAX_RIVI_PITUUS) {
				cout << "Virheelliset koordinaatit! ";
			}
		} while (laivat[shipIndex].x < 0 || laivat[shipIndex].x >= MAX_RIVI_PITUUS || laivat[shipIndex].y < 0 || laivat[shipIndex].y >= MAX_RIVI_PITUUS);
		shipChar = i + 48; // ascii muunnos
		laivat[shipIndex].size = i;
		ships[(laivat[shipIndex].y)][(laivat[shipIndex].x)] = shipChar;
		cout << "Anna suunta ( p(ohjoinen) / i(ta) / e(tela) / l(ansi) ): ";
		cin >> laivat[shipIndex].dir;
		clearInput();
		cout << endl;
		shipIndex++;
	}
	printGameStatus(ships, MAX_RIVI_PITUUS, MAX_RIVI_PITUUS);
}
// --- //
void printGameStatus(char shotsArray[][MAX_RIVI_PITUUS], short y, short x) {
	cout << "Pelitilanne on seuraava:" << endl << endl;
	// Ylärivin numerot
	cout << "    ";
	for (int i = 1; i <= x; i++) {
		cout << i << " ";
	}
	// Yläviiva
	cout << endl << "  ";
	for (int i = 0; i < (x * 2 + 3); i++) {
		cout << "-";
	}
	// Pelialue
	cout << endl;
	for (int i = 0; i < y; i++) {
		cout << static_cast<char>('A' + i) << " | ";
		for (int o = 0; o < x; o++) {
			cout << shotsArray[i][o] << " ";
		}
		cout << "| " << static_cast<char>('A' + i) << endl;
	}
	// Alaviiva
	cout << "  ";
	for (int i = 0; i < (x * 2 + 3); i++) {
		cout << "-";
	}
	// Alarivin numerot
	cout << endl << "    ";
	for (int i = 1; i <= x; i++) {
		cout << i << " ";
	}
	cout << endl << endl;
}
// --- //
void shoot(char shots[][MAX_RIVI_PITUUS], short cols, bool &jatka) {
	char coords[3]; // -1
	short x, y;
	do {
		cout << "Anna ampumiskoordinaatit: ";
		cin.read(coords, 2);
		clearInput();
		charcharToCoord(coords[0], y);
		charnumToCoord(coords[1], x);
		if (coords[0] == 'p' || coords[0] == 'P') {
			jatka = false;
			break;
		}
		if (x < 0 || x >= MAX_RIVI_PITUUS || y < 0 || y >= MAX_RIVI_PITUUS) {
			cout << "Virheelliset koordinaatit! ";
			x = y = -1;
		}
	} while (x < 0 || x >= MAX_RIVI_PITUUS || y < 0 || y >= MAX_RIVI_PITUUS);
	// --- //
	if (jatka) {
		cout << x << " " << y << endl;
		shots[y][x] = 'x';
	}
}
// --- //
void charnumToCoord(char &old, short &uusi) {
	uusi = static_cast<short>(old) - 49;
}
// --- //
void charcharToCoord(char &old, short &uusi) {
	if (old == 'a' || old == 'A') {
		uusi = 0;
	}
	else if (old  == 'b' || old == 'B') {
		uusi = 1;
	}
	else if (old == 'c' || old == 'C') {
		uusi = 2;
	}
	else if (old == 'd' || old == 'D') {
		uusi = 3;
	}
	else if (old == 'e' || old == 'E') {
		uusi = 4;
	}
	else if (old == 'f' || old == 'F') {
		uusi = 5;
	}
	else if (old == 'g' || old == 'G') {
		uusi = 6;
	}
	else if (old == 'h' || old == 'H') {
		uusi = 7;
	}
	else if (old == 'i' || old == 'I') {
		uusi = 8;
	}
	else if (old == 'j' || old == 'J') {
		uusi = 9;
	}
}