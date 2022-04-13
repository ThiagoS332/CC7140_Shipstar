// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Missile.h"
#include "AnimSpriteComponent.h"
#include "Game.h"
#include <iostream>

Missile::Missile(Game* game)
	:Actor(game)
	, mRightSpeed(350.0f)
	, mDownSpeed(0.0f)
	, mMissileDamage(10.5f)
{
	// Create an animated sprite component
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Shots/missile.png"),
	};
	asc->SetAnimTextures(anims);

	SetType(EProjectile);
}

void Missile::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	// Update position based on speeds and delta time
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	//pos.y += mDownSpeed * deltaTime;
	// Restrict position to left of screen
	if (pos.x > 1048.0f)
	{
		SetState(EDead);
	}
	SetPosition(pos);
}

