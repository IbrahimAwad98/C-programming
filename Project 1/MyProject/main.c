#include "FreeRTOS.h"
#include "task.h"
#include "gd32vf103.h"
#include "drivers.h"

// Global delad array där varje task skriver sitt räknarvärde.
// Den läses av displaytasken (ledPanel).
uint8_t sharedData[4] = {0};

// Task som kontinuerligt uppdaterar LED-panelen.
void ledPanel(void *pvParameters)
{
    volatile char data[1800]; // Reservminne som kan användas av colset()
    while (1)
    {
        l88row(colset()); // Uppdaterar rad för rad på LED-panelen
        vTaskDelay(1);    // Väntar 2ms → ger andra tasks CPU-tid

        // Visa räknarvärden från varje rad
        l88mem(0, sharedData[0]); // Rad 0 ← Task T1
        l88mem(1, sharedData[1]); // Rad 1 ← Task T2
        l88mem(2, sharedData[2]); // Rad 2 ← Task T3
        l88mem(3, sharedData[3]); // Rad 3 ← ev. T4 om du lägger till
        // Töm resterande rader (valfritt)
        l88mem(4, 0);
        l88mem(5, 0);
        l88mem(6, 0);
        l88mem(7, 0);
    }
}

// Task som räknar upp ett värde och visar det på en specifik rad.
void ledTask(void *pvParameters)
{
    int id = *(int *)pvParameters; // Hämtar ID från main (radnummer 0–3)
    int counter = 0;               // Lokalt räknarvärde
    while (1)                      // super loopen
    {
        counter++;                   // Öka med 1
        sharedData[id] = counter;    // Skriv till rätt plats i sharedData
        vTaskDelay((id + 1) * 1000); // Vänta olika tid:
        // id 0 → 1s, id 1 → 2s, id 2 → 3s
        // annars i samma takt då vTaskDelay(1000);
    }
}

int main(void)
{
    int id[4] = {0, 1, 2, 3}; // Array med ID:n till varje räknar-task

    // Initiera LED-panelens drivrutiner
    colinit();
    l88init();

    // Skapa task som uppdaterar displayen hela tiden
    xTaskCreate(ledPanel, "LED", 512, NULL, 2, NULL);

    // Skapa tre räknar-tasks med olika ID
    xTaskCreate(ledTask, "T1", 128, (void *)&id[0], 1, NULL); // Rad 0
    xTaskCreate(ledTask, "T2", 128, (void *)&id[1], 1, NULL); // Rad 1
    xTaskCreate(ledTask, "T3", 128, (void *)&id[2], 1, NULL); // Rad 2
    xTaskCreate(ledTask, "T4", 128, (void *)&id[3], 1, NULL); // Rad 3

    // Starta FreeRTOS-schemaläggaren (börjar köra tasks)
    vTaskStartScheduler();

    // Om RTOS inte startar av någon anledning hamnar vi här
    while (1)
        ;
}