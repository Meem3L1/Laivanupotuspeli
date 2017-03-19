#ifndef UPOTUS_H
#define UPOTUS_H

#include <iostream>
#include <string>
#include <algorithm>

// DEFAULTS
const char UPONNUT = '#';
const char OSUMA = '*';
const char HUTI = 'x';
const short DEF_RIVI_PITUUS = 7;
const short MAX_X_SIZE = 7;
const short MAX_Y_SIZE = 7;
const short E_KOORD_VAARIN = 0;
const short E_KOORD_OIKEIN = 1;
const short E_KOORD_POISTU = 2;

// PROTOTYPES
void clearInput();
void printMenu();
void gameLoop();
void askDefaultShips(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE]);
void printGameStatus(char taulukko[][MAX_X_SIZE], short y, short x);
void shoot(char shots[][MAX_X_SIZE], short cols, bool &jatka);
short muunnaAmpumiskoordinaatit(std::string coord, short *row, short *col);
void nollaaLaivatJaAmmukset(char ships[][MAX_X_SIZE], char shots[][MAX_X_SIZE]);
bool tarkistaLaivanSuunta(char dir, short y, short x, short size);
void syotaLaivaKoordinaatistoon(char ships[][MAX_X_SIZE], short y, short x, char dir, short size, char shipChar);

// STRUCTS
struct Ship {
	short x;
	short y;
	short size;
	char dir;
};

#endif // !UPOTUS_H