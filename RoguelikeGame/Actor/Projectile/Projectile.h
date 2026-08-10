#pragma once

#include "Actor/Actor.h"
#include "Component/MovementComponent.h"

/* 게임 내 폰들이 발사하는 Projectile */
class Projectile : public Craft::Actor
{
	TYPE_DECLARATIONS(Projectile, Actor)

public:
	Projectile(const Craft::Vector2Float& inPosition, 
				const std::wstring& inImage, 
				Craft::Color inColor, 
				const Craft::Vector2Float& inDestination,
				float moveDelay);
	virtual ~Projectile() = default;

public:
	virtual void Tick(float deltaTime) override;

private:
	/* Projectile의 조준 지점 */
	Craft::Vector2Float destinationPos = Craft::Vector2Float::Zero;

	/* 이동 컴포넌트 */
	std::shared_ptr<Craft::MovementComponent> movementComponent;
};	



