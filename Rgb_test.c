#pragma once
#include "header.h"

void rgb_color_test(char* address)
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

    //********************
    unsigned char* outputImg_R = NULL, * outputImg_G = NULL, * outputImg_B = NULL, * outputImg_A = NULL;
    outputImg_R = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg_G = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg_B = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg_A = (unsigned char*)calloc(size, sizeof(unsigned char));
    //********************

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            if (i < 100)
            {
                outputImg_A[j * stride + 3 * i + 0] = 0;
                outputImg_A[j * stride + 3 * i + 1] = 0;
                outputImg_A[j * stride + 3 * i + 2] = 0;
            }
            else if (i < 200)
            {
                outputImg_A[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];
                outputImg_A[j * stride + 3 * i + 1] = 0;
                outputImg_A[j * stride + 3 * i + 2] = 0;
            }
            else if (i < 300)
            {
                outputImg_A[j * stride + 3 * i + 0] = 0;
                outputImg_A[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];
                outputImg_A[j * stride + 3 * i + 2] = 0;
            }
            else if (i < 400)
            {
                outputImg_A[j * stride + 3 * i + 0] = 0;
                outputImg_A[j * stride + 3 * i + 1] = 0;
                outputImg_A[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];
            }
            else
            {
                outputImg_A[j * stride + 3 * i + 0] = 255;
                outputImg_A[j * stride + 3 * i + 1] = 255;
                outputImg_A[j * stride + 3 * i + 2] = 255;
            }

            outputImg_R[j * stride + 3 * i + 0] = 0;
            outputImg_R[j * stride + 3 * i + 1] = 0;
            outputImg_R[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];

            outputImg_G[j * stride + 3 * i + 0] = 0;
            outputImg_G[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];
            outputImg_G[j * stride + 3 * i + 2] = 0;

            outputImg_B[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];
            outputImg_B[j * stride + 3 * i + 1] = 0;
            outputImg_B[j * stride + 3 * i + 2] = 0;
        }
    }

    //***********************************
    FILE* outputFile_R = fopen("./image/Output_R.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_R);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_R);
    fwrite(outputImg_R, sizeof(unsigned char), size, outputFile_R);

    FILE* outputFile_G = fopen("./image/Output_G.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_G);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_G);
    fwrite(outputImg_G, sizeof(unsigned char), size, outputFile_G);

    FILE* outputFile_B = fopen("./image/Output_B.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_B);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_B);
    fwrite(outputImg_B, sizeof(unsigned char), size, outputFile_B);

    FILE* outputFile_A = fopen("./image/Output_A.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_A);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_A);
    fwrite(outputImg_A, sizeof(unsigned char), size, outputFile_A);

    free(outputImg_A);
    fclose(outputFile_A);

    free(outputImg_R);
    fclose(outputFile_R);

    free(outputImg_G);
    fclose(outputFile_G);

    free(outputImg_B);
    fclose(outputFile_B);
    //************************************

    free(inputImg);
    fclose(inputFile);

}
