// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Comet.h"
#include "AnimSpriteComponent.h"
#include "Game.h"
#include <iostream>

Comet::Comet(Game* game)
	:Actor(game)
	, mRightSpeed(0.0f)
	, mDownSpeed(0.0f)
	, mHealth(10.0f)
	, mScoreValue(5)
{
	// Create an animated sprite component
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/comet.png"),
	};
	asc->SetAnimTextures(anims);

	SetType(EEnemy);
}

void Comet::SetRightSpeed(float mRightSpeed)
{
	this->mRightSpeed = mRightSpeed;
}

void Comet::SetDownSpeed(float mDownSpeed)
{
	this->mDownSpeed = mDownSpeed;
}

void Comet::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	// Update position based on speeds and delta time
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;
	// Kill actor if it leaves the screen
	if (pos.x < -50.0f || pos.x > 1024.0f + 50.0f) // 1074
	{
		SetState(EDead);
	}
	else if (pos.y < -50.0f || pos.y > 768.0f + 50.0f) // 818
	{
		SetState(EDead);
	}
	SetPosition(pos);
}

void Comet::SpawnComet(class Game* game, class Comet* mComet)
{
	srand(time(0));

	// Should You Spawn Comets = SYSC
	int SYSC = 1 + (rand() % 100000);

	if (SYSC > 10000) {

		SYSC = 0;

		mComet = new Comet(game);
		mComet->SetScale(1.0f);
		//mComet->SetRotation((rand() % 50));

		// Comet Spwan Point = CSP
		int CSP = (rand() % 3);

		// Spawn comet in the lower right corner
		if (CSP == 0) {
			mComet->SetPosition(Vector2((rand() % 178) + 896, (rand() % 50) + 768));
			mComet->SetRightSpeed(-1 * (51 + (rand() % 450)));
			mComet->SetDownSpeed(-1 * (101 + (rand() % 400)));
		}
		// Spawn comet in the right side
		else if (CSP == 1) {
			mComet->SetPosition(Vector2((rand() % 178) + 1024, (rand() % 50) + 768));
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
			mComet->SetPosition(Vector2((rand() % 178) + 1024, (rand() % 50) - 50));
			mComet->SetRightSpeed(-1 * (51 + (rand() % 450)));
			mComet->SetDownSpeed(101 + (rand() % 400));
		}

	}
}