#include "header.h"

#define address "./image/original/IU.bmp"

int main()
{
    FILE* inputFile = NULL;
    inputFile = fopen(address, "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    fclose(inputFile);

    rgb_color_test(address);
    ycbcr_color_test(address);
    HSI(address);
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

    histogram("./image/Output_Y.bmp", "./image/Output_histogram.bmp");
    ns_histogram("./image/Output_Y.bmp", "./image/Output_ns_histogram.bmp");
    threshold("./image/Output_Y.bmp");

    stretch_histogram("./image/Output_Y.bmp");
    histogram("./image/Output_Stretched_histogram.bmp", "./image/Output_histogram_s.bmp");

    // ***************************************************************

    gamma_encoding("./image/Output_Y.bmp", "./image/Output_gamma_encoding.bmp", 2.5);
    gamma_encoding("./image/Output_gamma_encoding.bmp", "./image/Output_gamma_encoding_2.bmp", 0.4);

    printf("\nGamma correction\n"); // (Gamma correction을 한다고 원본이 되지는 않음)
    PSNR("./image/Output_gamma_encoding_2.bmp");

    // ****************************************************************

    down_sampling_sub("./image/Output_Y.bmp", "./image/Output_dsampling_sub.bmp", 2);
    down_sampling_avg("./image/Output_Y.bmp", "./image/Output_dsampling_avg.bmp", 2);

    up_sampling_nearest("./image/Output_dsampling_sub.bmp", "./image/Output_Usampling_dsub.bmp", 2);
    printf("\nUpsampling after Downsampling(subsampling)\n");
    PSNR("./image/Output_Usampling_dsub.bmp");

    up_sampling_nearest("./image/Output_dsampling_avg.bmp", "./image/Output_Usampling_davg.bmp", 2);
    printf("\nUpsampling after Downsampling(average dowmsampling)\n");
    PSNR("./image/Output_Usampling_davg.bmp");

    //up_sampling_nearest("./image/original/AICenterY_Subsampling.bmp", "./image/Output_Usampling_n.bmp", 1);
    //printf("\nUpsampling after Downsampling\n");
    //PSNR("./image/Output_Usampling_n.bmp");

    //up_sampling_biint("./image/original/AICenterY_Subsampling.bmp", "./image/Output_Usampling_b.bmp", 1);
    //printf("\nUpsampling after Downsampling\n");
    //PSNR("./image/Output_Usampling_b.bmp");

    // ****************************************************************

    //up_sampling_nearest("./image/original/AICenterY_128X128.bmp", "./image/Output_Usampling_n.bmp", 2);
    //printf("\nNEAREST UPSCAILING\n");
    //PSNR("./image/Output_Usampling_n.bmp");

    //upsampling_midterm("./image/original/AICenterY_128X128.bmp", "./image/midterm.bmp", 2, 4);
    //printf("\nRESULT\n");
    //PSNR("./image/midterm.bmp");

    // ****************************************************************

    edge("./image/Output_Y.bmp");
    sobel_edge("./image/Output_Y.bmp");
    prewitt_edge("./image/Output_Y.bmp");
    edge_thresholding("./image/Output_sobel_Edge.bmp", 150);

    printf("\n* Edge detection complete! *\n");

    // ****************************************************************    
    
    compression("./image/Output_Y.bmp");
    decoding("bitstream.txt", bmpFile, bmpInfo);

    printf("\nDecoding\n");
    PSNR("./image/decoding.bmp");

    quantization_comp("./image/Output_Y.bmp", "bitstream_test.txt");
    printf("\nQuantization\n");
    PSNR("./image/quantization.bmp");

    test_decoding("bitstream_test.txt", bmpFile, bmpInfo);
    printf("\ntest_Decoding\n");
    PSNR("./image/test_decoding.bmp");

    // **************************************************************** 

    Jalhaja_horizontal("./image/Output_Y.bmp", "./image/Jalhaza_horizontal.bmp");
    printf("\nJalhaja_horizontal\n");
    PSNR("./image/Jalhaza_horizontal.bmp");

    Jalhaja_vertical("./image/Output_Y.bmp", "./image/Jalhaza_vertical.bmp");
    printf("\nJalhaja_vertical\n");
    PSNR("./image/Jalhaza_vertical.bmp");

    printf("\n* Processing Complete! *\n");

    return 0;
}
