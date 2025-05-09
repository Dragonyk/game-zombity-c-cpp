

#define _CRT_SECURE_NO_DEPRECATE
#include <gl/glut.h>
#include <math.h>
#include <time.h> 
#include <vector>
#include "ImgUtils.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "Layer.h"
#include "Player.h"
#include "Enemy.h"
#include "Game.h"
#include "Item.h"
#include "Chrono_Time.h"
#include "Wave.h"
using namespace std;

//Outros dados
#define PI 3.14159265359
#define LAYER_NUMBER 6
#define LAYER_BASE 4
#define LAYER_GUI 5
#define SPRITESHEET_NUMBER 10
#define ORT_DEFAULT_SIZE 1.0f

//Estados do jogo
#define STATE_MAINMENU 0
#define STATE_PLAYING 1
#define STATE_GAMEOVER 2
#define STATE_GAMEWIN 3

//Aceleracao e velocidade do jogador
#define MAX_ACC_PLAYER 0.15
#define PLAYER_SPEED 1.0f

//Numero maximo de zumbis no jogo
#define MAX_ENEMIES 25
#define MAX_WAVES 10

//######################################

//Variaveis para controle dos estados e das opcoes
int STATE		   = 0;	//Estado atual do jogo
int option_ini	   = 0; //Opcao selecionada no estado do menu
int option_dif	   = 0; //Opcao de dificuldade selecionada no estado do menu
int option_players = 0; //Opcao de numero de jogadores no estado do menu
int option_restart = 0; //Se foi apertado ENTER ao terminar uma partida

//Variaveis para controle da camera e do cenario
GLint sWidth  = 1024;				//Largura da tela
GLint sHeight = 720;				//Altura da tela
float ortSize = ORT_DEFAULT_SIZE;	//Tamanho do ortho
float xCam	  = 0;					//posicao x da camera
float yCam	  = 0;					//posicao y da camera

//Outras variaveis de controle da camera
//absCam	 = x absoluto da camera
//endCam	 = final do cenario
//disFromEnd = calculo de distancia entre o fim do cenario e o jogador no eixo x
float absCam, endCam, disFromEnd;

//Variaveis de botoes do jogador
int up_k	 = 0;	//Movimento para cima ( W )
int down_k   = 0;	//Movimento para baixo ( S )
int left_k   = 0;	//Movimento para esquerda ( A )
int right_k  = 0;	//Movimento para direita ( D )
int attack_k = 0;   //Ataque do jogador ( ESPACO )
int melee_k  = 0;   //Ataque melee (F)

//Variaveis de botoes do jogador 2
int up_k2     = 0;  //Movimento para cima (NumPad 8)
int down_k2   = 0;  //Movimento para baixo (NumPad 2)
int left_k2   = 0;  //Movimento para esquerda (NumPad 4)
int right_k2  = 0;  //Movimento para direita (NumPad 6)
int attack_k2 = 0;  //Ataque do jogador 2 (NumPad 0)
int melee_k2  = 0;  //Ataque melee (NumPad -)

//Variaveis para controle de velocidade e outras informacoes
float game_speed = 1;		//Velocidade do jogo
int time_fpms	 = 30;		//Velocidade do Timer milisegundos
float factSpeedX = 100;		//Fator de velocidade x para divisao
float factSpeedY = 50;		//Fator de velocidade y para divisao
float dmgY		 = 0.01;	//Distancia para deteccao do player e do zumbi no eixo y
float rangeAtk	 = 0.4;		//Alcance maximo da arma do player
float rangeAtk2  = 0.4;     //Alcance maximo da arma do player 2
float disToY	 = 0.3;		//Distancia para zumbi seguir player no eixo y
int cur_enemies  = 5;		//Zumbis na primeira wave

//Objeto para carregar imagens e spritesheets
ImgUtils imgUtils = ImgUtils(sWidth, sHeight);

//Objetos para carregar camadas, spritesheets e imagens
Layer * layers = new Layer[LAYER_NUMBER];
Spritesheet * ssheet = new Spritesheet[SPRITESHEET_NUMBER];
std::vector<Image> imgs(20, Image(0, 0));

//Objetos para carregar jogo, waves, e tempo decorrido
Game game = Game();
Wave waveT = Wave(MAX_WAVES, MAX_ENEMIES);
Wave * wave = &waveT; //ponteiro para wave
Chrono_Time chrono = Chrono_Time();

//Objetos para carregar jogador, zumbis e itens
Player player = Player();
Player player2 = Player();
Enemy * zombies = new Enemy[MAX_ENEMIES];
Enemy * p1_target; //Inimigo no alvo do player
Enemy * p2_target; //Inimigo no alvo do player 2
Item item_ammo = Item();

//Prototipos de metodos
void RespawnEnemies();

