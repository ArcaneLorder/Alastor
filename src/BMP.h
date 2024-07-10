#pragma once

#pragma pack(push,2)
typedef struct _BITMAPFILEHEADER { // BMP�ļ�ͷ�ṹ�嶨��
    unsigned short bfType;      // 2�ֽڣ���ʾ�ļ����ͣ�����Ϊ"BM")
    unsigned int   bfSize;       // 4�ֽڣ���ʾ�ļ���С
    unsigned short bfReserved1;  // 2�ֽڣ�����λ
    unsigned short bfReserved2;  // 2�ֽڣ�����λ
    unsigned int   bfOffBits;     // 4�ֽڣ����ļ���ʼ������������ʼ����ƫ����
}BITMAPFILEHEADER;


typedef struct _BITMAPINFOHEADER { // BMP��Ϣͷ�ṹ�嶨��
    unsigned int   biSize;        // 4�ֽڣ��ýṹ������Ҫ���ֽ���
    signed int     biWidth;       // 4�ֽڣ�ͼƬ���
    signed int     biHeight;      // 4�ֽڣ�ͼƬ�߶�
    unsigned short biPlanes;      // 2�ֽڣ�Ŀ���豸��ƽ������ͨ��Ϊ1��
    unsigned short biBitCount;    // 2�ֽڣ�ÿ�����ص��λ��
    unsigned int   biCompression; // 4�ֽڣ�ѹ�����ͣ�0��ʾ��ѹ����
    unsigned int   biSizeImage;   // 4�ֽڣ�ͼ���������Ĵ�С
    signed int     biXPelsPerMeter;// 4�ֽڣ�ˮƽ�ֱ���
    signed int     biYPelsPerMeter;// 4�ֽڣ���ֱ�ֱ���
    unsigned int   biClrUsed;     // 4�ֽڣ�ʹ�õĵ�ɫ��������
    unsigned int   biClrImportant;// 4�ֽڣ���Ҫ�ĵ�ɫ��������
}BITMAPINFOHEADER;
#pragma pack(pop)