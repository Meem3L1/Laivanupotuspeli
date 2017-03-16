#ifndef UPOTUS_H
#define UPOTUS_H

// DEFAULTS
const char UPONNUT = '#';
const char OSUMA = '*';
const char HUTI = 'x';
const short DEF_RIVI_PITUUS = 7;
const short MIN_RIVI_PITUUS = 5;
const short MAX_RIVI_PITUUS = 7;

// PROTOTYPES
void clearInput();
void printMenu();
void gameLoop();
void askDefaultShips(char ships[][MAX_RIVI_PITUUS]);
void printGameStatus(char shotsArray[][MAX_RIVI_PITUUS], short y, short x);
void shoot(char shots[][MAX_RIVI_PITUUS], short cols, bool &jatka);
void charnumToCoord(char &old, short &uusi);
void charcharToCoord(char &old, short &uusi);

// STRUCTS
struct Ship {
	short x;
	short y;
	short size;
	char dir;
};

#endif // !UPOTUS_H