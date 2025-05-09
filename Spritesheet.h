#ifndef SPRITESHEET_H

#include "Sprite.h"

class Spritesheet
{
public:
	Spritesheet(int num, int w, int h);
	Spritesheet();
	void addSprite(Sprite value);
	Sprite getSprite(int index);
	Sprite * getSpritesheet();
	int getSize();

	int		wSprite;
	int		hSprite;
private:
	Sprite	 *sprites;
	int		 numSprites;
	int		 curSprite;
};
#endif // !SPRITESHEET_H

