#ifndef LAYER_H
#define LAYER_H
#include "Image.h"
#define MAX_IMGS 10

class Layer
{
public:
	Layer(int, int);
	Layer();
	void setXScrolling(float);
	void setXScrolling();		//DEFAULT
	void setYScrolling(float);
	void setYScrolling();		//DEFAULT
	void addImage(Image);
	Image * getLayer();

	float scrollX;
	float scrollY;
	int numImgs;
private:
	Image * imgs;
	int wBase;
	int hBase;
};
#endif // !LAYER_H
