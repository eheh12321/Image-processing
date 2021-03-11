#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>

int main()
{
    // 사진에 대한 정보를 헤더에서 뽑아와서 사용한다.
    BITMAPFILEHEADER bmpFile;
    BITMAPINFOHEADER bmpInfo;
    
    // 가로 x 세로 해서 이차원 배열이 아니다..! 일차원.
    FILE* inputFile = NULL;

    // "rb" : 읽기모드
    inputFile = fopen("./image/AICenter.bmp", "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    // 가로-세로 길이 (512 x 512)
    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;

    // 일반적으로 가로 x 세로 (x 3 : R,G,B)
    int size = bmpInfo.biSizeImage;

    // bit-depth : RGB 파일이므로 8bit * 3 = 24 => 한 픽셀당 24bit.
    int bitCnt = bmpInfo.biBitCount;

    // BMP 파일은 가로크기가 항상 4의배수(4Byte 단위)로 저장되어 나옴
    // 만약 영상의 가로 크기가 4의배수로 안떨어지는 경우 
    // (영상의 크기가 4byte단위로 구성되있다 해도 픽셀 정보 자체는 4byte 단위로 구성이 안돼있을 수 있다)
    // (= 이미지의 가로크기랑 파일의 가로크기가 다를 수 있다..?)
    // 4byte 배수로 맞춰놓고, processing 할때는 width 만큼만 반복.
    // 파일의 가로크기를 4의 배수로 맞춰주는 작업.
    // + 3을 하는 이유 => 올림을 하기 위해
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
    
    printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);

    // unsigned char를 쓰는 이유 -> 하나의 픽셀이 0~255를 표현하는데
    // unsigned char의 자료형 크기가 딱 0~255랑 맞아서.
    unsigned char* inputImg = NULL, * outputImg = NULL, * outputImg_I = NULL, * outputImg_I_R = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    outputImg_I = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg_I_R = (unsigned char*)calloc(size, sizeof(unsigned char));

    double Y, Cb, Cr, I, R, G, B;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            // stride = 파일의 가로길이. j++가 되어 j * stride를 하면 다음줄로 내려가는 구조가 된다.
            // (일차원 배열이니까!)
            // 한 픽셀에 B, G, R 세개의 구성요소가 있으므로!! *중요

            // Y : 명도. RGB값 대신 Y값을 넣으면 흑백사진이 된다.
            Y = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0];
            Cb = -0.169 * inputImg[j * stride + 3 * i + 2] - 0.331 * inputImg[j * stride + 3 * i + 1] + 0.500 * inputImg[j * stride + 3 * i + 0];
            Cr = 0.500 * inputImg[j * stride + 3 * i + 2] - 0.419 * inputImg[j * stride + 3 * i + 1] + 0.0813 * inputImg[j * stride + 3 * i + 0];

            // I
            I = (unsigned char)(((inputImg[j * stride + 3 * i + 0]) + (inputImg[j * stride + 3 * i + 1]) + (inputImg[j * stride + 3 * i + 2])) / 3);

            // RGB에 전부 같은 값을 넣으면 Gray tone이 된다 (검정 > 하양 그 사이)
            outputImg[j * stride + 3 * i + 0] = (int)Y;
            outputImg[j * stride + 3 * i + 1] = (int)Y;
            outputImg[j * stride + 3 * i + 2] = (int)Y;

            outputImg_I[j * stride + 3 * i + 0] = (int)I;
            outputImg_I[j * stride + 3 * i + 1] = (int)I;
            outputImg_I[j * stride + 3 * i + 2] = (int)I;


            // YCbCr to RGB
            R = Y + 1.402 * Cr;
            if (R > 255)
                R = 255;
            else if (R < 0)
                R = 0;

            G = Y - 0.714 * Cr - 0.344 * Cb;
            if (G > 255)
                G = 255;
            else if (G < 0)
                G = 0;

            B = Y + 1.772 * Cb;
            if (B > 255)
                B = 255;
            else if (B < 0)
                B = 0;

            outputImg_I_R[j * stride + 3 * i + 0] = (int)B;
            outputImg_I_R[j * stride + 3 * i + 1] = (int)G;
            outputImg_I_R[j * stride + 3 * i + 2] = (int)R;
        }

    }

    FILE* outputFile = fopen("./image/Output.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    FILE* outputFile_I = fopen("./image/Output_I.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_I);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_I);
    fwrite(outputImg_I, sizeof(unsigned char), size, outputFile_I);

    free(outputImg_I);
    fclose(outputFile_I);

    FILE* outputFile_I_R = fopen("./image/Output_I_R.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_I_R);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_I_R);
    fwrite(outputImg_I_R, sizeof(unsigned char), size, outputFile_I_R);

    free(outputImg_I_R);
    fclose(outputFile_I_R);


    free(outputImg);
    free(inputImg);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}