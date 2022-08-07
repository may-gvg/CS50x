#include <stdio.h>
#include <cs50.h>
int main(void)
{
    string s = get_string("What is Your Name:  \n");
    printf("hello, %s\n", s);
}
