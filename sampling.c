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
    
    int width2 = bmpInfo.biWidth >> ratio; // >> b : b번만큼 / 2 수행
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
            Y2[j * width2 + i] = Y1[(j << ratio) * width + (i << ratio)]; // 단순히 x칸마다 한번씩 추출하는 방식

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
    FILE* inputFile = NULL; // 원본 파일을 받을 포인터 변수
    inputFile = fopen(address, "rb"); // 원본 파일을 파라미터로 받아와서 엽니다.

    // #### 128x128 file (원본)
    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile); // 원본파일의 fileHeader를 가져와 bmpFile에 저장
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile); // 원본파일의 infoHeader를 가져와 bmpInfo에 저장

    int width = bmpInfo.biWidth; // 가로 길이
    int height = bmpInfo.biHeight; // 세로 길이
    int size = bmpInfo.biSizeImage; // size = width * height * 3
    int bitCnt = bmpInfo.biBitCount; // 8bit 영상
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4; // width를 4의 배수로 떨어지게 세팅

    // #### 512x512 file (upsampling)
    int width2 = bmpInfo.biWidth << ratio; // 시프트 연산을 이용하였습니다. << 는 2를 곱하는 효과가 있습니다.
    int height2 = bmpInfo.biHeight << ratio; // ratio=2이므로 4를 곱한것과 같음
    int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4; // width2를 4의 배수로 떨어지게 세팅
    int size2 = stride2 * height2; // stride2(가로) * height2(세로) = size

    // 원본 파일을 담을 inputImg 포인터 변수
    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char)); // 원본은 128x128 파일
    fread(inputImg, sizeof(unsigned char), size, inputFile); // 파일을 읽어들입니다.

    // inputImg에서 Y값만 추출해서 담아낼 Y1 포인터 변수
    unsigned char* Y1 = NULL;
    Y1 = (unsigned char*)calloc(size / 3, sizeof(unsigned char)); // R,G,B 중 하나만 뽑아오면 되므로 size / 3 의 크기만 필요

    // Y1에서 upsampling된 깂을 담을 Y2 포인터 변수 
    unsigned char* Y2 = NULL;
    Y2 = (unsigned char*)calloc(size2 / 3, sizeof(unsigned char)); // R,G,B 중 하나만 뽑아오면 되므로 size2 / 3 의 크기만 필요

    // 출력 파일을 위한 outputImg 포인터 변수
    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size2, sizeof(unsigned char)); // 512x512 파일

    // inputImg에서 Y값을 뽑아 Y1 배열에 저장하는 과정
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y1[j * width + i] = inputImg[j * stride + 3 * i + 0]; // R G B 중 하나만 뽑아서 저장합니다.
        }
    }

    // *****************************************************************************************************************************
    //
    //                                               Y1 배열에 패딩을 씌우는 과정
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
    //                                                         Upsampling
    //
    // *****************************************************************************************************************************

    double ntapfilter[4] = { -0.0625, 0.5625, 0.5625, -0.0625 }; // 기본 4-tap filter
    double ntapfilterh[3] = { 0.25, 0.5, 0.25 }; // 양방향을 모두 고려하기 위해서 추가한 필터.

    int fsize = 4; // 필터 길이
    double value = 0; // ntapfilter 계산용
    double value2 = 0; // ntapfilterh 계산용 
    int rr = 1 << ratio; // ratio=2 -> rr=4, ratio=3 -> rr=8 등등등... ratio에 맞게 이동하기 위한 변수

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int pj = psize + j; // 패딩을 씌우고 난 이후 j 위치
            int pi = psize + i; // 패딩을 씌우고 난 이후 i 위치
            int ir = i << ratio; // ratio에 맞게 이동하기 위한 변수.. rr씩 이동합니다.
            int jr = j << ratio; // ratio에 맞게 이동하기 위한 변수.. rr씩 이동합니다. 

            // ******* 가로방향 4-tap filter | (i, j)와 (i + rr, j) 사이 (i + (rr / 2), j) 삽입
            value = 0; // 변수 초기화
            value2 = 0; // 변수 초기화

            for (int k = 0; k < fsize; k++) // 4-tap filter 계산과정
            {
                value += (double)(padding[pj * pwidth + (pi - 1 + k)]) * ntapfilter[k];
            }
            for (int k = 0; k < 3; k++) // 세로방향의 값을 추가로 계산하는 과정
            {
                value2 += (double)(padding[(pj - 1 + k) * pwidth + pi + 1]) * ntapfilterh[k];
                value2 += (double)(padding[(pj - 1 + k) * pwidth + pi + 0]) * ntapfilterh[k];
            }
            value = (value + (value2 / 2)) / 2; // value와 value2의 값을 평균을 내어 interpolation합니다.
            value > 255 ? (value = 255) : value; // 255가 넘으면 255로 처리

            Y2[jr * width2 + ir] = padding[pj * pwidth + pi]; // (i, j) 위치에는 원본값을 그대로 넣습니다
            Y2[jr * width2 + ir + rr / 2] = (unsigned char)value; // (i + (rr / 2), j) 위치에는 위에서 계산한 value 값을 넣습니다.

            // ******* 세로방향 4-tap filter | (i, j)와 (i, j+rr) 사이 (i, j + (rr / 2)) 삽입
            value = 0; // 변수 초기화
            value2 = 0; // 변수 초기화

            for (int k = 0; k < fsize; k++) // 4-tap filter 계산과정
            {
                value += (double)(padding[(pj - 1 + k) * pwidth + pi]) * ntapfilter[k];
            }
            for (int k = 0; k < 3; k++) // 가로방향의 값을 추가로 계산하는 과정
            {
                value2 += (double)(padding[pj * pwidth + (pi - 1 + k)]) * ntapfilterh[k];
                value2 += (double)(padding[(pj + 1) * pwidth + (pi - 1 + k)]) * ntapfilterh[k];
            }
            value = (value + (value2 / 2)) / 2; // value와 value2의 값을 평균을 내어 interpolation합니다.
            value > 255 ? (value = 255) : value; // 255가 넘으면 255로 처리

            Y2[(jr + rr / 2) * width2 + ir] = (unsigned char)value; // (i, j + (rr / 2)) 위치에 위에서 계산한 value 값을 넣습니다.
            // (i + (rr / 2), j + (rr / 2))는 아래에서 따로 계산합니다
        }
    }

    // 비어있는 (i + (rr / 2), j + (rr / 2)) 계산하는 과정
    double a, b, c, d, e, f, g, h;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int ir = i << ratio; // ratio에 맞게 이동하기 위한 변수.. rr만큼 이동합니다
            int jr = j << ratio; // ratio에 맞게 이동하기 위한 변수.. rr만큼 이동합니다

            if (j >= height - 1) // 세로방향 마지막줄 예외처리
            {
                Y2[(jr + rr / 2) * width2 + ir + rr / 2] = Y2[jr * width2 + ir + rr / 2]; // 이전에 있는 값을 단순히 복사해 채워넣습니다.
                continue;
            }
            if (i >= width - 1) // 가로방향 끝부분 예외처리
            {
                Y2[(jr + rr / 2) * width2 + ir + rr / 2] = Y2[(jr + rr / 2) * width2 + ir]; // 이전에 있는 값을 단순히 복사해 채워넣습니다.
                continue;
            }

            // 원본 픽셀값
            a = (double)Y2[(jr + rr * 0) * width2 + ir + rr * 0];
            b = (double)Y2[(jr + rr * 0) * width2 + ir + rr * 1];
            c = (double)Y2[(jr + rr * 1) * width2 + ir + rr * 0];
            d = (double)Y2[(jr + rr * 1) * width2 + ir + rr * 1];

            // 4-tap filter로 interpolation된 픽셀값
            e = (double)Y2[jr * width2 + ir + (rr / 2)];
            f = (double)Y2[(jr + (rr / 2)) * width2 + ir];
            g = (double)Y2[(jr + (rr / 2)) * width2 + ir + rr];
            h = (double)Y2[(jr + rr) * width2 + ir + (rr / 2)];

            value = (a + b + c + d + e + f + g + h) / 8; // value는 둘레의 평균값입니다
            value > 255 ? (value = 255) : value; // 255가 넘으면 255로 처리

            Y2[(jr + rr / 2) * width2 + ir + rr / 2] = (unsigned char)value; // (i + (rr / 2), j + (rr / 2)) 위치에 value값 대입
        }
    }

    // binary filter입니다. ratio에 맞게 반복하고 (rr / k)(=kr) 씩 이동하면서 사이에 평균을 구해 채워넣습니다.
    for (int k = 2; k < rr; k = k << 1)
    {
        int kr = rr / k; // 반복문을 돌때마다 2씩 크기가 작아지면서 빈칸을 빠짐없이 채웁니다. (8, 4, 2, ...)

        // ******* 가로축 binary filter | (i + kr, j)과 (i, j) 사이 (i + (kr / 2), j) 삽입
        for (int j = 0; j < height2; j++)
        {
            for (int i = 0; i < width2 - rr / 2; i += kr) // 가로축 끝부분은 별도로 처리합니다
            {
                Y2[j * width2 + i + kr / 2] = (Y2[j * width2 + i] + Y2[j * width2 + i + kr]) / 2;
            }      
        }

        // ******* 세로축 binary filter | (i, j + kr)과 (i, j) 사이 (i, j + (kr / 2)) 삽입
        for (int j = 0; j < height2 - rr / 2; j += kr) // 세로축 마지막 줄은 별도로 처리합니다.
        {
            for (int i = 0; i < width2; i++) 
            {
                Y2[(j + kr / 2) * width2 + i] = (Y2[j * width2 + i] + Y2[(j + kr) * width2 + i]) / 2;
            }
        }
    }

    // 예외처리 파트(가로축)
    for (int j = 0; j < height2 - rr / 2; j++)
    {
        for (int i = width2 - rr / 2; i < width2; i++)
        {
            Y2[j * width2 + i] = Y2[j * width2 + i - 1]; // 이전값을 그대로 복사합니다
        }
    }
    // 예외처리 파트(세로축)
    for (int j = height2 - rr / 2; j < height2; j++)
    {
        for (int i = 0; i < width2; i++)
        {
            Y2[j * width2 + i] = Y2[(j - 1) * width2 + i]; // 이전값을 그대로 복사합니다
        }
    }

    // upsampling이 끝난 Y2 배열을 출력하기 위해 outputImg로 옮겨담는 과정
    for (int j = 0; j < height2; j++)
    {
        for (int i = 0; i < width2; i++)
        {
            outputImg[j * stride2 + 3 * i + 0] = Y2[j * width2 + i]; // B
            outputImg[j * stride2 + 3 * i + 1] = Y2[j * width2 + i]; // G
            outputImg[j * stride2 + 3 * i + 2] = Y2[j * width2 + i]; // R
        }
    }

    FILE* outputFile = fopen(output, "wb"); // 파일 출력을 위한 outputFile 파일포인터
    bmpInfo.biWidth = width2; // 사전에 설정한 변수 값을 대입합니다
    bmpInfo.biHeight = height2; // 사전에 설정한 변수 값을 대입합니다
    bmpInfo.biSizeImage = size2; // 사전에 설정한 변수 값을 대입합니다
    bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;

    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile); // fileHeader 정보를 씁니다
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile); // infoHeader 정보를 씁니다
    fwrite(outputImg, sizeof(unsigned char), size2, outputFile); // 파일을 출력합니다

    free(inputImg); // 동적할당 해제
    fclose(inputFile); // 파일 닫기

    free(outputImg); // 동적할당 해제
    fclose(outputFile); // 파일 닫기

    free(Y1); // 동적할당 해제
    free(Y2); // 동적할당 해제
    free(padding); // 동적할당 해제
}