//Desenha uma imagem na tela
//O movFact é o fator de movimento ou seja, scrolling
void DrawImage(Image img, float x, float y, float movFact, int isSpr = 0) {
	float midSize = (ortSize / 2);

	float toY = 0;
	if (isSpr)
		toY = y;

	if (x < (absCam - midSize))
	{
		float sub = (midSize - absCam)*(movFact * 1000);
		glRasterPos2f((absCam - midSize), toY);
		glBitmap(0, 0, (absCam - midSize), 0, sub, 0, NULL);
	}
	else
		glRasterPos2f(x, y);

	glDrawPixels(img.getWidth(), img.getHeight(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.getPixels());
}

//Carrega imagens e sprites de um arquivo .txt
//Exemplo de leitura: 
//nome.ptm					----> o arquivo de imagem 
//#							----> o separador das dos spritesheets 
//spritesheet.ptm=10#10		----> o spritesheet com a largura e altura de cada sprite
void loadImgs(char * fileName) {
	ifstream file;
	string line;
	int readState = 0;
	int curImg = 0;
	int curSpr = 0;
	file.open(fileName);
	cout << "Carregando arquivo [" << fileName << "]\n";
	while (!file.eof()) {
		getline(file, line);

		if (line == "#") {
			readState = 1;
		}
		else {
			if (readState == 0) {
				char *cstr = new char[line.length() + 1];
				strcpy(cstr, line.c_str());
				imgs[curImg] = imgUtils.loadImageNormal(cstr);
				cout << "[" << line << "] carregado com sucesso! [" << curImg << "]\n";
				delete[] cstr;
				curImg++;
			}
			else {
				size_t pos = line.find("=");
				string str3 = line.substr(pos + 1);
				size_t pos2 = str3.find("#");
				int a = stoi(str3.substr(0, pos2));
				int b = stoi(str3.substr(pos2 + 1, str3.length()));

				line = line.substr(0, pos);
				char *cstr = new char[line.length() + 1];
				strcpy(cstr, line.c_str());
				ssheet[curSpr] = imgUtils.loadSpritesheet(cstr, b, a);
				cout << "[" << line << "] carregado com sucesso! [" << curSpr << "]\n";
				delete[] cstr;
				curSpr++;
			}
		}
	}
}

//Carrega dados iniciais do jogo como imagens, sprites, jogador, zumbis, a primeira wave, etc.. 
void loadSystem() {
	loadImgs("dados.txt");

	for (int i = 0; i < LAYER_NUMBER; i++)
		layers[i] = Layer(sWidth, sHeight);

	layers[0].addImage(imgs[0]);
	layers[0].setXScrolling(1);

	layers[1].addImage(imgs[8]);
	layers[1].setXScrolling(0.2);

	layers[2].addImage(imgs[1]);
	layers[2].setXScrolling();

	layers[3].addImage(imgs[2]);
	layers[3].setXScrolling(1);

	//Carrega player
	player.setLife(10);
	player.vmax = (PLAYER_SPEED/100)*game_speed;
	player.vmax2 = player.vmax * player.vmax;
	player.dx = 0;
	player.dy = 0;
	player.animMove.setAnimation(1, 12, 0.1f/(PLAYER_SPEED*game_speed), 0);
	player.animFireL.setAnimation(1, 3, 0.1f/game_speed, 0);
	player.animFireR.setAnimation(0, 2, 0.1f/game_speed, 2);
	player.animMeleeL.setAnimation(1, 3, 0.1f / game_speed, 0);
	player.animMeleeR.setAnimation(1, 3, 0.1f / game_speed, 0);
	player.animBlood.setAnimation(0, 2, 0.1f/game_speed, 0);

	//Carrega player 2
	player2.setLife(10);
	player2.vmax = (PLAYER_SPEED / 100)*game_speed;
	player2.vmax2 = player2.vmax * player2.vmax;
	player2.dx = 0;
	player2.dy = 0;
	player2.animMove.setAnimation(1, 12, 0.1f / (PLAYER_SPEED*game_speed), 0);
	player2.animFireL.setAnimation(1, 3, 0.1f / game_speed, 0);
	player2.animFireR.setAnimation(0, 2, 0.1f / game_speed, 2);
	player2.animMeleeL.setAnimation(1, 3, 0.1f / game_speed, 0);
	player2.animMeleeR.setAnimation(1, 3, 0.1f / game_speed, 0);
	player2.animBlood.setAnimation(0, 2, 0.1f / game_speed, 0);

	//Carrega primeira wave
	wave->enemies_byWave = cur_enemies;
	wave->enemies_alive  = cur_enemies;

	//Carrega zumbis
	float z_speed = 0.3;
	for (int i = 0; i < MAX_ENEMIES; i++) {
		zombies[i] = Enemy(z_speed*game_speed, 1, 0.5, 0.05);
		zombies[i].anim.setAnimation(0, 3, 0.3f/game_speed, 1);
		zombies[i].animDeath.setAnimation(0, 9, 1.0f, 0);
		z_speed += 0.05;

		//Carrega os zumbis da primeira wave
		if (i < cur_enemies)
			zombies[i].isActive = 1;
	}

	item_ammo.x = 0.1;
	item_ammo.y = 0.05;

	endCam = imgs[2].getWidth() / sWidth;

	disFromEnd = fabs(endCam - player.x);
}

//Faz a animação de um sprite
void animateSprite(int spriteNum, Sprite_Animation * anim, float posX, float posY, bool hasAnim, int hasLoop) {
	if (hasAnim) {
		if (hasLoop == 0) {
			if (anim->getCurFrame() < anim->maxAnim) {
				if (anim->getTime() >= anim->maxTimeAnim) {
					anim->nextFrame();
					anim->resetTime();
				}
			}
		}
		else {
			if (anim->getTime() >= anim->maxTimeAnim) {
				anim->nextFrame();
				anim->resetTime();
			}
		}
	}
	else
		anim->setFrame(anim->iddleFrame);
	//ssheet[animNum].setFrame(ssheet[animNum].maxAnim - 3);
	//ssheet[animNum].startAnimation();
	DrawImage(ssheet[spriteNum].getSprite(anim->getCurFrame()).getImage(), posX, posY, 1, 1);
}

//Carrega dados para diferentes niveis de dificuldade
void loadNewGame() {
	//Carrega diiculdades
	float z_dif_speed = 1;

	if (option_dif == 0) 
		z_dif_speed = 1;
	else if (option_dif == 1)
		z_dif_speed = 1.5;
	else
		z_dif_speed = 2;

	//Define a velocidade do zumbi
	float z_speed = 0.3;
	for (int i = 0; i < MAX_ENEMIES; i++) {
		zombies[i].setSpeed((z_speed*z_dif_speed)*game_speed);
		z_speed += 0.05;
		zombies[i].isActive = 0;
	}
}

//Reseta os dados para iniciar um novo jogo
void ResetGame() {
	STATE = STATE_PLAYING;

	xCam = 0;
	player.x = player.defX;
	player.y = player.defY;
	player.dirX = PLAYER_RIGHT;
	player.dirA = PLAYER_RIGHT;
	player.ammo = player.ammo_max;
	player.ammo_time = 0;

	player2.x = player2.defX;
	player2.y = player2.defY;
	player2.dirX = PLAYER_RIGHT;
	player2.dirA = PLAYER_RIGHT;
	player2.ammo = player2.ammo_max;
	player2.ammo_time = 0;

	wave->cur_wave = 1;
	wave->enemies_byWave = cur_enemies;
	wave->enemies_alive  = cur_enemies;
	
	loadNewGame();
	RespawnEnemies();

	item_ammo.x = 0.1;
	chrono.ResetTime();
	player.setLife(player.maxLife);
	player2.setLife(player2.maxLife);
}

//Faz checagem de estados do jogos
void GameUpdate() {
	if (STATE == STATE_PLAYING) {
		disFromEnd = fabs(endCam - player.x);

		if (wave->enemies_alive <= 0) {
			if (wave->cur_wave >= wave->num_waves) {
				STATE = STATE_GAMEWIN;
			}
			else {
				p1_target = NULL;
				p2_target = NULL;
				int checkWave = wave->NextWave(wave->enemies_byWave + 2, 1);
				if(checkWave == 1)
					RespawnEnemies();
			}
		}
		if (option_players == 1) {
			if (player.getLife() == 0 && player2.getLife() == 0) {
				STATE = STATE_GAMEOVER;
			}
		}
		else
			if (player.getLife() == 0) {
				STATE = STATE_GAMEOVER;
			}
	}
	else if (STATE == STATE_GAMEOVER || STATE == STATE_GAMEWIN) {
		if (option_restart) {
			STATE = STATE_MAINMENU;
			option_ini = 0;
			option_dif = 0;
			option_players = 0;
			option_restart = 0;
		}
	}
}

//Desenha um texto na tela
void DrawTxt(char *text, int txt_size, int r, int g, int b, float x, float y) {
	glColor3ub(r, g, b);
	glRasterPos2f(x, y);
	for (int ii = 0; ii < txt_size; ii++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *(text++));
	}
}

