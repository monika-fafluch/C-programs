#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    char *number = argv[1];
    int n = atoi(number);

    if (n < 0 || n > 100)
    {
        printf("Please try again\n");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "rb");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "wb");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 4;
    }


    //update headers for outfile


    int oldHeight = bi.biHeight;   // keep old values
    int oldWidth = bi.biWidth;

    bi.biHeight *= n;  //updated HEIGHT and WIDTH
    bi.biWidth *= n;

    // determine padding for scanlines
    int padding = (4 - (oldWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    int paddingWidth = oldWidth + padding;


    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + newPadding) * abs(
                         bi.biHeight);   //abs supports also negative numbers - in case od bottom-top bitmaps
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);





    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    int headers = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);




    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(oldHeight); i < biHeight; i++)
    {

        for (int f = 0; f < n; f++)
        {


            // iterate over pixels in scanline
            for (int j = 0; j < oldWidth; j++)
            {


                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile

                for (int d = 0; d < n; d++)

                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }



            if (newPadding > 0)      // add outfile's padding
            {
                for (int k = 0; k < newPadding; k++)

                {
                    fputc(0x00, outptr);
                }
            }


            if (padding > 0)     //skip over infile's padding

            {
                fseek(inptr, padding, SEEK_CUR);
            }


            if (f < n - 1)

            {
                fseek(inptr, - ((oldWidth * 3) + padding), SEEK_CUR);
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
