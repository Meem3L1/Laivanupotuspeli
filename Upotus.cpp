#include "Upotus.h"
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
/*--------------------------------------------------
*
* nimi: clearInput
* toiminta: puhdistaa syötteen ylimääräisistä ja viallisista merkeistä
* parametri(t): -
* paluuarvo(t): -
*
*--------------------------------------------------*/
void clearInput() { // Clears the input from any not-valid data
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}
/*--------------------------------------------------
*
* nimi: printMenu
* toiminta: Tulostaa päävalikon
* parametri(t): -
* paluuarvo(t): -
*
*--------------------------------------------------*/
void printMenu() { // Prints out the main menu
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
/*--------------------------------------------------
*
* nimi: gameLoop
* toiminta: Pääsilmukka, joka hoitaa pelin pyörittämisen
* parametri(t): -
* paluuarvo(t): -
*
*--------------------------------------------------*/
void gameLoop() { // Handles the game
	char choice = '0';
	short score = 0;
	bool laivat_ok = false;
	bool keep_playing = true;
	bool game_finished = true;
	char shots[MAX_Y_SIZE][MAX_X_SIZE] = { 0 }; // Ammukset / osumat
	char ships[MAX_Y_SIZE][MAX_X_SIZE] = { 0 }; // Laivat
	Ship defShips[DEF_SHIP_COUNT]; // Default ships
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
		// Valikko
		// ===
		if (choice == '1') {
			// Syötä laivat
			nollaaLaivatJaAmmukset(ships, shots, defShips, DEF_SHIP_COUNT, &score);
			askDefaultShips(ships, shots, defShips);
			laivat_ok = true;
			game_finished = false;
		} else if (choice == '2' && laivat_ok && !game_finished) {
			// Pelaa
			bool jatka_ampumista = true;
			do {
				printGameStatus(shots, MAX_Y_SIZE, MAX_X_SIZE);
				shoot(shots, ships, defShips, MAX_X_SIZE, jatka_ampumista, score);
				if (score == DEF_SHIP_COUNT) {
					game_finished = true;
					printGameStatus(shots, MAX_Y_SIZE, MAX_X_SIZE);
					cout << "Onnittelut! Upotit kaikki vihollisen laivat ja voitit pelin!" << endl;
					cout << "------------------------------------------------------------" << endl;
					cout << "Tekija: Emil Pirinen (2017)" << endl;
					cout << "============================================================" << endl << endl;
				}
			} while (jatka_ampumista && !game_finished);
		} else if (choice == '3') {
			// Arvo laivojen sijainnit
		} else if (choice == 'l' || choice == 'L') {
			keep_playing = false; // exit the gameLoop
		} else if (!laivat_ok) {
			cout << "Sinun tulee ensin syottaa laivojen sijainnit, jotta voit aloittaa pelaamisen!" << endl;
		} else if (game_finished) {
			cout << "Edellinen peli on paattynyt! Sinun tulee syottaa uudet laivat, jotta voit aloittaa pelaamisen!" << endl;
		} else {
			// ERROR
		}
	} while (keep_playing);
}
/*--------------------------------------------------
*
* nimi: askDefaultShips
* toiminta: Kysyy käyttäjältä peruslaivojen sijaintitiedot
* parametri(t): ships, shots, *defShips
* paluuarvo(t): -
*
*--------------------------------------------------*/
void askDefaultShips(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *defShips) {
	short shipIndex = 0;
	short validCoords;
	string coords;
	char *shipNames[4] = { "kahden", "kolmen", "neljan", "viiden" };
	char shipChar = '5';
	bool suunta_oikein = false;
	//
	printGameStatus(ships, MAX_Y_SIZE, MAX_X_SIZE);
	for (int i = 5; i > 1; i--) {
		do {
			cout << "Anna " << shipNames[i - 2] << " pituisen laivan alkupiste: ";
			cin >> coords;
			clearInput();
			validCoords = muunnaAmpumiskoordinaatit(coords, &defShips[shipIndex].y, &defShips[shipIndex].x);
			if (validCoords == E_KOORD_VAARIN || validCoords == E_KOORD_POISTU) {
				cout << "Virheelliset koordinaatit! ";
			}
		} while (validCoords == E_KOORD_VAARIN || validCoords == E_KOORD_POISTU);
		shipChar = i + 48; // ascii muunnos -> laivan merkki
		defShips[shipIndex].size = i; // laivan koko
		do {
			cout << "Anna suunta ( p(ohjoinen) / i(ta) / e(tela) / l(ansi) ): ";
			cin >> defShips[shipIndex].dir; // suunta
			clearInput();
			suunta_oikein = tarkistaLaivanSuunta(defShips[shipIndex].dir, defShips[shipIndex].y, defShips[shipIndex].x, defShips[shipIndex].size);
			if (!suunta_oikein) {
				cout << "Virhe! ";
			}
		} while (!suunta_oikein);
		cout << endl;
		//
		syotaLaivaKoordinaatistoon(ships, defShips[shipIndex].y, defShips[shipIndex].x, defShips[shipIndex].dir, defShips[shipIndex].size, shipChar);
		//
		shipIndex++;
		suunta_oikein = false;
	}
	printGameStatus(ships, MAX_Y_SIZE, MAX_X_SIZE);
}
/*--------------------------------------------------
*
* nimi: printGameStatus
* toiminta: Tulostaa pelialueen sekä pelitilanteen
* parametri(t): taulukko, y, x
* paluuarvo(t): -
*
*--------------------------------------------------*/
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
/*--------------------------------------------------
*
* nimi: shoot
* toiminta: Hoitaa ampumisen käsittelyn
* parametri(t): shots, ships, *laivat, cols, &jatka
* paluuarvo(t): -
*
*--------------------------------------------------*/
void shoot(char shots[][MAX_X_SIZE], char ships[][MAX_X_SIZE], Ship *laivat, short cols, bool &jatka, short &score) {
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
		// Osuma-aliohjelma
		tarkistaOsuma(ships, shots, laivat, y, x, score);
	}
}
/*--------------------------------------------------
*
* nimi: muunnaAmpumiskoordinaatit
* toiminta: Muuntaa syötetyt koordinaatit indeksimuotoon
* parametri(t): coord, *row, *row
* paluuarvo(t): *row, *col
*
*--------------------------------------------------*/
short muunnaAmpumiskoordinaatit(string coord, short *row, short *col) {
	coord.erase(remove_if(coord.begin(), coord.end(), isspace), coord.end());
	if (coord[0] == 'P' || coord[0] == 'p') {
		*row = *col = -1;
		return E_KOORD_POISTU;
	} else if ((isalpha(coord[0])) && isdigit(coord[1]) && isdigit(coord[2])) {
		if (isupper(coord[0])) {
			*row = static_cast<int>(coord[0]) - 65;
			*col = 9 + (static_cast<int>(coord[2]) - 48);
		} else if (islower(coord[0])) {
			*row = static_cast<int>(coord[0]) - 97;
			*col = 9 + (static_cast<int>(coord[2]) - 48);
		}
		if (*row < 0 || *row >= MAX_Y_SIZE || *col < 0 || *col >= MAX_X_SIZE) {
			return E_KOORD_VAARIN;
		} else {
			return E_KOORD_OIKEIN;
		}
	} else if ((isalpha(coord[0])) && isdigit(coord[1])) {
		if (isupper(coord[0])) {
			*row = static_cast<int>(coord[0]) - 65;
			*col = static_cast<int>(coord[1]) - 49;
		}
		else if (islower(coord[0])) {
			*row = static_cast<int>(coord[0]) - 97;
			*col = static_cast<int>(coord[1]) - 49;
		}
		if (*row < 0 || *row >= MAX_Y_SIZE || *col < 0 || *col >= MAX_X_SIZE) {
			return E_KOORD_VAARIN;
		}
		else {
			return E_KOORD_OIKEIN;
		}
	} else {
		return E_KOORD_VAARIN;
	}
}
/*--------------------------------------------------
*
* nimi: nollaaLaivatJaAmmukset
* toiminta: Nollaa laivat, ammukset ja laivojen tiedot
* parametri(t): ships, shots, *laivat, laivatCount
* paluuarvo(t): -
*
*--------------------------------------------------*/
void nollaaLaivatJaAmmukset(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *laivat, short laivatCount, short *score) {
	for (int y = 0; y < MAX_Y_SIZE; y++) {
		for (int x = 0; x < MAX_X_SIZE; x++) {
			ships[y][x] = ' ';
			shots[y][x] = ' ';
		}
	}
	*score = 0;
	for (int i = 0; i < laivatCount; i++) {
		laivat[i].y = NULL;
		laivat[i].x = NULL;
		laivat[i].dir = NULL;
		laivat[i].size = NULL;
		laivat[i].hits = 0;
	}
}
/*--------------------------------------------------
*
* nimi: tarkistaLaivanSuunta
* toiminta: Tarkistaa voidaanko laiva suunnata haluttuun suuntaan
* parametri(t): dir, y, x, size
* paluuarvo(t): boolean (true OR false)
*
*--------------------------------------------------*/
bool tarkistaLaivanSuunta(char dir, short y, short x, short size) {
	if (dir == 'p' || dir == 'P') { // Pohjoinen
		short k = y + 1 - size;
		if (k >= 0 && k <= MAX_Y_SIZE) {
			return true;
		} else {
			return false;
		}
	} else if (dir == 'i' || dir == 'I') { // Itä
		short k = x + size;
		if (k >= 0 && k <= MAX_X_SIZE) {
			return true;
		} else {
			return false;
		}
	} else if (dir == 'e' || dir == 'E') { // Etelä
		short k = y + size;
		if (k >= 0 && k <= MAX_Y_SIZE) {
			return true;
		} else {
			return false;
		}
	} else if (dir == 'l' || dir == 'L') { // Länsi
		short k = x + 1 - size;
		if (k >= 0 && k <= MAX_X_SIZE) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}
/*--------------------------------------------------
*
* nimi: syotaLaivaKoordinaatistoon
* toiminta: Syöttää laivan loput merkit taulukkoon alkaen aloituspisteestä
* parametri(t): ships, y, x, dir, size, shipChar
* paluuarvo(t): -
*
*--------------------------------------------------*/
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
/*--------------------------------------------------
*
* nimi: tarkistaOsuma
* toiminta: Tarkistaa ammutun koordinaatin mahdollisen osuman laivaan
* parametri(t): ships, shots, *laivat, y, x
* paluuarvo(t): -
*
*--------------------------------------------------*/
void tarkistaOsuma(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *laivat, short y, short x, short &score) {
	string coords;
	coords += static_cast<char>(y + 65);
	coords += static_cast<char>(x + 49);
	//
	if (shots[y][x] == OSUMA || shots[y][x] == HUTI || shots[y][x] == UPONNUT) {
		cout << "** Olet jo ampunut tahan koordinaattiin! (" << coords << ") **" << endl << endl;
	} else if (ships[y][x] == '5' || ships[y][x] == '4' || ships[y][x] == '3' || ships[y][x] == '2') {
		short k;
		if (ships[y][x] == '5') { k = 0; }
		if (ships[y][x] == '4') { k = 1; }
		if (ships[y][x] == '3') { k = 2; }
		if (ships[y][x] == '2') { k = 3; }
		laivat[k].hits++;
		//
		if (laivat[k].hits == laivat[k].size) {
			// Upota laiva
			score++;
			upotaLaiva(shots, laivat[k].y, laivat[k].x, laivat[k].dir, laivat[k].size);
			cout << "Laukaus kohtaan " << coords << " upotti laivan." << endl << endl;
		} else {
			shots[y][x] = OSUMA;
			cout << "Laukaus kohtaan " << coords << " osui laivaan." << endl << endl;
		}
	} else {
		shots[y][x] = HUTI;
		cout << "Laukaus kohtaan " << coords << " ei osunut." << endl << endl;
	}
}
/*--------------------------------------------------
*
* nimi: upotaLaiva
* toiminta: Upottaa laivan ==> muuntaa * merkit --> #
* parametri(t): shots, y, x, dir, size
* paluuarvo(t): -
*
*--------------------------------------------------*/
void upotaLaiva(char shots[][MAX_X_SIZE], short y, short x, char dir, short size) {
	if (dir == 'p' || dir == 'P') { // Pohjoinen
		for (int z = 0; z < size; z++) {
			shots[y - z][x] = UPONNUT;
		}
	} else if (dir == 'l' || dir == 'L') { // Länsi
		for (int z = 0; z < size; z++) {
			shots[y][x - z] = UPONNUT;
		}
	} else if (dir == 'e' || dir == 'E') { // Etelä
		for (int z = 0; z < size; z++) {
			shots[y + z][x] = UPONNUT;
		}
	} else {
		for (int z = 0; z < size; z++) { // Itä
			shots[y][x + z] = UPONNUT;
		}
	}
}