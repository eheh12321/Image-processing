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
    inputFile = fopen("AICenter.bmp", "rb");

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
    unsigned char* inputImg = NULL, * outputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    fread(inputImg, sizeof(unsigned char), size, inputFile);

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            // stride = ������ ���α���. j++�� �Ǿ� j * stride�� �ϸ� �����ٷ� �������� ������ �ȴ�.
            // (������ �迭�̴ϱ�!)
            // �� �ȼ��� B, G, R ������ ������Ұ� �����Ƿ�!! *�߿�
            outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];
            outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];
            outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];
        }
    }

    FILE* outputFile = fopen("Output.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    free(inputImg);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}