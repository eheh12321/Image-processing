#pragma once
#include "header.h"

int ycbcr_color_test()
{
    BITMAPFILEHEADER bmpFile;
    BITMAPINFOHEADER bmpInfo;

    FILE* inputFile = NULL;

    inputFile = fopen("./image/AICenter.bmp", "rb");

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
    unsigned char* outputImg_Y = NULL, * outputImg_Cr = NULL, * outputImg_Cb = NULL;
    outputImg_Y = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg_Cb = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg_Cr = (unsigned char*)calloc(size, sizeof(unsigned char));
    //********************

    double Y, Cb, Cr;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            //
            Y = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0];
            Cb = -0.169 * inputImg[j * stride + 3 * i + 2] - 0.331 * inputImg[j * stride + 3 * i + 1] + 0.500 * inputImg[j * stride + 3 * i + 0];
            Cr = 0.500 * inputImg[j * stride + 3 * i + 2] - 0.419 * inputImg[j * stride + 3 * i + 1] -0.0613 * inputImg[j * stride + 3 * i + 0];
            //
            outputImg_Y[j * stride + 3 * i + 0] = (int)Y;
            outputImg_Y[j * stride + 3 * i + 1] = (int)Y;
            outputImg_Y[j * stride + 3 * i + 2] = (int)Y;

            outputImg_Cb[j * stride + 3 * i + 0] = (int)Cb;
            outputImg_Cb[j * stride + 3 * i + 1] = (int)Cb;
            outputImg_Cb[j * stride + 3 * i + 2] = (int)Cb;

            outputImg_Cr[j * stride + 3 * i + 0] = (int)Cr;
            outputImg_Cr[j * stride + 3 * i + 1] = (int)Cr;
            outputImg_Cr[j * stride + 3 * i + 2] = (int)Cr;
        }
    }

    //***********************************
    FILE* outputFile_Y = fopen("./image/Output_Y.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_Y);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_Y);
    fwrite(outputImg_Y, sizeof(unsigned char), size, outputFile_Y);

    FILE* outputFile_Cb = fopen("./image/Output_Cb.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_Cb);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_Cb);
    fwrite(outputImg_Cb, sizeof(unsigned char), size, outputFile_Cb);

    FILE* outputFile_Cr = fopen("./image/Output_Cr.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_Cr);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_Cr);
    fwrite(outputImg_Cr, sizeof(unsigned char), size, outputFile_Cr);

    free(outputImg_Cr);
    fclose(outputFile_Cr);

    free(outputImg_Cb);
    fclose(outputFile_Cb);

    free(outputImg_Y);
    fclose(outputFile_Y);
    //************************************

    free(inputImg);
    fclose(inputFile);

    return 0;
}
