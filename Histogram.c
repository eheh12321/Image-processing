#include "header.h"

// x축을 512로 stretch
void histogram(char* address, char* output)
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

    int Ylist[513] = { 0 }, value, max = 0;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            outputImg[j * stride + 3 * i + 0] = (unsigned char)255;
            outputImg[j * stride + 3 * i + 1] = (unsigned char)255;
            outputImg[j * stride + 3 * i + 2] = (unsigned char)255;

            value = (int)inputImg[j * stride + 3 * i + 0];

            value *= 2;

            Ylist[value] += 1;
            Ylist[value + 1] += 1;

            max < Ylist[value] ? (max = Ylist[value]) : (max);
        }
    }

    if (max >= 512)
    {
        value = max / 512;

        for (int i = 0; i < 512; i++)
        {
            Ylist[i] /= (value * 3);
        }
    }


    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < Ylist[i]; j++)
        {
            outputImg[j * stride + 3 * i + 0] = (unsigned char)0;
            outputImg[j * stride + 3 * i + 1] = (unsigned char)0;
            outputImg[j * stride + 3 * i + 2] = (unsigned char)0;
        }
    }

    FILE* outputFile = fopen(output, "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);
}

// x축 stretch 안함
void ns_histogram(char* address, char* output)
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

    int Ylist[256] = { 0 }, value, max = 0;
    int len = sizeof(Ylist) / sizeof(Ylist[0]);

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            outputImg[j * stride + 3 * i + 0] = (unsigned char)255;
            outputImg[j * stride + 3 * i + 1] = (unsigned char)255;
            outputImg[j * stride + 3 * i + 2] = (unsigned char)255;

            value = (int)inputImg[j * stride + 3 * i + 0];

            Ylist[value] += 1;

            max < Ylist[value] ? (max = Ylist[value]) : (max);
        }
    }

    if (max >= 512)
    {
        value = max / 512;

        for (int i = 0; i < len; i++)
        {
            Ylist[i] /= (value * 3);
        }
    }

    // ****************** histogram sliding **************
    // 대단해 보이는데 사실 그냥 Ylist 요소의 값을 각각 밀어주기만 한 것 ==> 각 셀의 Y에 k값 더하기만 한것;;; :P
    int k = 200;

    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < Ylist[i]; j++)
        {
            outputImg[j * stride + 3 * (i + k) + 0] = (unsigned char)0;
            outputImg[j * stride + 3 * (i + k) + 1] = (unsigned char)0;
            outputImg[j * stride + 3 * (i + k) + 2] = (unsigned char)0;
        }
    }

    FILE* outputFile = fopen(output, "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);
}

void threshold(char* address)
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

    unsigned char value;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            value = inputImg[j * stride + 3 * i + 0];

            // *****************************************************
            //                      Threshold
            //
            //  value값을 조절해서 보여주고싶은 부분만 추출..(이외 0)
            // *****************************************************

            if (value > 100)
            {
                outputImg[j * stride + 3 * i + 0] = value;
                outputImg[j * stride + 3 * i + 1] = value;
                outputImg[j * stride + 3 * i + 2] = value;
            }
        }
    }

    FILE* outputFile = fopen("./image/Output_Threshold.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);
}

void stretch_histogram(char* address)
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

    unsigned char* Y1 = NULL;
    Y1 = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    int Hist[256] = { 0 };
    int sumHist[256] = { 0 };

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
            Hist[Y1[j * width + i]] += 1;
        }
    }

    // Cumulative Histogram (누적 히스토그램)
    sumHist[0] = Hist[0];
    for (int i = 1; i < 255; i++)
    {
        sumHist[i] = sumHist[i - 1] + Hist[i];
    }
    
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            outputImg[j * stride + 3 * i + 0] = (unsigned char)255 * sumHist[Y1[j * width + i]] / (width * height);
            outputImg[j * stride + 3 * i + 1] = (unsigned char)255 * sumHist[Y1[j * width + i]] / (width * height);
            outputImg[j * stride + 3 * i + 2] = (unsigned char)255 * sumHist[Y1[j * width + i]] / (width * height);
        }
    }
   
    FILE* outputFile = fopen("./image/Output_Stretched_histogram.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);
}