#include "headerExamain.h"

/**
 * @file functionsExamain.c
 * @brief This file contains functions used in examain.c.
 * 
 * @author BeXws
 * @date 2023-12-23
 * @version 1.0
 */

/**
 * @brief Initialize a board.
 *
 * This function takes a board pointer and allocate memory .
 * 
 * @param tab The board pointer.
 * @return Nothing.
 */
void initializeBoard(board* tab) {
    if(tab==NULL){exit(0);}

    tab->grid = NULL;  // Set the double pointer to NULL initially
    tab->size = SIZEOFBOARD;     // Set the size to SIZEOFBOARD initially
    tab->grid = (tile**)malloc(SIZEOFBOARD * sizeof(tile*));
    if (tab->grid == NULL) {
        printf("Memory allocation failed for rows.\n");
        exit(0);   // Exit the program if memory allocation fails
    }

    // Allocate memory for each individual integer array (columns)
    for (int i = 0; i < SIZEOFBOARD; i++) {
        tab->grid[i] = (tile*)malloc(SIZEOFBOARD * sizeof(tile));
        if (tab->grid[i] == NULL) {
            printf("Memory allocation failed for columns.\n");
            exit(0);   // Exit the program if memory allocation fails
        }
    }
}

/**
 * @brief Free all allocated memory.
 *
 * This function will free all memory allocated by the game.
 * 
 * @param play The game pointer.
 * @return Nothing.
 */
void freeGame(game* play){
    if(play==NULL){exit(0);}

    free(play->navy1);
    free(play->navy2);
    for (int i = 0; i < SIZEOFBOARD; i++) {
        free(play->player1->grid[i]);   // Free individual integer arrays
        free(play->player2->grid[i]);
    }
    free(play->player1->grid);
    free(play->player2->grid);
    free(play->player1);
    free(play->player2);
}

/**
 * @brief Initialize boats.
 *
 * This function will give each boat a different size and set predefined x and y coordinates plus orientation.
 * 
 * @param fleet The boat pointer.
 * @return Nothing.
 */
void initializeBoat(boat* fleet){
    if(fleet==NULL){exit(0);}

    for (int i = 0; i < NUMBEROFBOAT; i++) {
        fleet[i].size = NUMBEROFBOAT-i;                  // Set size 
        fleet[i].position[0] = -1;            // Set x-coordinate
        fleet[i].position[1] = -1;            // Set y-coordinate 
        fleet[i].orientation = NORTHSOUTH;    // Set orientation
    }    
}

/**
 * @brief Initialize game.
 *
 * This function will allocate memory for the boards of the 2 players, plus allocate memory for each navy and initialize both board and navy
 * 
 * @param play The game pointer.
 * @return Nothing.
 */
void initializeGame(game* play) {
    if(play==NULL){exit(0);}

    // Dynamically allocate memory for player1 and player2
    play->player1 = (board*)malloc(sizeof(board));
    if(play->player1==NULL){exit(1);}

    play->player2 = (board*)malloc(sizeof(board));
    if(play->player2==NULL){exit(1);}

    // Initialize board1 and board2
    initializeBoard(play->player1);
    initializeBoard(play->player2);

    // Dynamically allocate memory for navy1 and navy2
    play->navy1 = (boat*)malloc(NUMBEROFBOAT * sizeof(boat));
    if(play->navy1==NULL){exit(1);}

    play->navy2 = (boat*)malloc(NUMBEROFBOAT * sizeof(boat));
    if(play->navy2==NULL){exit(1);}

    // Initialize navy1 and navy2
    initializeBoat(play->navy1);
    initializeBoat(play->navy2);
}

/**
 * @brief Check Position.
 *
 * This function will check if the position is already used
 * 
 * @param fleet The boat pointer.
 * @param count Number of boats already placed
 * @param x The x coordinate
 * @param y The y coordinate
 * @return 1 if position already exists, 0 if not.
 */
int positionExists(boat* fleet,int count, int x, int y) {
    if(fleet==NULL){exit(0);}
    if( (count<0) || (count>NUMBEROFBOAT) ){exit(0);}
    if( (x<0) || (x>SIZEOFBOARD) ){exit(0);}
    if( (y<0) || (y>SIZEOFBOARD) ){exit(0);}

    int xpos, ypos;
    for (int i = 0; i < count; i++) {
        xpos = fleet[i].position[0];
        ypos = fleet[i].position[1];
        for (int j = 0; j < fleet[i].size; j++){
            if (fleet[i].orientation==0){
                xpos = fleet[i].position[0]+j;
            }
            else{
                ypos = fleet[i].position[1]+j;
            }
            if ( xpos== x && ypos == y) {
            return 1;  // Position already exists
            }
        }
    }
    return 0;  // Position does not exist
}

