#pragma once
#include "SDL_hints.h"

class SDL_Window;
class SDL_Renderer;

struct Vector2
{
	float x;
	float y;
};

class Game
{
private:
	SDL_Window* mWindow;
	bool mIsGameRunning;
	SDL_Renderer* mRenderer;

	Vector2 mPaddlePos;
	Vector2 mBallPos;
	Vector2 mBallVel;
	
	Uint32 mTicksCount;

	int mPaddleDir;

	Vector2 mPaddle2Pos;
	int mPaddle2Dir;

	const int mThickness = 10;
	const int mPaddleH = 8 * mThickness;

public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown() const;

	void ProcessEvents();
	void GenerateOutput();
	void UpdateGame();
};