#pragma once
#include "header.h"

void YI_diff()
{
    BITMAPFILEHEADER bmpFile_I;
    BITMAPINFOHEADER bmpInfo_I;

    FILE* inputFile_Y = NULL;
    FILE* inputFile_I = NULL;

    inputFile_Y = fopen("./image/Output_Y.bmp", "rb");
    inputFile_I = fopen("./image/Output_I.bmp", "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile_Y);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile_Y);

    fread(&bmpFile_I, sizeof(BITMAPFILEHEADER), 1, inputFile_I);
    fread(&bmpInfo_I, sizeof(BITMAPINFOHEADER), 1, inputFile_I);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    unsigned char* inputImg_Y = NULL;
    inputImg_Y = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg_Y, sizeof(unsigned char), size, inputFile_Y);

    unsigned char* inputImg_I = NULL;
    inputImg_I = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg_I, sizeof(unsigned char), size, inputFile_I);

    unsigned char* outputImg_YI_diff = NULL;
    outputImg_YI_diff = (unsigned char*)calloc(size, sizeof(unsigned char));

    double diff;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            diff = pow((inputImg_Y[j * stride + 3 * i + 2] - inputImg_I[j * stride + 3 * i + 2]), 2);

            diff > 255 ? (diff = 255) : diff;

            outputImg_YI_diff[j * stride + 3 * i + 2] = (unsigned char)diff;
            outputImg_YI_diff[j * stride + 3 * i + 1] = (unsigned char)diff;
            outputImg_YI_diff[j * stride + 3 * i + 0] = (unsigned char)diff;
        }
    }

    FILE* outputFile_YI_diff = fopen("./image/Output_YI_diff.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_YI_diff);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_YI_diff);
    fwrite(outputImg_YI_diff, sizeof(unsigned char), size, outputFile_YI_diff);

    free(outputImg_YI_diff);
    fclose(outputFile_YI_diff);

    free(inputImg_Y);
    fclose(inputFile_Y);

    free(inputImg_I);
    fclose(inputFile_I);
}