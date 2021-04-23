#include "header.h"

FILE* open_image()
{
    FILE* inputFile = NULL;
    return inputFile = fopen("./image/original/AICenter.bmp", "rb");
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

    // 1. Median filter�� saltpapper noise�� �����Ѵ�
    median_filter("./image/Output_SPnoise.bmp");

    printf("\n# Median Filter\n");
    PSNR("./image/Output_Median_Filter.bmp");

    mean_filter("./image/Output_SPnoise.bmp");

    printf("\n# Mean Filter\n");
    PSNR("./image/Output_Mean_Filter.bmp");

    // 2. �� ������ mean filter�� ������ noise�� �ѹ� �� �����Ѵ�

    mean_filter("./image/Output_Median_Filter.bmp");

    printf("\n# Median_Mean Filter\n");
    PSNR("./image/Output_Mean_Filter.bmp");

    gau_filter("./image/Output_Median_Filter.bmp");

    printf("\nGau Filter\n");
    PSNR("./image/Output_gau_Filter.bmp");

   //  *************************************************************

    histogram("./image/Output_Y.bmp", "./image/Output_histogram.bmp");
    ns_histogram("./image/Output_Y.bmp", "./image/Output_ns_histogram.bmp");
    threshold("./image/Output_Y.bmp");

    stretch_histogram("./image/Output_Y.bmp");
    histogram("./image/Output_Stretched_histogram.bmp", "./image/Output_histogram_s.bmp");

    // ***************************************************************

    gamma_encoding("./image/Output_Y.bmp", "./image/Output_gamma_encoding.bmp", 2.5);
    gamma_encoding("./image/Output_gamma_encoding.bmp", "./image/Output_gamma_encoding_2.bmp", 0.4);

    printf("\nGamma correction\n"); // (Gamma correction�� �Ѵٰ� ������ ������ ����)
    PSNR("./image/Output_gamma_encoding_2.bmp");

    // ****************************************************************

    down_sampling_sub("./image/Output_Y.bmp", "./image/Output_Dsampling.bmp", 1);
    down_sampling_avg("./image/Output_Y.bmp", "./image/Output_Dsampling_a.bmp", 1);

    up_sampling_nearest("./image/original/AICenterY_Subsampling.bmp", "./image/Output_Usampling_n.bmp", 1);
    printf("\nUpsampling after Downsampling\n");
    PSNR("./image/Output_Usampling_n.bmp");

    up_sampling_biint("./image/original/AICenterY_Subsampling.bmp", "./image/Output_Usampling_b.bmp", 1);
    printf("\nUpsampling after Downsampling\n");
    PSNR("./image/Output_Usampling_b.bmp");

    // ****************************************************************

    up_sampling_nearest("./image/original/AICenterY_128X128.bmp", "./image/Output_Usampling_n.bmp", 2);
    printf("\nNEAREST UPSCAILING\n");
    PSNR("./image/Output_Usampling_n.bmp");

    midterm_test("./image/original/AICenterY_128X128.bmp", "./image/midterm.bmp", 2, 4);
    printf("\nTEST\n");
    PSNR("./image/midterm.bmp");

    printf("\nProcessing Complete!\n");

    return 0;
}
