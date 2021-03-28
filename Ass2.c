#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

// 이미지의 File Header를 가져오는 bmpFile
BITMAPFILEHEADER bmpFile;
// 이미지의 Image Header를 가져오는 bmpInfo
BITMAPINFOHEADER bmpInfo;

// median filter에서 중간값을 찾기 위해 qsort를 할 때 이용할 함수
int compare(const void* first, const void* second)
{
    // int형으로 변환하여 비교
    int* a = (int*)first;
    // int형으로 변환하여 비교
    int* b = (int*)second;

    // 두 수의 크기를 비교해서 -1, (0), 1 을 리턴합니다. (오름차순 정렬)
    if (*a < *b)
        return -1;
    else
        return 1;
}

// median_filter(중간값 필터)로 salt & pepper noise 제거. (address 파일을 읽어서 result 파일을 출력합니다) >> main에서 조절해서 쓸 수 있게 함
void median_filter(char* address, char* result)
{
    // *****************************************************************************************************************************
    //
    //                                                    이미지 읽고 사전작업 하는 부분
    //
    // *****************************************************************************************************************************

   // 노이즈를 제거할 파일을 받을 inputFile 포인터 선언
    FILE* inputFile = NULL;
    // 이미지 파일을 읽기모드로 엽니다
    inputFile = fopen(address, "rb");

    // 파일의 정보를 읽어들여옵니다.
    // unsigned int fread(void *buf, unsigned int size, unsigned int count, File *fp)
    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    // 이미지 파일의 정보들
    int width = bmpInfo.biWidth; // 가로 길이
    int height = bmpInfo.biHeight; // 세로 길이
    int size = bmpInfo.biSizeImage; // height * width * 3 (R,G,B)
    int bitCnt = bmpInfo.biBitCount; // 8bit
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4; // (width * 3) >> 한 픽셀에 R,G,B 3개 값을 넣기 위해 3배로 늘림 

    // inputFile의 픽셀 값을 담아둘 inputImg  
    unsigned char* inputImg = NULL;
    // inputImg 동적할당
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    // inputImg에 inputFile에 담겨있던 픽셀 데이터들을 담습니다
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    // 노이즈를 제거한 이후 출력할 outputImg
    unsigned char* outputImg = NULL;
    // outputImg 동적할당
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

    // YCbCr의 Y값만 추출해서 담을 Y
    unsigned char* Y = NULL;
    // Y 배열은 R,G,B 가 아니라 Y 하나만 취급하기 때문에 size / 3
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    // Y 배열으로 값을 넣는 과정.
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            // Luminance 파일을 input으로 받았기 때문에 R, G, B 모두 Y값을 갖고 있음. 셋중 아무거나 골라서 Y배열에 넣습니다.
            Y[j * width + i] = inputImg[j * stride + 3 * i + 0];
        }
    }
    // *****************************************************************************************************************************
    //
    //                                  테두리에도 필터링 작업을 진행하기 위해 Padding을 만드는 부분 
    //
    // *****************************************************************************************************************************

    // padding 작업을 위한 변수
    int psize = 1; // 패딩의 길이 (한칸)
    int pheight = height + (2 * psize); // 패딩을 두르고 난 이후의 총 세로길이 (양옆으로 패딩이 붙기때문에 psize * 2)
    int pwidth = width + (2 * psize); // 패딩을 두르고 난 이후의 총 가로길이

    // 패딩 두른 이미지 파일을 담을 padding
    unsigned char* padding = NULL;
    // pheight * pwidth 칸 배열을 만듭니다
    padding = (unsigned char*)calloc(pheight * pwidth, sizeof(unsigned char));

    // padding을 만들 때 바로 앞에 있는 픽셀의 Y값을 그대로 복사해 채워넣습니다.
    // 따라서 테두리 부분에서 필터링을 할 때 조금더 성능이 개선될 수 있습니다
    // padding 위아래 채우기
    for (int i = 0; i < width; i++)
    {
        // 아래쪽
        padding[i + psize] = Y[i];
        // 위쪽
        padding[((pheight - 1) * pwidth) + (i + psize)] = Y[(height - 1) * width + i];
    }

    // padding 양옆 채우기
    for (int j = 0; j < height; j++)
    {
        // 왼쪽
        padding[(j + psize) * pwidth] = Y[j * width];
        // 오른쪽
        padding[(j + psize) * pwidth + (pwidth - 1)] = Y[j * width + (width - 1)];
    }

    // 이외 나머지 부분에 Y값 그대로 복사
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

    // median filter에 사용될 필터. 
    int filter[9]; // 3x3 사이즈 = 9 (qsort 정렬을 쉽게 하기위해 일차원 배열 사용)
    int fsize = 3; // 3x3 => 3
    int fmid = (fsize * fsize) / 2; // 중앙값

    // 필터가 패딩의 범위를 넘어가지 않을때 까지 (pheight - fsize, pwidth - fsize)
    for (int j = 0; j <= (pheight - fsize); j++)
    {
        for (int i = 0; i <= (pwidth - fsize); i++)
        {
            // "Salt & Pepper noise" 만 제거하기 위해 가운데 값이 0 혹은 255인 경우에만 수행합니다.
            if (padding[(j + 1) * pwidth + (i + 1)] == 0 || padding[(j + 1) * pwidth + (i + 1)] == 255)
            {
                // 3x3 필터에 값을 채웁니다.
                for (int y = 0, cnt = 0; y < fsize; y++)
                {
                    for (int x = 0; x < fsize; x++)
                    {
                        // filter 배열은 int형이기 때문에 형변환을 해줍니다
                        filter[cnt++] = (int)padding[(j + y) * pwidth + (i + x)];
                    }
                }
                // qsort로 filter 정렬
                qsort(filter, fsize * fsize, sizeof(filter[0]), compare);

                // 정렬한 배열의 중앙값을 넣습니다. padding 배열은 unsigned char형이고 filter은 int형이기 때문에 형변환 해줍니다
                padding[(j + 1) * pwidth + (i + 1)] = (unsigned char)filter[fmid];
            }
        }
    }

    // filtering이 모두 끝나고 패딩을 제외한 부분을 다시 Y에 붙여넣습니다.
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y[j * width + i] = padding[(j + psize) * pwidth + (i + psize)];
        }
    }
    
    // 출력을 위해 outputImg의 R,G,B 픽셀에 Y값을 각각 채워넣습니다.
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            outputImg[j * stride + i * 3 + 0] = Y[j * width + i]; // B
            outputImg[j * stride + i * 3 + 1] = Y[j * width + i]; // G
            outputImg[j * stride + i * 3 + 2] = Y[j * width + i]; // R
        }
    }

    // 결과 파일을 출력하기 위한 과정
    FILE* outputFile = fopen(result, "wb");
    // 이미지 파일 헤더 데이터를 씁니다
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    // 이미지 파일 헤더 데이터를 씁니다
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    // 픽셀값도 씁니다
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    // 동적할당 해제 및 파일 닫기
    free(outputImg);
    fclose(outputFile);
    // 동적할당 해제 및 파일 닫기
    free(inputImg);
    fclose(inputFile);
    // 동적할당 해제 및 파일 닫기
    free(Y);
    free(padding);

}

