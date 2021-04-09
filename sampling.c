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

void up_sampling_ntap(char* address, char* output, int ratio)
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

    // *****************************************************************************************************************************
    //
    //                                  �׵θ����� ���͸� �۾��� �����ϱ� ���� Padding�� ����� �κ� 
    //
    // *****************************************************************************************************************************

      // padding �۾��� ���� ����
    int psize = 1; // �е��� ���� (��ĭ)
    int pheight = height + (2 * psize); // �е��� �θ��� �� ������ �� ���α��� (�翷���� �е��� �ٱ⶧���� psize * 2)
    int pwidth = width + (2 * psize); // �е��� �θ��� �� ������ �� ���α���

    // �е� �θ� �̹��� ������ ���� padding
    unsigned char* padding = NULL;
    // pheight * pwidth ĭ �迭�� ����ϴ�
    padding = (unsigned char*)calloc(pheight * pwidth, sizeof(unsigned char));


    // padding�� ���� �� �ٷ� �տ� �ִ� �ȼ��� Y���� �״�� ������ ä���ֽ��ϴ�.
    // ���� �׵θ� �κп��� ���͸��� �� �� ���ݴ� ������ ������ �� �ֽ��ϴ�
    // padding ���Ʒ� ä���
    for (int i = 0; i < width; i++)
    {
        // �Ʒ���
        padding[i + psize] = Y1[i];
        // ����
        padding[((pheight - 1) * pwidth) + (i + psize)] = Y1[(height - 1) * width + i];
    }

    // padding �翷 ä���
    for (int j = 0; j < height; j++)
    {
        // ����
        padding[(j + psize) * pwidth] = Y1[j * width];
        // ������
        padding[(j + psize) * pwidth + (pwidth - 1)] = Y1[j * width + (width - 1)];
    }

    // �̿� ������ �κп� Y�� �״�� ����
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            padding[(j + psize) * pwidth + (i + psize)] = Y1[j * width + i];
        }
    }

    // *******************************************************************************

    int si, sj, pi, pj;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            si = i << ratio;
            sj = j << ratio;
            pi = psize + i;
            pj = psize + j;

            Y2[sj * width2 + si + 0] = padding[pj * pwidth + pi];
            //Y2[sj * width2 + si + 1] = padding[pj * pwidth + pi];
            Y2[sj * width2 + si + 1] = (unsigned char)((padding[pj * pwidth + pi] + padding[pj * pwidth + pi + 1]) / 2);
        }
    }

    for (int j = 1; j < height2 - 2; j += 2)
    {
        for (int i = 0; i < width2; i++)
        {
            //Y2[j * width2 + i] = Y2[(j - 1) * width2 + i];
            Y2[j * width2 + i] = (unsigned char)((Y2[(j - 1) * width2 + i] + Y2[(j + 1) * width2 + i]) / 2);
        }
    }

    for (int i = 0; i < width2; i++)
    {
        Y2[(height2 - 1) * width2 + i] = Y2[(height2 - 2) * width2 + i];
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

    free(padding);

    free(inputImg);
    fclose(inputFile);

    free(outputImg);
    fclose(outputFile);

    free(Y1);
    free(Y2);
}
