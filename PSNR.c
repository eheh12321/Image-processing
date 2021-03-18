#pragma once
#include "header.h"

void PSNR()
{
    // YCbCr의 Y값으로 서로 비교함.
    FILE* inputFile2 = NULL;
    inputFile2 = fopen("./image/Output_Y.bmp", "rb");

    FILE* inputFile = NULL;
    inputFile = fopen("./image/original/AICenter_Noise.bmp", "rb");

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

    // 배열 복사
    unsigned char* Y1 = NULL;
    Y1 = (unsigned char*)calloc(size, sizeof(unsigned char));

    unsigned char* Y2 = NULL;
    Y2 = (unsigned char*)calloc(size, sizeof(unsigned char));

    double mse = 0, psnr;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
           // R G B 중 아무거나. (어짜피 다 똑같은 Y값이니까)
           Y2[j * width + i] = inputImg2[j * stride + 3 * i + 0];
           Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
        }
    }    
    
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            mse += (double)(pow((Y2[j * width + i] - Y1[j * width + i]), 2));
        }
    }

    mse /= (width * height);
    psnr = mse != 0.0 ? (10.0 * log10(255 * 255 / mse)) : 99.99;
    printf("MSE = %.2lf\nPSNR = %.2lf dB\n", mse, psnr);

    free(inputImg);
    fclose(inputFile);

    free(inputImg2);
    fclose(inputFile2);
}