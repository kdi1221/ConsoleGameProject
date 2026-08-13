#pragma once

#include "Actor/ActorOnTile.h"
#include <functional>

class AttributeComponent;
class Pawn : public ActorOnTile
{
	TYPE_DECLARATIONS(Pawn, ActorOnTile)

public:
	using OnDeathEventType = std::function<void(std::shared_ptr<Pawn>)>;

public:
	Pawn(const Craft::Vector2Int& position,
		const std::wstring& image,
		Craft::Color color,
		int CollisionWidth,
		int initialHealth,
		eTeamID inTeamID);

	~Pawn() = default;

public:
	/* 다른 종류의 Actor와 Block되는지 체크 */
	virtual bool IsBlockActorOnTile(std::shared_ptr<ActorOnTile> otherActor) override;

public:
	void SetDeathEventCallback(OnDeathEventType deathEventCallback);
	void TakeDamage(const int inDamage);

public:
	bool IsDeath() const;

public:
	inline eTeamID GetTeamID() const { return teamID; }

protected:
	virtual void OnDeath();

private:
	void OnOutOfHealth();

private:
	/* 소속된 팀 ID */
	eTeamID teamID = eTeamID::None;

	/* 체력이 다 소모되어 사망할때 발생하는 이벤트 */
	OnDeathEventType onDeathEvent;

private:
	/* 속성값(Health) 관리 컴포넌트 */
	std::shared_ptr<AttributeComponent> attributeComponent;
};