// mean_filter(평균값 필터)로 전반적으로 이미지를 흐리게 만들어 noise 제거 (address 파일을 읽어서 result 파일을 출력합니다) >> main에서 조절해서 쓸 수 있게 함
void mean_filter(char* address, char* result)
{
    // *****************************************************************************************************************************
    //
    //                                  이미지 읽고 사전작업 + padding 하는 부분 (위 Median 필터와 동일합니다)
    //
    // *****************************************************************************************************************************

    // median_filter를 한번 돌고 난 이미지 파일을 불러옵니다
    FILE* inputFile = NULL;
    // 이미지 파일을 읽기모드로 엽니다
    inputFile = fopen(address, "rb");

    // 파일의 정보를 읽어들여옵니다.
    // unsigned int fread(void *buf, unsigned int size, unsigned int count, File *fp)
    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    // 이미지 파일의 정보들
    int width = bmpInfo.biWidth; // 가로 길이
    int height = bmpInfo.biHeight; // 세로 길이
    int size = bmpInfo.biSizeImage; // height * width * 3 (R,G,B)
    int bitCnt = bmpInfo.biBitCount; // 8bit
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4; // (width * 3) >> 한 픽셀에 R,G,B 3개 값을 넣기 위해 3배로 늘림 

    // inputFile의 픽셀 값을 담아둘 inputImg  
    unsigned char* inputImg = NULL;
    // inputImg 동적할당
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    // inputImg에 inputFile에 담겨있던 픽셀 데이터들을 담습니다
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    // 노이즈를 제거한 이후 출력할 outputImg
    unsigned char* outputImg = NULL;
    // outputImg 동적할당
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

    // YCbCr의 Y값만 추출해서 담을 Y
    unsigned char* Y = NULL;
    // Y 배열은 R,G,B 가 아니라 Y 하나만 취급하기 때문에 size / 3
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    // Y 배열으로 값을 넣는 과정.
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            // Luminance 파일을 input으로 받았기 때문에 R, G, B 모두 Y값을 갖고 있음. 셋중 아무거나 골라서 Y배열에 넣습니다.
            Y[j * width + i] = inputImg[j * stride + 3 * i + 0];
        }
    }

    // padding 작업을 위한 변수
    int psize = 1; // 패딩의 길이 (한칸)
    int pheight = height + (2 * psize); // 패딩을 두르고 난 이후의 총 세로길이 (양옆으로 패딩이 붙기때문에 psize * 2)
    int pwidth = width + (2 * psize); // 패딩을 두르고 난 이후의 총 가로길이

    // 패딩 두른 이미지 파일을 담을 padding
    unsigned char* padding = NULL;
    // pheight * pwidth 칸 배열을 만듭니다
    padding = (unsigned char*)calloc(pheight * pwidth, sizeof(unsigned char));

    // padding을 만들 때 바로 앞에 있는 픽셀의 Y값을 그대로 복사해 채워넣습니다.
    // 따라서 테두리 부분에서 필터링을 할 때 조금더 성능이 개선될 수 있습니다
    // padding 위아래 채우기
    for (int i = 0; i < width; i++)
    {
        // 아래쪽
        padding[i + psize] = Y[i];
        // 위쪽
        padding[((pheight - 1) * pwidth) + (i + psize)] = Y[(height - 1) * width + i];
    }

    // padding 양옆 채우기
    for (int j = 0; j < height; j++)
    {
        // 왼쪽
        padding[(j + psize) * pwidth] = Y[j * width];
        // 오른쪽
        padding[(j + psize) * pwidth + (pwidth - 1)] = Y[j * width + (width - 1)];
    }

    // 이외 나머지 부분에 Y값 그대로 복사
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

    // 3x3 필터
    int fsize = 3;
    // 평균값을 구하기 위한 fsum
    int fsum = 0;

    // 필터가 패딩의 범위를 넘어가지 않을때 까지 (pheight - fsize, pwidth - fsize)
    for (int j = 0; j <= (pheight - fsize); j++)
    {
        for (int i = 0; i <= (pwidth - fsize); i++)
        {
            // 평균값 구하기
            fsum = 0;
            // 3x3 크기를 돌면서 모든 Y값을 fsum 변수에 더합니다
            for (int y = 0; y < fsize; y++)
            {
                for (int x = 0; x < fsize; x++)
                {
                    // fsum은 int형이고 padding은 unsigned char형이기 때문에 형변환 합니다
                    fsum += (int)padding[(j + y) * pwidth + (i + x)];
                }
            }
            // 평균값을 구합니다
            fsum /= (fsize * fsize);

            // 평균값을 형 변환 이후 가운데 채워넣습니다.
            padding[(j + 1) * pwidth + (i + 1)] = (unsigned char)fsum;
        }
    }

    // filtering이 모두 끝나고 패딩을 제외한 부분을 다시 Y에 붙여넣습니다.
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y[j * width + i] = padding[(j + psize) * pwidth + (i + psize)];
        }
    }

    // 출력을 위해 outputImg의 R,G,B 픽셀에 Y값을 각각 채워넣습니다.
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            outputImg[j * stride + i * 3 + 0] = Y[j * width + i]; // B
            outputImg[j * stride + i * 3 + 1] = Y[j * width + i]; // G
            outputImg[j * stride + i * 3 + 2] = Y[j * width + i]; // R
        }
    }

    // 결과 파일을 출력하기 위한 과정
    FILE* outputFile = fopen(result, "wb");
    // 이미지 파일 헤더 데이터를 씁니다
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    // 이미지 파일 헤더 데이터를 씁니다
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    // 픽셀값도 씁니다
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    // 동적할당 해제 및 파일 닫기
    free(outputImg);
    fclose(outputFile);
    // 동적할당 해제 및 파일 닫기
    free(inputImg);
    fclose(inputFile);
    // 동적할당 해제 및 파일 닫기
    free(Y);
    free(padding);
}


int main()
{
    // 1. Median filter로 salt & papper noise를 제거한다
    median_filter("./AICenterY_CombinedNoise.bmp", "./17011753_이도형.bmp");
    // 2. 그 다음에 mean filter를 돌려서 noise를 한번 더 제거한다
    mean_filter("./17011753_이도형.bmp", "./17011753_이도형.bmp");
    
    // print.
    printf("Processing Complete!\n");

    // 함수 종료
    return 0;
}