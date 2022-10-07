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

int main(int argc, char* argv[]) {
    printf("usage: %s bmp-filename\n\n", argv[0]);
    if (argc < 2) return 1;
    char* filename = argv[1];

    // show bitmap header

    FILE* fd = fopen(filename, "rb");
    if (fd == NULL) {
        printf("read file failed.");
        return 1;
    }

    int len = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    unsigned char* buff = new unsigned char[len];
    int r = fread(buff, len, 1, fd);
    BITMAPFILEHEADER* file_header = (BITMAPFILEHEADER*)buff;
    printf("show image file header:\n");
    showBmpHead(file_header);

    printf("\n------------------\n");
    BITMAPINFOHEADER* bmp_header = (BITMAPINFOHEADER*)(buff + sizeof(BITMAPFILEHEADER));
    printf("show bitmap header:\n");
    showBmpInforHead(bmp_header);
    delete[] buff;
    fclose(fd);
	return 0;
}