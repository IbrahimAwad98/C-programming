#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NAMELENGTH 40
#define MAXGAMES 10
#define MAXUSERS 30
#define BUFFERSIZE 500
#define MAXRATE 10
#define MINRATE 1

typedef struct
{
    char gameName[NAMELENGTH + 1];
    int rate;
} Game;

typedef struct
{
    Game list[MAXGAMES];
    char userName[NAMELENGTH + 1];
    int nrOfGames;
} User;

void readFromFile(User users[MAXUSERS], int *pNrOfUsers, const char *pFileName);
void writeToFile(User users[MAXUSERS], int *pNrOfUsers, const char *pFileName);
void administration(User users[MAXUSERS], int *pNrOfUsers, const char *pFileName);
void loginsAlternativs(User users[MAXUSERS], int *pNrOfUsers, const char *pFileName);
void boardGames(User users[MAXUSERS], int *pNrOfUsers, const char *pFileName, int currentUser);
void registerUser(User users[MAXUSERS], int *pNrOfUsers);
void removeUser(User users[MAXUSERS], int *pNrOfUsers);
void printUser(User users[MAXUSERS], int *pNrOfUsers);
void printUserAndRates(User users[MAXUSERS], int *pNrOfUsers);
void printGames(User users[MAXUSERS], int currentUser);
void addGames(User users[MAXUSERS], int currentUser);
int searchGames(User users[MAXUSERS], int currentUser);
void removeGames(User users[MAXUSERS], int currentUser);
void sortGames(User users[MAXUSERS], int currentUser);

