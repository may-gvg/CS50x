#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

int main(void)
{
    string text = get_string("Text: \n");

    int letterCount = 0;
    int wordCount = 1;
    int sentenseCount = 0;

    for (int i = 0; i < strlen(text); i++)
    {

        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            letterCount++;
        }
        else if (text[i]  == ' ')
        {
            wordCount++;
        }

        else if (text[i] == '!' || text[i] == '?' || text[i] == '.')
        {
            sentenseCount++;
        }

    }

    float index = 0.0588 * ((100 * (float)letterCount / (float)wordCount)) -
                  (0.296 * (100 * (float)sentenseCount / (float)wordCount)) - 15.8;

    if (index >= 1 && index <= 16)
    {
        printf("Grade %i\n", (int)round(index)); //checking main condition
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Before Grade 1\n");
    }

}