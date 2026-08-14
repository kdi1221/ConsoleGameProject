#pragma once

#include "Actor/ActorOnTile.h"
#include "Component/PathMoveComponent.h"


/* 게임 내 폰들이 발사하는 Projectile */
class Projectile : public ActorOnTile
{
	TYPE_DECLARATIONS(Projectile, ActorOnTile)

public:
	Projectile(const Craft::Vector2Int& inPosition,
				const std::wstring& inImage, 
				Craft::Color inColor, 
				const Craft::Vector2Int& inDestination,
				float moveDelay,
				eTeamID teamID,
				float damage);
	virtual ~Projectile() = default;

private:
	virtual void BeginPlay() override;

	/* 특정 타일 위에 있는 동안 호출되는 함수 */
	virtual void OnTileOverlap(const eTileCategory tileCategory) override;

	/* 다른 종류의 Actor와 Block되는지 체크 */
	virtual bool IsBlockActorOnTile(std::shared_ptr<ActorOnTile> otherActor) override;

	//충돌 시 호출될 충돌 이벤트 함수.
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

private:
	void OnCallbackMoveFinish();

private:
	/* Projectile의 조준 지점 */
	Craft::Vector2Int destinationPos = Craft::Vector2Int::Zero;

	/* 탄환을 발사한 행위자의 teamID */
	eTeamID instigatorTeamID = eTeamID::None;

	/* 탄환의 데미지 */
	float damageValue = 0;

private:
	/* 이동 컴포넌트 */
	std::shared_ptr<Craft::PathMoveComponent> pathMoveComponent;
};	



