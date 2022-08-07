

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size

    int n;
    do
    {
        n = get_int("Start Size: ");
    }
    while (n < 9);


    int e;
    do
    {
        e = get_int("End Size: ");
    }
    while (e < n);

    int years = 0;
    while (n < e)
    {
        n = n + (n / 3) - (n / 4);
        years++;
    }


    // TODO: Print number of years

    printf("Years: %i\n", years);

}




