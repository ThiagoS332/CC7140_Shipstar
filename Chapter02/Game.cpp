// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include "SDL/SDL_image.h"
#include <algorithm>
#include "SpriteComponent.h"
#include "BGSpriteComponent.h"
#include "Actor.h"
#include "Ship.h"
#include "Missile.h"
#include "Comet.h"
#include <ctime>
#include <time.h> 
#include <cstdlib>
#include <iostream>

using namespace std;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
,mUpdatingActors(false)
{
	
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 2)", 100, 100, 1024, 768, 0);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();

	LastCometTick = mTicksCount + 1;

	this_comet_gonna_hit = true;
	
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// Process ship input
	mShip->ProcessKeyboard(state);
}

bool Game::Cooldown(Uint32 LastTick, float CooldowDuration)
{
	float time_elapsed = (mTicksCount - LastTick) / 1000.0f;

	if (time_elapsed >= CooldowDuration) {
		return true;
	}

	return false;

}

void Game::SpawnComet()
{
	srand(time(NULL));

	// Should You Spawn Comets = SYSC
	int SYSC = 1 + (rand() % 100000);

	//cout << "SYSC = " << SYSC << endl;

	if (SYSC > 1) {

		mComet = new Comet(this);
		mComet->SetScale(1.0f);
		//mComet->SetRotation((rand() % 50));
		
		// Comet Spwan Point = CSP
		int CSP = (rand() % 3);

		//cout << "CSP = " << CSP << endl;

		// Spawn comet in the lower right corner
		if (CSP == 0) {
			mComet->SetPosition(Vector2((rand() % 178) + 896, (rand() % 50) + 768));
			mComet->SetRightSpeed(-1 * (51 + (rand() % 450)));
			mComet->SetDownSpeed(-1 * (101 + (rand() % 400)));
		}
		// Spawn comet in the right side
		else if (CSP == 1) {
			mComet->SetPosition(Vector2((rand() % 50) + 1024, (rand() % 768)));
			mComet->SetRightSpeed(-1 * (51 + (rand() % 450)));
			if ((rand() % 2) == 0) {
				mComet->SetDownSpeed(-1 * (51 + (rand() % 450)));
			}
			else {
				mComet->SetDownSpeed(51 + (rand() % 450));
			}

		}
		// Spawn comet in the upper right corner
		else if (CSP == 2) {
			mComet->SetPosition(Vector2((rand() % 178) + 896, (rand() % 50) - 50));
			mComet->SetRightSpeed(-1 * (51 + (rand() % 450)));
			mComet->SetDownSpeed(101 + (rand() % 400));
		}

	}
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	if (mShip->GetShoot() && mShip->Cooldown(SDL_GetTicks())) {
		mShip->SetLastShoot(SDL_GetTicks());
		mMissile = new Missile(this);
		mMissile->SetPosition(Vector2(mShip->GetPosition().x + 60, mShip->GetPosition().y));
		mMissile->SetScale(0.04f);
	}

	// Spawn comets
	if (Cooldown(LastCometTick, 1.0f)) {

		LastCometTick = mTicksCount;

		//SpawnComet();

		/*mUpdatingActors = true;
		AddActor(mComet);
		mUpdatingActors = false;*/

	}

	if (this_comet_gonna_hit) {
		mComet = new Comet(this);
		mComet->SetPosition(Vector2(250.0f, 384.0f));
		mComet->SetScale(1.0f);
		this_comet_gonna_hit = false;
	}

	// Check collisions
	for (auto actor : mActors)
	{
		class Actor* actor;

		Vector2 ship_pos = mShip->GetPosition();

		for (int i = 0; i < mActors.size(); i++) {
			/*
			* mActors[i]->GetType() :
			*	0 = Ship
			*	1 = Background
			*	2 = Missile
			*	3 = Comet
			*/


			if (mActors[i]->GetType() == actor->EEnemy &&
				(mActors[i]->GetPosition().x >= ship_pos.x && mActors[i]->GetPosition().x <= ship_pos.x + 70) &&
				(mActors[i]->GetPosition().y >= ship_pos.y && mActors[i]->GetPosition().y <= ship_pos.y + 30)) {
				cout << "FOK, DEY RIT DA XHIP" << endl;
				mIsRunning = false;
			}
			/*if (mActors[i]->GetType() != 0 && mActors[i]->GetType() != 1 && mActors[i]->GetType() != 2) {
				cout << "i = " << i << endl;
				cout << "Type = " << mActors[i]->GetType() << endl;
				cout << "PosX = " << mActors[i]->GetPosition().x << endl;
				cout << "PosY = " << mActors[i]->GetPosition().y << endl;
			}*/
			
		}

		//mIsRunning = false;

	}

	// Update all actors
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// Move any pending actors to mActors
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// Add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// Delete dead actors (which removes them from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
	
	// Draw all sprite components
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	// Create player's ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, 384.0f));
	mShip->SetScale(1.5f);
	mShip->SetLastShoot(1.0f);

	// Create actor for the background (this doesn't need a subclass)
	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2(512.0f, 384.0f));
	// Create the "far back" background
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);
	// Create the closer background
	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// Is the texture already in the map?
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// Load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// Create texture from surface
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	// If we're updating actors, need to add to pending
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for ( ;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// (We can't swap because it ruins ordering)
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}
