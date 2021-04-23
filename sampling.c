#include "header.h"

void down_sampling_sub(char* address, char* output, int ratio)
{
    FILE* inputFile = NULL;
    inputFile = fopen(address, "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage; // size = width * height * 3
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
    
    // ****************
    
    int width2 = bmpInfo.biWidth >> ratio;
    int height2 = bmpInfo.biHeight >> ratio;
    int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
    int size2 = stride2 * height2; // width(=> stride) * height * 3

    // ****************

    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    // ****************

    unsigned char* Y1 = NULL;
    Y1 = (unsigned char*)calloc(size / 3, sizeof(unsigned char));
    
    unsigned char* Y2 = NULL;
    Y2 = (unsigned char*)calloc(size2 / 3, sizeof(unsigned char));

    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size2, sizeof(unsigned char));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
        }
    }

    for (int j = 0; j < height2; j++)
    {
        for (int i = 0; i < width2; i++)
        {
            Y2[j * width2 + i] = Y1[(j << ratio) * width + (i << ratio)];

            outputImg[j * stride2 + 3 * i + 0] = Y2[j * width2 + i];
            outputImg[j * stride2 + 3 * i + 1] = Y2[j * width2 + i];
            outputImg[j * stride2 + 3 * i + 2] = Y2[j * width2 + i];
        }
    }

    FILE* outputFile = fopen(output, "wb");
    bmpInfo.biWidth = width2;
    bmpInfo.biHeight = height2;
    bmpInfo.biSizeImage = size2;
    bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size2, outputFile);

    free(inputImg);
    fclose(inputFile);

    free(outputImg);
    fclose(outputFile);
    
    free(Y1);
    free(Y2);
}

void down_sampling_avg(char* address, char* output, int ratio)
{
    FILE* inputFile = NULL;
    inputFile = fopen(address, "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage; // size = width * height * 3
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    // ****************

    int width2 = bmpInfo.biWidth >> ratio;
    int height2 = bmpInfo.biHeight >> ratio;
    int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
    int size2 = stride2 * height2; // width(=> stride) * height * 3

    // ****************

    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    // ****************

    unsigned char* Y1 = NULL;
    Y1 = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    unsigned char* Y2 = NULL;
    Y2 = (unsigned char*)calloc(size2 / 3, sizeof(unsigned char));

    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size2, sizeof(unsigned char));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
        }
    }

    for (int j = 0; j < height2; j++)
    {
        for (int i = 0; i < width2; i++)
        {
            Y2[j * width2 + i] = (unsigned char)((double)(Y1[(j << ratio) * width + (i << ratio)] + Y1[(j << ratio) * width + (i << ratio) + 1] + Y1[((j << ratio) + 1) * width + (i << ratio) + 0] + Y1[((j << ratio) + 1) * width +  (i << ratio) + 1]) / 4);

            outputImg[j * stride2 + 3 * i + 0] = Y2[j * width2 + i];
            outputImg[j * stride2 + 3 * i + 1] = Y2[j * width2 + i];
            outputImg[j * stride2 + 3 * i + 2] = Y2[j * width2 + i];
        }
    }

    FILE* outputFile = fopen(output, "wb");
    bmpInfo.biWidth = width2;
    bmpInfo.biHeight = height2;
    bmpInfo.biSizeImage = size2;
    bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size2, outputFile);

    free(inputImg);
    fclose(inputFile);

    free(outputImg);
    fclose(outputFile);

    free(Y1);
    free(Y2);
}

void up_sampling_nearest(char* address, char* output, int ratio)
{
    FILE* inputFile = NULL;
    inputFile = fopen(address, "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage; // size = width * height * 3
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    // ****************

    int width2 = bmpInfo.biWidth << ratio;
    int height2 = bmpInfo.biHeight << ratio;
    int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
    int size2 = stride2 * height2; // width(=> stride) * height * 3

    // ****************

    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    // ****************

    unsigned char* Y1 = NULL;
    Y1 = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    unsigned char* Y2 = NULL;
    Y2 = (unsigned char*)calloc(size2 / 3, sizeof(unsigned char));

    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size2, sizeof(unsigned char));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
        }
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            for (int n = 0; n < 1 << ratio; n++)
            {
                for (int m = 0; m < 1 << ratio; m++)
                {
                    Y2[((j << ratio) + n) * width2 + (i << ratio) + m] = Y1[j * width + i];
                }
            }
        }
    }

    for (int j = 0; j < height2; j++)
    {
        for (int i = 0; i < width2; i++)
        {
            outputImg[j * stride2 + 3 * i + 0] = Y2[j * width2 + i];
            outputImg[j * stride2 + 3 * i + 1] = Y2[j * width2 + i];
            outputImg[j * stride2 + 3 * i + 2] = Y2[j * width2 + i];
        }
    }

    FILE* outputFile = fopen(output, "wb");
    bmpInfo.biWidth = width2;
    bmpInfo.biHeight = height2;
    bmpInfo.biSizeImage = size2;
    bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size2, outputFile);

    free(inputImg);
    fclose(inputFile);

    free(outputImg);
    fclose(outputFile);

    free(Y1);
    free(Y2);
}

