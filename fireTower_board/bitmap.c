#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "bitmap.h"

BITMAPDATA open_bitmap(char *filename)
{
	FILE* fp;
	unsigned char magicNum[2];
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	BITMAPDATA bmpData;
	memset(&bmpData, 0, sizeof(BITMAPDATA));

	fp = fopen(filename, "rb"); // Open file
	if(fp == NULL)
	{
		printf("%s open error\n", filename);
		return bmpData;
	}

	fread(magicNum, 2, sizeof(unsigned char), fp); // Read magic number
	if(!(magicNum[0] == 'B' && magicNum[1] == 'M')) // Check if bitmap file or not
	{
		printf("%s is not bitmap file\n", filename);
		fclose(fp);
		return bmpData;
	}

	fread(&bmpFileHeader, 1, sizeof(BITMAPFILEHEADER), fp); // Read bitmap file header
	fread(&bmpInfoHeader, 1, sizeof(BITMAPINFOHEADER), fp); // Read bitmap info header
	if(bmpInfoHeader.biBitCount != 24) // Check if 24bit bitmap file or not
	{
		printf("%s is not 24bit bitmap file\n", filename);
		fclose(fp);
		return bmpData;
	}

	unsigned char* data = (unsigned char*)malloc(bmpFileHeader.bfSize - bmpFileHeader.bfOffBits); // Allocate for bitmap data
	fseek(fp, bmpFileHeader.bfOffBits, SEEK_SET); // Move file pointer to data
	fread(data, 1, bmpFileHeader.bfSize - bmpFileHeader.bfOffBits, fp); // Read bitmap data
	fclose(fp);
	bmpData.Width = bmpInfoHeader.biWidth;
	bmpData.Height = bmpInfoHeader.biHeight;
	bmpData.Pixel = (int*)malloc(sizeof(int) * bmpData.Width * bmpData.Height);

	int i, j, k, t;

	for(j = 0; j < bmpData.Height; j++) // Move data to bmpData.Pixel in proper order
	{
		k = j * bmpInfoHeader.biSizeImage / bmpInfoHeader.biHeight; // Not used bmpData.Width because of padding
		t = (bmpData.Height - 1 - j) * bmpData.Width;

		for(i = 0; i < bmpData.Width; i++)
		{
			int r = *(data + (k + i * 3));
			int g = *(data + (k + i * 3 + 1));
			int b = *(data + (k + i * 3 + 2));

			int temp = ((r << 16) | (g << 8) | b);
			bmpData.Pixel[t + i] = temp;
		}
	}

	free(data);

	return bmpData;
}

void draw_bitmap(char* buffer, int buffer_Width, int buffer_Height, int start_x, int start_y, BITMAPDATA bmpData)
{
	int i, j, k, l;
	int end_x = start_x + bmpData.Width;
	int end_y = start_y + bmpData.Height;

	for(i = start_y, k = 0; i < end_y; i++, k++) // Copy bitmap data to buffer
	{
		for(j = start_x, l = 0; j < end_x; j++, l++)
		{
			if(((0 <= i) && (i < buffer_Height)) && ((0 <= j) && (j < buffer_Width))) // Check if valid point or not
			{
				int temp1 = (i * buffer_Width + j) * 4;
				int temp2 = k * bmpData.Width + l;

				buffer[temp1] = (char)(bmpData.Pixel[temp2] >> 16) & 0xFF;
				buffer[temp1 + 1] = (char)(bmpData.Pixel[temp2] >> 8) & 0xFF;
				buffer[temp1 + 2] = (char)bmpData.Pixel[temp2] & 0xFF;
			}
		}
	}
}

void close_bitmap(BITMAPDATA* bmpData)
{
	if(bmpData->Pixel)
		free(bmpData->Pixel);
	memset(bmpData, 0, sizeof(BITMAPDATA));
}
