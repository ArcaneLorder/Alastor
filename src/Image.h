#pragma once
#include "Common.h"
#include "String.h"

namespace Alastor
{

	typedef struct _RGB {
		uint8_t r, g, b;
		// uint8_t r, g, b;
		_RGB() : r(0), g(0), b(0) {}
		_RGB(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
		_RGB(uint32_t hexColor) {
			// 假设 hexColor 是以 0xRRGGBB 格式给出的  
			r = static_cast<uint8_t>((hexColor >> 16) & 0xFF);
			g = static_cast<uint8_t>((hexColor >> 8) & 0xFF);
			b = static_cast<uint8_t>(hexColor & 0xFF);
		}
		operator uint32_t() const {
			return (static_cast<uint32_t>(r) << 16) |
				(static_cast<uint32_t>(g) << 8) |
				static_cast<uint32_t>(b);
		}
		bool operator==(_RGB v) const
		{
			if (r != v.r || g != v.g || b != v.b)
			{
				return false;
			}
			return true;
		}
		bool operator==(uint32_t hexColor) const {
			// 假设 hexColor 是以 0xRRGGBB 格式给出的  
			uint8_t hexR = (hexColor >> 16) & 0xFF;
			uint8_t hexG = (hexColor >> 8) & 0xFF;
			uint8_t hexB = hexColor & 0xFF;
			return r == hexR && g == hexG && b == hexB;
		}
	}RGB;

	typedef enum _Type
	{
		Unknown,
		JPG,
		BMP,
		PNG,
		GIF,
		TIFF,
		ICO,
		TGA,
		CUR,
		PCX,
		IFF,
		ANI
	}Type;

	class ALASTOR_API Image
	{
	public:
		Image(String path);
		Image(ByteArray bytes);
	public:
		size_t size();
		int width();
		int height();
		RGB pixel(int x, int y);
		Type type();
		String typeName();
	private:
		ByteArray m_bytes;
	};
}

