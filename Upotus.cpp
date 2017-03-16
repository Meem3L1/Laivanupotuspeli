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
	char shots[MAX_Y_SIZE][MAX_X_SIZE] = { " " }; // Ammukset / osumat
	char ships[MAX_Y_SIZE][MAX_Y_SIZE] = { 0 }; // Laivat
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
				printGameStatus(shots, MAX_Y_SIZE, MAX_X_SIZE);
				shoot(shots, MAX_X_SIZE, jatka_ampumista);
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
void askDefaultShips(char ships[][MAX_X_SIZE]) {
	Ship laivat[4];
	short shipIndex = 0;
	char coords[3]; // +1
	char *shipNames[4] = { "kahden", "kolmen", "neljan", "viiden" };
	char shipChar = '5';
	bool suunta_oikein = false;

	for (int y = 0; y < MAX_Y_SIZE; y++) {
		for (int x = 0; x < MAX_X_SIZE; x++) {
			ships[y][x] = ' ';
		}
	}

	for (int i = 5; i > 1; i--) {
		do {
			cout << "Anna " << shipNames[i - 2] << " pituisen laivan alkupiste: ";
			cin >> coords;
			clearInput();
			charcharToCoord(coords[0], laivat[shipIndex].y);
			charnumToCoord(coords[1], laivat[shipIndex].x);
			if (laivat[shipIndex].x < 0 || laivat[shipIndex].x >= MAX_X_SIZE || laivat[shipIndex].y < 0 || laivat[shipIndex].y >= MAX_Y_SIZE) {
				cout << "Virheelliset koordinaatit! ";
			}
		} while (laivat[shipIndex].x < 0 || laivat[shipIndex].x >= MAX_X_SIZE || laivat[shipIndex].y < 0 || laivat[shipIndex].y >= MAX_Y_SIZE);
		shipChar = i + 48; // ascii muunnos -> laivan merkki
		laivat[shipIndex].size = i; // laivan koko
		do {
			cout << "Anna suunta ( p(ohjoinen) / i(ta) / e(tela) / l(ansi) ): ";
			cin >> laivat[shipIndex].dir; // suunta
			clearInput();
			if (laivat[shipIndex].dir == 'p' || laivat[shipIndex].dir == 'P') { // Pohjoinen
				short k = laivat[shipIndex].y + 1 - laivat[shipIndex].size;
				if (k >= 0 && k <= MAX_Y_SIZE) {
					suunta_oikein = true;
				} else { 
					suunta_oikein = false; 
				}
			} else if (laivat[shipIndex].dir == 'i' || laivat[shipIndex].dir == 'I') { // Itä
				short k = laivat[shipIndex].x + 1 + laivat[shipIndex].size;
				if (k >= 0 && k <= MAX_Y_SIZE) {
					suunta_oikein = true;
				}
				else {
					suunta_oikein = false;
				}
			} else if (laivat[shipIndex].dir == 'e' || laivat[shipIndex].dir == 'E') { // Etelä
				short k = laivat[shipIndex].y + 1 + laivat[shipIndex].size;
				if (k >= 0 && k <= MAX_Y_SIZE) {
					suunta_oikein = true;
				}
				else {
					suunta_oikein = false;
				}
			} else if (laivat[shipIndex].dir == 'l' || laivat[shipIndex].dir == 'L') { // Länsi
				short k = laivat[shipIndex].x + 1 - laivat[shipIndex].size;
				if (k >= 0 && k <= MAX_Y_SIZE) {
					suunta_oikein = true;
				}
				else {
					suunta_oikein = false;
				}
			}
			if (!suunta_oikein) {
				cout << "Virhe! ";
			}
		} while (!suunta_oikein);
		cout << endl;
		//
		if (laivat[shipIndex].dir == 'p' || laivat[shipIndex].dir == 'P') { // Pohjoinen
			for (int z = 0; z < laivat[shipIndex].size; z++) {
				ships[(laivat[shipIndex].y - z)][(laivat[shipIndex].x)] = shipChar;
			}
		} else if (laivat[shipIndex].dir == 'l' || laivat[shipIndex].dir == 'L') { // Länsi
			for (int z = 0; z < laivat[shipIndex].size; z++) {
				ships[(laivat[shipIndex].y)][(laivat[shipIndex].x - z)] = shipChar;
			}
		} else if (laivat[shipIndex].dir == 'e' || laivat[shipIndex].dir == 'E') { // Etelä
			for (int z = 0; z < laivat[shipIndex].size; z++) {
				ships[(laivat[shipIndex].y + z)][(laivat[shipIndex].x)] = shipChar;
			}
		} else {
			for (int z = 0; z < laivat[shipIndex].size; z++) { // Itä
				ships[(laivat[shipIndex].y)][(laivat[shipIndex].x + z)] = shipChar;
			}
		}
		//
		shipIndex++;
		suunta_oikein = false;
	}
	printGameStatus(ships, MAX_Y_SIZE, MAX_X_SIZE);
}
// --- //
void printGameStatus(char shotsArray[][MAX_X_SIZE], short y, short x) {
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
void shoot(char shots[][MAX_X_SIZE], short cols, bool &jatka) {
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
		if (x < 0 || x >= MAX_X_SIZE || y < 0 || y >= MAX_Y_SIZE) {
			cout << "Virheelliset koordinaatit! ";
			x = y = -1;
		}
	} while (x < 0 || x >= MAX_X_SIZE || y < 0 || y >= MAX_Y_SIZE);
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
	if (old == 'a' || old == 'A') { uusi = 0; }
	else if (old  == 'b' || old == 'B') { uusi = 1; }
	else if (old == 'c' || old == 'C') { uusi = 2; }
	else if (old == 'd' || old == 'D') { uusi = 3; }
	else if (old == 'e' || old == 'E') { uusi = 4; }
	else if (old == 'f' || old == 'F') { uusi = 5; }
	else if (old == 'g' || old == 'G') { uusi = 6; }
	else if (old == 'h' || old == 'H') { uusi = 7; }
	else if (old == 'i' || old == 'I') { uusi = 8; }
	else if (old == 'j' || old == 'J') { uusi = 9; }
}