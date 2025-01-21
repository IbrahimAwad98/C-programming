#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define NAMELENGTH 40
#define MAXRATE 11
#define MAXGAMES 20
#define MAXUSERS 30
#define MAXBUFFER 500

typedef struct
{
    char gamesName[NAMELENGTH + 1];
    int rate[MAXRATE];
} Game;

typedef struct
{
    Game list[MAXGAMES];
    char userName[NAMELENGTH + 1];
    int nrOfgames;
} User;

void readFromFile(User users[], int *pNrOfusers, const char *pFileName);
void writeToFile(User users[], int *pNrOfusers, const char *pFileName);
void administration(User users[], int *pNrOfusers, const char *pFileName);
void options(User users[], int *pNrOfusers, const char *pFileName);
void boardGames(User *user, int *pNrOfusers, const char *pFileName);
void registerUsers(User user[],int *pNrOfusers, const char *pFileName);
void sorting(User user[], int *pNrOfusers, const char *pFileName);


int main(void)
{
    int nrOFusers = 0;
    User users[MAXUSERS];
    char file[NAMELENGTH + 1];
    printf("Welcome to boardgame ratings.\n");
    printf("Which file do you want to use: ");
    fgets(file, sizeof(file), stdin);
    strtok(file, "\n");
    readFromFile(users, &nrOFusers, file);
    options(users, &nrOFusers, file);
    return 0;
}
void readFromFile(User users[], int *pNrOfusers, const char *pFileName)
{
    FILE *pFile = fopen(pFileName, "r");
    if (pFile == NULL)
    {
        printf("The file is not exists: %s\n", pFileName);
        return;
    }
    (*pNrOfusers = 0);
    while (fscanf(pFile, "%s", users[*pNrOfusers].userName) == 1)
    {
        (*pNrOfusers)++;
    }
    fclose(pFile);
}
void writeToFile(User users[], int *pNrOfusers, const char *pFileName)
{
    FILE *pFile = fopen(pFileName, "w");
    if (pFile == NULL)
    {
        printf("Error 404.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < *pNrOfusers; i++)
    {
        fprintf(pFile, "%s", users[i].userName);
    }
    fprintf(pFile, "\n");
    fclose(pFile);
    printf("Saving all data to %s\n", pFileName);
}
void administration(User users[], int *pNrOfusers, const char *pFileName)
{
    printf("Administration\n");
    int choice;
    bool isRunning = true;
    while (isRunning)
    {
        printf("          1) Add user\n");
        printf("          2) Remove user\n");
        printf("          3) Print all users\n");
        printf("          4) Print all user and thier ratings\n");
        printf("          5) Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            registerUsers(users, pNrOfusers, pFileName);
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;
        case 4:
            /* code */
            break;
        case 5:
            isRunning = false;
            break;
        default:
            printf("Unknown command. Try again.\n");
        }
    }
}
void options(User users[], int *pNrOfusers, const char *pFileName)
{
    char input[NAMELENGTH + 1];
    bool userFound;
    while (1)
    {
        printf("Please enter user name, admin or quit: ");
        scanf("%s", input);
        if (strcmp(input, "admin") == 0)
        {
            administration(users, pNrOfusers, pFileName);
        }
        else if (strcmp(input, "quit") == 0)
        {
            writeToFile(users, pNrOfusers, pFileName);
            printf("Goodbye!\n");
            break;
        }
        else
        {
            userFound = false;
            for (int i = 0; i < *pNrOfusers; i++)
            {
                if (strcmp(users[i].userName, input) == 0)
                {
                    userFound = true;
                    printf("%s's boardgames\n", users[i].userName);
                    boardGames(&users[i], pNrOfusers, pFileName);
                    break;
                }
            }
            if (!userFound)
            {
                printf("User dose not exist.\n");
            }
        }
    }
}
void boardGames(User *user, int *pNrOfusers, const char *pFileName)
{
    int choice;
    bool isRunning = true;
    while (isRunning)
    {
        printf("          1) Print games\n");
        printf("          2) Add games\n");
        printf("          3) Search games\n");
        printf("          4) Remove games\n");
        printf("          5) Exit\n");
        printf("choose: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            /* code */
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;
        case 4:
            /* code */
            break;
        case 5:
            isRunning = false;
            break;
        default:
            printf("Unknown command. Try again.\n");
        }
    }
}
void registerUsers(User user[],int *pNrOfusers, const char *pFileName)
{
    char userName[NAMELENGTH + 1];
    bool foundUser = false;
     if (*pNrOfusers >= MAXGAMES)
    {
            printf("The register is full!\n");
            return;
    }
    while (*pNrOfusers < MAXGAMES)
    {   
        printf("Register new user (q to quit): ");
        scanf("%s", userName);
        if (strcmp(userName, "q") == 0)
        {
            printf("\n");
            return;
        }
        for (int i = 0; i < *pNrOfusers; i++)
        {
            if (strcmp(user[i].userName, userName) == 0)
            {
                printf("User name exist! Please choose another.\n");
                foundUser = true;
                break;
            }
        }
        
    }
}
void sorting(User user[], int *pNrOfusers, const char *pFileName)
{
    for (int i = 0; i < *pNrOfusers - 1; i++)
    {
        for (int j = 0; j < *pNrOfusers; j++)
        {
            if (strcmp(user[i].userName, user[j].userName) > 0)
            {
                User temp = user[i];
                user[i] = user[j];
                user[j] = temp;
            }   
        }
    }
    
}