//Mostra a quantidade de vida restante
void DrawLife() {
	float posX = absCam - 0.49f;
	for (int i = 0; i < player.getLife(); i++) {
		DrawImage(imgs[4], posX, 0.95, 1);
		posX += 0.035;
	}
	if (option_players == 1) {
		posX = absCam - 0.49f;
		for (int i = 0; i < player2.getLife(); i++) {
			DrawImage(imgs[4], posX, 0.85, 1);
			posX += 0.035;
		}
	}
}

//Mostra a quantidade de munição restante
void DrawPoints() {
	float posX = absCam - 0.49f;
	for (int i = 0; i < player.ammo; i++) {
		DrawImage(imgs[5], posX, 0.90, 1);
		posX += 0.035;
	}
	if (option_players == 1) {
		posX = absCam - 0.49f;
		for (int i = 0; i < player2.ammo; i++) {
			DrawImage(imgs[5], posX, 0.80, 1);
			posX += 0.035;
		}
	}
}

//Mostra na tela a wave atual e quantos inimigos falta derrotar
void DrawWave() {
	char wave_txt[200];
	sprintf_s(wave_txt, 200, "Wave: %i", wave->cur_wave);
	DrawTxt(wave_txt, 15, 255, 255, 255, absCam + 0.4, 0.9);

	char remain_txt[200];
	sprintf_s(remain_txt, 200, "%i/%i", wave->enemies_alive, wave->enemies_byWave);
	DrawTxt(remain_txt, 15, 255, 0, 0, absCam + 0.4, 0.85);
}

//Desenha munição na tela
void CreateItemAmmo() {
	DrawImage(imgs[3], item_ammo.x, item_ammo.y, 1);
}

//Quando o jogador for atacado
void playerIsAttacked(Enemy * enemy) {
	if (enemy->dps_time > enemy->dps-1){
		player.setLife(player.getLife() - enemy->getAttack());
		enemy->dps_time = 0;
	}
}

void player2IsAttacked(Enemy * enemy) {
	if (enemy->dps_time > enemy->dps - 1) {
		player2.setLife(player2.getLife() - enemy->getAttack());
		enemy->dps_time = 0;
	}
}

//Ao coletar munição, se munição for menor que o maximo ela é acrescentada mas sempre recarrega o a munição atual
//Apos isso é trocado a posição da munição para um ponto aleatorio no cenario
void collectAmmoP1() {
	if (player.ammo < player.ammo_max) {
		player.ammo += item_ammo.bonus;
		if (player.ammo > player.ammo_max)
			player.ammo = player.ammo_max;
	}
	player.ammo_time = 0;
	item_ammo.x = float(rand() % 200 + 50) / 100;
}

void collectAmmoP2() {
	if (player2.ammo < player2.ammo_max) {
		player2.ammo += item_ammo.bonus;
		if (player2.ammo > player2.ammo_max)
			player2.ammo = player2.ammo_max;
	}
	player2.ammo_time = 0;
	item_ammo.x = float(rand() % 200 + 50) / 100;
}

//Mostra as animações do jogador
void DrawPlayer() {
	if (player.getLife() <= 0) {
		DrawImage(imgs[10], player.x, player.y, 1, 1);
	}
		
	else {
		if (player.dirA == PLAYER_RIGHT) {
			if (player.state == STATE_FREE) {
				if (player.dx == 0)
					animateSprite(0, &player.animMove, player.x, player.y, false, 1);
				else
					animateSprite(0, &player.animMove, player.x, player.y, true, 1);
			}
			else if (player.state == STATE_MELEE) {
				animateSprite(7, &player.animMeleeR, player.x, player.y, true, 1);
			}
			else {
				if (player.ammo > 0) {
					animateSprite(5, &player.animFireR, player.x, player.y, true, 1);
				}
				else
					animateSprite(5, &player.animFireR, player.x, player.y, false, 1);
			}

		}
		else{
			if (player.state == STATE_FREE) {
				if (player.dx == 0)
					animateSprite(1, &player.animMove, player.x, player.y, false, 1);
				else
					animateSprite(1, &player.animMove, player.x, player.y, true, 1);
			}
			else if (player.state == STATE_MELEE) {
				animateSprite(6, &player.animMeleeL, player.x, player.y, true, 1);
			}
			else {
				if (player.ammo > 0) {
					animateSprite(4, &player.animFireL, player.x - 0.07, player.y, true, 1);
				}
				else
					animateSprite(4, &player.animFireL, player.x - 0.07, player.y, false, 1);
			}

		}
	}
}

