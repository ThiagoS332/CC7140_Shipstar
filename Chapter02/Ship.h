// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
class Ship : public Actor
{
public:
	Ship(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyboard(const uint8_t* state);
	float GetRightSpeed() const { return mRightSpeed; }
	float GetDownSpeed() const { return mDownSpeed; }
	void SetShoot(bool mShoot) { this->mShoot = mShoot; }
	bool GetShoot() const { return mShoot; }
	void SetLastShoot(float mLastShoot) { this->mLastShoot = mLastShoot; };
	float GetLastShoot() const { return mLastShoot; }
	bool Cooldown(float Ticks);
private:
	float mRightSpeed;
	float mDownSpeed;
	bool mShoot;
	float mLastShoot;
};