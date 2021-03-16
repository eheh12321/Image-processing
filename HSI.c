#pragma once
#include "header.h"

void HSI()
{

    FILE* inputFile = NULL;
    inputFile = open_image();

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

    unsigned char* outputImg_I = NULL;
    outputImg_I = (unsigned char*)calloc(size, sizeof(unsigned char));

    double I;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            I = (inputImg[j * stride + 3 * i + 2] + inputImg[j * stride + 3 * i + 2] + inputImg[j * stride + 3 * i + 2]) / 3;

            I < 0 ? (I = 0) : (I > 255 ? (I = 255) : I);

            outputImg_I[j * stride + 3 * i + 0] = (unsigned char)I;
            outputImg_I[j * stride + 3 * i + 1] = (unsigned char)I;
            outputImg_I[j * stride + 3 * i + 2] = (unsigned char)I;

        }
    }

    FILE* outputFile_I = fopen("./image/Output_I.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_I);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_I);
    fwrite(outputImg_I, sizeof(unsigned char), size, outputFile_I);

    free(outputImg_I);
    fclose(outputFile_I);

    free(inputImg);
    fclose(inputFile);
}