void DrawPlayer2() {
	if (player2.getLife() <= 0)
		DrawImage(imgs[10], player2.x, player2.y, 1,1);
	else {
		if (player2.dirA == PLAYER_RIGHT) {
			if (player2.state == STATE_FREE) {
				if (player2.dx == 0)
					animateSprite(0, &player2.animMove, player2.x, player2.y, false, 1);
				else
					animateSprite(0, &player2.animMove, player2.x, player2.y, true, 1);
			}
			else if (player2.state == STATE_MELEE) {
				animateSprite(7, &player2.animMeleeR, player2.x, player2.y, true, 1);
			}
			else {
				if (player2.ammo > 0) {
					animateSprite(5, &player2.animFireR, player2.x, player2.y, true, 1);
				}
				else
					animateSprite(5, &player2.animFireR, player2.x, player2.y, false, 1);
			}

		}
		else {
			if (player2.state == STATE_FREE) {
				if (player2.dx == 0)
					animateSprite(1, &player2.animMove, player2.x, player2.y, false, 1);
				else
					animateSprite(1, &player2.animMove, player2.x, player2.y, true, 1);
			}
			else if (player2.state == STATE_MELEE) {
				animateSprite(6, &player2.animMeleeL, player2.x, player2.y, true, 1);
			}
			else {
				if (player2.ammo > 0) {
					animateSprite(4, &player2.animFireL, player2.x - 0.07, player2.y, true, 1);
				}
				else
					animateSprite(4, &player2.animFireL, player2.x - 0.07, player2.y, false, 1);
			}

		}
	}
}

// Metodo para pegar o zumbi mais proximo na direção do jogador
void EnemyInRange() {
	Enemy * enemy = NULL;
	float disMin = 10;
	for (int z = 0; z < MAX_ENEMIES; z++) {
		if (zombies[z].isActive) {
			if (zombies[z].x > player.x - rangeAtk && zombies[z].x < player.x + rangeAtk) {
				if (zombies[z].y > player.y - dmgY && zombies[z].y < player.y + dmgY) {
					if (player.dirA == PLAYER_RIGHT) {
						if (zombies[z].x >= player.x) {
							float disCur = fabs(zombies[z].x - player.x);
							if (disCur < disMin) {
								disMin = disCur;
								enemy = &zombies[z];
							}
						}
					}
					else {
						if (zombies[z].x < player.x) {
							float disCur = fabs(zombies[z].x - player.x);
							if (disCur < disMin) {
								disMin = disCur;
								enemy = &zombies[z];
							}
						}
					}
				}
			}
			else if (zombies[z].x > player2.x - rangeAtk && zombies[z].x < player2.x + rangeAtk) {
				if (zombies[z].y > player2.y - dmgY && zombies[z].y < player2.y + dmgY) {
					if (player2.dirA == PLAYER_RIGHT) {
						if (zombies[z].x >= player2.x) {
							float disCur = fabs(zombies[z].x - player2.x);
							if (disCur < disMin) {
								disMin = disCur;
								enemy = &zombies[z];
							}
						}
					}
					else {
						if (zombies[z].x < player2.x) {
							float disCur = fabs(zombies[z].x - player2.x);
							if (disCur < disMin) {
								disMin = disCur;
								enemy = &zombies[z];
							}
						}
					}
				}
			}
		}
	}

	p1_target = enemy;
	p2_target = enemy;
}

//Metodo para fazer o renascimento dos zumbis
void RespawnEnemies() {
	disFromEnd = fabs(endCam - player.x);
	for (int i = 0; i < wave->enemies_byWave; i++) {
		zombies[i].isActive = 1;
		zombies[i].life = 100;

		if (disFromEnd <= (endCam / 2)) {
			zombies[i].x = 0.2;
		}
		else {
			zombies[i].x = endCam-0.3;
		}
	}
	cout << wave->enemies_byWave << "\n";
}

//Ataque do jogador
void PlayerAttack() {
	if (STATE == STATE_PLAYING) {
		if (attack_k == 1) {
			player.state = STATE_ATTACK;
			rangeAtk = 0.4;
			if (player.ammo > 0) {
				if (p1_target != NULL) {
					p1_target->life -= 10;
					player.isAtk = 1;
				}
				else
					player.isAtk = 0;
				if (player.ammo_time > player.ammo_maxTime-1) {
					player.ammo -= 1;
					player.ammo_time = 0;
				}
			}
			else
				player.isAtk = 0;
		}
		else if (melee_k == 1) {
			player.state = STATE_MELEE;
			rangeAtk = 0.1;
			if (p1_target != NULL) {
				p1_target->life -= 30;
				player.isAtk = 1;
			}
		}
		else {
			player.isAtk = 0;
			player.state = STATE_FREE;
		}
			
	}
}

void Player2Attack() {
	if (STATE == STATE_PLAYING) {
		if (attack_k2 == 1) {
			player2.state = STATE_ATTACK;
			rangeAtk2 = 0.4;
			if (player2.ammo > 0) {
				if (p2_target != NULL) {
					p2_target->life -= 10;
					player2.isAtk = 1;
				}
				else
					player2.isAtk = 0;
				if (player2.ammo_time > player2.ammo_maxTime - 1) {
					player2.ammo -= 1;
					player2.ammo_time = 0;
				}
			}
			else
				player2.isAtk = 0;
		}
		else if (melee_k2 == 1) {
			player2.state = STATE_MELEE;
			rangeAtk2 = 0.1;
			if (p2_target != NULL) {
				p2_target->life -= 30;
				player2.isAtk = 1;
			}
		}
		else {
			player2.isAtk = 0;
			player2.state = STATE_FREE;
		}

	}
}

//Verifica se um zumbi esta morto
void CheckZombiesLife() {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (zombies[i].isActive) {
			if (zombies[i].life <= 0) {
				zombies[i].isActive = 0;
				wave->enemies_alive--;
			}
		}
	}
}

