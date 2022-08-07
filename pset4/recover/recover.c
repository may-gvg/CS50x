#include <stdio.h>
#include <bits/stdint-uintn.h>
#include <string.h>
#define BLOCK_SIZE  512
typedef uint8_t BYTE;

/*
 * This program assumes, that the SD media dump is in multiples of 512,
 * because it is impossible to read only half of the block.
 */

int main(int argc, char *argv[])
{

    BYTE buf[512];
    int c = 0;
    char filename[255];
    FILE *input = NULL;
    FILE *output = NULL;

    char *source = argv[1];

    //check if we have one argument
    if (argc != 2)
    {
        printf("Usage: ./recover infile\n");
        return 1;
    }

    //open the source file
    input = fopen(source, "r");
    if (!input)
    {
        printf("Can not open file %s\n", source);
        return 1;
    }

    //read the contest of the input file
    while (fread(buf, 1, BLOCK_SIZE, input))
    {
        //check if current block has JFIF signature
        if (buf[0] == 0xff && buf[1] == 0xd8 && buf[2] == 0xff && (buf[3] & 0b11110000) == 0xe0)
        {
            //if we previously opened a file and there's currently new JFIF signature,
            //increase the file number and close the previous file
            if (output)
            {
                c++;
                fclose(output);
            }
            //if this is beggining of the new file, open it for writing
            sprintf(filename, "%03i.jpg", c);
            output = fopen(filename, "w");
        }
        //if we currently have anything to write to, write the current block
        if (output)
        {
            fwrite(buf, 1, BLOCK_SIZE, output);
        }
    }
    //close the files but only if we managed to open them before
    if (output)
    {
        fclose(output);
    }
    if (input)
    {
        fclose(input);
    }
    return 0;
}