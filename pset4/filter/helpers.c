#include "helpers.h"
#include <math.h>
#include <string.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            //load values
            int r = image[i][k].rgbtRed;
            int g = image[i][k].rgbtGreen;
            int b = image[i][k].rgbtBlue;

            //convert to greyscale
            float grey = round((r + g + b) / 3.0);
            int rounded = (grey);

            //apply newvalues to image
            image[i][k].rgbtRed = rounded;
            image[i][k].rgbtGreen = rounded;
            image[i][k].rgbtBlue = rounded;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            //load values
            int r = image[i][k].rgbtRed;
            int g = image[i][k].rgbtGreen;
            int b = image[i][k].rgbtBlue;

            //convert to sepia
            int newr = round(0.393 * r + 0.769 * g + 0.189 * b);
            int newg = round(0.349 * r + 0.686 * g + 0.168 * b);
            int newb = round(0.272 * r + 0.534 * g + 0.131 * b);

            //make sure they are in range
            if (newr > 255)
            {
                newr = 255;
            }
            if (newg > 255)
            {
                newg = 255;
            }
            if (newb > 255)
            {
                newb = 255;
            }
            //apply values
            image[i][k].rgbtRed = newr;
            image[i][k].rgbtGreen = newg;
            image[i][k].rgbtBlue = newb;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;

    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width / 2; k++)
        {
            //flip the pixels
            int x = width - k - 1;
            temp = image[i][k];
            image[i][k] = image[i][x];
            image[i][x] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Temporary storage
    RGBTRIPLE copy[height][width];

    // Copying the image to keep an unaltered version to loop over
    int len = sizeof(RGBTRIPLE[height][width]);
    memcpy(copy, image, len);

    // Iterate over every row of the image
    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            float c = 0;

            for (int m = -1; m <= 1; m++)
            {
                for (int p = -1; p <= 1; p++)
                {
                    int im = i + m;
                    int kp = k + p;
                    if (im != height && kp != width && im != -1 && kp != -1)
                    {
                        r += copy[im][kp].rgbtRed;
                        g += copy[im][kp].rgbtGreen;
                        b += copy[im][kp].rgbtBlue;
                        c++;
                    }
                }
            }
            int fr = round(r / c);
            int fg = round(g / c);
            int fb = round(b / c);

            image[i][k].rgbtRed = fr;
            image[i][k].rgbtGreen = fg;
            image[i][k].rgbtBlue = fb;
        }
    }
    return;
}