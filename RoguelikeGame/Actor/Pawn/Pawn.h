#pragma once

#include "Actor/ActorOnTile.h"
#include "Component/AbilitySystemComponent.h"
#include "Types/Defines.h"
#include <Math/Vector2Float.h>
#include <functional>

class AttributeComponent;
class AbilitySystemComponent;
class Pawn : public ActorOnTile
{
	TYPE_DECLARATIONS(Pawn, ActorOnTile)

public:
	using OnDeathEventType = std::function<void(std::shared_ptr<Pawn>)>;
	using OnChangeHealthType = std::function<void(float, float)>;

private:
	static PawnUniqueIDType GenerateUnqueID();

public:
	Pawn(const Craft::Vector2Int& position,
		const std::wstring& image,
		Craft::Color color,
		eTeamID inTeamID);

	Pawn(const Craft::Vector2Int& position, 
		eTeamID inTeamID);

	~Pawn() = default;

public:
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Destroy() override;

	/* 다른 종류의 Actor와 Block되는지 체크 */
	virtual bool IsBlockActor(std::shared_ptr<Actor> otherActor) override;

public:
	/* 체력 초기화 */
	void InitializeHealthValue(const float currentHealth, const float maxHealth);

	/* 체력 변경 이벤트 콜백 바인딩 */
	void SetHealthChangeEventCallback(OnChangeHealthType callback);

	/* 사망 이벤트 콜백 바인딩 */
	void SetDeathEventCallback(OnDeathEventType deathEventCallback);

	/* 데미지 처리 함수 */
	void TakeDamage(const float inDamage);

	/* 체력 회복 함수 */
	void AddHealthValue(const float inHealValue);

	/* 보유한 Ability Cooldown 이벤트 콜백 바인딩 */
	void SetAbilityCooldownChangeCallback();

public:
	/* 폰의 사망 여부 */
	bool IsDeath() const;

public:
	inline PawnUniqueIDType GetUniqueID() const { return uniqueID; }
	inline eTeamID GetTeamID() const { return teamID; }
	inline const Craft::Vector2Float& GetAimingDirection() const { return aimingDirection; }

protected:
	inline std::shared_ptr<AbilitySystemComponent> GetAbilitySystemComponent() const { return abilitySystemComponent; }

protected:
	virtual std::shared_ptr<AttributeComponent> CreateAttributeComponent();	

protected:
	/* Pawn의 초기 Ability 구성 */
	virtual void InitializeAbility();

	/* Pawn의 Death 이벤트 */
	virtual void OnDeath();

	/* 조준 방향 지정 */
	virtual void SetAimingDirection(const Craft::Vector2Float& direction);

private:
	/* Pawn 사망 설정 */
	void SetDeath();

	/* Health 값 업데이트 이벤트 */
	void OnChangeHealthValue(float currentValue, float maxValue);

private:
	/* Pawn의 고유 유니크 ID */
	PawnUniqueIDType uniqueID = 0;

	/* 소속된 팀 ID */
	eTeamID teamID = eTeamID::None;

	/* 사망 여부 */
	bool bDeath = false;

	/* 조준 방향 */
	Craft::Vector2Float aimingDirection = Craft::Vector2Float::Zero;

private:
	/* 체력 수치 변경시 호출되는 이벤트 */
	OnChangeHealthType onChangeHealthEvent;

	/* 체력이 다 소모되어 사망할때 발생하는 이벤트 */
	OnDeathEventType onDeathEvent;

protected:
	/* 속성값(Health) 관리 컴포넌트 */
	std::shared_ptr<AttributeComponent> attributeComponent;

	/* Pawn의 Ability 관리 컴포넌트 */
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent;
};

