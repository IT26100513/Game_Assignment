#include <stdio.h>
#include<stdlib.h>
#include<time.h>

#define SIZE 15
#define MAX_PLAYERS 2

#define WALLS 30
#define TREASURES 12
#define HEALTH_PACKS 5
#define KEYS 3
#define DOORS 3
#define TRAPS 10

typedef struct {
    char name[50];
    int row, col;
    int score;
    int health;
    int keys;
    char symbol;

    // Statistics
    int movesMade;
    int treasuresFound;
    int trapsTriggered;
    int damageTaken;
    int healthPacksUsed;
    int keysCollected;
    int doorsUnlocked;

} Player;

int hiddenTrap[SIZE][SIZE];
			    
char arr[17][17];

Player players[MAX_PLAYERS];

int playerCount;
int roundNumber = 1;

void initializeMap() {

    // Top border
    for (int i = 0; i < 17; i++) {
        arr[0][i] = '_';
    }

    // Left and right borders
    for (int i = 1; i < 16; i++) {
        arr[i][0] = '|';
        arr[i][16] = '|';   // Changed from 17 to 16
    }

    // Bottom border
    for (int i = 0; i < 17; i++) {
        arr[16][i] = '_';   // Changed from 17 to 16
    }

    // Fill the inside with spaces
    for (int i = 1; i < 16; i++) {
        for (int j = 1; j < 16; j++) {
            arr[i][j] = ' ';
        }
    }
}

void placeWalls(){
	for(int i=0; i<30; i++){
		arr[rand() % 15 + 1][rand() % 15 + 1] = '#';
	}
}

void placeTreasures(){
	int r, c;

	for(int i=0; i<TREASURES; i++){
		do{
			r=rand() % 12 + 1;
			c=rand() % 12 + 1;
		}while(arr[r][c]!=' ');

		arr[r][c] = 'T';
	}
}


void placeHealthPacks(){
	int r, c;
	for(int i=0; i<HEALTH_PACKS; i++){
		int r, c;

		do{
			r = rand() % SIZE;
			c = rand() % SIZE;
		}while(arr[r][c] != ' ');
		arr[r][c] = 'H';
	}
}

void placeTraps() {
    int count = 0;

    while (count < TRAPS) {

        int r = rand() % SIZE;
        int c = rand() % SIZE;

        
        if (arr[r][c] == ' ' && hiddenTrap[r][c] == 0) {

            hiddenTrap[r][c] = 1;   
            count++;
        }
    }
}

void printMap() {

    for (int i = 0; i < 17; i++) {

        for (int j = 0; j < 17; j++) {

            int playerFound = 0;

            // Check whether a player is at (i, j)
            for (int p = 0; p < MAX_PLAYERS ; p++) {

                if (players[p].row == i &&
                    players[p].col == j &&
                    players[p].health > 0) {

                    printf("%c ", players[p].symbol);
                    playerFound = 1;
                    break;
                }
            }

            // If no player is here, print the map tile
            if (!playerFound) {
                printf("%c ", arr[i][j]);
            }
        }

        printf("\n");
    }
}

void placeKeys(){


	for(int i=0; i<KEYS; i++){
		int r, c;

		do {
			r = rand() % SIZE;
			c = rand() % SIZE;
		} while(arr[r][c] != ' ');
		arr[r][c] = 'K';

	}
}

void placeDoors(){

	for(int i=0; i<DOORS; i++){
		int r, c;

		do {
			r = rand() % SIZE;
			c = rand() % SIZE;
		} while(arr[r][c] != ' ');
		arr[r][c] = 'D';

	}
}


void placePlayers(){
	int r, c;
	for(int i=0; i<MAX_PLAYERS; i++){
		int r, c;

		do {
			r = rand() % SIZE;
			c = rand() % SIZE;
		} while(arr[r][c] != ' ');

		players[i].row = r;
        	players[i].col = c;

        	players[i].score = 0;
       	 	players[i].health = 100;
        	players[i].keys = 0;

        	players[i].symbol = '1' + i;

	}
}


int main() {

    srand(time(NULL));

    for(int i = 0; i < SIZE; i++) {
    	for(int j = 0; j < SIZE; j++) {
        	hiddenTrap[i][j] = 0;
   	}
    }

    initializeMap();
    placeWalls();
    placeTreasures();
    placeTraps();
    placeHealthPacks();
    placeKeys();
    placeDoors();
    placePlayers();
    printMap();
    

    return 0;
}