/**
 * @brief Check if boat already on case.
 *
 * This function will check if the position is already used
 * 
 * @param fleet The boat pointer.
 * @param count Number of boats already placed
 * @param x The x coordinate
 * @param y The y coordinate
 * @param size Size of the boat to add to board
 * @param isHorizontal If the boat is placed looking south(0) or east(1)
 * @return 1 if position already exists, 0 if not.
 */
int overlapsWithExistingBoats(boat* fleet,int count, int x, int y, int size, int isHorizontal) {
    if( (fleet==NULL) ){exit(0);}
    if( (count<0) || (count>NUMBEROFBOAT) ){exit(0);}
    if( (x<0) || (x>SIZEOFBOARD) ){exit(0);}
    if( (y<0) || (y>SIZEOFBOARD) ){exit(0);}
    if( (size<0) || (size>fleet[0].size) ){exit(0);}
    if( (isHorizontal<0) || (isHorizontal>1) ){exit(0);}

    if (isHorizontal) {
        for (int i = 0; i < size; i++) {
            if (positionExists(fleet, count, x, y + i)) {
                return 1;  // Overlaps with existing boat
            }
        }
    } else {
        for (int i = 0; i < size; i++) {
            if (positionExists(fleet, count, x + i, y)) {
                return 1;  // Overlaps with existing boat
            }
        }
    }
    return 0;  // Does not overlap with existing boats
}

/**
 * @brief Give boats position
 *
 * This function will give each boat a different position
 * 
 * @param fleet The boat pointer.
 * @return nothing
 */
void boatOnBoard(boat* fleet){
    if(fleet==NULL){exit(0);}
    srand(INITRANDOM);

    for (int i = 0; i < NUMBEROFBOAT; i++){
        int x, y, isHorizontal;
        int big = fleet[i].size;
        do {
            x = rand() % (SIZEOFBOARD-big);  // Generate random x-coordinate (0 to 9)
            y = rand() % (SIZEOFBOARD-big);  // Generate random y-coordinate (0 to 9)
            isHorizontal = rand() % 2;  // Generate random orientation (0 for vertical, 1 for horizontal)
        } while ( overlapsWithExistingBoats(fleet, i, x, y, big, isHorizontal) );
        fleet[i].position[0]=x;
        fleet[i].position[1]=y;
        fleet[i].orientation=isHorizontal;
    }
}

/**
 * @brief Display Boats Line
 *
 * This function will display only boats, their position and orientation
 * 
 * @param fleet The boat pointer.
 * @return nothing
 */
void displayBoatOnLine(boat* fleet){
    if(fleet==NULL){exit(0);}

    for (int k = 0; k < NUMBEROFBOAT; k++) {
        printf("Boat %d - Size: %d, Position: (%d, %d), Orientation: %d \n",
               k + 1, fleet[k].size, fleet[k].position[0],
               fleet[k].position[1],
               fleet[k].orientation);
    }
}

/**
 * @brief Display Boats Grid
 *
 * This function will display the whole grid, depending on what's there
 * 
 * @param tab The board pointer.
 * @param isBool 1 To hide boat, 0 to show them
 * @return nothing
 */
void displayGrid(board* tab, int isBool) {
    if(tab==NULL){exit(0);}
    if( (isBool<0) || (isBool>1) ){exit(0);}

    printf("   ");  // Print initial space for alignment

    // Print column labels (0-9)
    for (int i = 0; i < SIZEOFBOARD; i++) {
        printf("%2d ", i);
    }
    printf("\n");

    // Print grid rows and boats
    for (int i = 0; i < SIZEOFBOARD; i++) {
        printf("%2d ", i);  // Print row label
        for (int j = 0; j < SIZEOFBOARD; j++) {
            switch (tab->grid[i][j]){
                // Print " " if water exists at this position
                case WATER:
                    printf("   ");
                    break;

                // Print "X" if water has been shot at this position
                case WATER_SHOT:
                        printf(" X ");
                    break;

                // Print "B" if a boat exists at this position
                case BOAT:
                    if (isBool==0){
                        printf(" B ");
                    }
                    break;

                // Print "W" if a boat has been shot at this position
                case WRECK:
                    printf(" W ");
                    break;

                default:
                    break;
            }
        }
        printf("\n");
    }
}

/**
 * @brief Initialize grid
 *
 * This function will initialize the whole grid, depending on what's there
 * 
 * @param fleet The boat pointer.
 * @param player The board pointer.
 * @return nothing
 */
