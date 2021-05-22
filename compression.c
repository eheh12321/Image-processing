#include "header.h"


void quantization_comp(char* address)
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

    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

    unsigned char* Y = NULL;
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    for (int j = 0; j < height; j++) // 16레벨 quantization
    {
        for (int i = 0; i < width; i++)
        {
            Y[j * width + i] = inputImg[j * stride + 3 * i + 0] / 16 * 16;
        }
    }

    printf("\n[Difference]\n");
    for (int j = 0; j < 24; j++)
    {
        for (int i = 0; i < 24; i++)
        {
            printf("%3d ", Y[j * width + i] / 16);
        }printf("\n");
    }

    int bsize = 2;
    int ar[8][8];

    char bitlist[16][5] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };
    FILE* fp = fopen("bitstream_test.txt", "w");

    int a, b, c, d;
    for (int jj = 0; jj < height; jj += bsize)
    {
        for (int ii = 0; ii < width; ii += bsize)
        {
            a = (int)Y[(jj + 0) * width + ii + 0] / 16;
            b = (int)Y[(jj + 0) * width + ii + 1] / 16;
            c = (int)Y[(jj + 1) * width + ii + 0] / 16;
            d = (int)Y[(jj + 1) * width + ii + 1] / 16;
            
            
            
            if (a == b && b == c && c == d)
            {
                fputs("0", fp);
                fputs(bitlist[a], fp);
            }
            else
            {
                fputs("1", fp);
                fputs(bitlist[a], fp);
                fputs(bitlist[b], fp);
                fputs(bitlist[c], fp);
                fputs(bitlist[d], fp);
            }
        }
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y[j * width + i] = inputImg[j * stride + 3 * i + 0] / 16 * 16;

            outputImg[j * stride + 3 * i + 0] = Y[j * width + i];
            outputImg[j * stride + 3 * i + 1] = Y[j * width + i];
            outputImg[j * stride + 3 * i + 2] = Y[j * width + i];
        }
    }

    FILE* outputFile = fopen("./image/quantization.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(inputImg);
    free(outputImg);
    fclose(inputFile);
    fclose(outputFile);
    fclose(fp);
}

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

    printf("\n[Original]\n");
    for (int j = 0; j < 10; j++)
    {
        for (int i = 0; i < 10; i++)
        {
            printf("%ld ", inputImg[j * stride + 3 * i]);
        }
        printf("\n");
    }
    printf("\n[quantization]\n");
    for (int j = 0; j < 10; j++)
    {
        for (int i = 0; i < 10; i++)
        {
            printf("%ld ", Y[j * width + i]);
        }
        printf("\n");
    }
    printf("\n\n");

    //for (int j = 0; j < 10; j++)
    //{
    //    for (int i = 0; i < 10; i++)
    //    {
    //        tmp = Y[j * width + i] / 16;
    //        printf("%4s ", arr[tmp]);
    //    }
    //    printf("\n");
    //}

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i += 2)
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

void test_decoding(char* address, BITMAPFILEHEADER bmpFile, BITMAPINFOHEADER bmpInfo)
{
    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    unsigned char* Y = NULL;
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    char info[2];
    char buffer[5];
    int bsize = 2;

    FILE* fp = fopen(address, "r");

    int a, b, c, d;
    for (int jj = 0; jj < height; jj += bsize)
    {
        for (int ii = 0; ii < width; ii += bsize)
        {
            fgets(info, sizeof(info), fp);

            if (strcmp(info, "0") == 0)
            {
                fgets(buffer, sizeof(buffer), fp);
                a = convert(buffer);
                b = c = d = a;
            }
            else
            {
                fgets(buffer, sizeof(buffer), fp);
                a = convert(buffer);
                fgets(buffer, sizeof(buffer), fp);
                b = convert(buffer);
                fgets(buffer, sizeof(buffer), fp);
                c = convert(buffer);
                fgets(buffer, sizeof(buffer), fp);
                d = convert(buffer);
            }
            Y[(jj + 0) * width + ii + 0] = (unsigned char)(a * 16);
            Y[(jj + 0) * width + ii + 1] = (unsigned char)(b * 16);
            Y[(jj + 1) * width + ii + 0] = (unsigned char)(c * 16);
            Y[(jj + 1) * width + ii + 1] = (unsigned char)(d * 16);
        }
    }
    fclose(fp);

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

    FILE* outputFile = fopen("./image/test_decoding.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    fclose(outputFile);
    free(outputImg);
}

void decoding(char* address, BITMAPFILEHEADER bmpFile, BITMAPINFOHEADER bmpInfo)
{
    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    unsigned char* Y = NULL;
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    char buffer[5];
    FILE* fp = fopen(address , "r");
    int n = 0;
    while (feof(fp) == 0)
    {
        fgets(buffer, sizeof(buffer), fp);
        Y[n] = (unsigned char)convert(buffer);
        n += 2;
    }

    fclose(fp);

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y[j * width + i] = (unsigned char)(Y[j * width + i] * 16);
        }
    }

    //for (int j = 0; j < 10; j++)
    //{
    //    for (int i = 0; i < 10; i++)
    //    {
    //        printf("%ld ", Y[j * width + i]);
    //    }
    //    printf("\n");
    //}

    double value;
    for (int j = 0; j < height; j++)
    {
        for (int i = 1; i < width - 1; i+=2)
        {
            value = (Y[j * width + i - 1] + Y[j * width + i + 1]) / 2;
            Y[j * width + i] = (unsigned char)value;
        }
        Y[j * width + (width - 1)] = Y[j * width + width - 2];
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

    FILE* outputFile = fopen("./image/decoding.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    fclose(outputFile);
    free(outputImg);
}