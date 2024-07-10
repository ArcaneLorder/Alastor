#include "Image.h"
#include "File.h"
#include "BMP.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#define GETBYTELEN   2
#define IMAGE_YES    1
#define IMAGE_NO     0

/*---JPEG/JPG(1)---*/
#define IMAGE_JPEG_TYPE  0x01
#define IMAGE_JPEG_JPG "JPEG/JPG"
#define JPEG_FIRSTBIT  0xFF
#define JPEG_SECONDBIT 0xD8

/*---BMP(2)---*/
#define IMAGE_BMP_TYPE  0x02
#define IMAGE_BMP     "BMP"
#define BMP_FIRSTBIT  0x42
#define BMP_SECONDBIT 0x4D
/*---PNG(3)---*/
#define IMAGE_PNG_TYPE  0x04
#define IMAGE_PNG     "PNG"
#define PNG_FIRSTBIT  0x89
#define PNG_SECONDBIT 0x50

/*---GIF(4)---*/
#define IMAGE_GIF_TYPE 0x08
#define IMAGE_GIF     "GIF"
#define GIF_FIRSTBIT  0x47
#define GIF_SECONDBIT 0x49
#define GIF_THIRDBIT  0x46
#define GIF_FIFTHBIT1 0x39   // 可能存在是37
#define GIF_FIFTHBIT2 0x37
/*---TIFF(5)---*/
#define IMAGE_TIFF_TYPE  0x10
#define IMAGE_TIFF      "TIFF"
#define TIFF_FIRSTBIT   0x4D
#define TIFF_SECONDBIT  0x4D
#define TIFF_FIRSTBIT2  0x49  // 或者
#define TIFF_SECONDBIT2 0x49

/*---ICO(6)---*/
#define IMAGE_ICO    "ICO"
#define ICO_THIRDBIT 0x1  // 01
#define ICO_FIFTHBIT 0x1  // 01
/*---TGA(7)---*/
#define IMAGE_TGA    "TGA"
#define TGA_THIRDBIT 0x2  // 02
#define TGA_FIFTHBIT 0x0  // 00
/*---CUR(8)---*/
#define IMAGE_CUR    "CUR"
#define CUR_THIRDBIT 0x2  // 02
#define CUR_FIFTHBIT 0x1  // 01
/*---PCX(9)---*/
#define IMAGE_PCX    "PCX"
#define PCX_FIRSTBIT 0xA  // 0A
/*---IFF(10)---*/
#define IMAGE_IFF     "IFF"
#define IFF_FIRSTBIT  0x46
#define IFF_SECONDBIT 0x4F
#define IFF_THIRDBIT  0x52
#define IFF_FORTHBIT  0x4D

/*---ANI(11)---*/
#define IMAGE_ANI     "ANI"
#define ANI_FIRSTBIT  0x52
#define ANI_SECONDBIT 0x49
#define ANI_THIRDBIT  0x46
#define ANI_FORTHBIT  0x46

namespace Alastor
{
	Image::Image(String path)
	{
		File::read(path,m_bytes);
	}
	Image::Image(ByteArray bytes)
	{
		m_bytes = bytes;
	}


	Type Image::type()
	{
		if (m_bytes.size() == NULL) 
		{
			return Unknown;
		}

		if (m_bytes[0] == JPEG_FIRSTBIT && m_bytes[1] == JPEG_SECONDBIT)
		{
			return JPG;
		}

		if (m_bytes[0] == BMP_FIRSTBIT && m_bytes[1] == BMP_SECONDBIT)
		{
			return BMP;
		}

		if (m_bytes[0] == PNG_FIRSTBIT && m_bytes[1] == PNG_SECONDBIT)
		{
			return PNG;
		}

		if (m_bytes[0] == GIF_FIRSTBIT && m_bytes[1] == GIF_SECONDBIT && m_bytes[2] == GIF_THIRDBIT
			&& (m_bytes[3] == GIF_FIFTHBIT1 || m_bytes[3] == GIF_FIFTHBIT2))
		{
			return GIF;
		}

		if ((m_bytes[0] == TIFF_FIRSTBIT && m_bytes[1] == TIFF_SECONDBIT)
			|| (m_bytes[0] == TIFF_FIRSTBIT2 && m_bytes[1] == TIFF_SECONDBIT2))
		{
			return TIFF;
		}

		if (m_bytes[2] == ICO_THIRDBIT && m_bytes[4] == ICO_FIFTHBIT)
		{
			return ICO;
		}

		if (m_bytes[2] == TGA_THIRDBIT && m_bytes[4] == TGA_FIFTHBIT)
		{
			return TGA;
		}

		if (m_bytes[2] == CUR_THIRDBIT && m_bytes[4] == CUR_FIFTHBIT)
		{
			return CUR;
		}

		if (m_bytes[0] == PCX_FIRSTBIT)
		{
			return PCX;
		}

		if (m_bytes[0] == IFF_FIRSTBIT && m_bytes[1] == IFF_SECONDBIT && m_bytes[2] == IFF_THIRDBIT && m_bytes[3] == IFF_FORTHBIT)
		{
			return IFF;
		}

		if (m_bytes[0] == ANI_FIRSTBIT && m_bytes[1] == ANI_SECONDBIT && m_bytes[2] == ANI_THIRDBIT && m_bytes[3] == ANI_FORTHBIT)
		{
			return ANI;
		}
		return Unknown;
	}

