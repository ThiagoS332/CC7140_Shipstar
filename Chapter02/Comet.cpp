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
#include <stdio.h>
#include <stdlib.h> 
#include <ctime>
#include <time.h> 
#include <cstdlib>
#include <iostream>

using namespace std;

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
