/**
 * @file functionsExamain.h
 * @brief This file contains inclusions, definitions, and functions'prototype for functions used in examain.c and functionsExamain.c.
 * 
 * @author BeXws
 * @date 2023-12-23
 * @version 1.0
 */

#ifndef HEADER_EXAMAIN_H
#define HEADER_EXAMAIN_H

#include <stdio.h> // the standard input output library
#include <stdlib.h> // the standard library
#include <time.h> // for the use of random function
#define SIZEOFBOARD 10 // the grid is 10*10, but you could possibly go to 99 whithout any problems
#define NUMBEROFBOAT 5 // the number of boat is 5, and you could possibly go higher, but if the board is not big enough, the program will be stuck
#define INITRANDOM 40 // the initialisation of the random function, in this configuration you have the same boat setup as the computer and should win easily, you could change it to "time(0)" and see if you're better than the CPU 

// 4 different types of tile
typedef enum {
    WATER,
    WATER_SHOT,
    BOAT = 100,
    WRECK
} tile;

// 2 different orientation for boats
typedef enum{
    NORTHSOUTH,
    WESTEAST
} oriented;

// structure for boat, position is the upper left point
typedef struct{
    int size;
    oriented orientation;
    int position[2];
} boat;

// structure for the board with a double pointer
typedef struct{
    tile** grid;
    int size;
} board;

// structure for the game
typedef struct {
    board* player1; // Pointer to the first player board
    board* player2; // Pointer to the second player board
    boat* navy1;    // Array of boats for first player
    boat* navy2;    // Array of boats for second player
} game;

// initialisation of the board
void initializeBoard(board* tab);

// Function to free dynamically allocated memory for the game
void freeGame(game* play);

// initialisation of the boats
void initializeBoat(boat* fleet);

// Function to initialize the game
void initializeGame(game* play);

// check if a position is already used
int positionExists(boat* fleet,int count, int x, int y);

// Function to check if a boat with a given position and size would overlap with other boats
int overlapsWithExistingBoats(boat* fleet,int count, int x, int y, int size, int isHorizontal);

// generate coordinates for boats
void boatOnBoard(boat* fleet);

// Display the positions and sizes of boats
void displayBoatOnLine(boat* fleet);

// Function to display the grid with marks
void displayGrid(board* tab, int isbool);

// put boats on the grid
void boatOnGrid(boat* fleet, board* player);

// flush the standard input 
void flushStdIn();

// check if a shot result in a wrecked ship
int isWrecked(board* player, int x, int y);

// let the player choose where to shoot
int firePlayer(game* myGame);

// function to lead a player turn
int playerTurn(game* myGame);

// function to lead a computer turn
int computerTurn(game* myGame);

#endif // HEADER_EXAMAIN_H