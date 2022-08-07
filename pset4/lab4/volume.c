// Modifies the volume of an audio file
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // copy the header
    uint8_t header[HEADER_SIZE];
    fread(&header, sizeof(header), 1, input);
    fwrite(&header, sizeof(header), 1, output);

    // Read samples from input file and write updated data to output file
    int16_t buffer = 0;
    long temp = 0;
    while (fread(&buffer, sizeof(buffer), 1, input))
    {
        // when increasing volume, make sure it doest exceed int_16 values so it doesnt disort the sound
        temp = (long)buffer * factor;
        if (temp > 32767)
        {
            temp = 32767;
        }
        if (temp < -32767)
        {
            temp = -32767;
        }
        buffer = (int16_t)temp;
        fwrite(&buffer, sizeof(buffer), 1, output);
    }

    // close streams
    fclose(input);
    fclose(output);
}

