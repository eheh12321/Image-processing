#include "header.h"


void compression(char* address)
{
    FILE* inputFile = NULL;
    inputFile = fopen(address, "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage; // height * width * 3 (R,G,B) !!!
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4; // (width * 3) >> 한 픽셀에 R,G,B 3개 값을 넣기 위해 3배로 늘림 

    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    unsigned char* Y = NULL;
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y[j * width + i] = inputImg[j * stride + 3 * i + 0] / 16 * 16;
        }
    }

    char arr[17][5] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };
    int tmp;
    FILE* fp = fopen("bitstream.txt", "w");
    
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            tmp = (int)(Y[j * width + i] / 16);
            fputs(arr[tmp], fp);
        }
    }
    fclose(fp);
    free(inputImg);
    fclose(inputFile);

    free(Y);
}

int convert(char* str)
{
    int n = atoi(str);
    int dec = 0, i = 0, rem;

    while (n != 0)
    {
        rem = n % 10;
        n /= 10;
        dec += rem * (int)pow(2, i);
        i += 1;
    }
    return dec;
}

void decoding(char* address)
{
    int width = 512;
    int height = 512;
    int size = height * width * 3;
    int bitCnt = 8;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    unsigned char* Y = NULL;
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    char buffer[5];
    FILE* fp = fopen("bitstream.txt", "r");
    int n = 0;
    while (feof(fp) == 0)
    {
        fgets(buffer, sizeof(buffer), fp);
        Y[n++] = (unsigned char)convert(buffer);
    }

    fclose(fp);

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y[j * width + i] = (unsigned char)(Y[j * width + i] * 16);
        }
    }

    for (int j = 0; j < 30; j++)
    {
        for (int i = 0; i < 30; i++)
        {
            printf("%3d ", Y[j * width + i]);
        }
    }
    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            outputImg[j * stride + i * 3 + 0] = Y[j * width + i];
            outputImg[j * stride + i * 3 + 1] = Y[j * width + i];
            outputImg[j * stride + i * 3 + 2] = Y[j * width + i];
        }
    }

    FILE* outputFile = fopen("./image/test.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    fclose(outputFile);
    free(outputImg);
    free(Y);
}