#ifndef WAVE_H
#define WAVE_H

#define WAVE_TYPE_NORMAL 0
#define WAVE_TYPE_SPECIAL 1

class Wave
{
public:
	//Wave(numero de waves, numero maximo de inimigos)
	Wave(int,int);

	//NextWave( numero de inimigos da proxima wave, tipo da wave)
	//Retorna 1 se tem proxima wave e 0 se acabou as waves
	int NextWave(int, int);

	int cur_wave, num_waves, type, enemies_byWave ,enemies_alive, enemies_max;
};
#endif // !WAVE_H