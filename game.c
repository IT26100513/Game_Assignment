#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<ctype.h>

#define SIZE 15
#define WALLS 30
#define TREASURES 12
#define HEALTH_PACKS 5
#define KEYS 3
#define DOORS 3
#define TRAPS 10

#define MAX_LOGS 100
#define LOG_LENGTH 100
#define LOG_DISPLAY 5

char logs[MAX_LOGS][LOG_LENGTH];
int logCount = 0;

int MAX_PLAYERS = 2;


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

int hiddenTrap[17][17];
			    
char arr[17][17];

Player players[MAX_PLAYERS];
Player playerssorted[MAX_PLAYERS];

int playerCount;
int roundNumber = 1;


void placeWalls(){
	int r, c;

	for(int i=0; i<WALLS; i++){
		do{
			r = rand() % SIZE+1;
			c = rand() % SIZE+1;
		}while(arr[r][c]!=' ');

		arr[r][c] = '#';
	}
}

void placeTreasures(){
	int r, c;

	for(int i=0; i<TREASURES; i++){
		do{
			r = rand() % SIZE+1;
			c = rand() % SIZE+1;
		}while(arr[r][c]!=' ');

		arr[r][c] = 'T';
	}
}

void placeHealthPacks(){
	int r, c;
	for(int i=0; i<HEALTH_PACKS; i++){
		int r, c;

		do{
			r = rand() % SIZE+1;
			c = rand() % SIZE+1;
		}while(arr[r][c] != ' ');
		arr[r][c] = 'H';
	}
}

void placeTraps() {
    int count = 0;

    while (count < TRAPS) {

        int r = rand() % SIZE+1;
        int c = rand() % SIZE+1;

        
        if (arr[r][c] == ' ' && hiddenTrap[r][c] == 0) {

            hiddenTrap[r][c] = 1;   
            count++;
        }
    }
}


