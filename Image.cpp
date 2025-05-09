#include "Image.h"

//Define um construtor com parametros de largura(w) e altura(h)
Image::Image(int w, int h) {
	width  = w;
	height = h;
	pixels = new int[w*h];
}

//Construtor padrão, altura e largura igual a zero
Image::Image() {
	width  = 0;
	height = 0;
}

//Verifica se a imagem é validada pelo tamanho, 
//se tamanho total (altura x largura) for igual a um é invalida senão é valida.
int Image::isInvalid() {
	if ((width*height) == 0) 
		return 1;
	else
		return 0;
}

//Define um pixel(rgb) para a imagem(array de pixels)
void Image::setPixel(int rgb, int x, int y) {
	pixels[x + y*width] = rgb;
}

//Retorna um pixel da imagem
int Image::getPixel(int x, int y) {
	return pixels[x + y*width];
}

//Retorn o array de pixels(imagem)
int * Image::getPixels() {
	return pixels;
}

//Retorn largura da imagem
int Image::getWidth() { return width; }

//Retorna altura da imagem
int Image::getHeight() { return height; }