#ifndef _FUNCTION_H
#define _FUNCTION_H
#include <stdint.h>
#pragma pack (1)
struct bmpsign{
	unsigned char data[2];
};
#pragma pack (1)
struct Header{
	bmpsign signature;
	int32_t size;
	int16_t reserved1;
	int16_t reserved2;
	int32_t PAaddr;
};
#pragma pack (1)
struct BmpDib{
	uint32_t dibsize;
	int32_t ImageWidth;
	int32_t ImageHeight;
	uint16_t colorPlane;
	uint16_t pixelSize;
	uint32_t compress;
	uint32_t PASize;
	int32_t NgangPG;
	int32_t DocPG;
	uint32_t ColorCount;
	uint32_t MainColorCount;
};
#pragma pack (1)
struct Color{
	uint8_t blue;
	uint8_t green;
	uint8_t red;
};
#pragma pack (1)
struct pixelArray{
	uint32_t row;
	uint32_t col;
	Color** pixels;
};
#pragma pack (1)
struct BMP{
	Header head;
	BmpDib dib;
};

struct Inf{
	char a[50];
	char b[50];
	Color in, out;
};
void InputInf(FILE* p, Inf &ttin);
bool IsBMP(FILE* f);
void ReadBMP(FILE* f, BMP &bitmap);
void OutputInf(BMP bitmap, FILE* outfile);
void ChangeColor(pixelArray &data, Color in, Color out);
void ReadPixelArray(FILE* f, BMP &bitmap, pixelArray &data);
void CopyBMP(BMP &bitmap, pixelArray &data, FILE* p);
void outputBmp(pixelArray &pixelArray);
void outputBmpBW(pixelArray &pixelArray, Color **&a);
void free(pixelArray data);
void freeBW(pixelArray data, Color **&a);
void ChangeToBlackandWhite(pixelArray &data, Color **&a);
void resizeConsole(int width, int height);
void CopyBMPBW(BMP &bitmap, pixelArray &data, FILE* p, Color **&a);
void Nhac();
#endif