void up_sampling_biint(char* address, char* output, int ratio)
{
    FILE* inputFile = NULL;
    inputFile = fopen(address, "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage; // size = width * height * 3
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    // ****************

    int width2 = bmpInfo.biWidth << ratio;
    int height2 = bmpInfo.biHeight << ratio;
    int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
    int size2 = stride2 * height2; // width(=> stride) * height * 3

    // ****************

    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    // ****************

    unsigned char* Y1 = NULL;
    Y1 = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    unsigned char* Y2 = NULL;
    Y2 = (unsigned char*)calloc(size2 / 3, sizeof(unsigned char));

    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size2, sizeof(unsigned char));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
        }
    }    

    int si, sj;
    
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            sj = j << ratio;
            si = i << ratio;

            if (i == 255)
            {
                Y2[sj * width2 + si + 0] = Y1[j * width + i];
                Y2[sj * width2 + si + 1] = Y1[j * width + i];
            }
            else
            {
                Y2[sj * width2 + si + 0] = Y1[j * width + i];
                Y2[sj * width2 + si + 1] = (unsigned char)((Y1[j * width + i] + Y1[j * width + i + 1]) / 2);
            }

        }
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            sj = j << ratio;
            si = i << ratio;

            if (j == 255)
            {
                Y2[(sj + 1) * width2 + si + 0] = Y2[sj * width2 + si + 0];
                Y2[(sj + 1) * width2 + si + 1] = Y2[sj * width2 + si + 1];
            }
            else
            {
                Y2[(sj + 1) * width2 + si + 0] = (unsigned char)((Y2[sj * width2 + si + 0] + Y2[(sj + 2) * width2 + si + 0]) / 2);
                Y2[(sj + 1) * width2 + si + 1] = (unsigned char)((Y2[sj * width2 + si + 1] + Y2[(sj + 2) * width2 + si + 1]) / 2);
            }
        }
    }
    
    for (int j = 0; j < height2; j++)
    {
        for (int i = 0; i < width2; i++)
        {
            outputImg[j * stride2 + 3 * i + 0] = Y2[j * width2 + i];
            outputImg[j * stride2 + 3 * i + 1] = Y2[j * width2 + i];
            outputImg[j * stride2 + 3 * i + 2] = Y2[j * width2 + i];
        }
    }

    FILE* outputFile = fopen(output, "wb");
    bmpInfo.biWidth = width2;
    bmpInfo.biHeight = height2;
    bmpInfo.biSizeImage = size2;
    bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size2, outputFile);

    free(inputImg);
    fclose(inputFile);

    free(outputImg);
    fclose(outputFile);

    free(Y1);
    free(Y2);
}

