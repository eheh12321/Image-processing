#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

// �̹����� File Header�� �������� bmpFile
BITMAPFILEHEADER bmpFile;
// �̹����� Image Header�� �������� bmpInfo
BITMAPINFOHEADER bmpInfo;

// median filter���� �߰����� ã�� ���� qsort�� �� �� �̿��� �Լ�
int compare(const void* first, const void* second)
{
    // int������ ��ȯ�Ͽ� ��
    int* a = (int*)first;
    // int������ ��ȯ�Ͽ� ��
    int* b = (int*)second;

    // �� ���� ũ�⸦ ���ؼ� -1, (0), 1 �� �����մϴ�. (�������� ����)
    if (*a < *b)
        return -1;
    else
        return 1;
}

// median_filter(�߰��� ����)�� salt & pepper noise ����. (address ������ �о result ������ ����մϴ�) >> main���� �����ؼ� �� �� �ְ� ��
void median_filter(char* address, char* result)
{
    // *****************************************************************************************************************************
    //
    //                                                    �̹��� �а� �����۾� �ϴ� �κ�
    //
    // *****************************************************************************************************************************

   // ����� ������ ������ ���� inputFile ������ ����
    FILE* inputFile = NULL;
    // �̹��� ������ �б���� ���ϴ�
    inputFile = fopen(address, "rb");

    // ������ ������ �о�鿩�ɴϴ�.
    // unsigned int fread(void *buf, unsigned int size, unsigned int count, File *fp)
    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    // �̹��� ������ ������
    int width = bmpInfo.biWidth; // ���� ����
    int height = bmpInfo.biHeight; // ���� ����
    int size = bmpInfo.biSizeImage; // height * width * 3 (R,G,B)
    int bitCnt = bmpInfo.biBitCount; // 8bit
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4; // (width * 3) >> �� �ȼ��� R,G,B 3�� ���� �ֱ� ���� 3��� �ø� 

    // inputFile�� �ȼ� ���� ��Ƶ� inputImg  
    unsigned char* inputImg = NULL;
    // inputImg �����Ҵ�
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    // inputImg�� inputFile�� ����ִ� �ȼ� �����͵��� ����ϴ�
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    // ����� ������ ���� ����� outputImg
    unsigned char* outputImg = NULL;
    // outputImg �����Ҵ�
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

    // YCbCr�� Y���� �����ؼ� ���� Y
    unsigned char* Y = NULL;
    // Y �迭�� R,G,B �� �ƴ϶� Y �ϳ��� ����ϱ� ������ size / 3
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    // Y �迭���� ���� �ִ� ����.
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            // Luminance ������ input���� �޾ұ� ������ R, G, B ��� Y���� ���� ����. ���� �ƹ��ų� ��� Y�迭�� �ֽ��ϴ�.
            Y[j * width + i] = inputImg[j * stride + 3 * i + 0];
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
        padding[i + psize] = Y[i];
        // ����
        padding[((pheight - 1) * pwidth) + (i + psize)] = Y[(height - 1) * width + i];
    }

    // padding �翷 ä���
    for (int j = 0; j < height; j++)
    {
        // ����
        padding[(j + psize) * pwidth] = Y[j * width];
        // ������
        padding[(j + psize) * pwidth + (pwidth - 1)] = Y[j * width + (width - 1)];
    }

    // �̿� ������ �κп� Y�� �״�� ����
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            padding[(j + psize) * pwidth + (i + psize)] = Y[j * width + i];
        }
    }

    // *****************************************************************************************************************************
    //
    //                                                        Median Filter 
    //
    // *****************************************************************************************************************************

    // median filter�� ���� ����. 
    int filter[9]; // 3x3 ������ = 9 (qsort ������ ���� �ϱ����� ������ �迭 ���)
    int fsize = 3; // 3x3 => 3
    int fmid = (fsize * fsize) / 2; // �߾Ӱ�

    // ���Ͱ� �е��� ������ �Ѿ�� ������ ���� (pheight - fsize, pwidth - fsize)
    for (int j = 0; j <= (pheight - fsize); j++)
    {
        for (int i = 0; i <= (pwidth - fsize); i++)
        {
            // "Salt & Pepper noise" �� �����ϱ� ���� ��� ���� 0 Ȥ�� 255�� ��쿡�� �����մϴ�.
            if (padding[(j + 1) * pwidth + (i + 1)] == 0 || padding[(j + 1) * pwidth + (i + 1)] == 255)
            {
                // 3x3 ���Ϳ� ���� ä��ϴ�.
                for (int y = 0, cnt = 0; y < fsize; y++)
                {
                    for (int x = 0; x < fsize; x++)
                    {
                        // filter �迭�� int���̱� ������ ����ȯ�� ���ݴϴ�
                        filter[cnt++] = (int)padding[(j + y) * pwidth + (i + x)];
                    }
                }
                // qsort�� filter ����
                qsort(filter, fsize * fsize, sizeof(filter[0]), compare);

                // ������ �迭�� �߾Ӱ��� �ֽ��ϴ�. padding �迭�� unsigned char���̰� filter�� int���̱� ������ ����ȯ ���ݴϴ�
                padding[(j + 1) * pwidth + (i + 1)] = (unsigned char)filter[fmid];
            }
        }
    }

    // filtering�� ��� ������ �е��� ������ �κ��� �ٽ� Y�� �ٿ��ֽ��ϴ�.
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y[j * width + i] = padding[(j + psize) * pwidth + (i + psize)];
        }
    }
    
    // ����� ���� outputImg�� R,G,B �ȼ��� Y���� ���� ä���ֽ��ϴ�.
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            outputImg[j * stride + i * 3 + 0] = Y[j * width + i]; // B
            outputImg[j * stride + i * 3 + 1] = Y[j * width + i]; // G
            outputImg[j * stride + i * 3 + 2] = Y[j * width + i]; // R
        }
    }

    // ��� ������ ����ϱ� ���� ����
    FILE* outputFile = fopen(result, "wb");
    // �̹��� ���� ��� �����͸� ���ϴ�
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    // �̹��� ���� ��� �����͸� ���ϴ�
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    // �ȼ����� ���ϴ�
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    // �����Ҵ� ���� �� ���� �ݱ�
    free(outputImg);
    fclose(outputFile);
    // �����Ҵ� ���� �� ���� �ݱ�
    free(inputImg);
    fclose(inputFile);
    // �����Ҵ� ���� �� ���� �ݱ�
    free(Y);
    free(padding);

}

