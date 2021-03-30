#include "header.h"

void histogram(char* address)
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

            // Luminance 파일을 input으로 받았기 때문에 R, G, B 모두 Y값을 갖고 있음. 셋중 아무거나 골라서 Y배열에 넣음.
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
            Ylist[i] /= (value + 1);
        }
    }

    for (int i = 0; i < 512; i++)
    {
        printf("[%d : %d]\n", i, Ylist[i]);

        if (max < Ylist[i])
        {
            max = Ylist[i];
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


    FILE* outputFile = fopen("./image/Output_histogram.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);
}