void boatOnGrid(boat* fleet, board* player){
    if(fleet==NULL){exit(0);}
    if(player==NULL){exit(0);}

    int x, y;
    // first place water everywhere
    for (int i = 0; i < SIZEOFBOARD; i++){
        for (int j = 0; j < SIZEOFBOARD; j++){
            player->grid[i][j]=WATER;
        }
    }
    // now place boat where they are
    for (int i = 0; i < NUMBEROFBOAT; i++){
            x=fleet[i].position[0];
            y=fleet[i].position[1];
        for (int j = 0; j < fleet[i].size; j++){
            player->grid[x][y]=BOAT;
            if(fleet[i].orientation==0){
                x=x+1;
            }
            if (fleet[i].orientation==1){
                y=y+1;
            }
        }
    } 
}

/**
 * @brief Flush Standard Input
 *
 * This function will flush the standard input
 * 
 * @return nothing
 */
void flushStdIn(){
    char c;
    
    do{
        c= getchar();
    }while(c!= '\n');
}

/**
 * @brief Check if Boat is hit
 *
 * This function will check is a boat was hit by a shot
 * 
 * @param player The board pointer.
 * @param x the x coordinate
 * @param y the y coordinate
 * @return 1 if a boat was wrecked, 0 otherwise
 */
int isWrecked(board* player, int x, int y){
    if(player==NULL){exit(0);}
    if( (x<0) || (x>SIZEOFBOARD) ){exit(0);}
    if( (y<0) || (y>SIZEOFBOARD) ){exit(0);}

    printf("%d %d\n", x, y);
    if (player->grid[x][y]==101){
        printf("Hit\n");
        return 1;
    }
    printf("Miss\n");
    return 0;
}

/**
 * @brief Used when a player is firing
 *
 * This function will ask coordinates to fire, and check if the player already shot there
 * 
 * @param myGame The game pointer.
 * @param x the x coordinate
 * @param y the y coordinate
 * @return 1 if a boat was wrecked, 0 otherwise
 */
int firePlayer(game* myGame){
    if(myGame==NULL){exit(0);}
    int shoot = 0;
    int res =-1;
    int xvalue =-1;
    int yvalue =-1;
    
    while(shoot!=1){
        printf("Where do you want to fire ?\n");
        printf("Enter a column number between 0 and 9\n");
        do{
            res = scanf("%d", &xvalue);
            flushStdIn();
        }while( xvalue <0 || xvalue>9 || res != 1);

        printf("Enter a row number between 0 and 9\n");
        do{
            res = scanf("%d", &yvalue);
            flushStdIn();
        }while( yvalue <0 || yvalue>9 || res != 1);
        if ( (myGame->player2->grid[xvalue][yvalue]%2) == 1){
            printf("You already shot there\n");
        }
        else{
            myGame->player2->grid[xvalue][yvalue]+=1;
            shoot=1;
        }
    }
    printf("Player1 is firing\n");
    return isWrecked(myGame->player2, xvalue, yvalue);
}

/**
 * @brief Player turn
 *
 * This function will lead a player turn 
 * 
 * @param myGame The game pointer.
 * 
 * @return 1 if a boat was wrecked, 0 if not, 999 if player surrendered, and 404 if an error occured
 */
int playerTurn(game* myGame){
    if(myGame==NULL){exit(0);}

    printf("What do you want to do ?\n");
    printf("1: Fire\n2: Check Ennemy Side\n3: Inspect Own Boats\n4: Surrender\n");
    int res =-1;
    int value =-1;
    do{
        res = scanf("%d", &value);
        flushStdIn();
    }while( value <1 || value>4 || res != 1);
    switch (value){
    case 1:
        return firePlayer(myGame);
        break;
    
    case 2:
        displayGrid(myGame->player2, 1);
        break;

    case 3:
        displayGrid(myGame->player1, 0);
        break;
    
    case 4:
        return 999;
        break;

    default:
    //should not happen
        printf("It's not a bug, it's a feature");
        return 404;
        break;
    }
    return 2;
}

/**
 * @brief Computer turn
 *
 * This function will lead a computer turn 
 * 
 * @param myGame The game pointer.
 * 
 * @return 1 if a boat was wrecked, 0 otherwise
 */
int computerTurn(game* myGame){
    if(myGame==NULL){exit(0);}

    srand(INITRANDOM);
    int x,y;
    do{
        x = rand() % SIZEOFBOARD;
        y = rand() % SIZEOFBOARD;
    } while ( (myGame->player1->grid[x][y]%2) == 1);
    myGame->player1->grid[x][y]+=1;
    printf("Computer is firing\n");
    return isWrecked(myGame->player1, x, y);
    
}