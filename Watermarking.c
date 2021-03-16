#pragma once
#include "header.h"

void watermark()
{
    FILE* inputFile = NULL;
    inputFile = fopen("./image/Output_Y.bmp", "rb");

    FILE* inputFile2 = NULL;
    inputFile2 = fopen("./image/original/SejongMark.bmp", "rb");

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

    unsigned char* outputImg_w = NULL;
    outputImg_w = (unsigned char*)calloc(size, sizeof(unsigned char));

    double Y, Y1, Y2;
    int a, b;
    a = 1;
    b = 3;
    //scanf("%d %d", &a, &b);

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y1 = inputImg[j * stride + i * 3 + 2];
            Y2 = 0.299 * inputImg2[j * stride + i * 3 + 2] + 0.587 * inputImg2[j * stride + i * 3 + 1] + 0.114 * inputImg2[j * stride + i * 3 + 0];

            Y = Y1 / a + Y2 / b;
            Y < 0 ? (Y = 0) : (Y > 255 ? Y = 255 : Y);

            outputImg_w[j * stride + i * 3 + 0] = (unsigned char)Y;
            outputImg_w[j * stride + i * 3 + 1] = (unsigned char)Y;
            outputImg_w[j * stride + i * 3 + 2] = (unsigned char)Y;
        }
    }

    FILE* outputFile_w = fopen("./image/Output_W.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_w);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_w);
    fwrite(outputImg_w, sizeof(unsigned char), size, outputFile_w);

    free(outputImg_w);
    fclose(outputFile_w);

    free(inputImg);
    fclose(inputFile);
}