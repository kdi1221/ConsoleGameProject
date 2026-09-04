#pragma once

#include <Util/Timer.h>
#include <Math/Vector2Int.h>
#include <Math/Vector2Float.h>
#include <Actor/Pawn/NPC/NPCBase.h>

class ResourceBossEye;
class Pawn;

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
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	/* 점유하는 타일 좌표들에 대한 조회 */
	virtual void ForEachOccupiedTileOffset(std::function<void(const Craft::Vector2Int&)> callbackFunc) const override;

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

	/* 실제 활성화되었는지 여부 */
	bool bAwake = false;

	/* 타겟 추적 여부  */
	bool bChaseTarget = false;


	/* 보스 액터를 표시할 리소스(One eye) */
	std::shared_ptr<ResourceBossEye> resourceBossEye;
};

