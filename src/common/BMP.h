#pragma once

#pragma pack(push,2)
typedef struct _BITMAPFILEHEADER { // BMP文件头结构体定义
    unsigned short bfType;      // 2字节，表示文件类型（必须为"BM")
    unsigned int   bfSize;       // 4字节，表示文件大小
    unsigned short bfReserved1;  // 2字节，保留位
    unsigned short bfReserved2;  // 2字节，保留位
    unsigned int   bfOffBits;     // 4字节，从文件开始到像素数组起始处的偏移量
}BITMAPFILEHEADER;


typedef struct _BITMAPINFOHEADER { // BMP信息头结构体定义
    unsigned int   biSize;        // 4字节，该结构体所需要的字节数
    signed int     biWidth;       // 4字节，图片宽度
    signed int     biHeight;      // 4字节，图片高度
    unsigned short biPlanes;      // 2字节，目标设备的平面数（通常为1）
    unsigned short biBitCount;    // 2字节，每个像素点的位数
    unsigned int   biCompression; // 4字节，压缩类型（0表示不压缩）
    unsigned int   biSizeImage;   // 4字节，图像数据区的大小
    signed int     biXPelsPerMeter;// 4字节，水平分辨率
    signed int     biYPelsPerMeter;// 4字节，垂直分辨率
    unsigned int   biClrUsed;     // 4字节，使用的调色板索引数
    unsigned int   biClrImportant;// 4字节，重要的调色板索引数
}BITMAPINFOHEADER;
#pragma pack(pop)