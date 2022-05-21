#ifndef IS_DEFINED_GAME
#define IS_DEFINED_GAME

#include "Audio.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

#define WIDTH 50
#define HEIGHT 20

#define INIT_SPEED 50
#define DEC_SPEED_PER_SCORE 5

#define IS_GAMEOVER_WHEN_TOUCH_BOARD 0

class Game
{
	HANDLE hConsole;
	enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };

public:
	Game(HANDLE* hConsole);
	
private:
	int x, y, coinX, coinY, score;
	int tailX[100], tailY[100]; // record sub-snake
	int nTail;
	eDirecton dir;

public:
	bool isStarted, isGameOver;
	int speed;

	void Setup();
	void DrawBorder();
	void UpdateMap();
	void GetInput();
	void Logic();
	void UpdateScoreText();
};
#endif // !IS_DEFINED_GAME