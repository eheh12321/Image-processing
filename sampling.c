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

void upsampling_midterm(char* address, char* output, int ratio, int psize)
{
    FILE* inputFile = NULL; // ���� ������ ���� ������ ����
    inputFile = fopen(address, "rb"); // ���� ������ �Ķ���ͷ� �޾ƿͼ� ���ϴ�.

    // #### 128x128 file (����)
    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile); // ���������� fileHeader�� ������ bmpFile�� ����
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile); // ���������� infoHeader�� ������ bmpInfo�� ����

    int width = bmpInfo.biWidth; // ���� ����
    int height = bmpInfo.biHeight; // ���� ����
    int size = bmpInfo.biSizeImage; // size = width * height * 3
    int bitCnt = bmpInfo.biBitCount; // 8bit ����
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4; // width�� 4�� ����� �������� ����

    // #### 512x512 file (upsampling)
    int width2 = bmpInfo.biWidth << ratio; // ����Ʈ ������ �̿��Ͽ����ϴ�. << �� 2�� ���ϴ� ȿ���� �ֽ��ϴ�.
    int height2 = bmpInfo.biHeight << ratio; // ratio=2�̹Ƿ� 4�� ���ѰͰ� ����
    int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4; // width2�� 4�� ����� �������� ����
    int size2 = stride2 * height2; // stride2(����) * height2(����) = size

    // ���� ������ ���� inputImg ������ ����
    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char)); // ������ 128x128 ����
    fread(inputImg, sizeof(unsigned char), size, inputFile); // ������ �о���Դϴ�.

    // inputImg���� Y���� �����ؼ� ��Ƴ� Y1 ������ ����
    unsigned char* Y1 = NULL;
    Y1 = (unsigned char*)calloc(size / 3, sizeof(unsigned char)); // R,G,B �� �ϳ��� �̾ƿ��� �ǹǷ� size / 3 �� ũ�⸸ �ʿ�

    // Y1���� upsampling�� ���� ���� Y2 ������ ���� 
    unsigned char* Y2 = NULL;
    Y2 = (unsigned char*)calloc(size2 / 3, sizeof(unsigned char)); // R,G,B �� �ϳ��� �̾ƿ��� �ǹǷ� size2 / 3 �� ũ�⸸ �ʿ�

    // ��� ������ ���� outputImg ������ ����
    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size2, sizeof(unsigned char)); // 512x512 ����

    // inputImg���� Y���� �̾� Y1 �迭�� �����ϴ� ����
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y1[j * width + i] = inputImg[j * stride + 3 * i + 0]; // R G B �� �ϳ��� �̾Ƽ� �����մϴ�.
        }
    }

    // *****************************************************************************************************************************
    //
    //                                               Y1 �迭�� �е��� ����� ����
    //
    // *****************************************************************************************************************************

    // padding �۾��� ���� ����
    int pheight = height + (2 * psize); // �е��� �θ��� �� ������ �� ���α��� (�翷���� �е��� �ٱ⶧���� psize * 2)
    int pwidth = width + (2 * psize); // �е��� �θ��� �� ������ �� ���α���

    // �е� �θ� �̹��� ������ ���� padding
    unsigned char* padding = NULL;
    // pheight * pwidth ĭ �迭�� ����ϴ�
    padding = (unsigned char*)calloc(pheight * pwidth, sizeof(unsigned char));

    // padding ���Ʒ� ä���
    for (int n = 0; n < psize; n++)
    {
        for (int i = 0; i < width; i++)
        {
            // �Ʒ���
            padding[(n * pwidth) + i + psize] = Y1[i];
            // ����
            padding[(pheight - n - 1) * pwidth + i + psize] = Y1[(height - 1) * width + i];
        }
    }

    // padding �翷 ä���
    for (int n = 0; n < psize; n++)
    {
        for (int j = 0; j < height; j++)
        {
            // ����
            padding[(j + psize) * pwidth + n] = Y1[j * width];
            // ������
            padding[(j + psize) * pwidth + (pwidth - 1 - n)] = Y1[j * width + (width - 1)];
        }
    }

    // �̿� ������ �κп� Y�� �״�� ����
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            padding[(j + psize) * pwidth + (i + psize)] = Y1[j * width + i];
        }
    }

    // *****************************************************************************************************************************
    //
    //                                                         Upsampling
    //
    // *****************************************************************************************************************************

    double ntapfilter[4] = { -0.0625, 0.5625, 0.5625, -0.0625 }; // �⺻ 4-tap filter
    double ntapfilterh[3] = { 0.3333, 0.3333, 0.3333 }; // ������� ��� ����ϱ� ���ؼ� �߰��� ����. ����ġ�� �����ϴ�.

    int fsize = 4; // ���� ����
    double value = 0; // ntapfilter ����
    double value2 = 0; // ntapfilterh ���� 

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int pj = psize + j; // �е��� ����� �� ���� j ��ġ
            int pi = psize + i; // �е��� ����� �� ���� i ��ġ

            // ******* ���ι��� 4-tap filter
            value = 0; // ���� �ʱ�ȭ
            value2 = 0; // ���� �ʱ�ȭ

            for (int k = 0; k < fsize; k++) // 4-tap filter ������
            {
                value += (double)(padding[pj * pwidth + (pi - 1 + k)]) * ntapfilter[k];
            }
            for (int k = 0; k < 3; k++) // ���ι����� ���� �߰��� ����ϴ� ����
            {
                value2 += (double)(padding[(pj - 1 + k) * pwidth + pi + 1]) * ntapfilterh[k];
                value2 += (double)(padding[(pj - 1 + k) * pwidth + pi + 0]) * ntapfilterh[k];
            }
            value = (value + (value2 / 2)) / 2; // value�� value2�� ���� ����� ���� interpolation�մϴ�.
            value > 255 ? (value = 255) : value; // 255�� ������ 255�� ó��

            Y2[(j << ratio) * width2 + (i << ratio)] = padding[pj * pwidth + pi]; // (i, j) ��ġ���� �������� �״�� �ֽ��ϴ�
            Y2[(j << ratio) * width2 + (i << ratio) + 2] = (unsigned char)value; // (i, j+2) ��ġ���� ������ ����� value ���� �ֽ��ϴ�.

            // ******* ���ι��� 4-tap filter
            value = 0; // ���� �ʱ�ȭ
            value2 = 0; // ���� �ʱ�ȭ

            for (int k = 0; k < fsize; k++) // 4-tap filter ������
            {
                value += (double)(padding[(pj - 1 + k) * pwidth + pi]) * ntapfilter[k];
            }
            for (int k = 0; k < 3; k++) // ���ι����� ���� �߰��� ����ϴ� ����
            {
                value2 += (double)(padding[pj * pwidth + (pi - 1 + k)]) * ntapfilterh[k];
                value2 += (double)(padding[(pj + 1) * pwidth + (pi - 1 + k)]) * ntapfilterh[k];
            }
            value = (value + (value2 / 2)) / 2; // value�� value2�� ���� ����� ���� interpolation�մϴ�.
            value > 255 ? (value = 255) : value; // 255�� ������ 255�� ó��

            Y2[((j << ratio) + 2) * width2 + (i << ratio)] = (unsigned char)value; // (i+2, j) ��ġ�� ������ ����� value ���� �ֽ��ϴ�.
            // (i+2, j+2)�� �Ʒ����� ���� ����մϴ�
        }
    }

    // ����ִ� (i+2, j+2) ����ϴ� ����
    double a, b, c, d, e, f, g, h;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            if (j >= height - 1) // ���ι��� �������� ����ó��
            {
                Y2[((j << ratio) + 2) * width2 + (i << ratio) + 2] = Y2[((j << ratio) + 0) * width2 + (i << ratio) + 2]; // (i+2, j)�� �ִ� ���� (i+2, j+2)�� �ܼ��� �����մϴ�.
                continue;
            }
            if (i >= width - 1) // ���ι��� ���κ� ����ó��
            {
                Y2[((j << ratio) + 2) * width2 + (i << ratio) + 2] = Y2[((j << ratio) + 2) * width2 + (i << ratio) + 0]; // (i, j+2)�� �ִ� ���� (i+2, j+2)�� �ܼ��� �����մϴ�.
                continue;
            }

            // ���� �ȼ���
            a = (double)Y2[((j << ratio) + 0) * width2 + (i << ratio) + 0];
            b = (double)Y2[((j << ratio) + 0) * width2 + (i << ratio) + 4];
            c = (double)Y2[((j << ratio) + 4) * width2 + (i << ratio) + 0];
            d = (double)Y2[((j << ratio) + 4) * width2 + (i << ratio) + 4];

            // 4-tap filter�� interpolation�� �ȼ���
            e = (double)Y2[((j << ratio) + 0) * width2 + (i << ratio) + 2];
            f = (double)Y2[((j << ratio) + 2) * width2 + (i << ratio) + 0];
            g = (double)Y2[((j << ratio) + 2) * width2 + (i << ratio) + 4];
            h = (double)Y2[((j << ratio) + 4) * width2 + (i << ratio) + 2];

            value = (a + b + c + d + e + f + g + h) / 8; // value�� �ѷ��� ��հ��Դϴ�
            value > 255 ? (value = 255) : value; // 255�� ������ 255�� ó��

            Y2[((j << ratio) + 2) * width2 + (i << ratio) + 2] = (unsigned char)value; // (i+2, j+2) ��ġ�� value�� ����
        }
    }

    // ******* ������ binary filter
    for (int j = 0; j < height2; j++)
    {
        for (int i = 0; i < width2 - 3; i += 2)
        {
            Y2[j * width2 + i + 1] = (Y2[j * width2 + i] + Y2[j * width2 + i + 2]) / 2; // (i, j) �� (i, j+2)�� ����� (i, j+1)�� ä���ֽ��ϴ�.
        }
        Y2[j * width2 + (width2 - 1)] = (unsigned char)(Y2[j * width2 + (width2 - 2)] * 0.5 + Y2[j * width2 + (width2 - 3)] * 0.5); // ���κ� ����ó�� (���� �� ���� ���)
    }

    // ******* ������ binary filter
    for (int j = 0; j < height2; j += 2)
    {
        for (int i = 0; i < width2; i++)
        {
            if (j > height2 - 3) // �������� ����ó��
            {
                Y2[(j + 1) * width2 + i] = (unsigned char)(Y2[j * width2 + i] * 0.5 + Y2[(j - 1) * width2 + i] * 0.5); // ���� �� ���� ������� ó���մϴ�
            }
            else
            {
                Y2[(j + 1) * width2 + i] = (unsigned char)((Y2[j * width2 + i] + Y2[(j + 2) * width2 + i]) / 2); // (i, j)�� (i+2, j)�� ����� (i+1, j)�� ä���ֽ��ϴ�.
            }
        }
    }

    // upsampling�� ���� Y2 �迭�� ����ϱ� ���� outputImg�� �Űܴ�� ����
    for (int j = 0; j < height2; j++)
    {
        for (int i = 0; i < width2; i++)
        {
            outputImg[j * stride2 + 3 * i + 0] = Y2[j * width2 + i]; // B
            outputImg[j * stride2 + 3 * i + 1] = Y2[j * width2 + i]; // G
            outputImg[j * stride2 + 3 * i + 2] = Y2[j * width2 + i]; // R
        }
    }

    FILE* outputFile = fopen(output, "wb"); // ���� ����� ���� outputFile ����������
    bmpInfo.biWidth = width2; // ������ ������ ���� ���� �����մϴ�
    bmpInfo.biHeight = height2; // ������ ������ ���� ���� �����մϴ�
    bmpInfo.biSizeImage = size2; // ������ ������ ���� ���� �����մϴ�
    bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;

    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile); // fileHeader ������ ���ϴ�
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile); // infoHeader ������ ���ϴ�
    fwrite(outputImg, sizeof(unsigned char), size2, outputFile); // ������ ����մϴ�

    free(inputImg); // �����Ҵ� ����
    fclose(inputFile); // ���� �ݱ�

    free(outputImg); // �����Ҵ� ����
    fclose(outputFile); // ���� �ݱ�

    free(Y1); // �����Ҵ� ����
    free(Y2); // �����Ҵ� ����
    free(padding); // �����Ҵ� ����
}