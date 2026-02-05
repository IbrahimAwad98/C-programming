#include "../include/game.h"

char board[ROWS][COLUMNS];
const char Player = 'X';
const char Computer = 'Y';

void resetBoard()
{
    // rows
    for (int i = 0; i < ROWS; i++)
    {
        // columns
        for (int j = 0; j < COLUMNS; j++)
        {
            board[i][j] = ' ';
        }
    }
}
void printBoard()
{
    printf(" %c | %c | %c ", board[0][0], board[0][1], board[0][2]);
    printf("\n---|---|---\n");
    printf(" %c | %c | %c ", board[1][0], board[1][1], board[1][2]);
    printf("\n---|---|---\n");
    printf(" %c | %c | %c ", board[2][0], board[2][1], board[2][2]);
    printf("\n");
}
int checkFreeSpaces()
{
    int freeSpaces = 9;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            if (board[i][j] != ' ')
            {
                freeSpaces--;
            }
        }
    }
    return freeSpaces;
}
void playerMove()
{
    int x, y;
    printf("Enter row #(1-3): ");
    if (scanf("%d", &x) != 1)
    {
        printf("Invalid input! Please enter a number.\n");
        while (getchar() != '\n')
            ; // Rensa bufferten
        playerMove();
        return;
    }

    printf("Enter column #(1-3): ");
    if (scanf("%d", &y) != 1)
    {
        printf("Invalid input! Please enter a number.\n");
        while (getchar() != '\n')
            ; // Rensa bufferten
        playerMove();
        return;
    }

    x--;
    y--;

    if (x < 0 || x >= ROWS || y < 0 || y >= COLUMNS)
    {
        printf("Invalid move! Please enter numbers between 1 and 3.\n");
        playerMove();
        return;
    }

    if (board[x][y] != ' ')
    {
        printf("Invalid move! That spot is already taken.\n");
        playerMove();
        return;
    }

    board[x][y] = Player;
}
void computerMove()
{

    int x, y;
    if (checkFreeSpaces() > 0)
    {
        do
        {
            x = rand() % 3;
            y = rand() % 3;
        } while (board[x][y] != ' ');
        board[x][y] = Computer;
    }
    else
    {
        printWinner(' ');
    }
}
char checkWinner()
{
    // rows
    for (int i = 0; i < ROWS; i++)
    {
        if (board[i][0] == board[i][1] && board[i][0] == board[i][2])
        {
            return board[i][0];
        }
    }
    // columns
    for (int i = 0; i < COLUMNS; i++)
    {
        if (board[0][i] == board[1][i] && board[0][i] == board[2][i])
        {
            return board[0][i];
        }
    }
    // check diganols
    if (board[0][0] == board[1][1] && board[0][0] == board[2][2])
    {
        return board[0][0];
    }
    if (board[0][2] == board[1][1] && board[0][2] == board[2][0])
    {
        return board[0][2];
    }
    return ' '; // no winner
}
void printWinner(char winner)
{
    if (winner == Player)
    {
        printf("You win!\n");
    }
    else if (winner == Computer)
    {
        printf("You Lose!\n");
    }
    else
    {
        printf("Its draw!\n");
    }
}