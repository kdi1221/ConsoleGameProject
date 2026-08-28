#pragma once

#include "Actor/Pawn/Pawn.h"
#include "Component/InputComponent.h"
#include "Component/MovementComponent.h"
#include <unordered_set>

namespace Craft
{
	class CameraComponent;
}

class PlayerAbilityInfo;
class FieldSkillItem;
class PlayerAttributeComponent;

class PlayerPawn : public Pawn
{
	TYPE_DECLARATIONS(PlayerPawn, Pawn)

public:
	using OnItemGainEventType = std::function<void(int)>;
	using OnChangeManaType = std::function<void(float, float)>;

public:
	PlayerPawn(const Craft::Vector2Int& position);
	~PlayerPawn() = default;

public:
	virtual void Initialize() override;

private:
	virtual void PreTick(float deltaTime) override;
	virtual void Tick(float deltaTime) override;
	virtual void OnUpdatedPosition(const Craft::Vector2Int& prevLocalPosition,
									const Craft::Vector2Int& prevWorldPosition,
									const Craft::Vector2Int& localPosition,
									const Craft::Vector2Int& worldPosition) override;

private:
	virtual std::shared_ptr<AttributeComponent> CreateAttributeComponent() override;


public:
	/* 마나 초기화 */
	void InitializeManaValue(const float currentMana, const float maxMana);

	/* 마나 변경 이벤트 콜백 바인딩 */
	void SetManaChangeEventCallback(OnChangeManaType callback);

	/* 플레이어가 특정 스킬 아이템 획득 */
	void GainSkillItem(std::shared_ptr<FieldSkillItem> gainItem);

	/* 플레이어가 특정 스킬 아이템 획득시 호출되는 이벤트 콜백 설정 */
	void SetOnItemGainEvent(OnItemGainEventType callback);

	/* 플레이어 폰, 특정 스킬 부여 */
	void GrantAbility(const PlayerAbilityInfo& abilityInfo);


	/* 플레이어에 특정 스킬 부여(폐기 예정) */
	//void GrantAbility(int abilityID, int abilityLevel);

public:
	/* 마나 소모 */
	void ConsumeMana(float consumeValue);

	/* 마나 회복 */
	void RestoreMana(float amount);

public:
	/* 현재 마나 수치 반환 */
	float GetCurrentMana() const;

private:
	/* 마나 속성 값 업데이트 콜백 */
	void OnChangeManaValue(float currentValue, float maxValue);

private:
	/* 이동 입력 키 입력 콜백 */
	void OnMoveKeyInput(int keyCode, Craft::eInputTrigger inputTrigger);

	/* Ability 사용 키 입력 콜백 */
	void OnAbilityActiveKeyDown(int keyCode, Craft::eInputTrigger inputTrigger);

	//void OnProjectileFireKeyInput(int keyCode, Craft::eInputTrigger inputTrigger);

	/* 치트키 입력 콜백 */
	void OnCheatInputTrigger(int keyCode, Craft::eInputTrigger inputTrigger);

private:
	/* 시간에 따라 마나 회복하는 함수 */
	void ProcessManaRegeneration(float deltaTime);

	/* 위치 이동 시 호출되는 카메라 뷰 위치 변경 함수 */
	void UpdateViewCameraPosition(const Craft::Vector2Int& viewPosition);

	/* 발사 입력 처리 */
	//void ProcessFireInput();

	/* 이동 입력 처리 */
	void ProcessMoveInput();

	/* 조준 방향 결정(마우스 커서 위치를 향해) */
	void UpdateAimingDirectionToCursorPos();

	/* Projectile Ability의 트리거 ON/OFF*/
	//void SetProjectileAbilityTrigger(bool bTrigger);

private:
	/* 초당 마나 회복 속도 */
	float manaRegenerationSpeed = 3.f;

	/* 현재 프레임에서의 이동 입력 방향 */
	Craft::eDirection moveInputDirection = Craft::eDirection::None;

	/* 바인딩된 입력 별 Ability ID */
	std::unordered_map<int, ABILITY_ID_TYPE> mapInputGrantAbilities;

	/* 부여된 Projectile Ability ID들(폐기 예정) */
	//std::unordered_set<ABILITY_ID_TYPE> grantProjectileAbilities;

private:
	/* 아이템 획득 시 호출되는 이벤트 */
	OnItemGainEventType onItemGainEvent;

	/* 마나 수치 변경시 호출되는 이벤트 */
	OnChangeManaType onChangeManaEvent;

private:
	/* 플레이어 폰을 바라보는 카메라 컴포넌트 */
	std::shared_ptr<Craft::CameraComponent> cameraComponent;

	/* 이동 컴포넌트 */
	std::shared_ptr<Craft::MovementComponent> movementComponent;

	/* 입력 컴포넌트 */
	std::shared_ptr<Craft::InputComponent> inputComponent;

	/* 플레이어 속성 컴포넌트 */
	std::shared_ptr<PlayerAttributeComponent> playerAttributeComponent;
};

