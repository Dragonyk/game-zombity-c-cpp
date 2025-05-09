#ifndef GAME_H
#define GAME_H

class Game
{
public:
	Game(int);
	Game();
	void setPoints(int);
	int getPoints();

	int pointsToWin;
private:
	int remPoints;
};


#endif // !GAME_H