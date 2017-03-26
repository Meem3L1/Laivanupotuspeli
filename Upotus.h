#ifndef UPOTUS_H
#define UPOTUS_H

#include <iostream>
#include <string>
#include <algorithm>

// DEFAULTS
const char UPONNUT = '#';
const char OSUMA = '*';
const char HUTI = 'x';
const short DEF_SHIP_COUNT = 4; // Default ship count
const short DEF_RIVI_PITUUS = 7; // Default max value for (x, y)
const short MAX_X_SIZE = 7;
const short MAX_Y_SIZE = 7;
const short E_KOORD_VAARIN = 0;
const short E_KOORD_OIKEIN = 1;
const short E_KOORD_POISTU = 2;
const short E_KOORD_CHEAT = 3;

// STRUCTS
struct Ship {
	short x;
	short y;
	short size;
	short hits = 0;
	char dir;
	char shipChar;
};

// PROTOTYPES
void clearInput();
void printMenu();
void gameLoop();
void askDefaultShips(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *defShips);
void printGameStatus(char taulukko[][MAX_X_SIZE], short y, short x, bool useCheat);
void shoot(char shots[][MAX_X_SIZE], char ships[][MAX_X_SIZE], Ship *laivat, short cols, bool &jatka, bool &cheat_used, short &score);
short muunnaKoordinaatit(std::string coord, short *row, short *col);
void nollaaLaivatJaAmmukset(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *defShips, short laivatCount, short *score);
bool tarkistaLaivanSuunta(char ships[][MAX_X_SIZE], char dir, short y, short x, short size);
void syotaLaivaKoordinaatistoon(char ships[][MAX_X_SIZE], short y, short x, char dir, short size, char shipChar);
void tarkistaOsuma(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *laivat, short y, short x, short &score);
void upotaLaiva(char ships[][MAX_X_SIZE], short y, short x, char dir, short size);
void paljastaLaivat(char ships[][MAX_X_SIZE]);
void arvoLaivojenSijainti(char ships[][MAX_X_SIZE], Ship *defShips);
bool tarkistaAloituspiste(char ships[][MAX_X_SIZE], short y, short x, short size);
void tallennaPelitilanne(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *defShips, short score);
bool lataaPelitilanne(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *laivat, short &score);
void syotaLaivanTiedot(Ship *laivat, short index, short y, short x, short size, short hits, char dir, char shipChar);

#endif // !UPOTUS_H