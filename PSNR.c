#pragma once
#include "header.h"

void PSNR(char* address)
{
    // YCbCr의 Y값으로 서로 비교함.
    FILE* Original_input = NULL;
    Original_input = fopen("./image/Output_Y.bmp", "rb");

    FILE* inputFile = NULL;
    inputFile = fopen(address, "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, Original_input);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, Original_input);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    unsigned char* Original_Img = NULL;
    Original_Img = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(Original_Img, sizeof(unsigned char), size, Original_input);

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
           Y2[j * width + i] = Original_Img[j * stride + 3 * i + 0];
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
    printf(" MSE = %.2lf\n PSNR = %.2lf dB\n", mse, psnr);

    free(inputImg);
    fclose(inputFile);

    free(Original_Img);
    fclose(Original_input);
}