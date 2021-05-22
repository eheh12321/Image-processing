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

    //printf("\n[Difference]\n");
    //for (int j = 0; j < 256; j++)
    //{
    //    for (int i = 0; i < 50; i++)
    //    {
    //        printf("%3d ", Y[j * width + i] / 16);
    //    }printf("\n");
    //}

    int bsize = 3;

    char bitlist_4[17][5] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };
    char bitlist_3[8][4] = {"000", "001", "010", "011", "100", "101", "110", "111"};
    //char unbitlist[10][11] = { "0", "10", "110", "1110", "111110", "1111110", "11111110", "111111110", "1111111110", "11111111110"};
    char unbitlist[5][5] = { "0", "10", "110", "1110" };
    int block[17] = { 0 }; // 1번 인덱스부터 1. 16번 인덱스가 16
    FILE* fp = fopen("bitstream_test.txt", "w");

    int a1, a2, a3, b1, b2, b3, c1, c2, c3;
    int maxidx, minidx, diff;
    for (int jj = 0; jj < height - bsize; jj += bsize)
    {
        for (int ii = 0; ii < width - bsize; ii += bsize)
        {
            for (int k = 0; k < 17; k++)
            {
                block[k] = 0;
            }

            a1 = (int)Y[(jj + 0) * width + ii + 0] / 16;
            a2 = (int)Y[(jj + 0) * width + ii + 1] / 16;
            a3 = (int)Y[(jj + 0) * width + ii + 2] / 16;
            b1 = (int)Y[(jj + 1) * width + ii + 0] / 16;
            b2 = (int)Y[(jj + 1) * width + ii + 1] / 16;
            b3 = (int)Y[(jj + 1) * width + ii + 2] / 16;
            c1 = (int)Y[(jj + 2) * width + ii + 0] / 16;
            c2 = (int)Y[(jj + 2) * width + ii + 1] / 16;
            c3 = (int)Y[(jj + 2) * width + ii + 2] / 16;

            block[a1] += 1;
            block[a2] += 1;
            block[a3] += 1;
            block[b1] += 1; 
            block[b2] += 1;
            block[b3] += 1;
            block[c1] += 1;
            block[c2] += 1;
            block[c3] += 1;

            //블럭 내 최대 차이 구하기
            maxidx = minidx = 0;
            for (int k = 0; k < 16; k++)
            {
                if (block[k] != 0 && minidx == 0)
                {
                    minidx = k;
                }
                if (block[k] != 0)
                {
                    maxidx = k;
                }
            }
            diff = maxidx - minidx;

            a1 = (a1 - minidx);
            a2 = (a2 - minidx);
            a3 = (a3 - minidx);
            b1 = (b1 - minidx); 
            b2 = (b2 - minidx);
            b3 = (b3 - minidx);
            c1 = (c1 - minidx);
            c2 = (c2 - minidx);
            c3 = (c3 - minidx);
            
            a1 > 7 ? (a1 = 7) : a1;
            a2 > 7 ? (a2 = 7) : a2;
            a3 > 7 ? (a3 = 7) : a3;
            b1 > 7 ? (b1 = 7) : b1;
            b2 > 7 ? (b2 = 7) : b2;
            b3 > 7 ? (b3 = 7) : b3;
            c1 > 7 ? (c1 = 7) : c1;
            c2 > 7 ? (c2 = 7) : c2;
            c3 > 7 ? (c3 = 7) : c3;
            
            if (diff <= 3) // 차이가 0, 1, 2인 경우 -> 유동형 비트방식으로 처리
            {
                fputs("1", fp);
                fputs(bitlist_4[minidx], fp); // DC Gain값 전송

                fputs(unbitlist[a1], fp);
                fputs(unbitlist[a2], fp);
                fputs(unbitlist[a3], fp);
                fputs(unbitlist[b1], fp);
                fputs(unbitlist[b2], fp);
                fputs(unbitlist[b3], fp);
                fputs(unbitlist[c1], fp);
                fputs(unbitlist[c2], fp);
                fputs(unbitlist[c3], fp);
            }
            else // 4 이상인 경우 -> 고정형 비트 방식으로 처리
            {
                fputs("0", fp);
                fputs(bitlist_4[minidx], fp); // DC Gain값 전송

                fputs(bitlist_3[a1], fp);
                fputs(bitlist_3[a2], fp);
                fputs(bitlist_3[a3], fp);
                fputs(bitlist_3[b1], fp);
                fputs(bitlist_3[b2], fp);
                fputs(bitlist_3[b3], fp);
                fputs(bitlist_3[c1], fp);
                fputs(bitlist_3[c2], fp);
                fputs(bitlist_3[c3], fp);
            }

            Y[(jj + 0) * width + ii + 0] = (unsigned char)((minidx + a1) * 16);
            Y[(jj + 0) * width + ii + 1] = (unsigned char)((minidx + a2) * 16);
            Y[(jj + 0) * width + ii + 2] = (unsigned char)((minidx + a3) * 16);
            Y[(jj + 1) * width + ii + 0] = (unsigned char)((minidx + b1) * 16);
            Y[(jj + 1) * width + ii + 1] = (unsigned char)((minidx + b2) * 16);
            Y[(jj + 1) * width + ii + 2] = (unsigned char)((minidx + b3) * 16);
            Y[(jj + 2) * width + ii + 0] = (unsigned char)((minidx + c1) * 16);
            Y[(jj + 2) * width + ii + 1] = (unsigned char)((minidx + c2) * 16);
            Y[(jj + 2) * width + ii + 2] = (unsigned char)((minidx + c3) * 16);
        }
    }
    fclose(fp);
    
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
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

    char buffer[5];
    int bsize = 2;

    FILE* fp = fopen(address, "r");
    
    //int a, b, c, d;
    //for (int jj = 0; jj < height; jj += bsize)
    //{
    //    for (int ii = 0; ii < width; ii += bsize)
    //    {

    //        Y[(jj + 0) * width + ii + 0] = (unsigned char)(a * 16);
    //        Y[(jj + 0) * width + ii + 1] = (unsigned char)(b * 16);
    //        Y[(jj + 1) * width + ii + 0] = (unsigned char)(c * 16);
    //        Y[(jj + 1) * width + ii + 1] = (unsigned char)(d * 16);
    //    }
    //}
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