void printMap(){

    for (int i = 0; i < 17; i++){

        for (int j = 0; j < 17; j++){

            int playerFound = 0;

            
            for (int p = 0; p < MAX_PLAYERS ; p++){

                if (players[p].row == i &&
                    players[p].col == j &&
                    players[p].health > 0) {

                    printf("%c ", players[p].symbol);
                    playerFound = 1;
                    break;
                }
            }
            if (!playerFound){
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
			r = rand() % SIZE+1;
			c = rand() % SIZE+1;
		} while(arr[r][c] != ' ');
		arr[r][c] = 'K';

	}
}

void placeDoors(){

	for(int i=0; i<DOORS; i++){
		int r, c;

		do {
			r = rand() % SIZE+1;
			c = rand() % SIZE+1;
		} while(arr[r][c] != ' ');
		arr[r][c] = 'D';

	}
}

void initializeMap() {

    for (int i = 0; i < 17; i++){
        arr[0][i] = '=';
		arr[16][i] = '=';   
    }

    for (int i = 1; i < 17; i++){
        arr[i][0] = '|';
        arr[i][16] = '|';   
    }

    for (int i = 1; i < 16; i++){
        for (int j = 1; j < 16; j++) {
            arr[i][j] = ' ';
        }
    }

	for(int i = 0; i < 17; i++){
		for(int j = 0; j < 17; j++){
			hiddenTrap[i][j] = 0;
		}
	}

	placeWalls();
    placeTreasures();
    placeTraps();
    placeHealthPacks();
    placeKeys();
    placeDoors();

}

void placePlayers(){
	int r, c;
	for(int i=0; i<MAX_PLAYERS; i++){
		int r, c;

		do {
			r = rand() % SIZE+1;
			c = rand() % SIZE+1;
		} while(arr[r][c] != ' ');

		players[i].row = r;
        players[i].col = c;

        players[i].score = 0;
       	players[i].health = 100;
        players[i].keys = 0;

        players[i].symbol = '1' + i;

		printf("\nEnter player ( %d ) Name: ", i+1);
		scanf("%s", players[i].name);

	}
}

int isValidMove(int x, int y){
	if(x<1 || x>=SIZE || y<1 || y>=SIZE){
		return 0;
	}
	if(arr[x][y]=='#'){
		return 0;
	}
	return 1;
}

void addLog(int r, char *m)
{
    sprintf(logs[logCount % MAX_LOGS],
            "[R%d] %s",
            r,
            m);

    logCount++;
}

void printRecentLog()
{
    printf("\n===== Recent Events =====\n");

    int total = (logCount < MAX_LOGS) ? logCount : MAX_LOGS;

    int start = (total > LOG_DISPLAY) ? total - LOG_DISPLAY : 0;

    for(int i = start; i < total; i++)
    {
        printf("%s\n", logs[i]);
    }
}

void processTile(int index){
	int r = players[index].row;
	int c = players[index].col;

	if(hiddenTrap[r][c]){
		players[index].health -= 20;
		players[index].trapsTriggered++;
		players[index].damageTaken += 20;
	}

	for(int i=0; i<HEALTH_PACKS; i++){
		hiddenTrap[r][c] = 0;
	}

	switch(toupper(arr[r][c])){
		case 'T' : players[index].treasuresFound++;
			   players[index].score += 10;
			   arr[r][c]=' ';
			   break;

		case 'H' : 
				players[index].healthPacksUsed++;
			   	players[index].health += 20;
			   	arr[r][c]=' ';
			   	players[index].health +=20;

				if(players[index].health>100)
				players[index].health=100;
			   	break;

		case 'K' : 
			   players[index].keysCollected++;
			   players[index].keys++;
			   arr[r][c]=' ';
			   break;

		case 'D' : if(players[index].keys > 0){
			   	players[index].keys--;
				players[index].doorsUnlocked++;
				arr[r][c]=' ';
			   }else{
				printf("\nPlayer needs a key !");
			   }
			   break;
	}
}

void movePlayer(int index){
	char moves[10];
	printf("\n%s, Enter Moves: \n", players[index].name);

	scanf("%s", moves);

	if(strlen(moves)>4){
		printf("Too many moves\n");
		return;
	}

	for(int i=0; i<4; i++){
		char move = toupper(moves[i]);

		int nrow = players[index].row;
		int ncol = players[index].col;

		switch(move){
			case 'W' : nrow--;
				   break;
			case 'A' : ncol--;
				   break;
			case 'S' : nrow++;
				   break;
			case 'D' : ncol++;
				   break;
			default:
				   printf("Invalid Character: %c\n", move);
				   continue;
		}

		if(isValidMove(nrow, ncol)){

			players[index].row = nrow;
			players[index].col = ncol;
			players[index].movesMade++;

			processTile(index);

		}else{
			printf("\nMove Blocked.\n");
		}
	}
}

void showScores(){
	for(int i=0; i<MAX_PLAYERS; i++){
		if(players[i].health > 0){
			players[i].score += players[i].health / 2;
		}
	}

	for(int i=0; i<MAX_PLAYERS; i++){
		playerssorted[i] = players[i];
	}

	for(int i=0; i<MAX_PLAYERS-1; i++){
		for(int j=0; j<MAX_PLAYERS-i-1; j++){
			if(playerssorted[j].score < playerssorted[j+1].score){
				Player temp = playerssorted[j];
				playerssorted[j] = playerssorted[j+1];
				playerssorted[j+1] = temp; 
			}
		}
	}

	printf("\n-------------THE LEADERBOARD-------------\n");

	for(int i=0; i<MAX_PLAYERS; i++){
		printf("%d. %s\n", i + 1, playerssorted[i].name);
        printf("   Score : %d\n", playerssorted[i].score);
        printf("   Health: %d\n", playerssorted[i].health);
	}

	if(playerssorted[0].score == playerssorted[1].score){
		printf("\nMatch Drawn. It's a Tie.\n");
	}else{
		printf("\nThe Winner is: %s\n", playerssorted[0].name);
	}
}

int allPlayersDead(){
	for(int i=0; i<MAX_PLAYERS; i++){
		if(players[i].health > 0){
			return 0;
		}
	}
	return 1;
}

void loadGame(){
    FILE *fp;

    fp = fopen("savegame.dat", "rb");

    if (fp == NULL)
    {
        printf("No save file found.\n");
        return;
    }

    fread(&MAX_PLAYERS, sizeof(int), 1, fp);

    fread(players, sizeof(Player), MAX_PLAYERS, fp);

    fread(arr, sizeof(arr), 1, fp);

    fread(hiddenTrap, sizeof(hiddenTrap), 1, fp);

    fread(&roundNumber, sizeof(int), 1, fp);

    fclose(fp);

    printf("Game loaded successfully!\n");
}


void saveGame(){
    FILE *fp;

    fp = fopen("savegame.dat", "wb");

    if (fp == NULL)
    {
        printf("Error creating save file!\n");
        return;
    }

    fwrite(&MAX_PLAYERS, sizeof(int), 1, fp);

    fwrite(players, sizeof(Player), playerCount, fp);

    fwrite(arr, sizeof(arr), 1, fp);

    fwrite(hiddenTrap, sizeof(hiddenTrap), 1, fp);

    fwrite(&roundNumber, sizeof(int), 1, fp);

    fclose(fp);

    printf("Game saved successfully!\n");
}

int remainingTreasures(){
    int count = 0;

    for(int i=1;i<=SIZE;i++)
    {
        for(int j=1;j<=SIZE;j++)
        {
            if(arr[i][j]=='T')
                count++;
        }
    }

    return count;
}

void showStats(){
	for(int i=0; i<MAX_PLAYERS; i++){
		printf("\n==========Player %d : Statistics==========", i);
		printf("\n # Moves Made: %d", players[i].movesMade);
		printf("\n # treasures Found: %d", players[i].treasuresFound);
		printf("\n # Traps Triggered: %d", players[i].trapsTriggered);
		printf("\n # Damage Taken: %d", players[i].damageTaken);
		printf("\n # Health Packs Used: %d", players[i].healthPacksUsed);
		printf("\n # Keys Collected: %d", players[i].keysCollected);
		printf("\n # Doors Unlocked: %d", players[i].doorsUnlocked);

	}

}

void saveLog()
{
    FILE *fp = fopen("gamelog.txt", "w");

    if(fp == NULL)
    {
        printf("Error creating log file!\n");
        return;
    }

    int start = (logCount > MAX_LOGS) ? logCount - MAX_LOGS : 0;

    for(int i = start; i < logCount; i++)
    {
        fprintf(fp, "%s\n", logs[i % MAX_LOGS]);
    }

    fclose(fp);
}



void gameLoop(){
    while (1){
		printf("\nRound Number : %d", roundNumber);
        printMap();

		printf("\n");
    	printRecentLog();

    	printf("\n");

        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (players[i].health > 0)
            {
                movePlayer(i);
            }
        }

        roundNumber++;

        char choice;

        printf("\nSave game? (Y/N): \n");
        scanf(" %c", &choice);

        if(toupper(choice)=='Y'){
            saveGame();
        }

        if(remainingTreasures() == 0)
            break;

        if(allPlayersDead())
            break;
    }

    showScores();
	showStats();
	saveLog();

}

int main(){
    int choice;
	int choice2;

    srand(time(NULL));

    printf("=====================================\n");
    printf("   SEEK FOR THE LOST TREASURE\n");
    printf("=====================================\n");

    printf("1. New Game\n");
    printf("2. Load Game\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if(choice == 1){
        printf("Enter number of players (2-3): ");
        scanf("%d", &choice2);

		if(choice2 == 2 || choice2 == 3){
			MAX_PLAYERS = choice2;
			initializeMap();

        	placePlayers();

        	gameLoop();
		}else{
			printf("\nWrong Player Count !");
		}

    }
    else if(choice == 2){
        loadGame();

        gameLoop();
    }
    else{
        printf("Invalid choice!\n");
    }

    return 0;
}
