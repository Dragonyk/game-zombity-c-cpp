#include "Wave.h"

Wave::Wave(int nWaves, int maxEnemies)
{
	if (nWaves <= 0)
		nWaves = 1;
	if (maxEnemies <= 0)
		maxEnemies = 1;
	num_waves = nWaves;
	enemies_max = maxEnemies;

	cur_wave = 1;
	type = 0;
}

int Wave::NextWave(int enemiesCur, int waveType)
{
	if (cur_wave < num_waves) {
		if (enemiesCur > enemies_max) 
			enemiesCur = enemies_max;
		enemies_byWave = enemiesCur;
		enemies_alive = enemiesCur;

		type = waveType;
		cur_wave++;
		return 1;
	}
	return 0;
}