float glide = 0.01;
//Movimento do player
void MovePlayer() {
	if (STATE == STATE_PLAYING) {
		if (player.state == STATE_FREE) {
			if (up_k == 1){
				if (player.y < 0.1)
					player.dy = player.dy + MAX_ACC_PLAYER;
				else
					player.dy = 0;
			}
			if (down_k == 1){
				player.dy = player.dy - MAX_ACC_PLAYER;
			}

			if (up_k == 0 && down_k == 0){
				player.dy = 0;
			}

			if (left_k == 1 && right_k == 1) {}
			else if (left_k == 1) {
				player.dx = player.dx - MAX_ACC_PLAYER;
				player.dirX = PLAYER_LEFT;
				player.dirA = PLAYER_LEFT;
			}
			else if (right_k == 1) {
				player.dx = player.dx + MAX_ACC_PLAYER;
				player.dirX = PLAYER_RIGHT;
				player.dirA = PLAYER_RIGHT;
			}
			else if (left_k == 0 && right_k == 0) {
				if (player.dirX == PLAYER_RIGHT) {
					if (player.dx > 0) {
						player.dx -= glide;
					}
					else if (player.dx < 0) {
						player.dx = 0.0;
						player.dirX = 0;
					}
				}
				else if (player.dirX == PLAYER_LEFT) {
					if (player.dx < 0) {
						player.dx += glide;
					}
					else if (player.dx > 0) {
						player.dx = 0.0;
						player.dirX = 0;
					}
				}
			}

			double temp;
			if ((temp = (player.dx * player.dx + player.dy * player.dy)) > player.vmax2) {
				temp = player.vmax / sqrt(temp);
				player.dx *= temp;
				player.dy *= temp;
			}
			player.y = player.y + player.dy;
			player.x = player.x + player.dx;

			//PLAYER coleta munição
			if (player.x >= item_ammo.x - 0.05f && player.x <= item_ammo.x + 0.05f) {
				if (player.y <= item_ammo.y + 0.05f) {
					collectAmmoP1();
				}
			}
		}
	}
}

void MovePlayer2() {
	if (STATE == STATE_PLAYING) {
		if (player2.state == STATE_FREE) {
			if (up_k2 == 1) {
				if (player2.y < 0.1)
					player2.dy = player2.dy + MAX_ACC_PLAYER;
				else
					player2.dy = 0;
			}
			if (down_k2 == 1) {
				player2.dy = player2.dy - MAX_ACC_PLAYER;
			}

			if (up_k2 == 0 && down_k2 == 0) {
				player2.dy = 0;
			}

			if (left_k2 == 1 && right_k2 == 1) {}
			else if (left_k2 == 1) {
				player2.dx = player2.dx - MAX_ACC_PLAYER;
				player2.dirX = PLAYER_LEFT;
				player2.dirA = PLAYER_LEFT;
			}
			else if (right_k2 == 1) {
				player2.dx = player2.dx + MAX_ACC_PLAYER;
				player2.dirX = PLAYER_RIGHT;
				player2.dirA = PLAYER_RIGHT;
			}
			else if (left_k2 == 0 && right_k2 == 0) {
				if (player2.dirX == PLAYER_RIGHT) {
					if (player2.dx > 0) {
						player2.dx -= glide;
					}
					else if (player2.dx < 0) {
						player2.dx = 0.0;
						player2.dirX = 0;
					}
				}
				else if (player2.dirX == PLAYER_LEFT) {
					if (player2.dx < 0) {
						player2.dx += glide;
					}
					else if (player2.dx > 0) {
						player2.dx = 0.0;
						player2.dirX = 0;
					}
				}
			}

			double temp;
			if ((temp = (player2.dx * player2.dx + player2.dy * player2.dy)) > player2.vmax2) {
				temp = player2.vmax / sqrt(temp);
				player2.dx *= temp;
				player2.dy *= temp;
			}
			player2.y = player2.y + player2.dy;
			player2.x = player2.x + player2.dx;

			//PLAYER coleta munição
			if (player2.x >= item_ammo.x - 0.05f && player2.x <= item_ammo.x + 0.05f) {
				if (player2.y <= item_ammo.y + 0.05f) {
					collectAmmoP2();
				}
			}
		}
	}
}

//Verifica se o player colidiu com as bordas do cenario ou com o limite do eixo y
void checkBorder() {
	if (player.y > 0.9f)
		player.y = 0.9f;
	if (player.y < 0.02f)
		player.y = 0.02f;

	if (player.x < PLAYER_SPEED/100)
		player.x = PLAYER_SPEED / 100;
	if ((player.x + 0.08f) >= endCam)
		player.x = endCam - 0.08;	
}

void checkBorderP2() {
	if (player2.y > 0.9f)
		player2.y = 0.9f;
	if (player2.y < 0.02f)
		player2.y = 0.02f;

	if (player2.x < PLAYER_SPEED / 100)
		player2.x = PLAYER_SPEED / 100;
	if ((player2.x + 0.08f) >= endCam)
		player2.x = endCam - 0.08;
}

