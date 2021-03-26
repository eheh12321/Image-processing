#pragma once
#include "header.h"

void masking()
{
    FILE* inputFile = NULL;
    inputFile = fopen("./image/Output_Y.bmp", "rb");

    FILE* inputFile2 = NULL;
    inputFile2 = fopen("./image/original/FilteringMask2.bmp", "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile2);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile2);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    unsigned char* inputImg2 = NULL;
    inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg2, sizeof(unsigned char), size, inputFile2);

    unsigned char* outputImg_F = NULL;
    outputImg_F = (unsigned char*)calloc(size, sizeof(unsigned char));

    int filter = 1;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {   
            filter = 1;

            if (inputImg2[j * stride + i * 3 + 0] == 255)
            {
                filter = 0;
            }
            else
            {
                outputImg_F[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0] * filter;
                outputImg_F[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1] * filter;
                outputImg_F[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2] * filter;
            }
        }
    }

    FILE* outputFile_F = fopen("./image/Output_M.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile_F);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile_F);
    fwrite(outputImg_F, sizeof(unsigned char), size, outputFile_F);

    free(outputImg_F);
    fclose(outputFile_F);

    free(inputImg);
    fclose(inputFile);

    free(inputImg2);
    fclose(inputFile2);
}

void random_noise_generate()
{
    FILE* inputFile = NULL;
    inputFile = fopen("./image/Output_Y.bmp", "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

    int Err = 100;
    //srand(time(NULL));
    double Y;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y = inputImg[j * stride + 3 * i + 0];
            Y += rand() % Err - (Err >> 1);

            Y > 255 ? (Y = 255) : (Y < 0 ? (Y = 0) : Y);
            outputImg[j * stride + 3 * i + 0] = (unsigned char)Y;
            outputImg[j * stride + 3 * i + 1] = (unsigned char)Y;
            outputImg[j * stride + 3 * i + 2] = (unsigned char)Y;
        }
    }

    FILE* outputFile = fopen("./image/Output_Rnoise.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);

}

void sp_noise_generate()
{
    FILE* inputFile = NULL;
    inputFile = fopen("./image/Output_Y.bmp", "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

    int filter;
    srand(time(NULL));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            filter = rand() % 10;

            if (filter == 0)
            {
                outputImg[j * stride + 3 * i + 0] = 0;
                outputImg[j * stride + 3 * i + 1] = 0;
                outputImg[j * stride + 3 * i + 2] = 0;
            }
            else if (filter == 1)
            {
                outputImg[j * stride + 3 * i + 0] = 255;
                outputImg[j * stride + 3 * i + 1] = 255;
                outputImg[j * stride + 3 * i + 2] = 255;
            }
            else
            {
                outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];
                outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];
                outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];
            }
        }
    }

    FILE* outputFile = fopen("./image/Output_SPnoise.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);

}

int compare(const void* first, const void* second)
{
    int *a = (int*)first;
    int *b = (int*)second;

    if (*a < *b)
        return -1;
    else
        return 1;
}

