#pragma once
#include "header.h"

void filtering()
{
    FILE* inputFile = NULL;
    inputFile = fopen("./image/Output_Y.bmp", "rb");

    FILE* inputFile2 = NULL;
    inputFile2 = fopen("./image/original/FilteringMask2.bmp", "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile2);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile2);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    unsigned char* inputImg2 = NULL;
    inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg2, sizeof(unsigned char), size, inputFile2);

    unsigned char* outputImg_F = NULL;
    outputImg_F = (unsigned char*)calloc(size, sizeof(unsigned char));

    int filter = 1;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {   
            filter = 1;

            if (inputImg2[j * stride + i * 3 + 0] == 255)
            {
                filter = 0;
            }
            else
            {
                outputImg_F[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0] * filter;
                outputImg_F[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1] * filter;
                outputImg_F[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2] * filter;
            }
        }
    }

    FILE* outputFile_F = fopen("./image/Output_F.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_F);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_F);
    fwrite(outputImg_F, sizeof(unsigned char), size, outputFile_F);

    free(outputImg_F);
    fclose(outputFile_F);

    free(inputImg);
    fclose(inputFile);

    free(inputImg2);
    fclose(inputFile2);
}

void random_noise_generate()
{
    FILE* inputFile = NULL;
    inputFile = fopen("./image/Output_Y.bmp", "rb");

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

    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

    int Err = 100;
    //srand(time(NULL));
    double Y;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y = inputImg[j * stride + 3 * i + 0];
            Y += rand() % Err - (Err >> 1);

            Y > 255 ? (Y = 255) : (Y < 0 ? (Y = 0) : Y);
            outputImg[j * stride + 3 * i + 0] = (unsigned char)Y;
            outputImg[j * stride + 3 * i + 1] = (unsigned char)Y;
            outputImg[j * stride + 3 * i + 2] = (unsigned char)Y;
        }
    }

    FILE* outputFile = fopen("./image/Output_Rnoise.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);

}

void sp_noise_generate()
{
    FILE* inputFile = NULL;
    inputFile = fopen("./image/Output_Y.bmp", "rb");

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

    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

    int filter;
    srand(time(NULL));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            filter = rand() % 30;

            if (filter == 0)
            {
                outputImg[j * stride + 3 * i + 0] = 0;
                outputImg[j * stride + 3 * i + 1] = 0;
                outputImg[j * stride + 3 * i + 2] = 0;
            }
            else if (filter == 1)
            {
                outputImg[j * stride + 3 * i + 0] = 255;
                outputImg[j * stride + 3 * i + 1] = 255;
                outputImg[j * stride + 3 * i + 2] = 255;
            }
            else
            {
                outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];
                outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];
                outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];
            }
        }
    }

    FILE* outputFile = fopen("./image/Output_SPnoise.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);

}