// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
class Comet : public Actor
{
public:
	Comet(class Game* game);
	void UpdateActor(float deltaTime) override;
	void SetRightSpeed(float mRightSpeed);
	void SetDownSpeed(float mDownSpeed);
	float GetRightSpeed() const { return mRightSpeed; }
	float GetDownSpeed() const { return mDownSpeed; }
private:
	float mRightSpeed;
	float mDownSpeed;
	float mHealth;
	int mScoreValue;
};