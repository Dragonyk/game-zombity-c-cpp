#include "ImgUtils.h"
#include <gl\glut.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

ImgUtils::ImgUtils(int width, int height)
{
	if (width < 0)
		width = 1;
	if (height < 0)
		height = 1;

	winWidth  = width;
	winHeight = height;
}

Spritesheet ImgUtils::loadSpritesheet(char *fileName, int rSize, int cSize) {
	ifstream ifs;
	ifs.open(fileName, std::ios::binary);
	std::string header;
	std::string comm;

	char c;
	int w, h, b;
	ifs >> header;

	ifs >> c;
	if (c == '#') {
		ifs.ignore(256, '\n');
		ifs >> w >> h >> b;
	}
	else {
		ifs >> comm >> h >> b;
		w = atoi(comm.c_str());
		w = ((c - '0')*pow(10, comm.length())) + w;
	}
	int absSpriteSize = cSize*rSize;
	int cols = w / cSize;
	int rows = h / rSize;
	const int numSprites = cols*rows;
	Sprite * sprites = new Sprite[numSprites];
	Spritesheet spritesheet = Spritesheet(numSprites, cSize, rSize);

	Image * imgsSprite = new Image[numSprites];
	for (int i = 0; i < numSprites; i++) {
		imgsSprite[i] = Image(cSize, rSize);
	}
	ifs.ignore(256, '\n');

	float dmax = sqrt(pow(255, 2) + pow(255, 2) + pow(255, 2));
	unsigned char pix[4];
	const int absSize = w*h;
	unsigned char* px = new unsigned char[absSpriteSize * 3];
	glReadBuffer(GL_FRONT);
	glReadPixels(0, winHeight, cSize, rSize, GL_RGB, GL_UNSIGNED_BYTE, px);
	int cont = 0;
	for (int l = 0; l < rows; ++l) {
		for (int y = 0; y < rSize; ++y) {
			for (int i = 0; i < cols; ++i) {
				for (int x = 0; x < cSize; ++x) {
					ifs.read(reinterpret_cast<char *>(pix), 4);
					int ta = pix[0];
					int tr = pix[1];
					int tg = pix[2];
					int tb = pix[3];
					float alpha = float(ta) / 255;
					tr = (int)px[(i * 3)] * (1 - alpha) + (tr * alpha);
					tg = (int)px[(i * 3) + 1] * (1 - alpha) + (tg * alpha);
					tb = (int)px[(i * 3) + 2] * (1 - alpha) + (tb * alpha);
					int argb = (ta << 24 | tr << 16) | (tg << 8) | tb;
					int curSprite = i + (l*cols);
					imgsSprite[curSprite].setPixel(argb, x, (rSize - 1) - y);
				}
			}
		}
	}

	for (int i = 0; i < numSprites; i++) {
		sprites[i].setId(i);
		sprites[i].setImage(imgsSprite[i]);
		spritesheet.addSprite(sprites[i]);
	}
	return spritesheet;
}

Image ImgUtils::loadImageNormal(char *fileName) {
	ifstream ifs;
	ifs.open(fileName, std::ios::binary);
	std::string header;
	std::string comm;

	char c;
	int w, h, b;
	ifs >> header;

	ifs >> c;
	if (c == '#') {
		ifs.ignore(256, '\n');
		ifs >> w >> h >> b;
	}
	else {
		ifs >> comm >> h >> b;
		w = atoi(comm.c_str());
		w = ((c - '0')*pow(10, comm.length())) + w;
	}
	Image img = Image(w, h);
	ifs.ignore(256, '\n');

	GLfloat dmax = sqrt(pow(255, 2) + pow(255, 2) + pow(255, 2));

	unsigned char pix[4];
	const int absSize = w*h;
	unsigned char* px = new unsigned char[absSize * 3];
	glReadBuffer(GL_FRONT);
	glReadPixels(0, winHeight, w, h, GL_RGB, GL_UNSIGNED_BYTE, px);

	for (int i = 0; i < absSize; ++i) {
		ifs.read(reinterpret_cast<char *>(pix), 4);
		int ta = pix[0];
		int tr = pix[1];
		int tg = pix[2];
		int tb = pix[3];
		float alpha = float(ta) / 255;
		tr = (int)px[(i * 3)] * (1 - alpha) + (tr * alpha);
		tg = (int)px[(i * 3) + 1] * (1 - alpha) + (tg * alpha);
		tb = (int)px[(i * 3) + 2] * (1 - alpha) + (tb * alpha);

		int argb = (ta << 24 | tr << 16) | (tg << 8) | tb;

		img.setPixel(argb, i % w, ((absSize - 1) - i) / w);
	}
	return img;
}
