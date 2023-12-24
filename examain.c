#include "headerExamain.h"
#include "functionsExamain.c"

/**
 * @file examain.c
 * @brief A naval battle against a computer for an exam
 *
 * @author BeXws
 * @date 2023-12-23
 * @version 1.0
 */
// the main function
int main() {
    int numberOfWreck1=0;
    int numberOfWreck2=0;
    int turn=0;

    // Create an instance of the Game structure
    game myGame;

    // Initialize the game
    initializeGame(&myGame);

    boatOnBoard(myGame.navy1);
    boatOnBoard(myGame.navy2);

    boatOnGrid(myGame.navy1, myGame.player1);
    boatOnGrid(myGame.navy2, myGame.player2);
    displayBoatOnLine(myGame.navy1);
    // calculate how much cases are occupied by boats
    int cases= (NUMBEROFBOAT * (NUMBEROFBOAT + 1))/2;

    while (numberOfWreck1!=cases && numberOfWreck2!=cases){
        int result =0;
        if ( (turn%2) == 0){
            do{
            result=playerTurn(&myGame);
            }while (result==2);

            if(result==999){
                numberOfWreck2=cases;
                printf("You concede this battle\n");
            }
            else if(result==404){
                exit(404);
            }
            else{
                numberOfWreck1+=result;
            }
        }
        else{
            numberOfWreck2+=computerTurn(&myGame);
        }
        printf("Player 1 destroyed %d sections\n", numberOfWreck1);
        printf("Computer destroyed %d sections\n", numberOfWreck2);
        turn+=1;
    }
    if(numberOfWreck1>numberOfWreck2){
        printf("You win\n");
    }
    else{
        printf("You lose\n");
    }

    // Free dynamically allocated memory when done
    freeGame(&myGame);
    return 0;
}