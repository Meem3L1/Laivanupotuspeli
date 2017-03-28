#include "Upotus.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
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
	cout << "4) Tallenna pelitilanne" << endl;
	cout << "5) Lataa pelitilanne" << endl;
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
void gameLoop() { // Handles the game # MAIN LOOP
	char choice = '0';
	bool laivat_ok = false;
	bool keep_playing = true;
	bool game_finished = true;
	char shots[MAX_Y_SIZE][MAX_X_SIZE] = { 0 }; // Ammukset / osumat
	char ships[MAX_Y_SIZE][MAX_X_SIZE] = { 0 }; // Laivat
	Ship defShips[DEF_SHIP_COUNT]; // Default ships
	Score score;
	do {
		printMenu();
		bool isValid = false;
		do {
			choice = '0';
			cout << "Valintasi: ";
			cin >> choice;
			clearInput();
			if (choice != '1' && choice != '2' && choice != '3' && choice != '4' && choice != '5' && choice != 'L' && choice != 'l') {
				cout << "Virhe! ";
				isValid = false;
			} else {
				isValid = true;
			}
		} while (!isValid);
		cout << endl;
		// ===
		// Valikko
		// ===
		if (choice == '1') {
			// Syötä laivat
			nollaaLaivatJaAmmukset(ships, shots, defShips, DEF_SHIP_COUNT, score);
			askDefaultShips(ships, shots, defShips);
			laivat_ok = true;
			game_finished = false;
		} else if (choice == '2' && laivat_ok && !game_finished) {
			// Pelaa
			bool jatka_ampumista = true;
			bool cheat_used = false;
			do {
				//printGameStatus(shots, MAX_Y_SIZE, MAX_X_SIZE);
				shoot(shots, ships, defShips, MAX_X_SIZE, jatka_ampumista, cheat_used, score);
				if (score.submerged == DEF_SHIP_COUNT) {
					game_finished = true;
					printGameStatus(shots, MAX_Y_SIZE, MAX_X_SIZE, false);
					cout << "ONNITTELUT! UPOTIT KAIKKI VIHOLLISEN LAIVAT JA VOITIT PELIN!" << endl;
					naytaTilastot(score, defShips);
					cout << "Tekija: Emil Pirinen (2017)" << endl;
					cout << "============================================================" << endl << endl;
				}
			} while (jatka_ampumista && !game_finished);
		} else if (choice == '3') {
			// Arvo laivojen sijainnit
			nollaaLaivatJaAmmukset(ships, shots, defShips, DEF_SHIP_COUNT, score);
			arvoLaivojenSijainti(ships, defShips);
			laivat_ok = true;
			game_finished = false;
		} else if (choice == '4') {
			// Tallenna pelitilanne
			if (laivat_ok && !game_finished) {
				tallennaPelitilanne(ships, shots, defShips, score);
			} else {
				cout << "VIRHE! Et ole aloittanut uutta pelia tai edellinen pelisi on paattynyt!" << endl;
			}
		} else if (choice == '5') {
			// Lataa pelitilanne
			bool loadFromFile = lataaPelitilanne(ships, shots, defShips, score);
			if (loadFromFile) {
				cout << "Pelitilanne ladattu!" << endl;
				game_finished = false;
				laivat_ok = true;
			}
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
	char *shipNames[4] = { "kahden", "kolmen", "neljan", "viiden" };
	printGameStatus(ships, MAX_Y_SIZE, MAX_X_SIZE, false);
	for (int i = 5; i > 1; i--) {
		bool validPlace = false;
		short validCoords = 0;
		bool isValid = false;
		bool suunta_oikein = false;
		string coords;
		defShips[shipIndex].shipChar = i + 48; // ascii muunnos -> laivan merkki
		defShips[shipIndex].size = i; // laivan koko
		defShips[shipIndex].shipName = shipNames[i - 2]; // laivan nimi
		do {
			cout << "Anna " << defShips[shipIndex].shipName << " pituisen laivan alkupiste: ";
			cin >> coords;
			clearInput();
			validCoords = muunnaKoordinaatit(coords, &defShips[shipIndex].y, &defShips[shipIndex].x);
			validPlace = tarkistaAloituspiste(ships, defShips[shipIndex].y, defShips[shipIndex].x, defShips[shipIndex].size);
			if (!validPlace || validCoords == E_KOORD_VAARIN || validCoords == E_KOORD_POISTU || validCoords == E_KOORD_CHEAT || ships[defShips[shipIndex].y][defShips[shipIndex].x] != ' ') {
				cout << "Virheelliset koordinaatit! ";
				isValid = false;
			} else {
				isValid = true;
			}
		} while (!isValid || !validPlace);
		do {
			cout << "Anna suunta ( p(ohjoinen) / i(ta) / e(tela) / l(ansi) ): ";
			cin >> defShips[shipIndex].dir; // suunta
			clearInput();
			suunta_oikein = tarkistaLaivanSuunta(ships, defShips[shipIndex].dir, defShips[shipIndex].y, defShips[shipIndex].x, defShips[shipIndex].size);
			if (!suunta_oikein) {
				cout << "Virhe! ";
			}
		} while (!suunta_oikein);
		cout << endl;
		//
		syotaLaivaKoordinaatistoon(ships, defShips[shipIndex].y, defShips[shipIndex].x, defShips[shipIndex].dir, defShips[shipIndex].size, defShips[shipIndex].shipChar);
		//
		shipIndex++;
	}
	printGameStatus(ships, MAX_Y_SIZE, MAX_X_SIZE, false);
}
/*--------------------------------------------------
*
* nimi: printGameStatus
* toiminta: Tulostaa pelialueen sekä pelitilanteen
* parametri(t): taulukko, y, x, useCheat
* paluuarvo(t): -
*
*--------------------------------------------------*/
void printGameStatus(char taulukko[][MAX_X_SIZE], short y, short x, bool useCheat) {
	if (useCheat) {
		cout << endl << "Laivat sijaitsevat seuraavissa paikoissa:" << endl << endl;
	} else {
		cout << "Pelitilanne on seuraava:" << endl << endl;
	}
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
* parametri(t): shots, ships, *laivat, cols, &jatka, &cheat_used, &score
* paluuarvo(t): -
*
*--------------------------------------------------*/
void shoot(char shots[][MAX_X_SIZE], char ships[][MAX_X_SIZE], Ship *laivat, short cols, bool &jatka, bool &cheat_used, Score &score) {
	string coords;
	short validCoord;
	short x, y;
	if (!cheat_used) {
		printGameStatus(shots, MAX_Y_SIZE, MAX_X_SIZE, false);
	}
	do {
		cout << "Anna ampumiskoordinaatit: ";
		cin >> coords;
		clearInput();
		validCoord = muunnaKoordinaatit(coords, &y, &x);
		if (validCoord == E_KOORD_VAARIN) {
			cout << "Virheelliset koordinaatit! ";
			x = y = -1;
		} else if (validCoord == E_KOORD_POISTU) {
			cheat_used = false;
			jatka = false;
		} else if (validCoord == E_KOORD_CHEAT) {
			paljastaLaivat(ships);
			cheat_used = true;
		}
	} while (validCoord == E_KOORD_VAARIN);
	// --- //
	if (jatka && validCoord != E_KOORD_CHEAT) {
		// Osuma-aliohjelma
		cheat_used = false;
		tarkistaOsuma(ships, shots, laivat, y, x, score);
	}
}
/*--------------------------------------------------
*
* nimi: muunnaKoordinaatit
* toiminta: Muuntaa syötetyt koordinaatit indeksimuotoon
* parametri(t): coord, *row, *row
* paluuarvo(t): *row, *col
*
*--------------------------------------------------*/
short muunnaKoordinaatit(string coord, short *row, short *col) {
	coord.erase(remove_if(coord.begin(), coord.end(), isspace), coord.end());
	if (coord[0] == 'P' || coord[0] == 'p') {
		*row = *col = -1;
		return E_KOORD_POISTU;
	}
	else if (coord[0] == '\\' && coord[1] == '@') {
		*row = *col = -1;
		return E_KOORD_CHEAT;
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
* parametri(t): ships, shots, *laivat, laivatCount, &score
* paluuarvo(t): -
*
*--------------------------------------------------*/
void nollaaLaivatJaAmmukset(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *laivat, short laivatCount, Score &score) {
	for (int y = 0; y < MAX_Y_SIZE; y++) {
		for (int x = 0; x < MAX_X_SIZE; x++) {
			ships[y][x] = ' ';
			shots[y][x] = ' ';
		}
	}
	score = { };
	for (int i = 0; i < DEF_SHIP_COUNT; i++) {
		laivat[i] = {};
	}
}
/*--------------------------------------------------
*
* nimi: tarkistaLaivanSuunta
* toiminta: Tarkistaa voidaanko laiva suunnata haluttuun suuntaan
* parametri(t): ships, dir, y, x, size
* paluuarvo(t): boolean (true OR false)
*
*--------------------------------------------------*/
bool tarkistaLaivanSuunta(char ships[][MAX_X_SIZE], char dir, short y, short x, short size) {
	if (dir == 'p' || dir == 'P') { // Pohjoinen
		short k = y + 1 - size;
		if (k >= 0 && k <= MAX_Y_SIZE) {
			bool valid = true;
			for (int i = 0; i < size; i++) {
				if (ships[y - i][x] != ' ') {
					valid = false;
					break; // No need to check more
				}
			} return valid;
		} else {
			return false;
		}
	} else if (dir == 'i' || dir == 'I') { // Itä
		short k = x + size;
		if (k >= 0 && k <= MAX_X_SIZE) {
			bool valid = true;
			for (int i = 0; i < size; i++) {
				if (ships[y][x + i] != ' ') {
					valid = false;
					break; // No need to check more
				}
			} return valid;
		} else {
			return false;
		}
	} else if (dir == 'e' || dir == 'E') { // Etelä
		short k = y + size;
		if (k >= 0 && k <= MAX_Y_SIZE) {
			bool valid = true;
			for (int i = 0; i < size; i++) {
				if (ships[y + i][x] != ' ') {
					valid = false;
					break; // No need to check more
				}
			} return valid;
		} else {
			return false;
		}
	} else if (dir == 'l' || dir == 'L') { // Länsi
		short k = x + 1 - size;
		if (k >= 0 && k <= MAX_X_SIZE) {
			bool valid = true;
			for (int i = 0; i < size; i++) {
				if (ships[y][x - i] != ' ') {
					valid = false;
					break; // No need to check more
				}
			} return valid;
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
* parametri(t): ships, shots, *laivat, y, x, &score
* paluuarvo(t): -
*
*--------------------------------------------------*/
void tarkistaOsuma(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *laivat, short y, short x, Score &score) {
	string coords;
	coords += static_cast<char>(y + 65);
	coords += static_cast<char>(x + 49);
	//
	if (shots[y][x] == OSUMA || shots[y][x] == HUTI || shots[y][x] == UPONNUT) {
		cout << endl << "** Olet jo ampunut tahan koordinaattiin! (" << coords << ") **" << endl << endl;
	} else if (ships[y][x] == '5' || ships[y][x] == '4' || ships[y][x] == '3' || ships[y][x] == '2') {
		// Yleiset osumalle
		short k;
		if (ships[y][x] == '5') { k = 0; }
		if (ships[y][x] == '4') { k = 1; }
		if (ships[y][x] == '3') { k = 2; }
		if (ships[y][x] == '2') { k = 3; }
		if (laivat[k].hits == 0) { laivat[k].firstHit = score.totalShots + 1; };
		laivat[k].hits++;
		score.totalHits++;
		if (!score.lastShotWasHit) {
			score.missStreakEnd = score.totalShots;
			if ((score.missStreakEnd - score.missStreakStart) >= score.missStreakRecord) {
				score.missStreakEndRec = score.missStreakEnd;
				score.missStreakStartRec = score.missStreakStart;
				score.missStreakRecord = (score.missStreakEndRec - score.missStreakStartRec) + 1;
			}
		}
		score.totalShots++;
		score.lastShotWasHit = true;
		// --- //
		if (laivat[k].hits == laivat[k].size) { // Upota laiva
			laivat[k].sinkNumber = score.submerged;
			laivat[k].shotsFromStartToDeath = score.totalShots;
			laivat[k].lastHit = score.totalShots;
			score.submerged++;
			upotaLaiva(shots, laivat[k].y, laivat[k].x, laivat[k].dir, laivat[k].size);
			cout << endl << "Laukaus kohtaan " << coords << " upotti laivan." << endl << endl;
		} else { // Merkitse osuma
			shots[y][x] = OSUMA;
			cout << endl << "Laukaus kohtaan " << coords << " osui laivaan." << endl << endl;
		}
	} else { // Hutilaukaus
		shots[y][x] = HUTI;
		score.totalShots++;
		score.totalMissed++;
		cout << endl << "Laukaus kohtaan " << coords << " ei osunut." << endl << endl;
		if (score.lastShotWasHit) {
			score.missStreakStart = score.totalShots;
		} else {
			score.missStreakEnd = score.totalShots;
		}
		score.lastShotWasHit = false;
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
/*--------------------------------------------------
*
* nimi: paljastaLaivat
* toiminta: "Huijauskoodi", joka paljastaa laivojen sijainnin
* parametri(t): ships
* paluuarvo(t): -
*
*--------------------------------------------------*/
void paljastaLaivat(char ships[][MAX_X_SIZE]) {
	printGameStatus(ships, MAX_Y_SIZE, MAX_X_SIZE, true);
}
/*--------------------------------------------------
*
* nimi: arvoLaivojenSijainti
* toiminta: Arpoo laivojen sijainnin
* parametri(t): ships, *defShips
* paluuarvo(t): -
*
*--------------------------------------------------*/
void arvoLaivojenSijainti(char ships[][MAX_X_SIZE], Ship *defShips) {
	srand(time(NULL));
	syotaLaivanTiedot(defShips, 0, "viiden", NULL, NULL, 5, NULL, NULL, '5');
	syotaLaivanTiedot(defShips, 1, "neljan", NULL, NULL, 4, NULL, NULL, '4');
	syotaLaivanTiedot(defShips, 2, "kolmen", NULL, NULL, 3, NULL, NULL, '3');
	syotaLaivanTiedot(defShips, 3, "kahden", NULL, NULL, 2, NULL, NULL, '2');
	//
	for (int i = 0; i < DEF_SHIP_COUNT; i++) {
		bool validStart = false;
		bool validDir = false;
		do {
			defShips[i].x = (rand() % MAX_X_SIZE);
			defShips[i].y = (rand() % MAX_Y_SIZE);
			validStart = tarkistaAloituspiste(ships, defShips[i].y, defShips[i].x, defShips[i].size);
			short x = (rand() % 4);
			if (x == 0) {
				defShips[i].dir = 'p';
			} else if (x == 1) {
				defShips[i].dir = 'i';
			} else if (x == 2) {
				defShips[i].dir = 'e';
			} else {
				defShips[i].dir = 'l';
			}
			validDir = tarkistaLaivanSuunta(ships, defShips[i].dir, defShips[i].y, defShips[i].x, defShips[i].size);
		} while (!validStart || !validDir);
		syotaLaivaKoordinaatistoon(ships, defShips[i].y, defShips[i].x, defShips[i].dir, defShips[i].size, defShips[i].shipChar);
	}
	cout << "Laivojen sijainnit arvottu!" << endl;
}
/*--------------------------------------------------
*
* nimi: tarkistaAloituspiste
* toiminta: Tarkistaa laivan syöttämisen aloituspisteen kelvollisuuden
* parametri(t): ships, y, x, size
* paluuarvo(t): boolean (true or false)
*
*--------------------------------------------------*/
bool tarkistaAloituspiste(char ships[][MAX_X_SIZE], short y, short x, short size) {
	bool p = tarkistaLaivanSuunta(ships, 'p', y, x, size);
	bool i = tarkistaLaivanSuunta(ships, 'i', y, x, size);
	bool e = tarkistaLaivanSuunta(ships, 'e', y, x, size);
	bool l = tarkistaLaivanSuunta(ships, 'l', y, x, size);
	//
	if (p || i || e || l) {
		return true;
	} else {
		return false;
	}
}
/*--------------------------------------------------
*
* nimi: tallennaPelitilanne
* toiminta: Tallentaa pelitilanteen erilliseen tekstitiedostoon
* parametri(t): ships, shots, *laivat, score
* paluuarvo(t): -
*
*--------------------------------------------------*/
void tallennaPelitilanne(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *laivat, Score score) {
	bool korvaa = false;
	char syote;
	if (ifstream("pelitilanne.txt")) {
		cout << "Edellinen tallennus loydetty! Haluatko varmasti korvata aiemman tallennuksen uudella? (k/e) ";
		cin >> syote;
		clearInput();
		if (syote == 'k' || syote == 'K') {
			korvaa = true;
		} else {
			korvaa = false;
		}
	} else {
		korvaa = true;
	}

	if (korvaa) {
		ofstream coordsToFile("pelitilanne.txt", ios::out);
		if (!coordsToFile) {
			cerr << "VIRHE! Tiedostoa ei voi muokata tai sita ei voitu luoda!" << endl;
		}
		else {
			coordsToFile << "# === PELIALUE ===" << endl;
			coordsToFile << "X:" << MAX_X_SIZE << endl;
			coordsToFile << "Y:" << MAX_Y_SIZE << endl;
			coordsToFile << "# === TILASTOT ===" << endl;
			coordsToFile << "totalShots:" << score.totalShots << endl;
			coordsToFile << "totalMissed:" << score.totalMissed << endl;
			coordsToFile << "totalHits:" << score.totalHits << endl;
			coordsToFile << "missStreakStart:" << score.missStreakStart << endl;
			coordsToFile << "missStreakStartRec:" << score.missStreakStartRec << endl;
			coordsToFile << "missStreakEnd:" << score.missStreakEnd << endl;
			coordsToFile << "missStreakEndRec:" << score.missStreakEndRec << endl;
			coordsToFile << "lastShotWasHit:" << score.lastShotWasHit << endl;
			coordsToFile << "missStreakRecord:" << score.missStreakRecord << endl;
			coordsToFile << "submerged:" << score.submerged << endl;
			coordsToFile << "# === LAIVATIEDOT === (index:y.x.size.hits.shotsFromStartToDeath.sinkNumber.firstHit.lastHit.dir.shipchar.shipName)" << endl;
			for (int i = 0; i < DEF_SHIP_COUNT; i++) {
				coordsToFile << i << ":" << laivat[i].y << "." << laivat[i].x << "." << laivat[i].size << "." << laivat[i].hits << ".";
				coordsToFile << laivat[i].shotsFromStartToDeath << "." << laivat[i].sinkNumber << "." << laivat[i].firstHit << ".";
				coordsToFile << laivat[i].lastHit << "." << laivat[i].dir << "." << laivat[i].shipChar << "." << laivat[i].shipName << endl;
			}
			coordsToFile << "# === SHOTS ===" << endl;
			for (int y = 0; y < MAX_Y_SIZE; y++) {
				for (int x = 0; x < MAX_X_SIZE; x++) {
					coordsToFile << shots[y][x] << ".";
				}
				coordsToFile << endl;
			}
			cout << "Pelitilanne tallennettu!" << endl;
		}
	}
	else {
		cout << "Tallennusta ei suoritettu!" << endl;
	}
}
/*--------------------------------------------------
*
* nimi: lataaPelitilanne
* toiminta: Lataa pelitilanteen erillisestä tekstitiedostosta
* parametri(t): ships, shots, *laivat, &score
* paluuarvo(t): ships, shots
*
*--------------------------------------------------*/
bool lataaPelitilanne(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *laivat, Score &score) {
	ifstream coordsFromFile("pelitilanne.txt", ios::in);
	if (!coordsFromFile) {
		cerr << "Tallennettua pelia ei voitu avata tai tiedostoa ei loydy!" << endl;
		return false;
	} else {
		nollaaLaivatJaAmmukset(ships, shots, laivat, DEF_SHIP_COUNT, score);
		string line;
		short row = 1;
		while (getline(coordsFromFile, line))
		{
			size_t posDoubleDot = line.find(':') + 1;
			if (row == 5) {
				score.totalShots = stringNumberToShort(line.substr(posDoubleDot));
			} else if (row == 6) {
				score.totalMissed = stringNumberToShort(line.substr(posDoubleDot));
			} else if (row == 7) {
				score.totalHits = stringNumberToShort(line.substr(posDoubleDot));
			} else if (row == 8) {
				score.missStreakStart = stringNumberToShort(line.substr(posDoubleDot));
			} else if (row == 9) {
				score.missStreakStartRec = stringNumberToShort(line.substr(posDoubleDot));
			} else if (row == 10) {
				score.missStreakEnd = stringNumberToShort(line.substr(posDoubleDot));
			} else if (row == 11) {
				score.missStreakEndRec = stringNumberToShort(line.substr(posDoubleDot));
			} else if (row == 12) {
				score.lastShotWasHit = stringNumberToShort(line.substr(posDoubleDot));
			} else if (row == 13) {
				score.missStreakRecord = stringNumberToShort(line.substr(posDoubleDot));
			} else if (row == 14) {
				score.submerged = stringNumberToShort(line.substr(posDoubleDot));
			} else if (row >= 16 && row <= 19) {
				short index = stringNumberToShort(line.substr(0, 1));
				string new_line = line.substr(posDoubleDot);
				laivat[index].y = stringNumberToShort(new_line);
				size_t posSingleDot = new_line.find('.') + 1;
				new_line = new_line.substr(posSingleDot);
				laivat[index].x = stringNumberToShort(new_line);
				posSingleDot = new_line.find('.') + 1;
				new_line = new_line.substr(posSingleDot);
				laivat[index].size = stringNumberToShort(new_line);
				posSingleDot = new_line.find('.') + 1;
				new_line = new_line.substr(posSingleDot);
				laivat[index].hits = stringNumberToShort(new_line);
				posSingleDot = new_line.find('.') + 1;
				new_line = new_line.substr(posSingleDot);
				laivat[index].shotsFromStartToDeath = stringNumberToShort(new_line);
				posSingleDot = new_line.find('.') + 1;
				new_line = new_line.substr(posSingleDot);
				laivat[index].sinkNumber = stringNumberToShort(new_line);
				posSingleDot = new_line.find('.') + 1;
				new_line = new_line.substr(posSingleDot);
				laivat[index].firstHit = stringNumberToShort(new_line);
				posSingleDot = new_line.find('.') + 1;
				new_line = new_line.substr(posSingleDot);
				laivat[index].lastHit = stringNumberToShort(new_line);
				posSingleDot = new_line.find('.') + 1;
				new_line = new_line.substr(posSingleDot);
				laivat[index].dir = new_line[0];
				posSingleDot = new_line.find('.') + 1;
				new_line = new_line.substr(posSingleDot);
				laivat[index].shipChar = new_line[0];
				posSingleDot = new_line.find('.') + 1;
				new_line = new_line.substr(posSingleDot);
				laivat[index].shipName = new_line;
			} else if (row >= 21) {
				short loc = 0;
				short y = row - 21;
				for (int i = 0; i < MAX_X_SIZE; i++) {
					shots[y][i] = line.at(loc);
					loc = loc + 2;
				}
			}
			row++;
		}
		for (int i = 0; i < DEF_SHIP_COUNT; i++) {
			syotaLaivaKoordinaatistoon(ships, laivat[i].y, laivat[i].x, laivat[i].dir, laivat[i].size, laivat[i].shipChar);
		}
	}
	return true;
}
/*--------------------------------------------------
*
* nimi: syotaLaivanTiedot
* toiminta: Syöttää laivan tiedot 'struct Ship':iin
* parametri(t): *laivat, index, name, y, x, size, hits, dir, shipChar
* paluuarvo(t): -
*
*--------------------------------------------------*/
void syotaLaivanTiedot(Ship *laivat, short index, string name, short y, short x, short size, short hits, char dir, char shipChar) {
	if (name[0]) {
		laivat[index].shipName = name;
	} 
	if (y) {
		laivat[index].y = y;
	}
	if (x) {
		laivat[index].x = x;
	}
	if (size) {
		laivat[index].size = size;
	}
	if (hits) {
		laivat[index].hits = hits;
	}
	if (dir) {
		laivat[index].dir = dir;
	}
	if (shipChar) {
		laivat[index].shipChar = shipChar;
	}
}
/*--------------------------------------------------
*
* nimi: naytaTilastot
* toiminta: Näyttää tilastotietoja pelistä
* parametri(t): score, *laivat
* paluuarvo(t): -
*
*--------------------------------------------------*/
void naytaTilastot(Score score, Ship *laivat) {
	cout << "============================================================" << endl;
	cout << "Kokonaislaukaukset: " << score.totalShots << endl;
	cout << "Hutilaukaukset: " << score.totalMissed << " (osumaprosentti: " << setprecision(4) << ((static_cast<double>(score.totalHits) / static_cast<double>(score.totalShots)) * 100) << "%)" << endl;
	cout << "Upottamiseen tarvitut ammukset alusta alkaen:" << endl;
	for (int i = 0; i < DEF_SHIP_COUNT; i++) {
		cout << "=> " << laivat[i].shipName << " pituinen: " << laivat[i].shotsFromStartToDeath << " laukausta" << endl;
	}
	cout << "Uppoamisjarjestys: (laukaukset 1. osuman jalkeen)" << endl;
	for (int i = 0; i < DEF_SHIP_COUNT; i++) {
		for (int z = 0; z < DEF_SHIP_COUNT; z++) {
			if (laivat[z].sinkNumber == i) {
				cout << "=> " << i + 1 << ". " << laivat[z].shipName << " pituinen: (" << (laivat[z].lastHit - laivat[z].firstHit) + 1 << " laukausta)" << endl;
			}
		}
	}
	short powerShip = 0;
	short power = 999;
	for (int i = 0; i < DEF_SHIP_COUNT; i++) {
		if (((laivat[i].lastHit - laivat[i].firstHit) / laivat[i].size) <= power) {
			power = (laivat[i].lastHit - laivat[i].firstHit) / laivat[i].size;
			powerShip = i;
		}
	}
	cout << "Tehokkain osumisjakso: laukaukset " << laivat[powerShip].firstHit << ". - " << laivat[powerShip].lastHit << ". joilla upotettiin " << laivat[powerShip].size << ":n pituinen laiva." << endl;
	cout << "Pisin ohilaukausjakso: " << score.missStreakRecord; 
	if (score.missStreakRecord == 0) {
		cout << " ohilaukausta. Olet varsinainen tarkka-ampuja!" << endl;
	} else if (score.missStreakRecord == 1) {
		cout << " ohilaukaus, " << score.missStreakStartRec << ". ammunta." << endl;
	} else {
		cout << " ohilaukausta, valilla " << score.missStreakStartRec << ". - " << score.missStreakEndRec << "." << endl;
	}
	cout << "============================================================" << endl;
}
/*--------------------------------------------------
*
* nimi: stringNumberToShort
* toiminta: Muuntaa tekstimuotoisen numeron short-muotoiseksi
* parametri(t): x
* paluuarvo(t): y
*
*--------------------------------------------------*/
short stringNumberToShort(string x) {
	short y;
	if (isdigit(x[0]) && !isdigit(x[1])) {
		y = static_cast<short>(x[0]) - 48;
	} else if (isdigit(x[0]) && isdigit(x[1])) {
		y = ((static_cast<short>(x[0]) - 48) * 10) + (static_cast<short>(x[1]) - 48);
	}
	return y;
}