void midterm_test(char* address, char* output, int ratio, int psize)
{
    FILE* inputFile = NULL;
    inputFile = fopen(address, "rb");

    // 128x128 file 
    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth; 
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage; // size = width * height * 3
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    // ****************

    // 512x512 file
    int width2 = bmpInfo.biWidth << ratio;
    int height2 = bmpInfo.biHeight << ratio;
    int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
    int size2 = stride2 * height2; // width(=> stride) * height * 3

    // ****************

    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    // ****************

    unsigned char* Y1 = NULL;
    Y1 = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    unsigned char* Y2 = NULL;
    Y2 = (unsigned char*)calloc(size2 / 3, sizeof(unsigned char));

    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size2, sizeof(unsigned char));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
        }
    }

    // *****************************************************************************************************************************
    //
    //                                                   Y1 패딩 (n-tap filter적용)
    //
    // *****************************************************************************************************************************

    // padding 작업을 위한 변수
    int pheight = height + (2 * psize); // 패딩을 두르고 난 이후의 총 세로길이 (양옆으로 패딩이 붙기때문에 psize * 2)
    int pwidth = width + (2 * psize); // 패딩을 두르고 난 이후의 총 가로길이

    // 패딩 두른 이미지 파일을 담을 padding
    unsigned char* padding = NULL;
    // pheight * pwidth 칸 배열을 만듭니다
    padding = (unsigned char*)calloc(pheight * pwidth, sizeof(unsigned char));

    // padding 위아래 채우기
    for (int n = 0; n < psize; n++)
    {
        for (int i = 0; i < width; i++)
        {
            // 아래쪽
            padding[(n * pwidth) + i + psize] = Y1[i];
            // 위쪽
            padding[(pheight - n - 1) * pwidth + i + psize] = Y1[(height - 1) * width + i];
        }
    }

    // padding 양옆 채우기
    for (int n = 0; n < psize; n++)
    {
        for (int j = 0; j < height; j++)
        {
            // 왼쪽
            padding[(j + psize) * pwidth + n] = Y1[j * width];
            // 오른쪽
            padding[(j + psize) * pwidth + (pwidth - 1 - n)] = Y1[j * width + (width - 1)];
        }
    }

    // 이외 나머지 부분에 Y값 그대로 복사
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            padding[(j + psize) * pwidth + (i + psize)] = Y1[j * width + i];
        }
    }

    // *****************************************************************************************************************************
    //
    //                                                        업스케일링
    //
    // *****************************************************************************************************************************

    double ntapfilterh[3] = { 0.3333, 0.3333, 0.3333 };
    double ntapfilter[4] = { -0.0625, 0.5625, 0.5625, -0.0625 };

    int fsize = 4;
    double value = 0;
    double value2 = 0;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int pj = psize + j;
            int pi = psize + i;

            // 가로축 4탭필터
            value = 0;
            value2 = 0;
            for (int k = 0; k < fsize; k++)
            {
                value += (double)(padding[pj * pwidth + (pi - 1 + k)]) * ntapfilter[k];
            }
            for (int k = 0; k < 3; k++)
            {
                value2 += (double)(padding[(pj - 1 + k) * pwidth + pi + 1]) * ntapfilterh[k];
                value2 += (double)(padding[(pj - 1 + k) * pwidth + pi + 0]) * ntapfilterh[k];
            }
            value = (value + (value2 / 2)) / 2;
            value > 255 ? (value = 255) : value;

            // 4-tap interpolation
            Y2[(j << ratio) * width2 + (i << ratio)] = padding[pj * pwidth + pi];
            Y2[(j << ratio) * width2 + (i << ratio) + 2] = (unsigned char)value;

            // 세로축 4탭필터
            value = 0;
            value2 = 0;
            for (int k = 0; k < fsize; k++)
            {
                value += (double)(padding[(pj - 1 + k) * pwidth + pi]) * ntapfilter[k];
            }
            for (int k = 0; k < 3; k++)
            {
                value2 += (double)(padding[pj * pwidth + (pi - 1 + k)]) * ntapfilterh[k];
                value2 += (double)(padding[(pj + 1) * pwidth + (pi - 1 + k)]) * ntapfilterh[k];
            }
            value = (value + (value2 / 2)) / 2;
            value > 255 ? (value = 255) : value;
            Y2[((j << ratio) + 2) * width2 + (i << ratio)] = (unsigned char)value;

        }
    }

    double a, b, c, d;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            if (j >= height - 1) // 마지막줄 처리
            {
                Y2[((j << ratio) + 2) * width2 + (i << ratio) + 2] = Y2[((j << ratio) + 0) * width2 + (i << ratio) + 2];
                continue;
            }
            if (i >= width - 1) // 끝부분 처리
            {
                Y2[((j << ratio) + 2) * width2 + (i << ratio) + 2] = Y2[((j << ratio) + 2) * width2 + (i << ratio) + 0];
                continue;
            }
            a  = (double)Y2[((j << ratio) + 0) * width2 + (i << ratio) + 0];
            b  = (double)Y2[((j << ratio) + 0) * width2 + (i << ratio) + 4];
            c  = (double)Y2[((j << ratio) + 4) * width2 + (i << ratio) + 0];
            d  = (double)Y2[((j << ratio) + 4) * width2 + (i << ratio) + 4];

            value = (a + b + c + d) / 4;
            value > 255 ? (value = 255) : value;

            Y2[((j << ratio) + 2) * width2 + (i << ratio) + 2] = (unsigned char)value;
        }
    }

    // 가로축 binary filter
    for (int j = 0; j < height2; j++)
    {
        for (int i = 0; i < width2 - 3; i+=2)
        {
            Y2[j * width2 + i + 1] = (Y2[j * width2 + i] + Y2[j * width2 + i + 2]) / 2;
        }
        Y2[j * width2 + (width2 - 1)] = Y2[j * width2 + (width2 - 2)]; // 끝부분 처리
    }
    
    // 세로축 binary filter
    for (int j = 0; j < height2; j+=2)
    {
        for (int i = 0; i < width2; i++)
        {
            if (j > height2 - 3) // 마지막줄 처리
            {
                Y2[(j + 1) * width2 + i] = (unsigned char)(Y2[j * width2 + i] * 0.7 + Y2[(j - 1) * width2 + i] * 0.3);
            }
            else
            {
                Y2[(j + 1) * width2 + i] = (unsigned char)((Y2[j * width2 + i] + Y2[(j + 2) * width2 + i]) / 2);
            }
        }
    }

    for (int j = 0; j < height2; j++)
    {
        for (int i = 0; i < width2; i++)
        {
            outputImg[j * stride2 + 3 * i + 0] = Y2[j * width2 + i];
            outputImg[j * stride2 + 3 * i + 1] = Y2[j * width2 + i];
            outputImg[j * stride2 + 3 * i + 2] = Y2[j * width2 + i];
        }
    }

    FILE* outputFile = fopen(output, "wb");
    bmpInfo.biWidth = width2;
    bmpInfo.biHeight = height2;
    bmpInfo.biSizeImage = size2;
    bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size2, outputFile);

    free(inputImg);
    fclose(inputFile);

    free(outputImg);
    fclose(outputFile);

    free(Y1);
    free(Y2);
    free(padding);
}