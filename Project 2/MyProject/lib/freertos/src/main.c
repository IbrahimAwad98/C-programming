#include "FreeRTOS.h"  // FreeRTOS funktioner (måste inkluderas först)
#include "task.h"      // Skapa och hantera trådar (tasks)
#include "semphr.h"    // Semaforer för synkronisering
#include <math.h>      // För matematiska funktioner
#include <stdlib.h>    // För rand()
#include <limits.h>    // För INT_MAX
#include "gd32vf103.h" // Specifika MCU-funktioner
#include "drivers.h"   // Drivrutiner för LED och tangentbord
#include "lcd.h"       // Drivrutin för LCD-skärm

// Globala variabler
volatile int nnb = 0;        // Nästa kundnummer (antal kunder som anlänt)
QueueHandle_t xLCDQ;         // Kö för att kommunicera med LCD-tråden
volatile int qnb = 0;        // Kundnummer för de som står i kö
SemaphoreHandle_t xQnbMutex; // Mutex för att skydda qnb
SemaphoreHandle_t xCustWait; // Semafor som signalerar om kunder finns

// Skapar en Poisson-fördelad slumpmässig tid
int poisson(int lambda)
{
    static double limit[7] = {0, 0, 0, 0.497870684, 0.017315639, 0.006737947, 0.00247875};
    double prod = (double)rand() / INT_MAX;
    int n;
    for (n = 0; prod >= limit[lambda]; n++)
        prod *= (double)rand() / INT_MAX;
    return n;
}

// Uppgift för att visa saker på LCD-panelen
void lcdPanel(void *pvParameters)
{
    int msg;
    Lcd_SetType(LCD_INVERTED); // Inverterad LCD-bakgrund
    Lcd_Init();                // Initiera LCD
    LCD_Clear(RED);            // Rensa skärmen med röd bakgrund

    while (1)
    {
        LCD_WR_Queue();    // Hämtar nästa meddelande från kön
        switch (msg >> 12) // Kolla typ av meddelande (översta 4 bitar)
        {
        case 0: // Visa nnb (nya nummer)
            LCD_ShowNum(10, 10, msg % 256, 3, WHITE);
            LCD_ShowNum(70, 10, msg >> 8, 2, WHITE);
            break;
        case 1: // Visa pågående kund (ex: köplats)
            if (msg % 256)
            {
                LCD_ShowNum(10, 30, msg % 256, 5, WHITE);
                LCD_ShowNum(70, 30, (msg >> 8) % 16, 2, WHITE);
            }
            else
            {
                LCD_ShowString(10, 30, "IDLE!", WHITE);
            }
            break;
        case 2: // Ytterligare en visning (möjligen annan kö)
            if (msg % 256)
            {
                LCD_ShowNum(10, 50, msg % 256, 5, WHITE);
                LCD_ShowNum(70, 50, (msg >> 8) % 16, 2, WHITE);
            }
            else
            {
                LCD_ShowString(10, 50, "IDLE!", WHITE);
            }
            break;
        }
    }
}

// Arrival task – skapar nya kunder slumpmässigt
void arrival(void *pvParameters)
{
    int delta;
    int msg;
    while (1)
    {
        // Msb kundenummer
        //  delta Antal nya kunder
        //  första kunden få största värde
        //  skickar till kön
        //  läser från kön
        
        delta = poisson(4); // Hur många kunder anländer
        for (int i = 0; i < delta; i++)
        {
            msg = nnb + ((delta - 1 - i) << 8); // Meddelande till LCD
            // Här skulle vi normalt skicka till kön:
            // xQueueSend(xLCDQ, &msg, portMAX_DELAY);
            vTaskDelay(500); // Vänta mellan varje kund
        }
        nnb++; // Öka totalt kundnummer
    }
}

// Clerk task – behandlar kunder en i taget
void clerk(void *pvParameters)
{
    int cid = (*(int *)pvParameters) << 12; // Unikt ID för kassören
    int cst;                                // Hur lång service-tid
    int cnb;                                // Kundnummer
    int msg;
    int idl = (*(int *)pvParameters) << 12; // Idle-meddelande

    while (1)
    {
        // Om det inte finns några kunder – visa idle
        if ("Det finns inga kunder som väntar!")
            xQueueSend(xLCDQ, &idl, portMAX_DELAY);

        // Vänta på kund, skydda access till qnb
        // xSemaphoreTake(xQnbMutex, portMAX_DELAY);
        cnb = ++qnb;
        // xSemaphoreGive(xQnbMutex);

        cst = poisson(6); // Service-tid
        for (int i = 0; i < cst; i++)
        {
            msg = cid + ((cst - 1 - i) << 8) + cnb; // Skapa meddelande
            xQueueSend(xLCDQ, &msg, portMAX_DELAY);
            vTaskDelay(500); // Simulera behandlingstid
        }
        vTaskDelay(500); // Paus innan nästa kund
    }
}

int main(void)
{
    int id[4] = {0, 1, 2, 3};

    xLCDQ = xQueueCreate(10, sizeof(int)); // Initiera LCD-kö

    // Skapa trådar
    xTaskCreate(lcdPanel, "LCD", 512, (void *)NULL, 2, NULL);
    xTaskCreate(arrival, "ARR", 512, (void *)NULL, 2, NULL);
    // xTaskCreate(clerk, "CL1", 512, (void*)&id[1], 2, NULL);
    // xTaskCreate(clerk, "CL2", 512, (void*)&id[2], 2, NULL);

    vTaskStartScheduler(); // Starta FreeRTOS-planeraren

    while (1)
        ; // Ska aldrig nås
}
