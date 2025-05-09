#include "Spritesheet.h"
#include <stdexcept>

//Define construtor do spritesheet com o numero de sprites, largura e altura de cada sprite
Spritesheet::Spritesheet(int num, int w, int h)
{
	sprites		= new Sprite[num];
	numSprites	= num;
	curSprite	= 0;
	wSprite		= w;
	hSprite		= h;
}

//Define construtor padrão
Spritesheet::Spritesheet()
{
	sprites = new Sprite[1];
	numSprites = 1;
	curSprite = 0;
	wSprite = 1;
	hSprite = 1;
}

//Adiciona um sprite se ouver espaço(curSprite < numsprites) no spritesheet
void Spritesheet::addSprite(Sprite value)
{
	if (curSprite < numSprites) {
		sprites[curSprite] = value;
		curSprite++;
	}
}

//Retorna um sprite determinado pelo index
Sprite Spritesheet::getSprite(int index)
{
	if (index < 0 || index >= numSprites)
		throw std::invalid_argument("Index fora do escopo!");

	return sprites[index];
}

//Retorna o spritesheet(array de sprites)
Sprite * Spritesheet::getSpritesheet()
{
	return sprites;
}

//Retorna o numero de sprites no spritesheet
int Spritesheet::getSize()
{
	return numSprites;
}