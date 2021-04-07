#include "header.h"

void gamma_encoding(char* address, char* output, double gamma)
{
    FILE* inputFile = NULL;
    inputFile = fopen(address, "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    unsigned char* Y1 = NULL, *Y2 = NULL;
    Y1 = (unsigned char*)calloc(size / 3, sizeof(unsigned char));
    Y2 = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

    unsigned char value;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
            
            value = (unsigned char)(255 * pow((Y1[j * width + i] / 255.0), gamma));

            value > 255 ? (value = 255) : ((value < 0) ? (value = 0) : value);
            Y2[j * width + i] = value;
        }
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            outputImg[j * stride + 3 * i + 0] = Y2[j * width + i];
            outputImg[j * stride + 3 * i + 1] = Y2[j * width + i];
            outputImg[j * stride + 3 * i + 2] = Y2[j * width + i];
        }
    }

    FILE* outputFile = fopen(output, "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);

    free(Y1);
    free(Y2);
}