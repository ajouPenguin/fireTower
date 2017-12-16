typedef struct{
	unsigned int Width;
	unsigned int Height;
	unsigned int* Pixel;
} BITMAPDATA;

BITMAPDATA open_bitmap(char* filename);
void draw_bitmap(char* buffer, int buffer_Width, int buffer_Height, int start_x, int start_y, BITMAPDATA bmpData);
void close_bitmap(BITMAPDATA* bmpData);

int CreateCamera(int index);
void DestroyCamera();
int startPreview();
int stopPreview();
int draw_camera(unsigned char* buffer, int buffer_width, int buffer_height);
