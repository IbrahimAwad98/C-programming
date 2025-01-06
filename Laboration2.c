#include <stdio.h>
#include <math.h>
// #include <limits.h>
// #include <stdlib.h>
// #include <stdbool.h>
// #include <time.h>
// #include <string.h>
#define Max_measurements 10

int enter(int measurements[], int nrOfmeasurement);
void view(int measurements[], int nrOfmeasurement);
void compute(int measurements[], int nrOfmeasurement);

int calcu_Max(int measurements[], int nrOfmeasurement);
int calcu_Min(int measurements[], int nrOfmeasurement);
float calcu_avr(int measurements[], int nrOfmeasurement);
void calcu_NorV(int measurements[], int nrOfmeasurement, int NormalizedValue[]);

int main()
{
    int measurements[Max_measurements];
    int nrOfMeasurement = 0;
    char Menyselection;

    printf("Measurement tool 2.0\n");

    while (1)
    {
        printf("VECRQ? ");
        scanf(" %c", &Menyselection);

        switch (Menyselection)
        {
        case 'v':
            view(measurements, nrOfMeasurement);
            break;
        case 'e':
            nrOfMeasurement = enter(measurements, nrOfMeasurement);
            break;
        case 'c':
            compute(measurements, nrOfMeasurement);
            break;
        case 'r':
            printf("Resetting completed.\n");
            nrOfMeasurement = 0;
            break;
        case 'q':
            printf("Exit measurement tool\n");
            return 0;
            break;
        default:
            printf("wrong input, try again\n");
            break;
        }
    }
    return 0;
}
int enter(int measurements[], int nrOfmeasurement)
{
    int oneMeasurement;
    char tmp;
    if (nrOfmeasurement >= Max_measurements)
    {
        nrOfmeasurement = Max_measurements;
    }
    for (int i = nrOfmeasurement; i < 10; i++)
    {
        printf("Enter measurement (or q to quit)%d: ", i + 1);

        int readInteger = scanf("%d", &oneMeasurement);
        if (readInteger == 1)
        {
            measurements[i] = oneMeasurement;
            nrOfmeasurement++;
        }
        else
        {
            scanf("%c", &tmp);
            if (tmp == tmp)
                break;
        }
    }
    return nrOfmeasurement;
}
void view(int measurements[], int nrOfmeasuremnet)
{
    if (nrOfmeasuremnet == 0)
    {
        printf("[No measurements]\n");
        return;
    }
    printf("[ ");
    for (int i = 0; i < nrOfmeasuremnet; i++)
    {
        printf("%d ", measurements[i]);
    }
    printf("]\n");
}
void compute(int measurements[], int nrOfmeasurement)
{
    int max = calcu_Max(measurements, nrOfmeasurement);
    int min = calcu_Min(measurements, nrOfmeasurement);
    float avr = calcu_avr(measurements, nrOfmeasurement);

    int NormalizedValue[Max_measurements];
    calcu_NorV(measurements, nrOfmeasurement, NormalizedValue);

    printf("Max value: %d\n", max);
    printf("Min value: %d\n", min);
    printf("Average value: %.2f\n", avr);
    printf("[");
    for (int i = 0; i < nrOfmeasurement; i++)
    {
        printf("%d ", NormalizedValue[i]);
    }
    printf("]\n");
}
int calcu_Max(int measurements[], int nrOfmeasurement)
{
    int max = measurements[0];
    for (int i = 0; i < nrOfmeasurement; i++)
    {
        if (max < measurements[i])
        {
            max = measurements[i];
        }
    }
    return max;
}

int calcu_Min(int measurements[], int nrOfmeasurement)
{
    int min = measurements[0];
    for (int i = 0; i < nrOfmeasurement; i++)
    {
        if (min > measurements[i])
        {
            min = measurements[i];
        }
    }
    return min;
}
float calcu_avr(int measurements[], int nrOfmeasurement)
{
    float sum = 0;
    for (int i = 0; i < nrOfmeasurement; i++)
    {
        sum += measurements[i];
    }
    return sum / nrOfmeasurement;
}
void calcu_NorV(int measurements[], int nrOfmeasurement, int NormalizedValue[])
{
    int max = calcu_Max(measurements, nrOfmeasurement);
    float avr = calcu_avr(measurements, nrOfmeasurement);

    for (int i = 0; i < nrOfmeasurement; i++)
    {
        NormalizedValue[i] = (measurements[i] - round(avr));
    }
}