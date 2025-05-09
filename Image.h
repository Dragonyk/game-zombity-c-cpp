#pragma once
class Image
{
public:
	Image(int w, int h);
	Image();
	int isInvalid();
	void setPixel(int rgb, int x, int y);
	int getPixel(int x, int y);
	int * getPixels();
	int getWidth();
	int getHeight();
private:
	int *pixels;
	int width, height;
};

