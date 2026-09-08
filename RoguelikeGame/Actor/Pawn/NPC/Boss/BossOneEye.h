#pragma once

#include <Util/Timer.h>
#include <Math/Vector2Int.h>
#include <Math/Vector2Float.h>
#include <Actor/Pawn/NPC/NPCBase.h>
#include <Util/Timer.h>

class ResourceBossEye;
class Pawn;
class BehaviorBossComponent;

/* Boss, One Eye*/
class BossOneEye : public NPCBase
{
	TYPE_DECLARATIONS(BossOneEye, NPCBase)

private:
	/* 보스 눈 뜨고 감는 애니메이션 */
	enum class eEyeOpenAnimation : unsigned int
	{
		None = 0,
		OpenEye = 1 << 0,
		CloseEye = 1 << 1,
	};

public:
	BossOneEye(const Craft::Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex);
	virtual ~BossOneEye();

public:
	virtual void Initialize() override;

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;
	virtual void Destroy() override;

	/* 점유하는 타일 좌표들에 대한 조회 */
	virtual void ForEachOccupiedTileOffset(std::function<void(const Craft::Vector2Int&)> callbackFunc) const override;

	/* 현재 데미지를 받을 수 있는지 여부 */
	virtual bool CanTakeDamage() const override;

	/* 보스몬스터 초기 Ability 구성 */
	virtual void InitializeAbility() override;

	/* AbilitySystemComponent에서 Ability 활성화 되었을때 호출 */
	virtual void OnActivateAbility(const AbilityObject& ability, bool bActivate) override;

	/* Pawn의 Death 이벤트 */
	virtual void OnDeath() override;

public:
	/* 소환 능력 활성화 */
	void ActivateSummonAbility();

	/* ShockWave 능력 활성화 */
	void ActivateShockWaveAbility();

	/* EnergyBeam 능력 활성화 */
	void ActivateEnergyBeamAbility();

public:
	/* 보스 이름 반환 */
	inline const std::wstring& GetBossName() const { return bossName; }
	
	/* 현재 눈을 완전히 뜨고 활성화되었는지 여부 */
	inline bool IsOpenEyeActivated() const { return bOpenEyeAfterActivated; }

private:
	/* 추격 대상을 향해 홍채와 동공 옮기기 */
	void UpdateChaseTargetOffset(float deltaTime);

	/* 선형보간 - 현재 TargetOffset을 향해 currentOffset 보간 */
	void LerpCurrentOffset(float deltaTime);

	/* 선형보간 - 눈 뜨기 애니메이션 재생 */
	void LerpOpenCloseAnimation(float deltaTime);

	/* 눈 뜨는 애니메이션 시작 */
	void StartOpenEye();

	/* 눈 감는 애니메이션 시작 */
	void StartCloseEye();

	/* 눈 뜨고 감는 애니메이션 종료 */
	void OnFinishEyeOpenCloseAnimation(eEyeOpenAnimation prevAnimation);

	/* 다음 실행 예약이 걸린 Ability에 대한 타이머 동작  */
	void UpdateReserveActivateAbilityTimer(float deltaTime);

	/* 다음 실행 예약이 걸린 Ability 활성화 */
	void ExecuteReserveAbilityActivate();

private:
	/* 홍채와 동공에 더해질 오프셋의 목표 지점(선형 보간) */
	Craft::Vector2Float targetIrisPupilOffset = Craft::Vector2Float::Zero;

	/* 홍채와 동공에 더해질 현재 오프셋 (선형 보간) */
	Craft::Vector2Float currentIrisPupilOffset = Craft::Vector2Float::Zero;

	/* 최종적으로 출력할 오프셋 */
	Craft::Vector2Int drawIrisPupilOffset = Craft::Vector2Int::Zero;

	/* 홍채 및 동공 애니메이션 기간 */
	float durationIrisPupilAnimation = 1.f;

	/* 홍채 및 동공 애니메이션 경과시간 */
	float elapsedtimeIrisPupilAnimation = 0.f;

	/* 최대 눈 감김 정도 */
	int maxEyeClosure = 11;

	/* 현재 눈 감김 정도 offset */
	int currentEyeClosureOffset = 11;

	/* 눈 뜨기/감기 애니메이션 기간 */
	float durationOpenCloseAnimation = 1.f;

	/* 눈 뜨기/감기 애니메이션 경과 시간 */
	float elapsedtimeOpenCloseAnimation = 0.f;

	/* 현재 눈 뜨기/감기 애니메이션 종류 */
	eEyeOpenAnimation currentEyeOpenAnimation = eEyeOpenAnimation::None;

	/* 생성 후 실제 활성화까지의 딜레이 */
	Timer timerAwakeDelay;

	/* 처음 생성 후 눈이 열렸는지 여부 */
	bool bStartOpenEye = false;

	/* 눈이 완전히 열리고 활성화되었는지 여부 */
	bool bOpenEyeAfterActivated = false;

	/* 눈이 열려있는동안 눈동자의 타겟 추적 여부  */
	bool bEyeChaseTarget = true;

	/* 딜레이 후에 실행할 ABILITY ID */
	ABILITY_ID_TYPE reserveActivateAbilityID = INVALID_ABILITY_ID;

	/* Ability 실행 예약에 대한 타이머 */
	Timer timerNextActivateAbility;

	/* 보스 액터를 표시할 리소스(One eye) */
	std::shared_ptr<ResourceBossEye> resourceBossEye;

	/* 보스 몬스터의 이름 */
	std::wstring bossName = L"The Abyssal Gazer";

	/* 부여된 소환 Ability ID */
	ABILITY_ID_TYPE grantedSummonAbilityID = INVALID_ABILITY_ID;

	/* 부여된 쇼크웨이브 Ability ID */
	ABILITY_ID_TYPE grantedShockWaveAbilityID = INVALID_ABILITY_ID;

	/* 부여된 EnergyBeam Ability ID */
	ABILITY_ID_TYPE grantedEnergyBeamAbilityID = INVALID_ABILITY_ID;
};

