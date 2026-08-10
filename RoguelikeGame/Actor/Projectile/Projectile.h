#pragma once

#include "Actor/Actor.h"

namespace Craft
{ 
	/* 게임 내 폰들이 발사하는 Projectile */
	class Projectile : public Actor
	{
		TYPE_DECLARATIONS(Projectile, Actor)

	public:
		Projectile(const Craft::Vector2Float& inPosition, 
					const std::wstring& inImage, 
					Craft::Color inColor, 
					float inRange,
					const Craft::Vector2Float& inDirection);
		virtual ~Projectile() = default;

	public:
		virtual void Tick(float deltaTime) override;

	private:
		float range = 0.f;
		Craft::Vector2Float direction = Craft::Vector2Float::Zero;
	};	
}


