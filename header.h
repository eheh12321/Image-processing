#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <time.h>

// ������ ���� ������ ������� �̾ƿͼ� ����Ѵ�.
BITMAPFILEHEADER bmpFile;
BITMAPINFOHEADER bmpInfo;


FILE* open_image();

void rgb_color_test();
void ycbcr_color_test();
void HSI();
void YI_diff();
void watermark();
void filtering();
void PSNR(char* address);
void random_noise_generate();
void sp_noise_generate();