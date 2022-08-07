#include <stdio.h>
#include <math.h>
#include <cs50.h>

float dollars;
int coins = 0;

int main(void)
{
    
    do
    {
        dollars = get_float("Cash owed: ");
    }
    while (dollars <= 0);

    int cents = round(dollars * 100);

    while (cents > 0)
    {
        if (cents >= 25)
        {
            cents -= 25;
        }
        else if (cents >= 10)
        {
            cents -= 10;
        }
        else if (cents >= 5)
        {
            cents -= 5;
        }
        else
        {
            cents --;
        }
        coins++;
    }
    printf("%i\n", coins);
}