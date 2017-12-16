typedef struct{
	unsigned int bfSize;		// BMP file size
	unsigned short bfReserved1;	// reserved data1, different value according to application which makes this BMP file
	unsigned short bfReserved2;	// reserved data2
	unsigned int bfOffBits;		// bitmap data's starting offset
}BITMAPFILEHEADER;

typedef struct{
	unsigned int headerSize;	// header's size
	unsigned int biWidth;		// bitmap horizontal size
	unsigned int biHeight;		// bitmap vertical size
	unsigned short biPlanes;	// the number of color plane
	unsigned short biBitCount;	// the number of bit
	unsigned int biCompression;	// methods of compression
	unsigned int biSizeImage;	// uncompressed image size
	unsigned int biXPelsPerMeter;	// horizontal pixels per meter
	unsigned int biYPelsPerMeter;	// vertical pixels per meter
	unsigned int biClrUsed;		// color used
	unsigned int biClrImportant;	// the number of important color. if all colors are important, then number is 0.
}BITMAPINFOHEADER;

typedef struct{
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
}RGBQUAD;

typedef struct{
	BITMAPINFOHEADER bmiHeader;	// header information
	RGBQUAD bmiColors[1];		// real data
}BITMAPINFO;

typedef struct{
	unsigned int Width;
	unsigned int Height;
	unsigned int* Pixel;
}BITMAPDATA;
