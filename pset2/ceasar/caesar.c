#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>

int atoi(string string);

//check if all characters are digits
int digits_only(char *s)
{
    int slen = strlen(s);
    for (int i = 0; i < slen; i++)
    {
        if (isdigit(s[i]) == 0)
        {
            return 0;
        }
    }

    return 1;
}

int main(int argc, string argv[])
{
    char s1, mod, fin;
    //first check if we have one argument, and if it's only digits
    if (!(argc == 2 && digits_only(argv[1])))
    {
        printf("Usage: ./caesar k\n");
        return 1;
    }

    //if all ok, save digit to int and get the str
    int key = atoi(argv[1]);

    string str = get_string("plaintext: ");
    int slen = strlen(str);
    printf("ciphertext: ");

    //encryption loop for both lowercase and uppercase characters
    //with mod 26 to make bigger digits working
    for (int i = 0; i < slen; i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            s1 = str[i] - 'a';
            mod = (s1 + key) % 26;
            fin = mod + 'a';
            printf("%c", fin);
        }
        else if (str[i] >= 'A' && str[i] <= 'Z')
        {
            s1 = str[i] - 'A';
            mod = (s1 + key) % 26;
            fin = mod + 'A';
            printf("%c", fin);
        }
        else
        {
            printf("%c", str[i]);
        }
    }

    printf("\n");
    return 0;
}