//Movimenta um zumbi especificado pelo ponteiro
void moveEnemy(Enemy * enemy) {
	float disZ = fabs(enemy->y - player.y);
	float disZ2 = fabs(enemy->y - player2.y);
	if (abs(enemy->x - player.x) < abs(enemy->x - player2.x) && player.getLife() > 0) {
		if (enemy->x > player.x - disToY && enemy->x < player.x + disToY) {
			if (enemy->y < player.y - dmgY) {
				//enemy->y += enemy->getSpeed() / factSpeedX;
				enemy->y += disZ*0.1;
			}
			else if (enemy->y > player.y + dmgY) {
				//enemy->y -= enemy->getSpeed() / factSpeedX;
				enemy->y -= disZ*0.1;
			}
		}

		if (enemy->x < player.x - 0.05) {
			enemy->x += (enemy->getSpeed() / factSpeedX)*game_speed;
			enemy->dir = ENEMY_RIGHT;
			player.isDmg = 0;
		}
		else if (enemy->x > player.x + 0.05) {
			enemy->x -= (enemy->getSpeed() / factSpeedX)*game_speed;
			enemy->dir = ENEMY_LEFT;
			player.isDmg = 0;
		}
		else {
			if (enemy->dir == ENEMY_LEFT)
				enemy->dir = ENEMY_LEFT_STAND;
			else if (enemy->dir == ENEMY_RIGHT)
				enemy->dir = ENEMY_RIGHT_STAND;

			if (enemy->y > player.y - dmgY && enemy->y < player.y + dmgY) {
				playerIsAttacked(enemy);
				player.isDmg = 1;
			}
		}
		if (enemy->dir == ENEMY_RIGHT)
			animateSprite(2, &enemy->anim, enemy->x, enemy->y, true, 1);
		else if (enemy->dir == ENEMY_LEFT)
			animateSprite(3, &enemy->anim, enemy->x, enemy->y, true, 1);
		else if (enemy->dir == ENEMY_RIGHT_STAND)
			animateSprite(2, &enemy->anim, enemy->x, enemy->y, false, 1);
		else
			animateSprite(3, &enemy->anim, enemy->x, enemy->y, false, 1);
	}

	else if (abs(enemy->x - player.x) >= abs(enemy->x - player2.x) && player2.getLife() > 0) {
		if (enemy->x > player2.x - disToY && enemy->x < player2.x + disToY) {
			if (enemy->y < player2.y - dmgY) {
				//enemy->y += enemy->getSpeed() / factSpeedX;
				enemy->y += disZ2*0.1;
			}
			else if (enemy->y > player2.y + dmgY) {
				//enemy->y -= enemy->getSpeed() / factSpeedX;
				enemy->y -= disZ2*0.1;
			}
		}

		if (enemy->x < player2.x - 0.05) {
			enemy->x += (enemy->getSpeed() / factSpeedX)*game_speed;
			enemy->dir = ENEMY_RIGHT;
			player2.isDmg = 0;
		}
		else if (enemy->x > player2.x + 0.05) {
			enemy->x -= (enemy->getSpeed() / factSpeedX)*game_speed;
			enemy->dir = ENEMY_LEFT;
			player2.isDmg = 0;
		}
		else {
			if (enemy->dir == ENEMY_LEFT)
				enemy->dir = ENEMY_LEFT_STAND;
			else if (enemy->dir == ENEMY_RIGHT)
				enemy->dir = ENEMY_RIGHT_STAND;

			if (enemy->y > player2.y - dmgY && enemy->y < player2.y + dmgY) {
				player2IsAttacked(enemy);
				player2.isDmg = 1;
			}
		}
		if (enemy->dir == ENEMY_RIGHT)
			animateSprite(2, &enemy->anim, enemy->x, enemy->y, true, 1);
		else if (enemy->dir == ENEMY_LEFT)
			animateSprite(3, &enemy->anim, enemy->x, enemy->y, true, 1);
		else if (enemy->dir == ENEMY_RIGHT_STAND)
			animateSprite(2, &enemy->anim, enemy->x, enemy->y, false, 1);
		else
			animateSprite(3, &enemy->anim, enemy->x, enemy->y, false, 1);
	}

	else if (player.getLife() == 0) {
		if (enemy->x > player2.x - disToY && enemy->x < player2.x + disToY) {
			if (enemy->y < player2.y - dmgY) {
				//enemy->y += enemy->getSpeed() / factSpeedX;
				enemy->y += disZ2*0.1;
			}
			else if (enemy->y > player2.y + dmgY) {
				//enemy->y -= enemy->getSpeed() / factSpeedX;
				enemy->y -= disZ2*0.1;
			}
		}

		if (enemy->x < player2.x - 0.05) {
			enemy->x += (enemy->getSpeed() / factSpeedX)*game_speed;
			enemy->dir = ENEMY_RIGHT;
			player2.isDmg = 0;
		}
		else if (enemy->x > player2.x + 0.05) {
			enemy->x -= (enemy->getSpeed() / factSpeedX)*game_speed;
			enemy->dir = ENEMY_LEFT;
			player2.isDmg = 0;
		}
		else {
			if (enemy->dir == ENEMY_LEFT)
				enemy->dir = ENEMY_LEFT_STAND;
			else if (enemy->dir == ENEMY_RIGHT)
				enemy->dir = ENEMY_RIGHT_STAND;

			if (enemy->y > player2.y - dmgY && enemy->y < player2.y + dmgY) {
				player2IsAttacked(enemy);
				player2.isDmg = 1;
			}
		}
		if (enemy->dir == ENEMY_RIGHT)
			animateSprite(2, &enemy->anim, enemy->x, enemy->y, true, 1);
		else if (enemy->dir == ENEMY_LEFT)
			animateSprite(3, &enemy->anim, enemy->x, enemy->y, true, 1);
		else if (enemy->dir == ENEMY_RIGHT_STAND)
			animateSprite(2, &enemy->anim, enemy->x, enemy->y, false, 1);
		else
			animateSprite(3, &enemy->anim, enemy->x, enemy->y, false, 1);
	}

	else if (player2.getLife() == 0) {
		if (enemy->x > player.x - disToY && enemy->x < player.x + disToY) {
			if (enemy->y < player.y - dmgY) {
				//enemy->y += enemy->getSpeed() / factSpeedX;
				enemy->y += disZ*0.1;
			}
			else if (enemy->y > player.y + dmgY) {
				//enemy->y -= enemy->getSpeed() / factSpeedX;
				enemy->y -= disZ*0.1;
			}
		}

		if (enemy->x < player.x - 0.05) {
			enemy->x += (enemy->getSpeed() / factSpeedX)*game_speed;
			enemy->dir = ENEMY_RIGHT;
			player.isDmg = 0;
		}
		else if (enemy->x > player.x + 0.05) {
			enemy->x -= (enemy->getSpeed() / factSpeedX)*game_speed;
			enemy->dir = ENEMY_LEFT;
			player.isDmg = 0;
		}
		else {
			if (enemy->dir == ENEMY_LEFT)
				enemy->dir = ENEMY_LEFT_STAND;
			else if (enemy->dir == ENEMY_RIGHT)
				enemy->dir = ENEMY_RIGHT_STAND;

			if (enemy->y > player.y - dmgY && enemy->y < player.y + dmgY) {
				playerIsAttacked(enemy);
				player.isDmg = 1;
			}
		}
		if (enemy->dir == ENEMY_RIGHT)
			animateSprite(2, &enemy->anim, enemy->x, enemy->y, true, 1);
		else if (enemy->dir == ENEMY_LEFT)
			animateSprite(3, &enemy->anim, enemy->x, enemy->y, true, 1);
		else if (enemy->dir == ENEMY_RIGHT_STAND)
			animateSprite(2, &enemy->anim, enemy->x, enemy->y, false, 1);
		else
			animateSprite(3, &enemy->anim, enemy->x, enemy->y, false, 1);
	}
}

//Movimenta os zumbis
void MoveEnemies() {
	if (STATE == STATE_PLAYING) {
		for (int i = 0; i < wave->enemies_byWave; i++) {
			if (zombies[i].isActive) {
				moveEnemy(&zombies[i]);
			}
		}
	}
}

//Mostra cronometro
void UpdateChronometer() {
	DrawTxt(chrono.getTime(), 15, 255, 255, 255, absCam + 0.4, 0.95);
}

