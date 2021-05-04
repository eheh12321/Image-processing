#include "header.h"

void edge(char* address)
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

    unsigned char* outputImg = NULL, *outputImg2 = NULL, *outputImg3 = NULL;
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg3 = (unsigned char*)calloc(size, sizeof(unsigned char));

    // Y 배열은 R,G,B 가 아니라 Y 하나만 취급하기 때문에 size / 3.
    // >>>> inputImg, outputImg 의 크기와 Y 의 크기가 다르다 !!!!!!!!!!!!!! <<<<
    unsigned char* Y = NULL, * Y2 = NULL;
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));
    Y2 = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

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
    
    int pi, pj;
    double value;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            pj = j + psize;
            pi = i + psize;

            value = (padding[pj * pwidth + (pi + 1)] - padding[pj * pwidth + (pi - 1)]) / 2;
            value > 255 ? (value = 255) : (value < 0 ? (value = 0) : value);

            Y[j * width + i] = (unsigned char)value;

            value = (padding[(pj + 1) * pwidth + pi] - padding[(pj - 1) * pwidth + pi]) / 2;
            value > 255 ? (value = 255) : (value < 0 ? (value = 0) : value);

            Y2[j * width + i] = (unsigned char)value;
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

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            outputImg2[j * stride + i * 3 + 0] = Y2[j * width + i];
            outputImg2[j * stride + i * 3 + 1] = Y2[j * width + i];
            outputImg2[j * stride + i * 3 + 2] = Y2[j * width + i];
        }
    }

    int gx, gy;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            gx = Y[j * width + i];
            gy = Y2[j * width + i];

            value = sqrt(gx * gx + gy * gy);
            value > 255 ? (value = 255) : (value < 0 ? (value = 0) : value);

            outputImg3[j * stride + i * 3 + 0] = (unsigned char)value;
            outputImg3[j * stride + i * 3 + 1] = (unsigned char)value;
            outputImg3[j * stride + i * 3 + 2] = (unsigned char)value;
        }
    }
    FILE* outputFile = fopen("./image/Output_Gx.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    outputFile = fopen("./image/Output_Gy.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg2, sizeof(unsigned char), size, outputFile);

    outputFile = fopen("./image/Output_Edge.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg3, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    free(outputImg2);
    free(outputImg3);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);

    free(Y);
    free(Y2);
    free(padding);

}

void sobel_edge(char* address)
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

    unsigned char* outputImg = NULL, * outputImg2 = NULL, * outputImg3 = NULL;
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg3 = (unsigned char*)calloc(size, sizeof(unsigned char));

    // Y 배열은 R,G,B 가 아니라 Y 하나만 취급하기 때문에 size / 3.
    // >>>> inputImg, outputImg 의 크기와 Y 의 크기가 다르다 !!!!!!!!!!!!!! <<<<
    unsigned char* Y = NULL, * Y2 = NULL;
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));
    Y2 = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

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

    // ***************************************************************

    int filter_x[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    int filter_y[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
    int pi, pj;
    double value = 0;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            pi = psize + i;
            pj = psize + j;
            value = 0;

            for (int k = 0; k < 3; k++)
            {
                for (int c = 0; c < 3; c++)
                {
                    value += (filter_x[k][c] * (double)padding[(pj - 1 + k) * pwidth + (pi - 1 + c)]);
                }
            }
            value > 255 ? (value = 255) : (value < 0 ? (value = 0) : value);
            Y[j * width + i] = (unsigned char)value;

            value = 0;

            for (int k = 0; k < 3; k++)
            {
                for (int c = 0; c < 3; c++)
                {
                    value += (filter_y[k][c] * (double)padding[(pj - 1 + k) * pwidth + (pi - 1 + c)]);
                }
            }
            value > 255 ? (value = 255) : (value < 0 ? (value = 0) : value);
            Y2[j * width + i] = (unsigned char)value;
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

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            outputImg2[j * stride + i * 3 + 0] = Y2[j * width + i];
            outputImg2[j * stride + i * 3 + 1] = Y2[j * width + i];
            outputImg2[j * stride + i * 3 + 2] = Y2[j * width + i];
        }
    }

    double gx, gy;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            gx = (double)Y[j * width + i];
            gy = (double)Y2[j * width + i];

            value = sqrt(pow(gx, 2) + pow(gy, 2));
            value > 255 ? (value = 255) : (value < 0 ? (value = 0) : value);

            outputImg3[j * stride + i * 3 + 0] = (unsigned char)value;
            outputImg3[j * stride + i * 3 + 1] = (unsigned char)value;
            outputImg3[j * stride + i * 3 + 2] = (unsigned char)value;
        }
    }

    FILE* outputFile = fopen("./image/Output_sobel_Gx.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    outputFile = fopen("./image/Output_sobel_Gy.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg2, sizeof(unsigned char), size, outputFile);

    outputFile = fopen("./image/Output_sobel_Edge.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg3, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    free(outputImg2);
    free(outputImg3);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);

    free(Y);
    free(Y2);
    free(padding);

}