void mean_filter()
{
    FILE* inputFile = NULL;
    inputFile = fopen("./image/Output_Median_Filter.bmp", "rb");

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

    // Y 배열은 R,G,B 가 아니라 Y 하나만 취급하기 때문에 size / 3.
    // >>>> inputImg, outputImg 의 크기와 Y 의 크기가 다르다 !!!!!!!!!!!!!! <<<<
    unsigned char* Y = NULL;
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            // Luminance 파일을 input으로 받았기 때문에 R, G, B 모두 Y값을 갖고 있음. 셋중 아무거나 골라서 Y배열에 넣음.
            Y[j * width + i] = inputImg[j * stride + 3 * i + 0];
        }
    }

    //for (int j = 0; j < 50; j++)
    //{
    //    for (int i = 0; i < 50; i++)
    //    {
    //        printf("%3d ", Y[j * width + i]);
    //    }
    //    printf("\n");
    //}
    //printf("\n\n");    
    //for (int j = 480; j < height; j++)
    //{
    //    for (int i = 480; i < width; i++)
    //    {
    //        printf("%3d ", Y[j * width + i]);
    //    }
    //    printf("\n");
    //}
    //printf("\n\n");

    // padding의 width와 Y의 width 길이는 다르다!
    int psize = 1;
    int pheight = height + (2 * psize);
    int pwidth = width + (2 * psize);

    int filter[3][3] = { 0 };
    int fsum = 0;
    int fsize = 3;

    unsigned char* padding = NULL;
    // 위아래로 psize칸 패딩
    padding = (unsigned char*)calloc(pheight * pwidth, sizeof(unsigned char));

    // padding 위아래 채우기
    for (int i = 0; i < width; i++)
    {
        padding[i + psize] = Y[i];
        padding[((pheight - 1) * pwidth) + (i + psize)] = Y[(height - 1) * width + i];
    }
    
    // padding 양옆 채우기
    for (int j = 0; j < height; j++)
    {
        padding[(j + psize) * pwidth] = Y[j * width];
        padding[(j + psize) * pwidth + (pwidth - 1)] = Y[j * width + (width - 1)];
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            padding[(j + psize) * pwidth + (i + psize)] = Y[j * width + i];
        }
    }

    //for (int j = 0; j < 50; j++)
    //{
    //    for (int i = 0; i < 50; i++)
    //    {
    //        printf("%3d ", padding[j * pwidth + i]);
    //    }
    //    printf("\n");
    //}
    //printf("\n\n");

    //for (int j = 480; j < pheight; j++)
    //{
    //    for (int i = 480; i < pwidth; i++)
    //    {
    //        printf("%3d ", padding[j * pwidth + i]);
    //    }
    //    printf("\n");
    //}
    //printf("\n\n");

    for (int j = 0; j <= (pheight - fsize); j++)
    {
        for (int i = 0; i <= (pwidth - fsize); i++)
        {
            fsum = 0;
            for (int y = 0; y < fsize; y++)
            {
                for (int x = 0; x < fsize; x++)
                {
                    filter[y][x] = (int)padding[(j + y) * pwidth + (i + x)];
                    fsum += filter[y][x];
                }
            }
            fsum /= (fsize * fsize);

            padding[(j + 1) * pwidth + (i + 1)] = (unsigned char)fsum;
        }
    }

   /* for (int j = 0; j < 50; j++)
    {
        for (int i = 0; i < 50; i++)
        {
            printf("%3d ", padding[j * pwidth + i]);
        }
        printf("\n");
    }
    printf("\n\n");*/
    
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y[j * width + i] = padding[(j + psize) * pwidth + (i + psize)];
        }
    }

  /*  for (int j = 0; j < 50; j++)
    {
        for (int i = 0; i < 50; i++)
        {
            printf("%3d ", Y[j * width + i]);
        }
        printf("\n");
    }
    printf("\n\n");*/

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            outputImg[j * stride + i * 3 + 0] = Y[j * width + i];
            outputImg[j * stride + i * 3 + 1] = Y[j * width + i];
            outputImg[j * stride + i * 3 + 2] = Y[j * width + i];
        }
    }

    FILE* outputFile = fopen("./image/Output_Mean_Filter.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);

    free(Y);
    free(padding);

}

