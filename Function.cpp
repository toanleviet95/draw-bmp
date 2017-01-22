#include <stdio.h>
#include <Windows.h>
#include "Function.h"
#pragma comment(lib,"Winmm.lib")
#pragma once

void resizeConsole(int width, int height) // Sửa lại kích thước Console theo ý muốn
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void InputInf(FILE* p, Inf &ttin){ // Hàm đọc thông tin từ file Bitmap.in
	if (p == NULL) return;
	fseek(p, 0, 0L);
	fgets(ttin.a, 50, p);
	fgets(ttin.b, 50, p);
	unsigned x, y, z, t, u, v;
	fscanf(p, "%d%d%d%d%d%d%d", &x, &y, &z, &t, &u, &v);
	ttin.in.red = x;
	ttin.in.green = y;
	ttin.in.blue = z;
	ttin.out.red = t;
	ttin.out.green = u;
	ttin.out.blue = v;
}
bool IsBMP(FILE* f){ // Hàm kiểm tra BMP
	if (f == NULL) return false;
	bmpsign signature;
	fseek(f, 0, 0L);
	fread(&signature, sizeof(bmpsign), 1, f);
	return signature.data[0] == 'B' && signature.data[1] == 'M';
}

char paddingCount; // Biến padding

void ReadBMP(FILE* f, BMP &bitmap){ // Hàm đọc cấu trúc BMP
	if (f == NULL) return;
	fseek(f, 0, 0L);
	fread(&bitmap, sizeof(BMP), 1, f);
}

void OutputInf(BMP bitmap, FILE* outFile){ // Hàm xuất thông tin ra file Bitmap.out
	fprintf(outFile, "%d ", bitmap.head.size);
	fprintf(outFile, "%d ", bitmap.dib.ImageWidth);
	fprintf(outFile, "%d ", bitmap.dib.ImageHeight);
	fprintf(outFile, "%d ", bitmap.dib.pixelSize);
}

void scanline(FILE* f, Color *&line, uint32_t length){ // Hàm quét dòng
	if (f == NULL) return;
	line = new Color[length];
	fread(line, sizeof(Color), length, f);
}

void ReadPixelArray(FILE* f, BMP &bitmap, pixelArray &data){ // Hàm đọc mảng Pixel
	paddingCount = (4 - (bitmap.dib.ImageWidth*(bitmap.dib.pixelSize / 8) % 4)) % 4;
	data.row = bitmap.dib.ImageHeight;
	data.col = bitmap.dib.ImageWidth;
	data.pixels = new Color*[data.row];
	fseek(f, sizeof(BMP), SEEK_SET);
	for (int i = 0; i<data.row; i++){
		scanline(f, data.pixels[data.row - 1 - i], data.col);
		if (paddingCount != 0) fseek(f, paddingCount, SEEK_CUR);
	}
}

void ChangeColor(pixelArray &data, Color in, Color out){ // Hàm đổi màu
	for (unsigned int i = 0; i<data.row; i++){
		for (unsigned int j = 0; j<data.col; j++){
			if (data.pixels[i][j].red == in.red && data.pixels[i][j].green == in.green && data.pixels[i][j].blue == in.blue)
				data.pixels[i][j] = out;
		}
	}
}

void CopyBMP(BMP &bitmap, pixelArray &data, FILE* p){ // Hàm chép ra file BMP đã được đổi màu 
	fwrite(&bitmap, sizeof(BMP), 1, p);
	for (unsigned int i = 0; i<data.row; i++){
		fwrite(data.pixels[data.row - 1 - i], sizeof(Color), data.col, p);
		uint8_t t = 0;
		if (paddingCount != 0) {
			fwrite(&t, sizeof(uint8_t), paddingCount, p);
		}
	}
}

void outputBmp(pixelArray &pixelArray) // Hàm xuất ảnh ra màn hình
{
	HWND console = GetConsoleWindow();
	HDC hdc = GetDC(console);
	for (int i = 0; i < pixelArray.row; i++){
		for (int j = (pixelArray.col - 1); j >= 0; j--){
			SetPixel(hdc, j, i, RGB(pixelArray.pixels[i][j].red, pixelArray.pixels[i][j].green, pixelArray.pixels[i][j].blue));
		}
		Sleep(10);
	}
	ReleaseDC(console, hdc);
}

void free(pixelArray data){ // Hàm free cho vùng nhớ đã cấp phát
	for (int i = 0; i<data.row; i++)
		delete[] data.pixels[i];
	delete[] data.pixels;
}

void ChangeToBlackandWhite(pixelArray &data, Color **&a){ // Đổi màu ảnh sang trắng đen
	a = new Color*[data.row];
	for (unsigned int i = 0; i<data.row; i++){
		a[i] = new Color[data.col];
		for (unsigned int j = 0; j<data.col; j++){
			if ((data.pixels[i][j].red * 0.1140 + data.pixels[i][j].green * 0.5870 + data.pixels[i][j].blue * 0.2989) >= 148)
				a[i][j].red = a[i][j].green = a[i][j].blue = 255;
			else a[i][j].red = a[i][j].green = a[i][j].blue = 80;
		}
	}
}

void outputBmpBW(pixelArray &pixelArray, Color **&a) // Hàm xuất ảnh trắng đen ra màn hình
{
	HWND console = GetConsoleWindow();
	HDC hdc = GetDC(console);
	for (int i = 0; i < pixelArray.row; i++){
		for (int j = (pixelArray.col - 1); j >= 0; j--){
			SetPixel(hdc, j, i, RGB(a[i][j].red, a[i][j].green, a[i][j].blue));
		}
		Sleep(5);
	}
	ReleaseDC(console, hdc);
}

void CopyBMPBW(BMP &bitmap, pixelArray &data, FILE* p, Color **&a){ // Hàm chép ra file BMP đã được đổi màu 
	fwrite(&bitmap, sizeof(BMP), 1, p);
	for (unsigned int i = 0; i<data.row; i++){
		fwrite(a[data.row - 1 - i], sizeof(Color), data.col, p);
		uint8_t t = 0;
		if (paddingCount != 0) {
			fwrite(&t, sizeof(uint8_t), paddingCount, p);
		}
	}
}

void freeBW(pixelArray data, Color **&a){ // Hàm free cho vùng nhớ đã cấp phát
	for (int i = 0; i<data.row; i++)
		delete[] a[i];
	delete[] a;
}

void Nhac()
{
	PlaySound(TEXT("Nhac.wav"), NULL, SND_FILENAME);
}