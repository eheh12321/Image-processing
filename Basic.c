#include "header.h"

FILE* open_image()
{
    FILE* inputFile = NULL;
    return inputFile = fopen("./image/original/IU_2.bmp", "rb");
}

int main()
{
    // 가로 x 세로 해서 이차원 배열이 아니다..! 일차원.
    FILE* inputFile = NULL;

    // "rb" : 읽기모드
    inputFile = open_image();

    // 필수!!!!!!!!!!!!!!!!!! 절대 빼먹지 마시오
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
    
    printf("W (가로) : %d(%d)\nH (세로) : %d\nS (크기) : %d\nD (bit-depth) : %d\n", width, stride, height, size, bitCnt);

    // unsigned char를 쓰는 이유 -> 하나의 픽셀이 0~255를 표현하는데
    // unsigned char의 자료형 크기가 딱 0~255랑 맞아서.
    unsigned char* inputImg = NULL, * outputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            // stride = 파일의 가로길이. j++가 되어 j * stride를 하면 다음줄로 내려가는 구조가 된다.
            // (일차원 배열이니까!)
            // 한 픽셀에 B, G, R 세개의 구성요소가 있으므로!! *중요
            outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];
            outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];
            outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];
        }
    }

    FILE* outputFile = fopen("./image/Output.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    free(inputImg);
    fclose(inputFile);
    fclose(outputFile);

    ////

    rgb_color_test();
    ycbcr_color_test();
    HSI();
    YI_diff();
    watermark();
    filtering();
    random_noise_generate();
    sp_noise_generate();
    printf("\nSalt-Pepper Noise\n");
    PSNR("./image/Output_SPnoise.bmp");
    printf("\nRandom Noise\n");
    PSNR("./image/Output_Rnoise.bmp");
    ////

    return 0;
}
