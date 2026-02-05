#include <stdio.h>
#include <math.h>

int main()
{

  int time = 0;
  int throttle = 0;
  float v1 = -25.0;
  float h1 = 250.0;
  int fuel = 500;

  printf("Hello World! \n");
  printf("Welcome to Lab1 in C! \n");
  printf("It's Lunar decent challenge! \n");
  printf("You will pilot a lunar decent the last 250m, each turn represent 1-second decent time.\n");
  printf("Set the throttle for each turn (0-100): \n");
  printf("Time   Height  Velocity  Fuel  Throttle\n");

  while (h1 > 0)
  {

    printf("%d      %0.1f    %0.1f    %d     ", time, h1, v1, fuel);

    if (fuel == 0)
    {
      throttle = 0;
      printf("OBS! your fuel is finsished.\n");
    }
    else
    {
      scanf("%d", &throttle);
    }

    h1 = h1 + v1 + (0.1 * throttle - 1.5) / 2; // h beroende på v.
    v1 = v1 + (0.1 * throttle - 1.5);
    fuel = fuel - throttle;

    time++;
  }
  if (v1 > -2)
  {
    // 0 0 0 0 0 0 100 100 100 15 100.
    printf("Success! landing at: %0.1f m/s\n", v1);
  }
  else
  {
    printf("Failed! crash landing at: %0.1f m/s\n", v1);
  }
  return 0;
}