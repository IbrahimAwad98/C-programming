#include "../include/game.h"

int main(void)
{
    srand(time(NULL));

    printf("Welcome to TicTacToc\n");
    printf("#######################\n");
    printf("Created by Ibrahim Awad\n\n");

    char winner = ' ';
    resetBoard();
    while (winner == ' ' && checkFreeSpaces() != 0)
    {
        printBoard();
        playerMove();
        winner = checkWinner();
        if (winner != ' ' || checkFreeSpaces() == 0)
        {
            break;
        }
        computerMove();
        winner = checkWinner();
        if (winner != ' ' || checkFreeSpaces() == 0)
        {
            break;
        }
    }
    printBoard();
    printWinner(winner);
    printf("Press enter to exit....\n");
    getchar();
    getchar();
    return 0;
}