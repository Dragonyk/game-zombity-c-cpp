#pragma once
class Sprite_Animation
{
public:
	Sprite_Animation();

	void setAnimation(int min, int max, float tmax, int);

	void nextFrame();

	int getCurFrame();

	void startAnimation();

	void setFrame(int value);

	void addTime(float value);

	float getTime();

	void resetTime();

	int		wSprite;
	int		hSprite;

	int		iddleFrame;
	int		minAnim;
	int		maxAnim;
	float	maxTimeAnim;
private:
	int		 curAnim;
	float	 curTimeAnim;
};