//Desenha componentes da camada base do jogo
void LayerBase() {
	CreateItemAmmo();
	DrawPlayer();
	if(option_players == 1)
		DrawPlayer2();
	MoveEnemies();
	if (p1_target != NULL) {
		if (p1_target->life <= 0) {
			animateSprite(8, &p1_target->animDeath, p1_target->x + 0.02, p1_target->y, true, 0);
		}
	}
	if (p2_target != NULL) {
		if (p2_target->life <= 0) {
			animateSprite(8, &p2_target->animDeath, p2_target->x + 0.02, p2_target->y, true, 0);
		}
	}
	if (player.isAtk == 1) 
		if (p1_target != NULL)
			animateSprite(9, &player.animBlood, p1_target->x + 0.02, p1_target->y + 0.05, true,1);

	if(player.isDmg == 1)
		animateSprite(9, &player.animBlood, player.x + 0.02, player.y + 0.05, true, 1);

	if (player2.isAtk == 1)
		if (p2_target != NULL)
			animateSprite(9, &player2.animBlood, p2_target->x + 0.02, p2_target->y + 0.05, true, 1);

	if (player2.isDmg == 1)
		animateSprite(9, &player2.animBlood, player2.x + 0.02, player2.y + 0.05, true, 1);
	
	UpdateChronometer();
}

//Desenha componentes do menu principal
void DrawMainMenu() {
	DrawImage(imgs[0], absCam - 0.5, 0.0f, 1);
	DrawImage(imgs[9], absCam - 0.2, 0.6f, 1);
	if (option_ini == 0) {
		DrawTxt("Iniciar Jogo", 12, 255, 0, 0, absCam - 0.2, 0.5);
		DrawTxt("Jogadores", 9, 255, 255, 255, absCam - 0.2, 0.45);
		DrawTxt("Sair", 4, 255, 255, 255, absCam - 0.2, 0.40);


		if (option_dif == 0) {
			DrawTxt("Facil", 5, 10, 255, 10, absCam + 0.01, 0.5);
			DrawTxt("Moderado", 8, 255, 255, 255, absCam + 0.1, 0.5);
			DrawTxt("Dificil", 7, 255, 255, 255, absCam + 0.23, 0.5);
		}
		else if (option_dif == 1) {
			DrawTxt("Facil", 5, 255, 255, 255, absCam + 0.01, 0.5);
			DrawTxt("Moderado", 8, 200, 200, 0, absCam + 0.1, 0.5);
			DrawTxt("Dificil", 7, 255, 255, 255, absCam + 0.23, 0.5);
		}
		else {
			DrawTxt("Facil", 5, 255, 255, 255, absCam + 0.01, 0.5);
			DrawTxt("Moderado", 8, 255, 255, 255, absCam + 0.1, 0.5);
			DrawTxt("Dificil", 7, 200, 0, 0, absCam + 0.23, 0.5);
		}
	}
	else if (option_ini == 1) {
		DrawTxt("Iniciar Jogo", 12, 255, 255, 255, absCam - 0.2, 0.5);
		DrawTxt("Jogadores", 9, 255, 0, 0, absCam - 0.2, 0.45);
		DrawTxt("Sair", 4, 255, 255, 255, absCam - 0.2, 0.40);

		if (option_players == 0) {
			DrawTxt("1 Jogador", 9, 10, 255, 10, absCam + 0.01, 0.45);
			DrawTxt("2 Jogadores", 11, 255, 255, 255, absCam + 0.15, 0.45);
		}
		else if (option_players == 1) {
			DrawTxt("1 Jogador", 9, 255, 255, 255, absCam + 0.01, 0.45);
			DrawTxt("2 Jogadores", 11, 10, 255, 10, absCam + 0.15, 0.45);
		}

	}
	else {
		DrawTxt("Iniciar Jogo", 12, 255, 255, 255, absCam - 0.2, 0.5);
		DrawTxt("Jogadores", 9, 255, 255, 255, absCam - 0.2, 0.45);
		DrawTxt("Sair", 4, 255, 0, 0, absCam - 0.2, 0.40);
	}
}

//Desenha componentes da camada de interface
void LayerGUI() {
	DrawLife();
	DrawPoints();
	DrawWave();

	if (p1_target != NULL) {
		//DrawTxt("V", 1, 255, 0, 0, p1_target->x + 0.02, p1_target->y + 0.15);

		char enemy_txt[200];
		sprintf_s(enemy_txt, 200, "%i", p1_target->life);
		DrawTxt(enemy_txt, 15, 255, 0, 0, p1_target->x + 0.02, p1_target->y + 0.15);
	}

	if (STATE == STATE_MAINMENU) {
		DrawMainMenu();
	}
	if (STATE == STATE_GAMEOVER || STATE == STATE_GAMEWIN) {
		if (STATE == STATE_GAMEOVER) {
			DrawImage(imgs[6], absCam - 0.5, 0.0f, 1);
		}
		else if (STATE == STATE_GAMEWIN) {
			DrawImage(imgs[7], absCam - 0.5, 0.0f, 1);
		}
		DrawTxt("Aperte ENTER para voltar", 24, 255, 0, 0, absCam - 0.2, 0.3);
	}
	
}

//Desenha as camadas
void DrawLayers() {
	for (int i = 0; i < LAYER_NUMBER; i++) {
		if (i == LAYER_BASE) {
			if (STATE == STATE_PLAYING)
				LayerBase();
		}
		else if (i == LAYER_GUI) {
			LayerGUI();
		}
		else {
			if (STATE == STATE_PLAYING) {
				for (int j = 0; j < layers[i].numImgs; j++) {
					if (layers[i].getLayer()[0].isInvalid() == 0) {
						Image img = layers[i].getLayer()[j];

						float posX = 0, posY = 0;

						if (i == 0) posX = absCam - 0.5;

						DrawImage(img, posX, posY, layers[i].scrollX);
					}
				}
			}
		}
	}
}

//Metodo de display
void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	DrawLayers();

	glutSwapBuffers();
}

