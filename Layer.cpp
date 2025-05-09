#include "Layer.h"

//Define construtor com os parametros de largura e altura
Layer::Layer(int xB, int yB)
{
	wBase	 = xB;
	hBase	 = yB;
	numImgs	 = 0;
	imgs	 = new Image[MAX_IMGS];
}

//Define construtor padrão com valores iniciais(800 x 600)
Layer::Layer()
{
	wBase	 = 800;
	hBase	 = 600;
	numImgs	 = 0;
	imgs	 = new Image[MAX_IMGS];
}

//Define scrolling no eixo x (deslocamento para efeito parallax)
void Layer::setXScrolling(float scroll)
{
	scrollX = scroll;
}

//Define scrolling padrão no eixo x (largura base dividido pela largura da imagem base da camada)
void Layer::setXScrolling()
{
	scrollX = float(wBase) / float(imgs[0].getWidth());
}

//Define scrolling no eixo y (deslocamento para efeito parallax)
void Layer::setYScrolling(float scroll)
{
	scrollY = scroll;
}

//Define scrolling padrão no eixo y (largura base dividido pela altura da imagem base da camada)
void Layer::setYScrolling()
{
	scrollY = float(hBase) / float(imgs[0].getHeight());
}

//Adiciona uma imagem na camada se ouver espaço(numImgs < MAX_IMGS)
void Layer::addImage(Image img)
{
	if (numImgs < MAX_IMGS) {
		imgs[numImgs] = img;
		numImgs++;
	}
}

//Retorna array de imagens
Image * Layer::getLayer()
{
	return imgs;
}