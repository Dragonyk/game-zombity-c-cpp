#pragma once
#ifndef _SPRITE_H_
#include "Image.h"

class Sprite
{
public:
	Sprite();
	int getId();
	Image getImage();

	void setId(int);
	void setImage(Image i);

private:
	int id;

	Image img = Image(0, 0);
};
#endif // !_SPRITE_H_