//Ao precionar um botão de ação do jogador
void keyPress(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'w':
		if (player.getLife() > 0)
			up_k = 1;
		break;
	case 'a':
		if (player.getLife() > 0)
			left_k = 1;
		break;
	case 's':
		if (player.getLife() > 0)
			down_k = 1;
		break;
	case 'd':
		if (player.getLife() > 0)
			right_k = 1;
		break;
	case ' ':
		if (player.getLife() > 0)
			attack_k = 1;
		break;
	case 'f':
		if (player.getLife() > 0)
			melee_k = 1;
		break;

	case '8':
		if (player2.getLife() > 0)
			up_k2 = 1;
		break;
	case '4':
		if (player2.getLife() > 0)
			left_k2 = 1;
		break;
	case '5':
		if (player2.getLife() > 0)
			down_k2 = 1;
		break;
	case '6':
		if (player2.getLife() > 0)
			right_k2 = 1;
		break;
	case '0':
		if (player2.getLife() > 0)
			if (option_players == 1)
				attack_k2 = 1;
		break;
	case '-':
		if (player2.getLife() > 0)
			if (option_players == 1)
				melee_k2 = 1;
		break;

	case 'r':
		if (STATE != STATE_PLAYING) {
			ResetGame();
		}
		break;
	case 13:
		if (STATE == STATE_MAINMENU) {
			if (option_ini == 0) {
				ResetGame();
				//STATE = STATE_PLAYING;
			}
			else {
				exit(0);
			}
		}
		else if (STATE == STATE_GAMEOVER || STATE == STATE_GAMEWIN) 
			option_restart = 1;
		break;
	}
}

//Ao soltar um botão de ação do jogador
void keyRelease(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'w':
		up_k = 0;
		break;
	case 'a':
		left_k = 0;
		break;
	case 's':
		down_k = 0;
		break;
	case 'd':
		right_k = 0;
		break;
	case ' ':
		attack_k = 0;
		break;
	case 'f':
		melee_k = 0;
		break;

	case '8':
		up_k2 = 0;
		break;
	case '4':
		left_k2 = 0;
		break;
	case '5':
		down_k2 = 0;
		break;
	case '6':
		right_k2 = 0;
		break;
	case '0':
		if (option_players == 1)
			attack_k2 = 0;
		break;
	case '-':
		if (option_players == 1)
			melee_k2 = 0;
		break;
	}
}

// Botões especiais para comando da interface
void specialKeys(int button, int x, int y) {
	if (STATE == STATE_MAINMENU) {
		switch (button) {
		case GLUT_KEY_UP:
			if (option_ini > 0)
				option_ini--;
			break;
		case GLUT_KEY_DOWN:
			if (option_ini < 2)
				option_ini++;
			break;
		case GLUT_KEY_RIGHT:
			if (option_ini == 0) {
				if (option_dif < 2)
					option_dif++;
			}
			else if (option_ini == 1)
				if (option_players < 1)
					option_players++;
			break;
		case GLUT_KEY_LEFT:
			if (option_ini == 0) {
				if (option_dif > 0)
					option_dif--;
			}
			else if (option_ini == 1)
				if (option_players > 0)
					option_players--;
			break;
		}
	}
}

//Movimenta a camera na posição do player
void moveCamera() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (player.getLife() > 0) {
		if ((player.x - 0.5) > 0 && (player.x + 0.5) < endCam)
			xCam = (ortSize / 2) - player.x;
	}
	else {
		if ((player2.x - 0.5) > 0 && (player2.x + 0.5) < endCam)
			xCam = (ortSize / 2) - player2.x;
	}
	absCam = -(xCam - (ortSize / 2));
	glTranslatef(xCam, 0, 0);
}

//Timer para animação
int curtime = 0;

void Timer(int iUnused) {
	if (wave->enemies_alive>0)
		EnemyInRange();
	
	CheckZombiesLife();
	PlayerAttack();
	MovePlayer();
	Player2Attack();
	MovePlayer2();
	checkBorder();
	checkBorderP2();
	GameUpdate();
	moveCamera();

	curtime += time_fpms;
	if (curtime >= 1000) {
		curtime = 0;
		if (player.state == STATE_ATTACK && player.ammo > 0) 
			player.ammo_time++;
		if (player2.state == STATE_ATTACK && player2.ammo > 0)
			player2.ammo_time++;

		for (int i = 0; i < wave->enemies_byWave; i++) {
			if (zombies[i].isActive)
				zombies[i].dps_time++;
		}

		chrono.addSecond();
		//cout << secs << endl;
	}

	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (zombies[i].isActive) {
			zombies[i].anim.addTime(0.1);
			zombies[i].animDeath.addTime(0.1);
		}
	}

	player.animMove.addTime(0.1);
	player.animBlood.addTime(0.1);

	player2.animMove.addTime(0.1);
	player2.animBlood.addTime(0.1);

	if (player.state == STATE_ATTACK) {
		if (player.dirA == PLAYER_LEFT)
			player.animFireL.addTime(0.1);
		else
			player.animFireR.addTime(0.1);
	}

	if (player.state == STATE_MELEE) {
		if (player.dirA == PLAYER_LEFT)
			player.animMeleeL.addTime(0.1);
		else
			player.animMeleeR.addTime(0.1);
	}

	if (player2.state == STATE_ATTACK) {
		if (player2.dirA == PLAYER_LEFT)
			player2.animFireL.addTime(0.1);
		else
			player2.animFireR.addTime(0.1);
	}

	if (player2.state == STATE_MELEE) {
		if (player2.dirA == PLAYER_LEFT)
			player2.animMeleeL.addTime(0.1);
		else
			player2.animMeleeR.addTime(0.1);
	}

	glutPostRedisplay();
	glutTimerFunc(time_fpms, Timer, 0);
}

//Metodo de inicio
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, ortSize, 0, ortSize, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//Reajuste da janela
void reshape(int w, int h) {
	if (h == 0)
		h = 1;
	sWidth = w;
	sHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, ortSize, 0, ortSize, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//MAIN
int main(int argc, char** argv) {
	srand(time(NULL));

	loadSystem();
	glutInit(&argc, argv);
	GLint center_x = (glutGet(GLUT_SCREEN_WIDTH) - sWidth) / 2;
	GLint center_y = (glutGet(GLUT_SCREEN_HEIGHT) - sHeight) / 2;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(sWidth, sHeight);
	glutInitWindowPosition(center_x, center_y);
	glutCreateWindow("Zombity");
	init();
	Timer(0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutSpecialFunc(specialKeys);
	glutMainLoop();
	return 0;
}
