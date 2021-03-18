#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <time.h>

// 사진에 대한 정보를 헤더에서 뽑아와서 사용한다.
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