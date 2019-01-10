// creates a resized copy a BMP file

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4 || (argc == 4 && ((float) *argv[1] < 0.0 || (float) *argv[1] > 100.0)))
    {
        fprintf(stderr, "Usage: resize_factor_from_0.0_to_100.0 infile outfile\n");
        return 1;
    }

    // remember user inputs
    float resize_factor = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf_in;
    fread(&bf_in, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi_in;
    fread(&bi_in, sizeof(BITMAPINFOHEADER), 1, inptr);

    //copy values into the outfiles headers
    BITMAPFILEHEADER bf_out = bf_in;
    BITMAPINFOHEADER bi_out = bi_in;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf_in.bfType != 0x4d42 || bf_in.bfOffBits != 54 || bi_in.biSize != 40 ||
        bi_in.biBitCount != 24 || bi_in.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // if the factor is 1 or more then round to an integer
    if (resize_factor >= 1)
    {
        resize_factor = round(resize_factor);
    }

    //update dimensions and size of the bitmap
    bi_out.biHeight = bi_in.biHeight * resize_factor;
    bi_out.biWidth = bi_in.biWidth * resize_factor;

    // determine bytes to be of padding
    int padding_infile = (4 - (bi_in.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_outfile = (4 - (bi_out.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    int infile_byte_width_with_padding = (bi_in.biWidth * sizeof(RGBTRIPLE)) + padding_infile;
    int outfile_byte_width_with_padding = (bi_out.biWidth * sizeof(RGBTRIPLE)) + padding_outfile;

    bf_out.bfSize = (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
                     (abs(bi_out.biHeight) * outfile_byte_width_with_padding)
                    );
    bi_out.biSizeImage = bf_out.bfSize - 54;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_out, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_out, sizeof(BITMAPINFOHEADER), 1, outptr);

    int inverse_resize_factor = 1 / resize_factor;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi_in.biHeight); i < biHeight; i++)
    {
        //drop lines when reducing resolution
        if (resize_factor < 1)
        {
            //The modulo condition below will drop the 0 element and then every element number divisible by inverse_resize_factor
            //multiply and divide by 10 to avoid round off from using ints
            if ((int)((i * 10 / inverse_resize_factor * 10) / 10) % inverse_resize_factor == 0)
            {
                //move file stream over a line
                fseek(inptr, infile_byte_width_with_padding, SEEK_CUR);

                //go to next iteration of for loop
                continue;
            }
        }

        //repeat writes of lines to increase size
        //will only go through this loop once when resize_factor <= 1
        for (int p = 0; p < resize_factor; p++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi_in.biWidth; j++)
            {
                //drop columns when reducing resolution
                if (resize_factor < 1)
                {
                    //The modulo condition below will drop the 0 element and then every element number divisible by inverse_resize_factor
                    //multiply and divide by 10 to avoid round off from using ints
                    if ((int)((j * 10 / inverse_resize_factor * 10) / 10) % inverse_resize_factor == 0)
                    {
                        // skip over a pixel
                        fseek(inptr, sizeof(RGBTRIPLE), SEEK_CUR);

                        //go to next iteration of for loop
                        continue;
                    }
                }

                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                //repeat writes of pixels to increase size
                for (int m = 0; m < resize_factor; m++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // skip over padding, if any
            fseek(inptr, padding_infile, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < padding_outfile; k++)
            {
                fputc(0x00, outptr);
            }

            //move file stream back a row unless this is the last pass
            if (p < (resize_factor - 1))
            {
                fseek(inptr, -infile_byte_width_with_padding, SEEK_CUR);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
