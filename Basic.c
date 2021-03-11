#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>

int main()
{
    // ������ ���� ������ ������� �̾ƿͼ� ����Ѵ�.
    BITMAPFILEHEADER bmpFile;
    BITMAPINFOHEADER bmpInfo;
    
    // ���� x ���� �ؼ� ������ �迭�� �ƴϴ�..! ������.
    FILE* inputFile = NULL;

    // "rb" : �б���
    inputFile = fopen("./image/AICenter.bmp", "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    // ����-���� ���� (512 x 512)
    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;

    // �Ϲ������� ���� x ���� (x 3 : R,G,B)
    int size = bmpInfo.biSizeImage;

    // bit-depth : RGB �����̹Ƿ� 8bit * 3 = 24 => �� �ȼ��� 24bit.
    int bitCnt = bmpInfo.biBitCount;

    // BMP ������ ����ũ�Ⱑ �׻� 4�ǹ��(4Byte ����)�� ����Ǿ� ����
    // ���� ������ ���� ũ�Ⱑ 4�ǹ���� �ȶ������� ��� 
    // (������ ũ�Ⱑ 4byte������ �������ִ� �ص� �ȼ� ���� ��ü�� 4byte ������ ������ �ȵ����� �� �ִ�)
    // (= �̹����� ����ũ��� ������ ����ũ�Ⱑ �ٸ� �� �ִ�..?)
    // 4byte ����� �������, processing �Ҷ��� width ��ŭ�� �ݺ�.
    // ������ ����ũ�⸦ 4�� ����� �����ִ� �۾�.
    // + 3�� �ϴ� ���� => �ø��� �ϱ� ����
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
    
    printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);

    // unsigned char�� ���� ���� -> �ϳ��� �ȼ��� 0~255�� ǥ���ϴµ�
    // unsigned char�� �ڷ��� ũ�Ⱑ �� 0~255�� �¾Ƽ�.
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
            // stride = ������ ���α���. j++�� �Ǿ� j * stride�� �ϸ� �����ٷ� �������� ������ �ȴ�.
            // (������ �迭�̴ϱ�!)
            // �� �ȼ��� B, G, R ������ ������Ұ� �����Ƿ�!! *�߿�

            // Y : ��. RGB�� ��� Y���� ������ �������� �ȴ�.
            Y = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0];
            Cb = -0.169 * inputImg[j * stride + 3 * i + 2] - 0.331 * inputImg[j * stride + 3 * i + 1] + 0.500 * inputImg[j * stride + 3 * i + 0];
            Cr = 0.500 * inputImg[j * stride + 3 * i + 2] - 0.419 * inputImg[j * stride + 3 * i + 1] + 0.0813 * inputImg[j * stride + 3 * i + 0];

            // I
            I = (unsigned char)(((inputImg[j * stride + 3 * i + 0]) + (inputImg[j * stride + 3 * i + 1]) + (inputImg[j * stride + 3 * i + 2])) / 3);

            // RGB�� ���� ���� ���� ������ Gray tone�� �ȴ� (���� > �Ͼ� �� ����)
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