// mean_filter(��հ� ����)�� ���������� �̹����� �帮�� ����� noise ���� (address ������ �о result ������ ����մϴ�) >> main���� �����ؼ� �� �� �ְ� ��
void mean_filter(char* address, char* result)
{
    // *****************************************************************************************************************************
    //
    //                                  �̹��� �а� �����۾� + padding �ϴ� �κ� (�� Median ���Ϳ� �����մϴ�)
    //
    // *****************************************************************************************************************************

    // median_filter�� �ѹ� ���� �� �̹��� ������ �ҷ��ɴϴ�
    FILE* inputFile = NULL;
    // �̹��� ������ �б���� ���ϴ�
    inputFile = fopen(address, "rb");

    // ������ ������ �о�鿩�ɴϴ�.
    // unsigned int fread(void *buf, unsigned int size, unsigned int count, File *fp)
    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    // �̹��� ������ ������
    int width = bmpInfo.biWidth; // ���� ����
    int height = bmpInfo.biHeight; // ���� ����
    int size = bmpInfo.biSizeImage; // height * width * 3 (R,G,B)
    int bitCnt = bmpInfo.biBitCount; // 8bit
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4; // (width * 3) >> �� �ȼ��� R,G,B 3�� ���� �ֱ� ���� 3��� �ø� 

    // inputFile�� �ȼ� ���� ��Ƶ� inputImg  
    unsigned char* inputImg = NULL;
    // inputImg �����Ҵ�
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    // inputImg�� inputFile�� ����ִ� �ȼ� �����͵��� ����ϴ�
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    // ����� ������ ���� ����� outputImg
    unsigned char* outputImg = NULL;
    // outputImg �����Ҵ�
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

    // YCbCr�� Y���� �����ؼ� ���� Y
    unsigned char* Y = NULL;
    // Y �迭�� R,G,B �� �ƴ϶� Y �ϳ��� ����ϱ� ������ size / 3
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    // Y �迭���� ���� �ִ� ����.
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            // Luminance ������ input���� �޾ұ� ������ R, G, B ��� Y���� ���� ����. ���� �ƹ��ų� ��� Y�迭�� �ֽ��ϴ�.
            Y[j * width + i] = inputImg[j * stride + 3 * i + 0];
        }
    }

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
        padding[i + psize] = Y[i];
        // ����
        padding[((pheight - 1) * pwidth) + (i + psize)] = Y[(height - 1) * width + i];
    }

    // padding �翷 ä���
    for (int j = 0; j < height; j++)
    {
        // ����
        padding[(j + psize) * pwidth] = Y[j * width];
        // ������
        padding[(j + psize) * pwidth + (pwidth - 1)] = Y[j * width + (width - 1)];
    }

    // �̿� ������ �κп� Y�� �״�� ����
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            padding[(j + psize) * pwidth + (i + psize)] = Y[j * width + i];
        }
    }

    // *****************************************************************************************************************************
    //
    //                                                        Mean Filter 
    //
    // *****************************************************************************************************************************

    // 3x3 ����
    int fsize = 3;
    // ��հ��� ���ϱ� ���� fsum
    int fsum = 0;

    // ���Ͱ� �е��� ������ �Ѿ�� ������ ���� (pheight - fsize, pwidth - fsize)
    for (int j = 0; j <= (pheight - fsize); j++)
    {
        for (int i = 0; i <= (pwidth - fsize); i++)
        {
            // ��հ� ���ϱ�
            fsum = 0;
            // 3x3 ũ�⸦ ���鼭 ��� Y���� fsum ������ ���մϴ�
            for (int y = 0; y < fsize; y++)
            {
                for (int x = 0; x < fsize; x++)
                {
                    // fsum�� int���̰� padding�� unsigned char���̱� ������ ����ȯ �մϴ�
                    fsum += (int)padding[(j + y) * pwidth + (i + x)];
                }
            }
            // ��հ��� ���մϴ�
            fsum /= (fsize * fsize);

            // ��հ��� �� ��ȯ ���� ��� ä���ֽ��ϴ�.
            padding[(j + 1) * pwidth + (i + 1)] = (unsigned char)fsum;
        }
    }

    // filtering�� ��� ������ �е��� ������ �κ��� �ٽ� Y�� �ٿ��ֽ��ϴ�.
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y[j * width + i] = padding[(j + psize) * pwidth + (i + psize)];
        }
    }

    // ����� ���� outputImg�� R,G,B �ȼ��� Y���� ���� ä���ֽ��ϴ�.
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            outputImg[j * stride + i * 3 + 0] = Y[j * width + i]; // B
            outputImg[j * stride + i * 3 + 1] = Y[j * width + i]; // G
            outputImg[j * stride + i * 3 + 2] = Y[j * width + i]; // R
        }
    }

    // ��� ������ ����ϱ� ���� ����
    FILE* outputFile = fopen(result, "wb");
    // �̹��� ���� ��� �����͸� ���ϴ�
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    // �̹��� ���� ��� �����͸� ���ϴ�
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    // �ȼ����� ���ϴ�
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    // �����Ҵ� ���� �� ���� �ݱ�
    free(outputImg);
    fclose(outputFile);
    // �����Ҵ� ���� �� ���� �ݱ�
    free(inputImg);
    fclose(inputFile);
    // �����Ҵ� ���� �� ���� �ݱ�
    free(Y);
    free(padding);
}


int main()
{
    // 1. Median filter�� salt & papper noise�� �����Ѵ�
    median_filter("./AICenterY_CombinedNoise.bmp", "./17011753_�̵���.bmp");
    // 2. �� ������ mean filter�� ������ noise�� �ѹ� �� �����Ѵ�
    mean_filter("./17011753_�̵���.bmp", "./17011753_�̵���.bmp");
    
    // print.
    printf("Processing Complete!\n");

    // �Լ� ����
    return 0;
}