void prewitt_edge(char* address)
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

    // Y 배열은 R,G,B 가 아니라 Y 하나만 취급하기 때문에 size / 3.
    // >>>> inputImg, outputImg 의 크기와 Y 의 크기가 다르다 !!!!!!!!!!!!!! <<<<
    unsigned char* Y = NULL, * Y2 = NULL;
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));
    Y2 = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

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

    // ***************************************************************

    int filter_x[3][3] = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
    int filter_y[3][3] = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
    int pi, pj;
    double value = 0;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            pi = psize + i;
            pj = psize + j;
            value = 0;

            for (int k = 0; k < 3; k++)
            {
                for (int c = 0; c < 3; c++)
                {
                    value += (filter_x[k][c] * (double)padding[(pj - 1 + k) * pwidth + (pi - 1 + c)]);
                }
            }
            value > 255 ? (value = 255) : (value < 0 ? (value = 0) : value);
            Y[j * width + i] = (unsigned char)value;

            value = 0;

            for (int k = 0; k < 3; k++)
            {
                for (int c = 0; c < 3; c++)
                {
                    value += (filter_y[k][c] * (double)padding[(pj - 1 + k) * pwidth + (pi - 1 + c)]);
                }
            }
            value > 255 ? (value = 255) : (value < 0 ? (value = 0) : value);
            Y2[j * width + i] = (unsigned char)value;
        }
    }

    double gx, gy;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            gx = (double)Y[j * width + i];
            gy = (double)Y2[j * width + i];

            value = sqrt(pow(gx, 2) + pow(gy, 2));
            value > 255 ? (value = 255) : (value < 0 ? (value = 0) : value);

            outputImg[j * stride + i * 3 + 0] = (unsigned char)value;
            outputImg[j * stride + i * 3 + 1] = (unsigned char)value;
            outputImg[j * stride + i * 3 + 2] = (unsigned char)value;
        }
    }

    FILE* outputFile = fopen("./image/Output_prewitt_Gx.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);

    free(Y);
    free(Y2);
    free(padding);

}

void edge_thresholding(char* address, int thr)
{
    FILE* inputFile = NULL;
    inputFile = fopen(address, "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage; // height * width * 3 (R,G,B) !!!
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4; // (width * 3)

    unsigned char* inputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

    unsigned char* Y = NULL;
    Y = (unsigned char*)calloc(size / 3, sizeof(unsigned char));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Y[j * width + i] = inputImg[j * stride + 3 * i + 0];
        }
    }

    int value;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            if ((int)Y[j * width + i] < thr)
            {
                value = 0;
            }
            else
            {
                value = Y[j * width + i];
            }

            outputImg[j * stride + 3 * i + 0] = (unsigned char)value;
            outputImg[j * stride + 3 * i + 1] = (unsigned char)value;
            outputImg[j * stride + 3 * i + 2] = (unsigned char)value;
        }
    }

    FILE* outputFile = fopen("./image/Output_Edge_thresholding.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    fclose(outputFile);

    free(inputImg);
    fclose(inputFile);

    free(Y);

}