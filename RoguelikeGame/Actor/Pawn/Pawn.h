#pragma once

#include "Actor/ActorOnTile.h"
#include "Component/AbilitySystemComponent.h"
#include <functional>

class AttributeComponent;
class AbilitySystemComponent;
class Pawn : public ActorOnTile
{
	TYPE_DECLARATIONS(Pawn, ActorOnTile)

public:
	using OnDeathEventType = std::function<void(std::shared_ptr<Pawn>)>;
	using OnChangeHealthType = std::function<void(float, float)>;

public:
	Pawn(const Craft::Vector2Int& position,
		const std::wstring& image,
		Craft::Color color,
		float initialHealth,
		eTeamID inTeamID);

	~Pawn() = default;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	/* 다른 종류의 Actor와 Block되는지 체크 */
	virtual bool IsBlockActorOnTile(std::shared_ptr<ActorOnTile> otherActor) override;

public:
	/* 체력 초기화 */
	void InitializeHealthValue(const float currentHealth, const float maxHealth);
	void SetHealthChangeEventCallback(OnChangeHealthType callback);
	void SetDeathEventCallback(OnDeathEventType deathEventCallback);
	void TakeDamage(const float inDamage);
	void AddHealthValue(const float inHealValue);

public:
	bool IsDeath() const;

public:
	inline eTeamID GetTeamID() const { return teamID; }

protected:
	inline std::shared_ptr<AbilitySystemComponent> GetAbilitySystemComponent() const { return abilitySystemComponent; }

protected:
	/* Pawn의 초기 Ability 구성 */
	virtual void InitializeAbility();

	/* Pawn의 Death 이벤트 */
	virtual void OnDeath();

private:
	/* Health가 0이하일때 호출되는 이벤트 */
	void OnOutOfHealth();

	/* Health 값 업데이트 이벤트 */
	void OnChangeHealthValue(float currentValue, float maxValue);

private:
	/* 소속된 팀 ID */
	eTeamID teamID = eTeamID::None;

	/* 체력 수치 변경시 호출되는 이벤트 */
	OnChangeHealthType onChangeHealthEvent;

	/* 체력이 다 소모되어 사망할때 발생하는 이벤트 */
	OnDeathEventType onDeathEvent;

private:
	/* 속성값(Health) 관리 컴포넌트 */
	std::shared_ptr<AttributeComponent> attributeComponent;

	/* Pawn의 Ability 관리 컴포넌트 */
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent;
};

