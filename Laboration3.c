#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define MAXPATIENTS 1000
#define IDLENGTH 11
#define NAMELENGTH 40
#define MAXIMAGES 10
#define BUFFERSIZE 500

typedef struct
{
    char personalID[IDLENGTH + 1];
    char personalName[NAMELENGTH + 1];
    int imageReferences[MAXIMAGES];
    int nrOfImages;
} Patient;

void readFromFile(Patient patients[], int *pNrOfPatients, const char *pFileName);
void writeToFile(Patient patients[], int nrOfPatients, const char *pFileName);
void databaseManagement(Patient patients[], int *pNrOfPatients, const char *pFileName);
void registerPatients(Patient patients[], int *pNrOfPatients);
int checkID(const char *input);
void printAllPatients(const Patient patients[], int nrOfPatients);
void printPatient(const Patient patient);
int searchPatients(const Patient patients[], int nrOfPatients, int foundIndex);
void addPics(Patient patients[], int nrOfPatients);
void deletePics(Patient patients[], int nrOfPatients);
void sortPatients(Patient patients[], int *pNrOfPatients);
void unregisterPatients(Patient patients[], int *pNrOfPatients);

int main(void)
{
    int nrOfPatients = 0;
    char file[NAMELENGTH + 1];
    Patient patients[MAXPATIENTS];
    printf("Patientsjournalsystem\n");
    printf("Vilken fil vill du anvanda: ");
    fgets(file, sizeof(file), stdin);
    strtok(file, "\n");

    readFromFile(patients, &nrOfPatients, file);
    databaseManagement(patients, &nrOfPatients, file);

    return 0;
}
void readFromFile(Patient patients[], int *pNrOfPatients, const char *pFileName)
{
    FILE *pFile = fopen(pFileName, "r");
    if (pFile == NULL)
    {
        printf("Kunde inte hitta filen: %s\n", pFileName);
        return;
    }
    (*pNrOfPatients) = 0;
    char buffer[BUFFERSIZE];
    while (fgets(buffer, sizeof(buffer), pFile) != NULL && *pNrOfPatients < MAXPATIENTS)
    {
        char *pToken = strtok(buffer, "\t"); // strtok delar strängen baserad på , eller t -
        if (pToken != NULL)
        {
            strncpy(patients[*pNrOfPatients].personalID, pToken, sizeof(patients[*pNrOfPatients].personalID) - 1);
            patients[*pNrOfPatients].personalID[sizeof(patients[*pNrOfPatients].personalID) - 1] = '\0';
        }
        pToken = strtok(NULL, "\t");
        if (pToken != NULL)
        {
            strncpy(patients[*pNrOfPatients].personalName, pToken, NAMELENGTH - 1);
            patients[*pNrOfPatients].personalName[NAMELENGTH - 1] = '\0';
        }
        int imagesIndex = 0;
        while ((pToken = strtok(NULL, "\t")) != NULL && imagesIndex < MAXIMAGES)
        {
            patients[*pNrOfPatients].imageReferences[imagesIndex++] = atoi(pToken); // konvertera till int
        }
        patients[*pNrOfPatients].nrOfImages = imagesIndex;
        (*pNrOfPatients)++;
    }
    printf("Antal patienter: %d.\n", *pNrOfPatients);
    fclose(pFile);
}
void writeToFile(Patient patients[], int nrOfPatients, const char *pFileName)
{
    FILE *pFile = fopen(pFileName, "w");
    if (pFile == NULL)
    {
        // lånat från föreläsning 13 //saknas behörighet
        printf("Misslyckades med sparning pga misslyckad filoppning");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nrOfPatients; i++)
    {
        fprintf(pFile, "%s\t%s", patients[i].personalID, patients[i].personalName);

        for (int j = 0; j < patients[i].nrOfImages; j++)
        {
            fprintf(pFile, "\t%d", patients[i].imageReferences[j]);
        }
        fprintf(pFile, "\n");
    }
    fclose(pFile);
    printf("Sparar patientdata i %s\n", pFileName);
}
void databaseManagement(Patient patients[], int *pNrOfPatients, const char *pFileName)
{
    int choice;
    bool isRunning = true;
    while (isRunning)
    {
        printf("Huvudmeny\n");
        printf("          1) Registrera nya patienter\n");
        printf("          2) Skriv ut alla patienter\n");
        printf("          3) Sok efter patienter\n");
        printf("          4) Lagg till bilder\n");
        printf("          5) Ta bort bilder\n");
        printf("          6) Sortera patienter\n");
        printf("          7) Avregistrera patienter\n");
        printf("          8) Avsluta programmet\n");
        printf("Ange alternativ: ");
        if (scanf("%d", &choice) != 1) // vid fel imatning.
        {
            while (getchar() != '\n')
                ;
        }
        switch (choice)
        {
        case 1:
            registerPatients(patients, pNrOfPatients);
            break;
        case 2:
            printAllPatients(patients, *pNrOfPatients);
            break;
        case 3:
            int foundIndex = 0;
            searchPatients(patients, *pNrOfPatients, foundIndex);
            break;
        case 4:
            addPics(patients, *pNrOfPatients);
            break;
        case 5:
            deletePics(patients, *pNrOfPatients);
            break;
        case 6:
            sortPatients(patients, pNrOfPatients);
            break;
        case 7:
            unregisterPatients(patients, pNrOfPatients);
            break;
        case 8:
            writeToFile(patients, *pNrOfPatients, pFileName);
            printf("Avslutar\n");
            isRunning = false;
            break;
        default:
            printf("Fel inmatning! Valj en siffra mellan 1-8.\n");
            printf("\n");
        }
    }
}
void registerPatients(Patient patients[], int *pNrOfPatients)
{
    if ((*pNrOfPatients) >= MAXPATIENTS)
    {
        printf("Patientlistan är full!\n");
        return;
    }
    char personalID[IDLENGTH + 1];
    while (*pNrOfPatients < MAXPATIENTS)
    {
        bool isIdCorrect = false;
        while (!isIdCorrect)
        {
            printf("Ange personnummer (q for att avsluta): ");
            scanf("%s", personalID);
            if (strcmp(personalID, "q") == 0)
            {
                printf("Avslutar registrering\n");
                return;
            }
            if (checkID(personalID))
            {
                isIdCorrect = true;
            }
        }
        bool isAlreadyRegistered = false;
        for (int i = 0; i < *pNrOfPatients; i++)
        {
            if (strcmp(personalID, patients[i].personalID) == 0)
            {
                printf("Redan registrerat.\n");
                isAlreadyRegistered = true;
                break;
            }
        }
        if (!isAlreadyRegistered)
        {
            strcpy(patients[*pNrOfPatients].personalID, personalID);
            getchar(); // Rensa bufferten
            printf("Ange namn: ");
            fgets(patients[*pNrOfPatients].personalName, sizeof(patients[*pNrOfPatients].personalName), stdin);
            patients[*pNrOfPatients].personalName[strcspn(patients[*pNrOfPatients].personalName, "\n")] = '\0';

            int nrOfImages = 0;
            while (nrOfImages < MAXIMAGES)
            {
                int imageRef;
                bool isDuplicateRef = false;
                printf("Ange bildreferens (eller 0 for att avsluta): ");
                scanf("%d", &imageRef);
                if (imageRef == 0)
                {
                    break;
                }
                for (int i = 0; i < *pNrOfPatients; i++)
                {
                    for (int j = 0; j < patients[i].nrOfImages; j++)
                    {
                        if (patients[i].imageReferences[j] == imageRef)
                        {
                            isDuplicateRef = true;
                            printf("Referensen finns redan!\n");
                            break;
                        }
                    }
                    if (isDuplicateRef)
                    {
                        break;
                    }
                }
                if (!isDuplicateRef)
                {
                    for (int k = 0; k < nrOfImages; k++)
                    {
                        if (patients[*pNrOfPatients].imageReferences[k] == imageRef)
                        {
                            isDuplicateRef = true;
                            printf("Referensen finns redan!\n");
                            break;
                        }
                    }
                }
                if (!isDuplicateRef)
                {
                    patients[*pNrOfPatients].imageReferences[nrOfImages] = imageRef;
                    nrOfImages++;
                }
                if (nrOfImages == MAXIMAGES)
                {
                    printf("Maximalt antal referenser uppnatt.\n");
                }
            }
            patients[*pNrOfPatients].nrOfImages = nrOfImages;
            (*pNrOfPatients)++;
        }
    }
}
int checkID(const char *input)
{
    char id[IDLENGTH - 1];
    int sum = 0, checkNumber, index = 0;

    for (int i = 0; input[i] != '\0'; i++)
    {
        if (input[i] >= '0' && input[i] <= '9')
        {
            id[index++] = input[i];
        }
        else if (input[i] != '-') // Tillåt bindestreck
        {
            printf("Fel format! personnumret maste vara (AAMMDD-XXXX).\n");
            return 0;
        }
    }
    id[index] = '\0';

    if (strlen(id) != IDLENGTH - 1)
    {
        printf("Ogiltigt personnummer! Det ska vara 10 siffror.\n");
        return 0;
    }

    for (int i = 0; i < IDLENGTH - 2; i++)
    {
        int number = id[i] - '0'; // omvandla till siffror
        if (i % 2 == 0)
        {
            number *= 2;
            if (number > 9)
            {
                number -= 9;
            }
        }
        sum += number;
    }
    checkNumber = (10 - (sum % 10)) % 10;

    if (checkNumber == id[IDLENGTH - 2] - '0')
    {
        return 1; // Giltigt personnummer
    }
    else
    {
        printf("Ogiltigt personnummer! Forsok igen.\n");
        return 0;
    }
}
void printAllPatients(const Patient patients[], int nrOfPatients)
{
    if (nrOfPatients == 0)
    {
        printf("Inga patienter registrerade.\n");
        return;
    }

    printf("Personnummer         Namn                Bildreferenser\n");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < nrOfPatients; i++)
    {
        printf("%-20s %-20s", patients[i].personalID, patients[i].personalName);
        printf(" [");
        for (int j = 0; j < patients[i].nrOfImages; j++)
        {
            printf("%d", patients[i].imageReferences[j]);
            if (j < patients[i].nrOfImages - 1)
            {
                printf(", ");
            }
        }
        printf("]\n");
    }
    printf("\n");
}
void printPatient(const Patient patient)
{
    printf("%-20s %-20s [", patient.personalID, patient.personalName);
    for (int i = 0; i < patient.nrOfImages; i++)
    {
        printf("%d", patient.imageReferences[i]);
        if (i < patient.nrOfImages - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");
}
int searchPatients(const Patient patients[], int nrOfPatients, int foundIndex)
{
    if (nrOfPatients == 0)
    {
        printf("Inga patienter registrerade.\n");
        return 0;
    }

    char searchID[IDLENGTH], nameId[NAMELENGTH];
    int tempReference, nrOfMatches, index, optionsOfSearching;
    bool matchFound;

    do
    {
        printf("Sok pa personnummer (1), namn (2), bildreferens (3), avsluta (4): ");
        scanf("%d", &optionsOfSearching);

        switch (optionsOfSearching)
        {
        case 1:
            printf("Ange personnummer: ");
            scanf("%12s%*c", searchID);
            matchFound = false;
            for (int i = 0; i < nrOfPatients; i++)
            {
                if (strcmp(patients[i].personalID, searchID) == 0)
                {
                    printf("Personnummer        Namn                 Bildreferenser\n");
                    printf("--------------------------------------------------------\n");
                    printPatient(patients[i]);
                    printf("\n");
                    matchFound = true;
                    if (foundIndex)
                    {
                        foundIndex = i;
                        return i;
                    }
                }
            }
            if (!matchFound)
            {
                printf("Personnumret saknas i databasen.\n");
                if (foundIndex)
                {
                    foundIndex = -1;
                    return foundIndex;
                }
            }
            break;
        case 2:
            printf("Ange sokstrang: ");
            scanf("%40s%*c", nameId);
            for (int i = 0; nameId[i]; i++)
            {
                nameId[i] = tolower(nameId[i]);
            }
            nrOfMatches = 0;
            index = -1;
            for (int j = 0; j < nrOfPatients; j++)
            {
                char tempName[NAMELENGTH + 1];
                strcpy(tempName, patients[j].personalName);

                for (int i = 0; tempName[i]; i++)
                {
                    tempName[i] = tolower(tempName[i]);
                }

                if (strstr(tempName, nameId) != NULL)
                {
                    if (nrOfMatches == 0)
                    {
                        printf("Personnummer        Namn                 Bildreferenser\n");
                        printf("--------------------------------------------------------\n");
                    }

                    printPatient(patients[j]);
                    nrOfMatches++;
                    index = j;
                }
            }
            if (nrOfMatches > 0)
            {
                printf("\n");
            }
            if (nrOfMatches == 1)
            {
                if (foundIndex)
                {
                    foundIndex = index;
                    return foundIndex;
                }
            }
            else if (nrOfMatches == 0)
            {
                printf("Namnet saknas i databasen.\n");
                if (foundIndex)
                {
                    foundIndex = -1;
                    return foundIndex;
                }
            }
            else
            {
                if (foundIndex)
                {

                    foundIndex = -1;
                    return foundIndex;
                }
            }
            break;
        case 3:
            printf("Ange bildreferens: ");
            scanf("%d", &tempReference);
            matchFound = false;
            for (int k = 0; k < nrOfPatients; k++)
            {
                for (int m = 0; m < patients[k].nrOfImages; m++)
                {
                    if (patients[k].imageReferences[m] == tempReference)
                    {
                        printf("Personnummer        Namn                 Bildreferenser\n");
                        printf("--------------------------------------------------------\n");
                        printPatient(patients[k]);
                        printf("\n");
                        matchFound = true;
                        if (foundIndex)
                        {
                            foundIndex = k;
                            return foundIndex;
                        }
                    }
                }
            }
            if (!matchFound)
            {
                printf("Bildreferensen saknas i databasen.\n");
                if (foundIndex)
                {
                    foundIndex = -1;
                    return foundIndex;
                }
            }
            break;
        case 4:
            printf("Avslutar sokning.\n");
            return -2;
        default:
            printf("Fel val!. valj ratt siffra 1-4.\n");
        }
    } while (optionsOfSearching != 4);
    return -1;
}
void addPics(Patient patients[], int nrOfPatients)
{
    if (nrOfPatients == 0)
    {
        printf("Inga patienter registrerade.\n");
        return;
    }
    int searchIndex = -1;
    while (nrOfPatients < MAXPATIENTS)
    {
        searchIndex = searchPatients(patients, nrOfPatients, searchIndex);

        if (searchIndex >= 0)
        {
            if (patients[searchIndex].nrOfImages >= MAXIMAGES)
            {
                printf("Patienten har redan maximalt antal bilder.\n");
                return;
            }

            int imgRef;
            while (patients[searchIndex].nrOfImages < MAXIMAGES)
            {
                printf("Ange ny bildreferens (eller 0 for att avsluta): ");
                scanf("%d", &imgRef);

                if (imgRef == 0)
                {
                    printf("Avslutar lagg till bilder.\n");
                    return;
                }

                bool duplicate = false;

                for (int i = 0; i < patients[searchIndex].nrOfImages; i++)
                {
                    if (patients[searchIndex].imageReferences[i] == imgRef)
                    {
                        printf("Referensen finns redan!\n");
                        duplicate = true;
                        break;
                    }
                }
                if (!duplicate)
                {
                    for (int j = 0; j < nrOfPatients; j++)
                    {
                        for (int k = 0; k < patients[j].nrOfImages; k++)
                        {
                            if (patients[j].imageReferences[k] == imgRef)
                            {
                                printf("Referensen finns redan!\n");
                                duplicate = true;
                                break;
                            }
                        }
                        if (duplicate)
                        {
                            break;
                        }
                    }
                }

                if (!duplicate)
                {
                    patients[searchIndex].imageReferences[patients[searchIndex].nrOfImages] = imgRef;
                    patients[searchIndex].nrOfImages++;

                    if (patients[searchIndex].nrOfImages == MAXIMAGES)
                    {
                        printf("Antalet referenser fyllt\n");
                        printf("Avslutar lagg till bilder\n");
                        return;
                    }
                }
            }
        }
        else if (searchIndex == -1)
        {
            printf("Du fick inte exakt en traff.\n");
        }
        else if (searchIndex == -2)
        {
            printf("Avslutar lagg till bilder\n");
            return;
        }
    }
}
void deletePics(Patient patients[], int nrOfPatients)
{
    if (nrOfPatients == 0)
    {
        printf("Inga patienter registrerade.\n");
        return;
    }
    int searchIndex = -1;
    while (1)
    {
        searchIndex = searchPatients(patients, nrOfPatients, searchIndex);
        if (searchIndex >= 0)
        {
            if (patients[searchIndex].nrOfImages == 0)
            {
                printf("Det finns inga bilder att ta bort.\n");
                return;
            }

            while (1)
            {
                int imgRefToDelete;
                printf("Ange bildreferensen du vill ta bort (eller 0 for att avsluta): ");
                scanf("%d", &imgRefToDelete);

                if (imgRefToDelete == 0)
                {
                    printf("Avslutar borttagning av bilder.\n");
                    return;
                }

                bool found = false;

                for (int i = 0; i < patients[searchIndex].nrOfImages; i++)
                {
                    if (patients[searchIndex].imageReferences[i] == imgRefToDelete)
                    {
                        for (int j = i; j < patients[searchIndex].nrOfImages - 1; j++)
                        {
                            patients[searchIndex].imageReferences[j] = patients[searchIndex].imageReferences[j + 1];
                        }

                        patients[searchIndex].nrOfImages--;
                        found = true;
                        printf("Bildreferensen har tagits bort.\n");

                        if (patients[searchIndex].nrOfImages == 0)
                        {
                            printf("Det finns inga fler bilder att ta bort.\n");
                            return;
                        }
                        break;
                    }
                }

                if (!found)
                {
                    printf("Ingen bildreferens hittades.\n");
                }
            }
        }
        else if (searchIndex == -1)
        {
            printf("Du fick inte exakt en traff.\n");
        }
        else if (searchIndex == -2)
        {
            printf("Avslutar borttagning av bilder.\n");
            return;
        }
    }
}
void sortPatients(Patient patients[], int *pNrOfPatients)
{
    int sortingOption;
    if (*pNrOfPatients == 0)
    {
        printf("Inga patienter registrerade.\n");
        return;
    }

    while (1)
    {
        printf("Valj sortering: 1) efter personnummer 2) efter namn 3) Avsluta: ");
        scanf("%d", &sortingOption);

        switch (sortingOption)
        {
        case 1:
            for (int i = 0; i < *pNrOfPatients - 1; i++)
            {
                for (int j = i + 1; j < *pNrOfPatients; j++)
                {
                    if (strcmp(patients[i].personalID, patients[j].personalID) > 0)
                    {
                        Patient temp = patients[i];
                        patients[i] = patients[j];
                        patients[j] = temp;
                    }
                }
            }
            printf("Patienterna har sorterats efter personnummer.\n");
            printf("\n");
            break;
        case 2:
            for (int i = 0; i < *pNrOfPatients - 1; i++)
            {
                for (int j = i + 1; j < *pNrOfPatients; j++)
                {
                    if (strcmp(patients[i].personalName, patients[j].personalName) > 0)
                    {
                        Patient temp = patients[i];
                        patients[i] = patients[j];
                        patients[j] = temp;
                    }
                }
            }
            printf("Patienterna har sorterats efter namn.\n");
            printf("\n");
            break;
        case 3:
            printf("Avslutar sortering.\n");
            printf("\n");
            return;
        default:
            printf("Fel val. valj ratt siffra mellan 1-3.\n");
        }
    }
}
void unregisterPatients(Patient patients[], int *pNrOfPatients)
{
    if (*pNrOfPatients == 0)
    {
        printf("Inga patienter registrerade.\n");
        return;
    }
    int searchIndex = -1;
    searchIndex = searchPatients(patients, *pNrOfPatients, searchIndex);

    if (searchIndex >= 0)
    {
        char choice;
        printf("Vill du avregistrera patienten (j/n)? ");
        while (1)
        {
            scanf(" %c", &choice);
            if (choice == 'J' || choice == 'j')
            {
                for (int i = searchIndex; i < *pNrOfPatients - 1; i++)
                {
                    patients[i] = patients[i + 1];
                }
                (*pNrOfPatients)--;
                printf("Patienten avregistreras.\n");
                break;
            }
            else if (choice == 'N' || choice == 'n')
            {
                printf("Patienten avregistrerades inte.\n");
                break;
            }
            else
            {
                printf("Ogiltigt val. Ange 'j' eller 'n'.\n");
            }
        }
    }
    else if (searchIndex == -1)
    {
        printf("Du fick inte exakt en traff.\n");
    }
    else if (searchIndex == -2)
    {
        printf("Avslutar avregistrering.\n");
    }
}