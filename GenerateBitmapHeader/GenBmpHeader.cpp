#include <stdio.h>
#include <windows.h>

void showBmpHead(BITMAPFILEHEADER* pBmpHead)
{
    printf("位图文件头:\n");
    unsigned char* type = (unsigned char*)(&(pBmpHead->bfType));
    printf("bmp type: %c%c\n", type[0], type[1]);
    printf("文件大小: %d\n", pBmpHead->bfSize);
    printf("保留字: %d\n", pBmpHead->bfReserved1);
    printf("保留字: %d\n", pBmpHead->bfReserved2);
    printf("实际位图数据的偏移字节数: %d\n", pBmpHead->bfOffBits);
}

void showBmpInforHead(BITMAPINFOHEADER* pBmpInforHead)
{
    printf("位图信息头:\n");
    printf("结构体的长度: %d\n", pBmpInforHead->biSize);
    printf("位图宽: %d\n", pBmpInforHead->biWidth);
    printf("位图高: %d\n", pBmpInforHead->biHeight);
    printf("biPlanes平面数: %d\n", pBmpInforHead->biPlanes);
    printf("biBitCount采用颜色位数: %d\n", pBmpInforHead->biBitCount);
    printf("压缩方式: %d\n", pBmpInforHead->biCompression);
    printf("biSizeImage实际位图数据占用的字节数: %d\n", pBmpInforHead->biSizeImage);
    printf("image width stride: %d\n", pBmpInforHead->biSizeImage / pBmpInforHead->biHeight);
    int stride = ((((pBmpInforHead->biWidth * pBmpInforHead->biBitCount) + 31) & ~31) >> 3);
    printf("calculated image width stride: %d\n", stride);
    printf("X方向分辨率: %d\n", pBmpInforHead->biXPelsPerMeter);
    printf("Y方向分辨率: %d\n", pBmpInforHead->biYPelsPerMeter);
    printf("使用的颜色数: %d\n", pBmpInforHead->biClrUsed);
    printf("重要颜色数: %d\n", pBmpInforHead->biClrImportant);
}

// bitmap reference url
// https://www.cnblogs.com/l2rf/p/5643352.html

int main(int argc, char* argv[]) {
    int width = 1080;
    int height = 1920;
    int bits = 32; // RGBA
    const char* out_filename = "default.bmp_header";

    printf("usage: %s width height bits_per_pixel out_filename\n\n", argv[0]);
    if (argc >= 5) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        bits = atoi(argv[3]);
        out_filename = argv[4];
    }
    printf("\nparameters, width: %d, height: %d, bits: %d, out_filename:%s\n", width, height, bits, out_filename);

    // generate BITMAPFILEHEADER
    BITMAPFILEHEADER file_header = { 0 };
    unsigned char* type = (unsigned char*)(&file_header.bfType);
    type[0] = 'B';
    type[1] = 'M';
    file_header.bfSize = 0;  // update later
    file_header.bfReserved1 = 0;
    file_header.bfReserved2 = 0;
    file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  // no RGBQUAD data


    // generate BITMAPINFOHEADER
    BITMAPINFOHEADER bmp_header = {0};
    bmp_header.biSize = sizeof(BITMAPINFOHEADER);
    bmp_header.biWidth = width;
    bmp_header.biHeight = height;
    bmp_header.biPlanes = 1;
    bmp_header.biBitCount = bits;
    bmp_header.biCompression = BI_RGB;

    // bytes per line
    // https://en.wikipedia.org/wiki/BMP_file_format
    // https://zhuanlan.zhihu.com/p/25119530
    // 
    // stride = ((BitsPerPixel * ImageWidth + 31)/32) * 4
    //
    int stride = ((((width * bits) + 31) & ~31) >> 3);
    int image_size = stride * height;
    int file_size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + image_size;
    printf("bytes per line, stride: %d, image_size: %d, file_size: %d\n", stride, image_size, file_size);
    bmp_header.biSizeImage = image_size;
    // Print resolution of the image, 72 DPI × 39.3701 inches per metre yields 2834.6472
    bmp_header.biXPelsPerMeter = 2835;
    bmp_header.biYPelsPerMeter = 2835;
    bmp_header.biClrUsed = 0;
    bmp_header.biClrImportant = 0;

    // update bmp file size
    file_header.bfSize = file_size;


    FILE* fd = fopen(out_filename, "wb");
    if (fd == NULL) {
        printf("open output file: %s failed.\n", out_filename);
        return 1;
    }

    int w_file_header = fwrite(&file_header, 1, sizeof(file_header), fd);
    int w_bmp_header = fwrite(&bmp_header, 1, sizeof(bmp_header), fd);
    fclose(fd);

    bool success = (w_file_header == sizeof(file_header) && w_bmp_header == sizeof(bmp_header));
    printf("write bmp header to file: %s, %s.\n", out_filename, success ? "Successed": "Failed");

    return success ? 0 : 1;    
}