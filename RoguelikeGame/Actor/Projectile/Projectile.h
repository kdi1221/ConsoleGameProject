#pragma once

#include "Actor/ActorOnTile.h"
#include "Component/PathMoveComponent.h"


/* 게임 내 폰들이 발사하는 Projectile */
class Projectile : public ActorOnTile
{
	TYPE_DECLARATIONS(Projectile, ActorOnTile)

public:
	Projectile(const Craft::Vector2Float& inPosition, 
				const std::wstring& inImage, 
				Craft::Color inColor, 
				const Craft::Vector2Float& inDestination,
				float moveDelay);
	virtual ~Projectile() = default;

private:
	virtual void BeginPlay() override;

	/* 특정 타일 위에 있는 동안 호출되는 함수 */
	virtual void OnTileOverlap(const eTileCategory tileCategory) override;

private:
	void OnCallbackMoveFinish();

private:
	/* Projectile의 조준 지점 */
	Craft::Vector2Float destinationPos = Craft::Vector2Float::Zero;

private:
	/* 이동 컴포넌트 */
	std::shared_ptr<Craft::PathMoveComponent> pathMoveComponent;
};	