	String Image::typeName()
	{
		switch (type())
		{
		case Unknown:
			return "Unknown";
		case JPG:
			return "JPG";
		case BMP:
			return "BMP";
		case PNG:
			return "PNG";
		case GIF:
			return "GIF";
		case TIFF:
			return "TIFF";
		case ICO:
			return "ICO";
		case TGA:
			return "TGA";
		case CUR:
			return "CUR";
		case PCX:
			return "PCX";
		case IFF:
			return "IFF";
		case ANI:
			return "ANI";
		default:
			return "Unknown";
		}
	}

	int Image::width()
	{
		if (type() == BMP)
		{
			BITMAPFILEHEADER bitmapFileHeader; // 创建BMP文件头对象
			BITMAPINFOHEADER bitmapInfoHeader; // 创建BMP信息头对象

			memcpy(&bitmapFileHeader, m_bytes.data(), sizeof(BITMAPFILEHEADER));
			memcpy(&bitmapInfoHeader, m_bytes.data() + sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
			return bitmapInfoHeader.biWidth;
		}
		return 0;
	}

	int Image::height()
	{
		if (type() == BMP)
		{
			BITMAPFILEHEADER bitmapFileHeader; // 创建BMP文件头对象
			BITMAPINFOHEADER bitmapInfoHeader; // 创建BMP信息头对象

			memcpy(&bitmapFileHeader, m_bytes.data(), sizeof(BITMAPFILEHEADER));
			memcpy(&bitmapInfoHeader, m_bytes.data() + sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
			return abs(bitmapInfoHeader.biHeight);
		}
		return 0;
	}

	size_t Image::size()
	{
		return m_bytes.size();
	}

	RGB Image::pixel(int x, int y)
	{
		if (type() != BMP)
		{
			// 如果不是BMP类型，返回黑色或抛出异常  
			return RGB();
		}

		BITMAPFILEHEADER bitmapFileHeader;
		BITMAPINFOHEADER bitmapInfoHeader;

		memcpy(&bitmapFileHeader, m_bytes.data(), sizeof(BITMAPFILEHEADER));
		memcpy(&bitmapInfoHeader, m_bytes.data() + sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));

		// 检查x和y是否在图像范围内  
		if (x >=bitmapInfoHeader.biWidth || y >= abs(bitmapInfoHeader.biHeight))
		{
			// 如果坐标超出范围，返回黑色或抛出异常  
			return RGB();
		}

		// 计算每行像素数据的字节数（考虑对齐）  
		int lineSize = (bitmapInfoHeader.biWidth * bitmapInfoHeader.biBitCount / 8 + 3) / 4 * 4;;
		// 根据图像高度的正负确定数据的起始位置  
		int dataOffset = bitmapFileHeader.bfOffBits;// sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		if (bitmapInfoHeader.biHeight > 0)
		{
			// 正常BMP（从图像底部开始）  
			dataOffset += lineSize * (bitmapInfoHeader.biHeight - y - 1);
		}
		else
		{
			// 一些BMP图像的高度可能是负数（从图像顶部开始）  
			dataOffset += lineSize * y;
		}

		// 计算像素位置并获取RGB值  
		int pixelPos = dataOffset + x * 3; // 24位BMP，每个像素3个字节  
		uint8_t b = m_bytes[pixelPos];
		uint8_t g = m_bytes[pixelPos + 1];
		uint8_t r = m_bytes[pixelPos + 2];
		return RGB(r, g, b);
	}
}

