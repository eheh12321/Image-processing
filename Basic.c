#include "header.h"

FILE* open_image()
{
    FILE* inputFile = NULL;
    return inputFile = fopen("./image/original/IU_2.bmp", "rb");
}

int main()
{
    rgb_color_test();
    ycbcr_color_test();
    HSI();
    YI_diff();
    watermark();
    masking();
    random_noise_generate();
    sp_noise_generate();

    //  *************************************************************

    // 1. Median filter로 saltpapper noise를 제거한다
    median_filter("./image/Output_SPnoise.bmp");

    printf("\n# Median Filter\n");
    PSNR("./image/Output_Median_Filter.bmp");

    mean_filter("./image/Output_SPnoise.bmp");

    printf("\n# Mean Filter\n");
    PSNR("./image/Output_Mean_Filter.bmp");

    // 2. 그 다음에 mean filter를 돌려서 noise를 한번 더 제거한다

    mean_filter("./image/Output_Median_Filter.bmp");

    printf("\n# Median_Mean Filter\n");
    PSNR("./image/Output_Mean_Filter.bmp");

    gau_filter("./image/Output_Median_Filter.bmp");

    printf("\nGau Filter\n");
    PSNR("./image/Output_gau_Filter.bmp");

   //  *************************************************************

    histogram("./image/Output_Y.bmp");

    return 0;
}
