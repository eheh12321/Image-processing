#pragma once
#include "header.h"

int ycbcr_color_test()
{
    BITMAPFILEHEADER bmpFile;
    BITMAPINFOHEADER bmpInfo;

    FILE* inputFile = NULL;

    inputFile = open_image(inputFile);

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
    unsigned char* outputImg_Y = NULL, * outputImg_Cr = NULL, * outputImg_Cb = NULL, * outputImg_R_Y_R = NULL;
    outputImg_Y = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg_Cb = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg_Cr = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg_R_Y_R = (unsigned char*)calloc(size, sizeof(unsigned char));
    //********************

    double Y, Cb, Cr, R, G, B;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            //
            Y = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0];
            Cb = -0.169 * inputImg[j * stride + 3 * i + 2] - 0.331 * inputImg[j * stride + 3 * i + 1] + 0.500 * inputImg[j * stride + 3 * i + 0];
            Cr = 0.500 * inputImg[j * stride + 3 * i + 2] - 0.419 * inputImg[j * stride + 3 * i + 1] - 0.0813 * inputImg[j * stride + 3 * i + 0];

            // ��� ����
            Y += 20;

            Y < 0 ? (Y = 0) : (Y > 255 ? Y = 255 : Y);
            Cb < 0 ? (Cb = 0) : (Cb > 255 ? Cb = 255 : Cb);
            Cr < 0 ? (Cr = 0) : (Cr > 255 ? Cr = 255 : Cr);

            //

            outputImg_Y[j * stride + 3 * i + 0] = (unsigned char)Y;
            outputImg_Y[j * stride + 3 * i + 1] = (unsigned char)Y;
            outputImg_Y[j * stride + 3 * i + 2] = (unsigned char)Y;

            outputImg_Cb[j * stride + 3 * i + 1] = (unsigned char)Cb;
            outputImg_Cb[j * stride + 3 * i + 2] = (unsigned char)Cb;
            outputImg_Cb[j * stride + 3 * i + 0] = (unsigned char)Cb;

            outputImg_Cr[j * stride + 3 * i + 0] = (unsigned char)Cr;
            outputImg_Cr[j * stride + 3 * i + 1] = (unsigned char)Cr;
            outputImg_Cr[j * stride + 3 * i + 2] = (unsigned char)Cr;

            //
            R = Y + 1.475 * Cr;
            G = Y - 0.571 * Cr - 0.165 * Cb;
            B = Y + 1.881 * Cb;

            R < 0 ? (R = 0) : (R > 255 ? R = 255 : R);
            G < 0 ? (G = 0) : (G > 255 ? G = 255 : G);
            B < 0 ? (B = 0) : (B > 255 ? B = 255 : B);
            //

            outputImg_R_Y_R[j * stride + 3 * i + 0] = (unsigned char)B;
            outputImg_R_Y_R[j * stride + 3 * i + 1] = (unsigned char)G;
            outputImg_R_Y_R[j * stride + 3 * i + 2] = (unsigned char)R;
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

    FILE* outputFile_R_Y_R = fopen("./image/Output_R_Y_R.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_R_Y_R);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_R_Y_R);
    fwrite(outputImg_R_Y_R, sizeof(unsigned char), size, outputFile_R_Y_R);

    free(outputImg_R_Y_R);
    fclose(outputFile_R_Y_R);

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