int main(void)
{
    User users[MAXUSERS];
    int nrOfUsers = 0;
    char fileName[NAMELENGTH + 1];
    printf("Welcome to boardgame ratings.\n");
    printf("Which file do you want to use: ");
    if (fgets(fileName, sizeof(fileName), stdin) == NULL)
    {
        printf("Error reading file name.\n");
        return 1;
    }
    fileName[strcspn(fileName, "\n")] = 0;
    readFromFile(users, &nrOfUsers, fileName);
    loginsAlternativs(users, &nrOfUsers, fileName);
    return 0;
}
void readFromFile(User users[MAXUSERS], int *pNrOfUsers, const char *pFileName)
{
    FILE *pFile = fopen(pFileName, "r");
    if (pFile == NULL)
    {
        printf("Error opening file: %s\n", pFileName);
        return;
    }
    *pNrOfUsers = 0;
    char buffer[BUFFERSIZE];
    while (fgets(buffer, sizeof(buffer), pFile) != NULL && *pNrOfUsers < MAXUSERS)
    {
        char *pToken = strtok(buffer, "\t");
        strncpy(users[*pNrOfUsers].userName, pToken, sizeof(users[*pNrOfUsers].userName) - 1);
        users[*pNrOfUsers].userName[sizeof(users[*pNrOfUsers].userName) - 1] = '\0';

        pToken = strtok(NULL, "\t");

        users[*pNrOfUsers].nrOfGames = atoi(pToken);

        for (int i = 0; i < users[*pNrOfUsers].nrOfGames; i++)
        {
            pToken = strtok(NULL, "\t");
            if (pToken == NULL)
                break;
            strncpy(users[*pNrOfUsers].list[i].gameName, pToken, sizeof(users[*pNrOfUsers].list[i].gameName) - 1);
            users[*pNrOfUsers].list[i].gameName[sizeof(users[*pNrOfUsers].list[i].gameName) - 1] = '\0';

            pToken = strtok(NULL, "\t");
            if (pToken == NULL)
                break;
            users[*pNrOfUsers].list[i].rate = atoi(pToken);
        }
        (*pNrOfUsers)++;
    }
    fclose(pFile);
}
void writeToFile(User users[MAXUSERS], int *pNrOfUsers, const char *pFileName)
{
    FILE *pFile = fopen(pFileName, "w");
    if (pFile == NULL)
    {
        printf("Failed to save due to unsuccessful file opening");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < *pNrOfUsers; i++)
    {
        fprintf(pFile, "%s\t%d", users[i].userName, users[i].nrOfGames);
        for (int j = 0; j < users[i].nrOfGames; j++)
        {
            fprintf(pFile, "\t%s\t%d", users[i].list[j].gameName, users[i].list[j].rate);
        }
        fprintf(pFile, "\n");
    }
    fclose(pFile);
    printf("Saving all data to %s\n", pFileName);
}
void administration(User users[MAXUSERS], int *pNrOfUsers, const char *pFileName)
{
    printf("Administartion\n");
    int choice;
    bool isRunning = true;
    while (isRunning)
    {
        printf("          1) Add user\n");
        printf("          2) Remove user\n");
        printf("          3) Print all users\n");
        printf("          4) Print all user and their ratings\n");
        printf("          5) Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            registerUser(users, pNrOfUsers);
            break;
        case 2:
            removeUser(users, pNrOfUsers);
            break;
        case 3:
            printUser(users, pNrOfUsers);
            break;
        case 4:
            printUserAndRates(users, pNrOfUsers);
            break;
        case 5:
            isRunning = false;
            loginsAlternativs(users, pNrOfUsers, pFileName);
            break;
        default:
            printf("Unknown command. Try again.\n");
        }
    }
}
void loginsAlternativs(User users[MAXUSERS], int *pNrOfUsers, const char *pFileName)
{
    char choice[NAMELENGTH + 1];
    bool userFound;
    while (1)
    {
        printf("Please enter user name, admin or quit: ");
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = '\0';
        if (strcmp(choice, "admin") == 0)
        {
            administration(users, pNrOfUsers, pFileName);
        }
        else if (strcmp(choice, "quit") == 0)
        {
            writeToFile(users, pNrOfUsers, pFileName);
            printf("Goodbye!\n");
            exit(0);
        }
        else
        {
            userFound = false;
            for (int i = 0; i < *pNrOfUsers; i++)
            {
                if (strcmp(choice, users[i].userName) == 0)
                {
                    userFound = true;
                    printf("%s's boardgames\n", users[i].userName);
                    boardGames(users, pNrOfUsers, pFileName, i);
                }
            }
            if (!userFound)
            {
                printf("User does not exist\n");
            }
        }
    }
}
void boardGames(User users[MAXUSERS], int *pNrOfUsers, const char *pFileName, int currentUser)
{
    int choice;
    bool isRunning = true;
    while (isRunning)
    {
        printf("          1) Print games\n");
        printf("          2) Add game\n");
        printf("          3) Search games\n");
        printf("          4) Remove games\n");
        printf("          5) sort games\n");
        printf("          6) Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            printGames(users, currentUser);
            break;
        case 2:
            addGames(users, currentUser);
            break;
        case 3:
            searchGames(users, currentUser);
            break;
        case 4:
            removeGames(users, currentUser);
            break;
        case 5:
            sortGames(users, currentUser);
            break;
        case 6:
            isRunning = false;
            loginsAlternativs(users, pNrOfUsers, pFileName);
            break;
        default:
            printf("Unknown command. Try again.\n");
        }
    }
}
void registerUser(User users[MAXUSERS], int *pNrOfUsers)
{
    char user[NAMELENGTH + 1];
    bool foundUser;
    while (1)
    {
        printf("Register new user (q to quit): ");
        fgets(user, sizeof(user), stdin);
        user[strcspn(user, "\n")] = '\0';

        if (strcmp(user, "q") == 0)
        {
            return;
        }

        if (*pNrOfUsers >= MAXUSERS)
        {
            printf("The register is full!\n");
            return;
        }
        if (*pNrOfUsers < MAXUSERS)
        {
            foundUser = false;
            for (int i = 0; i < *pNrOfUsers; i++)
            {
                if (strcmp(user, users[i].userName) == 0)
                {
                    foundUser = true;
                    printf("User name exist! Please choose another.\n");
                }
            }
            if (!foundUser)
            {
                strcpy(users[*pNrOfUsers].userName, user);
                (*pNrOfUsers)++;
            }
        }
    }
}
void removeUser(User users[MAXUSERS], int *pNrOfUsers)
{
    char name[NAMELENGTH + 1], choice;
    bool foundUser;

    while (1)
    {
        printf("Remove user (q to quit): ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';

        if (strcmp(name, "q") == 0)
        {
            break;
        }

        foundUser = false;
        for (int i = 0; i < *pNrOfUsers; i++)
        {
            if (strcmp(name, users[i].userName) == 0)
            {
                foundUser = true;
                if (users[i].nrOfGames > 0)
                {
                    printf("Warning: User has rated games.\n");
                    printf("Do you still want to remove %s (y/n)? ", name);
                    scanf(" %c", &choice);
                    getchar();
                    if (choice != 'y')
                    {
                        break;
                    }
                }

                for (int j = i; j < *pNrOfUsers - 1; j++)
                {
                    users[j] = users[j + 1];
                }
                (*pNrOfUsers)--;
                printf("Removed.\n");
            }
        }
        if (!foundUser)
        {
            printf("User does not exist! Please choose another.\n");
        }
    }
}
void printUser(User users[MAXUSERS], int *pNrOfUsers)
{
    printf("Users:\n");
    printf("-----------------------------------------------------------\n");
    if (*pNrOfUsers == 0)
    {
        printf("No users registrerad\n");
        return;
    }
    for (int i = 0; i < *pNrOfUsers; i++)
    {
        printf("%s\n", users[i].userName);
    }
    printf("\n");
}
void printUserAndRates(User users[MAXUSERS], int *pNrOfUsers)
{
    printf("Users and boardgames:\n");
    printf("-----------------------------------------------------------\n");
    if (*pNrOfUsers == 0)
    {
        printf("No users registrerad\n");
        return;
    }
    for (int i = 0; i < *pNrOfUsers; i++)
    {
        printf("%s\n", users[i].userName);
        if (users[i].nrOfGames == 0)
        {
            printf("  No games registered\n");
        }
        else
        {
            for (int j = 0; j < users[i].nrOfGames; j++)
            {
                printf("  %-15s %d\n", users[i].list[j].gameName, users[i].list[j].rate);
            }
        }
    }
    printf("\n");
}
void printGames(User users[MAXUSERS], int currentUser)
{
    for (int i = 0; i < users[currentUser].nrOfGames; i++)
    {
        printf("  %-15s %d\n", users[currentUser].list[i].gameName, users[currentUser].list[i].rate);
    }
    printf("\n");
}
void addGames(User users[MAXUSERS], int currentUser)
{
    char nameOfgame[NAMELENGTH + 1];
    int rate;
    bool foundGame;
    if (users[currentUser].nrOfGames >= MAXGAMES)
    {
        printf("Your game register is full.\n");
        return;
    }
    while (1)
    {
        printf("Register new boardgame (q to quit): ");
        fgets(nameOfgame, sizeof(nameOfgame), stdin);
        nameOfgame[strcspn(nameOfgame, "\n")] = '\0';
        if (strcmp(nameOfgame, "q") == 0)
        {
            return;
        }
        foundGame = false;
        for (int i = 0; i < users[currentUser].nrOfGames; i++)
        {
            if (strcmp(nameOfgame, users[currentUser].list[i].gameName) == 0)
            {
                foundGame = true;
                printf("Boardgame already added.\n");
                break;
            }
        }
        if (!foundGame)
        {
            while (1)
            {
                printf("Rating (1-10): ");
                scanf("%d", &rate);
                if (rate < MINRATE || rate > MAXRATE)
                {
                    printf("Illegal value!\n");
                }
                else
                {
                    break;
                }
            }
            int index = users[currentUser].nrOfGames;
            strcpy(users[currentUser].list[index].gameName, nameOfgame);
            users[currentUser].list[index].rate = rate;
            users[currentUser].nrOfGames++;
            getchar();
        }
    }
}
int searchGames(User users[MAXUSERS], int currentUser)
{
    char nameOfgame[NAMELENGTH + 1];

    if (users[currentUser].nrOfGames == 0)
    {
        printf("No games registered.\n");
        return -1;
    }

    while (1)
    {
        printf("Search (q to quit): ");
        fgets(nameOfgame, sizeof(nameOfgame), stdin);
        nameOfgame[strcspn(nameOfgame, "\n")] = '\0';

        if (strcmp(nameOfgame, "q") == 0)
        {
            return -1;
        }

        int found = 0;
        for (int i = 0; i < users[currentUser].nrOfGames; i++)
        {
            if (strstr(users[currentUser].list[i].gameName, nameOfgame) != NULL)
            {
                printf("%s  %d\n", users[currentUser].list[i].gameName, users[currentUser].list[i].rate);
                found = 1;
            }
        }
        if (!found)
        {
            printf("No matching games found.\n");
        }
    }
}
void removeGames(User users[MAXUSERS], int currentUser)
{
    char nameOfgame[NAMELENGTH + 1];
    char choice;
    while (1)
    {
        printf("Search boardgame to remove (q to quit): ");
        fgets(nameOfgame, sizeof(nameOfgame), stdin);
        nameOfgame[strcspn(nameOfgame, "\n")] = '\0';
        if (strcmp(nameOfgame, "q") == 0)
        {
            return;
        }
        if (users[currentUser].nrOfGames == 0)
        {
            printf("No game has been found.\n");
            break;
        }
        int matchIndex = -1;
        int matchCount = 0;
        for (int i = 0; i < users[currentUser].nrOfGames; i++)
        {
            if (strstr(users[currentUser].list[i].gameName, nameOfgame) != NULL)
            {
                printf("%s  %d\n", users[currentUser].list[i].gameName, users[currentUser].list[i].rate);
                matchIndex = i;
                matchCount++;
            }
        }
        if (matchCount > 1)
        {
            printf("You did not find one unique boardgame.\n");
        }
        if (matchCount == 1)
        {

            printf("Do you want to remove this game (y/n): ");
            scanf(" %c", &choice);
            getchar();
            if (choice == 'y' || choice == 'Y')
            {
                for (int i = matchIndex; i < users[currentUser].nrOfGames - 1; i++)
                {
                    users[currentUser].list[i] = users[currentUser].list[i + 1];
                }
            }
            users[currentUser].nrOfGames--;
            printf("Game removed successfully!\n");
        }
    }
}
void sortGames(User users[MAXUSERS], int currentUser)
{
    char choice;
    while (1)
    {
        printf("Do you want to sort by name or rating (n/r)? ");
        scanf(" %c", &choice);
        getchar();
        if (choice == 'n')
        {
            for (int i = 0; i < users[currentUser].nrOfGames - 1; i++)
            {
                for (int j = 0; j < users[currentUser].nrOfGames - i - 1; j++)
                {
                    if (strcmp(users[currentUser].list[j].gameName, users[currentUser].list[j + 1].gameName) > 0)
                    {
                        Game temp = users[currentUser].list[j];
                        users[currentUser].list[j] = users[currentUser].list[j + 1];
                        users[currentUser].list[j + 1] = temp;
                    }
                }
            }
            printf("Games have been sorted by name.\n");
            return;
        }
        else if (choice == 'r')
        {
            for (int i = 0; i < users[currentUser].nrOfGames - 1; i++)
            {
                for (int j = 0; j < users[currentUser].nrOfGames - i - 1; j++)
                {
                    if (users[currentUser].list[j].rate < users[currentUser].list[j + 1].rate)
                    {
                        Game temp = users[currentUser].list[j];
                        users[currentUser].list[j] = users[currentUser].list[j + 1];
                        users[currentUser].list[j + 1] = temp;
                    }
                }
            }
            printf("Games have been sorted by rating.\n");
            return;
        }
        else
        {
            printf("Invalid choice. Please enter 'n' for name or 'r' for rating.\n");
        }
    }
}