void median_filter()
{
    FILE* inputFile = NULL;
    inputFile = fopen("./image/original/AICenterY_CombinedNoise.bmp", "rb");

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

    // Y 배열은 R,G,B 가 아니라 Y 하나만 취급하기 때문에 size / 3.
    // >>>> inputImg, outputImg 의 크기와 Y 의 크기가 다르다 !!!!!!!!!!!!!! <<<<
    unsigned char* Y = NULL;
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            // Luminance 파일을 input으로 받았기 때문에 R, G, B 모두 Y값을 갖고 있음. 셋중 아무거나 골라서 Y배열에 넣음.
            Y[j * width + i] = inputImg[j * stride + 3 * i + 0];
        }
    }

    // padding의 width와 Y의 width 길이는 다르다!
    int psize = 1;
    int pheight = height + (2 * psize);
    int pwidth = width + (2 * psize);

    unsigned char* padding = NULL;
    // 위아래로 psize칸 패딩
    padding = (unsigned char*)calloc(pheight * pwidth, sizeof(unsigned char));

    // padding 위아래 채우기
    for (int i = 0; i < width; i++)
    {
        padding[i + psize] = Y[i];
        padding[((pheight - 1) * pwidth) + (i + psize)] = Y[(height - 1) * width + i];
    }

    // padding 양옆 채우기
    for (int j = 0; j < height; j++)
    {
        padding[(j + psize) * pwidth] = Y[j * width];
        padding[(j + psize) * pwidth + (pwidth - 1)] = Y[j * width + (width - 1)];
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            padding[(j + psize) * pwidth + (i + psize)] = Y[j * width + i];
        }
    }

    int filter[9];
    int fsize = 3;
    int fmid = (fsize * fsize) / 2;
    int cnt;

    for (int j = 0; j <= (pheight - fsize); j++)
    {
        for (int i = 0; i <= (pwidth - fsize); i++)
        {

            for (int y = 0, cnt = 0; y < fsize; y++)
            {
                for (int x = 0; x < fsize; x++)
                {
                    filter[cnt++] = (int)padding[(j + y) * pwidth + (i + x)];
                }
            }
            qsort(filter, fsize * fsize, sizeof(filter[0]), compare);

            if (filter[0] == 0 || filter[(fsize * fsize) - 1] == 255)
            {
                cnt = filter[fmid];
                padding[(j + 1) * pwidth + (i + 1)] = (unsigned char)cnt;
            }
            else
                continue;
        }
    }


    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y[j * width + i] = padding[(j + psize) * pwidth + (i + psize)];
        }
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            outputImg[j * stride + i * 3 + 0] = Y[j * width + i];
            outputImg[j * stride + i * 3 + 1] = Y[j * width + i];
            outputImg[j * stride + i * 3 + 2] = Y[j * width + i];
        }
    }

    FILE* outputFile = fopen("./image/Output_Median_Filter.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);

    free(Y);
    free(padding);

}

void gau_filter()
{
    FILE* inputFile = NULL;
    inputFile = fopen("./image/original/AICenterY_CombinedNoise.bmp", "rb");

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

    // Y 배열은 R,G,B 가 아니라 Y 하나만 취급하기 때문에 size / 3.
    // >>>> inputImg, outputImg 의 크기와 Y 의 크기가 다르다 !!!!!!!!!!!!!! <<<<
    unsigned char* Y = NULL;
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            // Luminance 파일을 input으로 받았기 때문에 R, G, B 모두 Y값을 갖고 있음. 셋중 아무거나 골라서 Y배열에 넣음.
            Y[j * width + i] = inputImg[j * stride + 3 * i + 0];
        }
    }

    // padding의 width와 Y의 width 길이는 다르다!
    int psize = 1;
    int pheight = height + (2 * psize);
    int pwidth = width + (2 * psize);

    int filter[3][3] = { {1, 2, 1}, {2, 4, 1},{1, 2, 1} };
    int fsum = 0;
    int fsize = 3;

    unsigned char* padding = NULL;
    // 위아래로 psize칸 패딩
    padding = (unsigned char*)calloc(pheight * pwidth, sizeof(unsigned char));

    // padding 위아래 채우기
    for (int i = 0; i < width; i++)
    {
        padding[i + psize] = Y[i];
        padding[((pheight - 1) * pwidth) + (i + psize)] = Y[(height - 1) * width + i];
    }

    // padding 양옆 채우기
    for (int j = 0; j < height; j++)
    {
        padding[(j + psize) * pwidth] = Y[j * width];
        padding[(j + psize) * pwidth + (pwidth - 1)] = Y[j * width + (width - 1)];
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            padding[(j + psize) * pwidth + (i + psize)] = Y[j * width + i];
        }
    }

    for (int j = 0; j <= (pheight - fsize); j++)
    {
        for (int i = 0; i <= (pwidth - fsize); i++)
        {
            fsum = 0;
            for (int y = 0; y < fsize; y++)
            {
                for (int x = 0; x < fsize; x++)
                {
                    fsum += filter[y][x] * (int)padding[(j + y) * pwidth + (i + x)];
                }
            }
            fsum /= 16;

            padding[(j + 1) * pwidth + (i + 1)] = (unsigned char)fsum;
        }
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y[j * width + i] = padding[(j + psize) * pwidth + (i + psize)];
        }
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            outputImg[j * stride + i * 3 + 0] = Y[j * width + i];
            outputImg[j * stride + i * 3 + 1] = Y[j * width + i];
            outputImg[j * stride + i * 3 + 2] = Y[j * width + i];
        }
    }

    FILE* outputFile = fopen("./image/Output_gau_Filter.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);

    free(Y);
    free(padding);
}