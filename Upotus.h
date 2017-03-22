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

// STRUCTS
struct Ship {
	short x;
	short y;
	short size;
	short hits = 0;
	char dir;
};

// PROTOTYPES
void clearInput();
void printMenu();
void gameLoop();
void askDefaultShips(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *defShips);
void printGameStatus(char taulukko[][MAX_X_SIZE], short y, short x);
void shoot(char shots[][MAX_X_SIZE], char ships[][MAX_X_SIZE], Ship *laivat, short cols, bool &jatka, short &score);
short muunnaAmpumiskoordinaatit(std::string coord, short *row, short *col);
void nollaaLaivatJaAmmukset(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *defShips, short laivatCount, short *score);
bool tarkistaLaivanSuunta(char dir, short y, short x, short size);
void syotaLaivaKoordinaatistoon(char ships[][MAX_X_SIZE], short y, short x, char dir, short size, char shipChar);
void tarkistaOsuma(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE], Ship *laivat, short y, short x, short &score);
void upotaLaiva(char ships[][MAX_X_SIZE], short y, short x, char dir, short size);

#endif // !UPOTUS_H