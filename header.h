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

// ************

void PSNR(char* address);
void mean_filter(char* address);
void median_filter(char* address);
void gau_filter(char* address);

// ************

void rgb_color_test();
void ycbcr_color_test();
void HSI();
void YI_diff();
void watermark();
void masking();
void random_noise_generate();
void sp_noise_generate();

// *************

void histogram(char* address, char* output);
void ns_histogram(char* address, char* output);
void threshold(char* address);
void stretch_histogram(char* address);

// *************

void gamma_encoding(char* address, char* output, double gamma);

// *************

void down_sampling_sub(char* address, char* output, int ratio);
void down_sampling_avg(char* address, char* output, int ratio);
void up_sampling_nearest(char* address, char* output, int ratio);
void up_sampling_biint(char* address, char* output, int ratio);
void upsampling_midterm(char* address, char* output, int ratio, int psize);

//

void edge(char* address);
void compression(char* address);
void decoding(char* address);