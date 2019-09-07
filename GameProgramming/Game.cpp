#include "Game.h"
#include <SDL.h>
#include <cmath>
#include <cstdio>

Game::Game()
	: mWindow(nullptr)
	, mIsGameRunning(true)
	, mRenderer(nullptr)
	, mPaddlePos({0, 384})
	, mBallPos({512, 384})
	, mBallVel({-50.f, 75.f})
	, mTicksCount(0)
	, mPaddleDir(0)
	, mPaddle2Pos({1024, 384})
	, mPaddle2Dir(0)
{ }

bool Game::Initialize()
{
	const int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize with error: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Game", 100, 50, 1024, 768, 0);
	if (!mWindow)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Unable to render: %s", SDL_GetError());
		return false;
	}
	return true;
}

void Game::RunLoop()
{
	while(mIsGameRunning)
	{
		ProcessEvents();
		GenerateOutput();
		UpdateGame();
	}
}

void Game::Shutdown() const
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::ProcessEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				mIsGameRunning = false;
				break;
		}
	}

	// Get the state of the keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsGameRunning = false;
	}

	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}

	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}

	mPaddle2Dir = 0;
	if (state[SDL_SCANCODE_UP])
	{
		mPaddle2Dir -= 1;
	}

	if (state[SDL_SCANCODE_DOWN])
	{
		mPaddle2Dir += 1;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(mRenderer);

	// Here draw the screen
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	const SDL_Rect topWall{ 0, 0, 1024,  mThickness};
	const SDL_Rect bottomWall{ 0, 768 - mThickness, 1024, mThickness };
	const SDL_Rect rightWall{ 1024 - mThickness, 0, mThickness, 768 };

	const SDL_Rect paddle{ static_cast<int>(mPaddlePos.x - 1.5 * mThickness), static_cast<int>(mPaddlePos.y - 4 * mThickness), 3 * mThickness, 8 * mThickness };
	const SDL_Rect paddle2{ static_cast<int>(mPaddle2Pos.x - 1.5 * mThickness), static_cast<int>(mPaddle2Pos.y - 4 * mThickness), 3 * mThickness, 8 * mThickness };
	const SDL_Rect ball{ static_cast<int>(mBallPos.x - mThickness / 2), static_cast<int>(mBallPos.y - mThickness / 2), mThickness, mThickness };

	SDL_RenderFillRect(mRenderer, &topWall);
	SDL_RenderFillRect(mRenderer, &bottomWall);
	//SDL_RenderFillRect(mRenderer, &rightWall);

	SDL_RenderFillRect(mRenderer, &paddle);
	SDL_RenderFillRect(mRenderer, &paddle2);
	SDL_RenderFillRect(mRenderer, &ball);
	
	SDL_RenderPresent(mRenderer);
}

void Game::UpdateGame()
{
	// Wait for 16 ms (frame limiting)
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// calculate the delta time
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;

	// clamp maximum delta time to 0.05f per frame
	deltaTime = deltaTime > 0.05f ? 0.05f : deltaTime;

	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 100.0f * deltaTime;

		// don't let the paddle go out of screen
		if (mPaddlePos.y < mPaddleH/2 - mThickness)
		{
			mPaddlePos.y = mPaddleH / 2 - mThickness;
		}
		else if (mPaddlePos.y > (768.f - mPaddleH /2 - mThickness))
		{
			mPaddlePos.y = 768.f - mPaddleH / 2 - mThickness;
		}
	}

	if (mPaddle2Dir != 0)
	{
		mPaddle2Pos.y += mPaddle2Dir * 100.0f * deltaTime;

		// don't let the paddle go out of screen
		if (mPaddle2Pos.y < mPaddleH / 2 - mThickness)
		{
			mPaddle2Pos.y = mPaddleH / 2 - mThickness;
		}
		else if (mPaddle2Pos.y > (768.f - mPaddleH / 2 - mThickness))
		{
			mPaddle2Pos.y = 768.f - mPaddleH / 2 - mThickness;
		}
	}

	mBallPos.y += mBallVel.y * deltaTime;
	mBallPos.x += mBallVel.x * deltaTime;

	// check for boundaries and make the ball go in the opposite direction
	if (mBallPos.y <= mThickness && mBallVel.y < 0.f)
	{
		mBallVel.y *= -1;
	}

	if (mBallPos.y >= 768.f - mThickness && mBallVel.y > 0.f)
	{
		mBallVel.y *= -1;
	}

	/*if (mBallPos.x >= 1024 - mThickness && mBallVel.x > 0.f)
	{
		mBallVel.x *= -1;
	}*/

	// check if the ball hits the paddle
	double yDiff = std::fabs(mBallPos.y - mPaddlePos.y);
	if (yDiff <= mPaddleH/2.f && mBallPos.x >= 20.f && mBallPos.x <= 29.f && mBallVel.x < 0.f)
	{
		mBallVel.x *= -1.f;
	}

	double yDiff2 = std::fabs(mBallPos.y - mPaddle2Pos.y);
	if (yDiff2 <= mPaddleH / 2.f && mBallPos.x >= 1015.f && mBallPos.x <= 1024.f && mBallVel.x > 0.f)
	{
		mBallVel.x *= -1.f;
	}

	// Did the ball go off the screen? (if so, end game)
	else if (mBallPos.x <= 0.0f)
	{
		mIsGameRunning = false;
	}
}

int main(int argc, char ** argv)
{
	Game game;
	const bool success = game.Initialize();
	if (success)
	{
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}
