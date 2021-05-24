#include "header.h"


void quantization_comp(char* address, char* output)
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
    int bsize = 4;
    char bitlist_4[17][5] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };
    char bitlist_3[8][4] = {"000", "001", "010", "011", "100", "101", "110", "111"};
    char unbitlist[5][5] = { "0", "10", "110", "1110" };
    int block[17] = { 0 }; // 1번 인덱스부터 1. 16번 인덱스가 16
    int ar[4][4];
    FILE* fp = fopen(output, "w");

    int maxidx, minidx, diff;
    for (int jj = 0; jj <= height - bsize; jj += bsize)
    {
        for (int ii = 0; ii <= width - bsize; ii += bsize)
        {
            for (int k = 0; k < 17; k++)
            {
                block[k] = 0;
            }

            for (int y = 0; y < bsize; y++)
            {
                for (int x = 0; x < bsize; x++)
                {
                    ar[y][x] = (int)(Y[(jj + y) * width + ii + x] / 16);
                    block[ar[y][x]] += 1;
                }
            }

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

            for (int y = 0; y < bsize; y++)
            {
                for (int x = 0; x < bsize; x++)
                {
                    ar[y][x] -= minidx;
                    ar[y][x] > 7 ? (ar[y][x] = 7) : ar[y][x];
                    ar[y][x] < 0 ? (ar[y][x] = 0) : ar[y][x];
                }
            }
            
            if (diff <= 3) // 차이가 0, 1, 2인 경우 -> 유동형 비트방식으로 처리
            {
                fputs("1", fp);
                fputs(bitlist_4[minidx], fp); // DC Gain값 전송

                for (int y = 0; y < bsize; y++)
                {
                    for (int x = 0; x < bsize; x++)
                    {
                        fputs(unbitlist[ar[y][x]], fp);
                    }
                }
            }
            else // 4 이상인 경우 -> 고정형 비트 방식으로 처리
            {
                fputs("0", fp);
                fputs(bitlist_4[minidx], fp); // DC Gain값 전송

                for (int y = 0; y < bsize; y++)
                {
                    for (int x = 0; x < bsize; x++)
                    {
                        fputs(bitlist_3[ar[y][x]], fp);
                    }
                }
            }

            for (int y = 0; y < bsize; y++)
            {
                for (int x = 0; x < bsize; x++)
                {
                    Y[(jj + y) * width + ii + x] = (unsigned char)((minidx + ar[y][x]) * 16);
                }
            }
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

    char info[2];
    char gain[5];
    char buffer[4];
    char ch;
    int n, cnt;
    int bsize = 4;
    int gain_value;
    int ar[4][4];
    FILE* fp = fopen(address, "r");

    for (int jj = 0; jj <= height - bsize; jj += bsize)
    {
        for (int ii = 0; ii <= width - bsize; ii += bsize)
        {
            fgets(info, sizeof(info), fp);

            if (strcmp(info, "1") == 0)
            {
                fgets(gain, sizeof(gain), fp);
                gain_value = convert(gain);

                for (int j = 0; j < bsize; j++)
                {
                    for (int i = 0; i < bsize; i++)
                    {
                        cnt = 0;
                        while (1)
                        {
                            ch = fgetc(fp);
                            n = (int)ch - 48;
                            if (n == 0 || cnt >= 3) break;
                            cnt += 1;
                        }
                        ar[j][i] = gain_value + cnt;
                    }
                }
            }
            else
            {
                fgets(gain, sizeof(gain), fp);
                gain_value = convert(gain);

                for (int j = 0; j < bsize; j++)
                {
                    for (int i = 0; i < bsize; i++)
                    {
                        fgets(buffer, sizeof(buffer), fp);
                        ar[j][i] = convert(buffer) + gain_value;
                    }
                }
            }

            for (int j = 0; j < bsize; j++)
            {
                for (int i = 0; i < bsize; i++)
                {
                    Y[(jj + j) * width + ii + i] = (unsigned char)(ar[j][i] * 16);
                }
            }
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