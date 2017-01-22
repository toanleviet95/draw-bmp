#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "Function.h"

int main(){
	FILE* inFile = fopen("Bitmap.in", "rt");
	FILE* outFile = fopen("Bitmap.out", "wt");
	FILE* f;
	Inf ttin;
	InputInf(inFile, ttin);
	int x = strlen(ttin.a) - 1;
	if (ttin.a[x] == '\n') ttin.a[x] = '\0';
	x = strlen(ttin.b) - 1;
	if (ttin.b[x] == '\n') ttin.b[x] = '\0';
	f = fopen(ttin.a, "rb");
	if (f == NULL){
		printf("File is not found!\n");
		return 0;
	}
	if (IsBMP(f)) fprintf(outFile, "1\n");
	else fprintf(outFile, "0\n");
	BMP bitmap;
	Color **a = NULL;
	pixelArray data;
	ReadBMP(f, bitmap);
	OutputInf(bitmap, outFile);
	FILE* p = fopen(ttin.b, "wb");
	if (p == NULL){
		printf("File is not found!\n");
		return 0;
	}
	ReadPixelArray(f, bitmap, data);
	ChangeToBlackandWhite(data, a);
	CopyBMPBW(bitmap, data, p, a);
	resizeConsole(410, 540);
	outputBmpBW(data, a);
	Nhac();
	outputBmp(data);
	fcloseall();
	freeBW(data, a);
	free(data);
	getch();
	return 0;
}