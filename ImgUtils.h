#ifndef IMG_UTILS_H
#define IMG_UTILS_H
#include "Spritesheet.h"

class ImgUtils
{
public:
	ImgUtils(int,int);
	Spritesheet loadSpritesheet(char * fileName, int rSize, int cSize);
	Image loadImageNormal(char * fileName);

	int winWidth;
	int winHeight;
};
#endif // !IMG_UTILS_H