#include "header.h"

FILE* open_image()
{
    FILE* inputFile = NULL;
    return inputFile = fopen("./image/original/AICenter.bmp", "rb");
}

int main()
{
    PSNR("./image/original/AICenterY_CombinedNoise.bmp");

    rgb_color_test();
    ycbcr_color_test();
    HSI();
    YI_diff();
    watermark();
    masking();
    random_noise_generate();
    sp_noise_generate();

    printf("\nSalt-Pepper Noise\n");
    PSNR("./image/Output_SPnoise.bmp");

    printf("\nRandom Noise\n");
    PSNR("./image/Output_Rnoise.bmp");

    // 1. Median filter�� saltpapper noise�� �����Ѵ�
    median_filter("./image/original/AICenterY_CombinedNoise.bmp");

    printf("\nMedian Filter\n");
    PSNR("./image/Output_Median_Filter.bmp");

    // 2. �� ������ mean filter�� ������ noise�� �ѹ� �� �����Ѵ�

    mean_filter("./image/Output_Median_Filter.bmp");

    printf("\nMedian_Mean Filter\n");
    PSNR("./image/Output_Mean_Filter.bmp");

    gau_filter("./image/Output_Median_Filter.bmp");

    printf("\nGau Filter\n");
    PSNR("./image/Output_gau_Filter.bmp");

    return 0;
}
