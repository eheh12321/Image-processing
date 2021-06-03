#include "header.h"

void down_sampling_sub(char* address, char* output, int ratio)
{
    FILE* inputFile = NULL;
    inputFile = fopen(address, "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage; // size = width * height * 3(R, G, B)
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    // ****************
    
    int width2 = bmpInfo.biWidth >> ratio; // >> b : b����ŭ / 2 ����
    int height2 = bmpInfo.biHeight >> ratio;
    int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
    int size2 = stride2 * height2; // stride (= width * 3) * height = size

    // ****************

    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

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
            Y2[j * width2 + i] = Y1[(j << ratio) * width + (i << ratio)]; // �ܼ��� xĭ���� �ѹ��� �����ϴ� ���

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

    double sum, avg;
    for (int j = 0; j < height2; j++)
    {
        for (int i = 0; i < width2; i++)
        {
            sum = avg = 0;

            for (int y = 0; y < ratio; y++)
            {
                for (int x = 0; x < ratio; x++)
                {
                    sum += Y1[((j << ratio) + y) * width + (i << ratio) + x];
                }
            }
            avg = sum / (ratio * ratio);

            Y2[j * width2 + i] = (unsigned char)avg;

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
    double ntapfilterh[3] = { 0.25, 0.5, 0.25 }; // ������� ��� ����ϱ� ���ؼ� �߰��� ����.

    int fsize = 4; // ���� ����
    double value = 0; // ntapfilter ����
    double value2 = 0; // ntapfilterh ���� 
    int rr = 1 << ratio; // ratio=2 -> rr=4, ratio=3 -> rr=8 ����... ratio�� �°� �̵��ϱ� ���� ����

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int pj = psize + j; // �е��� ����� �� ���� j ��ġ
            int pi = psize + i; // �е��� ����� �� ���� i ��ġ
            int ir = i << ratio; // ratio�� �°� �̵��ϱ� ���� ����.. rr�� �̵��մϴ�.
            int jr = j << ratio; // ratio�� �°� �̵��ϱ� ���� ����.. rr�� �̵��մϴ�. 

            // ******* ���ι��� 4-tap filter | (i, j)�� (i + rr, j) ���� (i + (rr / 2), j) ����
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

            Y2[jr * width2 + ir] = padding[pj * pwidth + pi]; // (i, j) ��ġ���� �������� �״�� �ֽ��ϴ�
            Y2[jr * width2 + ir + rr / 2] = (unsigned char)value; // (i + (rr / 2), j) ��ġ���� ������ ����� value ���� �ֽ��ϴ�.

            // ******* ���ι��� 4-tap filter | (i, j)�� (i, j+rr) ���� (i, j + (rr / 2)) ����
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

            Y2[(jr + rr / 2) * width2 + ir] = (unsigned char)value; // (i, j + (rr / 2)) ��ġ�� ������ ����� value ���� �ֽ��ϴ�.
            // (i + (rr / 2), j + (rr / 2))�� �Ʒ����� ���� ����մϴ�
        }
    }

    // ����ִ� (i + (rr / 2), j + (rr / 2)) ����ϴ� ����
    double a, b, c, d, e, f, g, h;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int ir = i << ratio; // ratio�� �°� �̵��ϱ� ���� ����.. rr��ŭ �̵��մϴ�
            int jr = j << ratio; // ratio�� �°� �̵��ϱ� ���� ����.. rr��ŭ �̵��մϴ�

            if (j >= height - 1) // ���ι��� �������� ����ó��
            {
                Y2[(jr + rr / 2) * width2 + ir + rr / 2] = Y2[jr * width2 + ir + rr / 2]; // ������ �ִ� ���� �ܼ��� ������ ä���ֽ��ϴ�.
                continue;
            }
            if (i >= width - 1) // ���ι��� ���κ� ����ó��
            {
                Y2[(jr + rr / 2) * width2 + ir + rr / 2] = Y2[(jr + rr / 2) * width2 + ir]; // ������ �ִ� ���� �ܼ��� ������ ä���ֽ��ϴ�.
                continue;
            }

            // ���� �ȼ���
            a = (double)Y2[(jr + rr * 0) * width2 + ir + rr * 0];
            b = (double)Y2[(jr + rr * 0) * width2 + ir + rr * 1];
            c = (double)Y2[(jr + rr * 1) * width2 + ir + rr * 0];
            d = (double)Y2[(jr + rr * 1) * width2 + ir + rr * 1];

            // 4-tap filter�� interpolation�� �ȼ���
            e = (double)Y2[jr * width2 + ir + (rr / 2)];
            f = (double)Y2[(jr + (rr / 2)) * width2 + ir];
            g = (double)Y2[(jr + (rr / 2)) * width2 + ir + rr];
            h = (double)Y2[(jr + rr) * width2 + ir + (rr / 2)];

            value = (a + b + c + d + e + f + g + h) / 8; // value�� �ѷ��� ��հ��Դϴ�
            value > 255 ? (value = 255) : value; // 255�� ������ 255�� ó��

            Y2[(jr + rr / 2) * width2 + ir + rr / 2] = (unsigned char)value; // (i + (rr / 2), j + (rr / 2)) ��ġ�� value�� ����
        }
    }

    // binary filter�Դϴ�. ratio�� �°� �ݺ��ϰ� (rr / k)(=kr) �� �̵��ϸ鼭 ���̿� ����� ���� ä���ֽ��ϴ�.
    for (int k = 2; k < rr; k = k << 1)
    {
        int kr = rr / k; // �ݺ����� �������� 2�� ũ�Ⱑ �۾����鼭 ��ĭ�� �������� ä��ϴ�. (8, 4, 2, ...)

        // ******* ������ binary filter | (i + kr, j)�� (i, j) ���� (i + (kr / 2), j) ����
        for (int j = 0; j < height2; j++)
        {
            for (int i = 0; i < width2 - rr / 2; i += kr) // ������ ���κ��� ������ ó���մϴ�
            {
                Y2[j * width2 + i + kr / 2] = (Y2[j * width2 + i] + Y2[j * width2 + i + kr]) / 2;
            }      
        }

        // ******* ������ binary filter | (i, j + kr)�� (i, j) ���� (i, j + (kr / 2)) ����
        for (int j = 0; j < height2 - rr / 2; j += kr) // ������ ������ ���� ������ ó���մϴ�.
        {
            for (int i = 0; i < width2; i++) 
            {
                Y2[(j + kr / 2) * width2 + i] = (Y2[j * width2 + i] + Y2[(j + kr) * width2 + i]) / 2;
            }
        }
    }

    // ����ó�� ��Ʈ(������)
    for (int j = 0; j < height2 - rr / 2; j++)
    {
        for (int i = width2 - rr / 2; i < width2; i++)
        {
            Y2[j * width2 + i] = Y2[j * width2 + i - 1]; // �������� �״�� �����մϴ�
        }
    }
    // ����ó�� ��Ʈ(������)
    for (int j = height2 - rr / 2; j < height2; j++)
    {
        for (int i = 0; i < width2; i++)
        {
            Y2[j * width2 + i] = Y2[(j - 1) * width2 + i]; // �������� �״�� �����մϴ�
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