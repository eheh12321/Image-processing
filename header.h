#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>

// ������ ���� ������ ������� �̾ƿͼ� ����Ѵ�.
BITMAPFILEHEADER bmpFile;
BITMAPINFOHEADER bmpInfo;

FILE* open_image(FILE *inputFile);

int rgb_color_test();
int ycbcr_color_test();
