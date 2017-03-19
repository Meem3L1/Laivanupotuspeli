#include "Upotus.h"
#include <iostream>
#include <string>
#include <algorithm>
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
	bool laivat_ok = false;
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
			laivat_ok = false;
			askDefaultShips(ships, shots);
			laivat_ok = true;
		}
		else if (choice == '2' && laivat_ok) {
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
		else if (!laivat_ok) {
			cout << "Sinun tulee ensin syottaa laivojen sijainnit, jotta voit aloittaa pelaamisen!" << endl;
		}
		else {
			keep_playing = false; // exit the game
		}
	} while (keep_playing);
}
// --- //
void askDefaultShips(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE]) {
	Ship laivat[4];
	short shipIndex = 0;
	short validCoords;
	string coords;
	char *shipNames[4] = { "kahden", "kolmen", "neljan", "viiden" };
	char shipChar = '5';
	bool suunta_oikein = false;

	nollaaLaivatJaAmmukset(ships, shots);

	for (int i = 5; i > 1; i--) {
		do {
			cout << "Anna " << shipNames[i - 2] << " pituisen laivan alkupiste: ";
			cin >> coords;
			clearInput();
			validCoords = muunnaAmpumiskoordinaatit(coords, &laivat[shipIndex].y, &laivat[shipIndex].x);
			if (validCoords == E_KOORD_VAARIN || validCoords == E_KOORD_POISTU) {
				cout << "Virheelliset koordinaatit! ";
			}
		} while (validCoords == E_KOORD_VAARIN || validCoords == E_KOORD_POISTU);
		shipChar = i + 48; // ascii muunnos -> laivan merkki
		laivat[shipIndex].size = i; // laivan koko
		do {
			cout << "Anna suunta ( p(ohjoinen) / i(ta) / e(tela) / l(ansi) ): ";
			cin >> laivat[shipIndex].dir; // suunta
			clearInput();
			suunta_oikein = tarkistaLaivanSuunta(laivat[shipIndex].dir, laivat[shipIndex].y, laivat[shipIndex].x, laivat[shipIndex].size);
			if (!suunta_oikein) {
				cout << "Virhe! ";
			}
		} while (!suunta_oikein);
		cout << endl;
		//
		syotaLaivaKoordinaatistoon(ships, laivat[shipIndex].y, laivat[shipIndex].x, laivat[shipIndex].dir, laivat[shipIndex].size, shipChar);
		//
		shipIndex++;
		suunta_oikein = false;
	}
	printGameStatus(ships, MAX_Y_SIZE, MAX_X_SIZE);
}
// --- //
void printGameStatus(char taulukko[][MAX_X_SIZE], short y, short x) {
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
			cout << taulukko[i][o] << " ";
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
	string coords;
	short validCoord;
	short x, y;
	do {
		cout << "Anna ampumiskoordinaatit: ";
		cin >> coords;
		clearInput();
		validCoord = muunnaAmpumiskoordinaatit(coords, &y, &x);
		if (validCoord == E_KOORD_VAARIN) {
			cout << "Virheelliset koordinaatit! ";
			x = y = -1;
		} else if (validCoord == E_KOORD_POISTU) {
			jatka = false;
		}
	} while (validCoord == E_KOORD_VAARIN);
	// --- //
	if (jatka) {
		shots[y][x] = 'x';
	}
}
// --- //
short muunnaAmpumiskoordinaatit(string coord, short *row, short *col) {
	coord.erase(remove_if(coord.begin(), coord.end(), isspace), coord.end());
	if (coord[0] == 'P' || coord[0] == 'p') {
		*row = *col = -1;
		return E_KOORD_POISTU;
	} else if ((isalpha(coord[0])) && isdigit(coord[1])) {
		if (isupper(coord[0])) {
			*row = static_cast<int>(coord[0]) - 65;
			*col = static_cast<int>(coord[1]) - 49;
		} else if (islower(coord[0])) {
			*row = static_cast<int>(coord[0]) - 97;
			*col = static_cast<int>(coord[1]) - 49;
		}
		if (*row < 0 || *row > 6 || *col < 0 || *col > 6) {
			return E_KOORD_VAARIN;
		} else {
			return E_KOORD_OIKEIN;
		}
	} else {
		return E_KOORD_VAARIN;
	}
}
// --- //
void nollaaLaivatJaAmmukset(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE]) {
	for (int y = 0; y < MAX_Y_SIZE; y++) {
		for (int x = 0; x < MAX_X_SIZE; x++) {
			ships[y][x] = ' ';
			shots[y][x] = ' ';
		}
	}
}
// --- //
bool tarkistaLaivanSuunta(char dir, short y, short x, short size) {
	if (dir == 'p' || dir == 'P') { // Pohjoinen
		short k = y + 1 - size;
		if (k >= 0 && k <= MAX_Y_SIZE) {
			return true;
		} else {
			return false;
		}
	} else if (dir == 'i' || dir == 'I') { // Itä
		short k = x + 1 + size;
		if (k >= 0 && k <= MAX_Y_SIZE) {
			return true;
		} else {
			return false;
		}
	} else if (dir == 'e' || dir == 'E') { // Etelä
		short k = y + 1 + size;
		if (k >= 0 && k <= MAX_Y_SIZE) {
			return true;
		} else {
			return false;
		}
	} else if (dir == 'l' || dir == 'L') { // Länsi
		short k = x + 1 - size;
		if (k >= 0 && k <= MAX_Y_SIZE) {
			return true;
		} else {
			return false;
		}
	}
}
// --- //
void syotaLaivaKoordinaatistoon(char ships[][MAX_X_SIZE], short y, short x, char dir, short size, char shipChar) {
	if (dir == 'p' || dir == 'P') { // Pohjoinen
		for (int z = 0; z < size; z++) {
			ships[y - z][x] = shipChar;
		}
	} else if (dir == 'l' || dir == 'L') { // Länsi
		for (int z = 0; z < size; z++) {
			ships[y][x - z] = shipChar;
		}
	} else if (dir == 'e' || dir == 'E') { // Etelä
		for (int z = 0; z < size; z++) {
			ships[y + z][x] = shipChar;
		}
	} else {
		for (int z = 0; z < size; z++) { // Itä
			ships[y][x + z] = shipChar;
		}
	}
}