#include "Sprite.h"


//Construtor padrão
Sprite::Sprite(){
	id = -1;
	img = Image(0, 0);
}

//Retorna o id do Sprite
int Sprite::getId()
{
	return id;
}

//Retorna a imagem do sprite
Image Sprite::getImage()
{
	return img;
}

//Define o id do sprite, "value" deve ser maior ou igual a zero
void Sprite::setId(int value)
{
	if(value >= 0)
		id = value;
}

//Define a imagem, "value" deve ser uma imagem valida
void Sprite::setImage(Image value)
{
	if (value.isInvalid() == 0) {
		img = value;
	}
}
