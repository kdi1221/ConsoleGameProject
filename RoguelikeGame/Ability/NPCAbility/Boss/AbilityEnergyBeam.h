#pragma once

#include "Ability/AbilityObject.h"
#include <Math/Vector2Int.h>
#include <unordered_map>

class AbilityEnergyBeamObject;

/* 보스몬스터 : 에너지 빔 */
class AbilityEnergyBeam : public AbilityObject
{
	TYPE_DECLARATIONS(AbilityEnergyBeam, AbilityObject)

private:
	/* 에너지 빔 액터 생성 정보 */
	struct FEnergyBeamActorSpawnInfo
	{
		/* 생성 위치 */
		Craft::Vector2Int spawnPosition;

		/* 빔이 도달해야하는 끝 위치 */
		Craft::Vector2Int endPosition;

		/* 빔의 수직방향 길이 */
		int beamVerticalLength = 0;

		FEnergyBeamActorSpawnInfo()
		{

		}

		FEnergyBeamActorSpawnInfo(const Craft::Vector2Int& spawnPos, 
								const Craft::Vector2Int& endPos,
								int length)
			:spawnPosition(spawnPos)
			,endPosition(endPos)
			,beamVerticalLength(length)
		{

		}
	};

public:
	AbilityEnergyBeam(ABILITY_ID_TYPE id, int level);
	virtual ~AbilityEnergyBeam() = default;

public:
	/* Ability 활성화 */
	virtual void ActivateAbility() override;

	/* Ability 취소 */
	virtual void CancelAbility() override;

private:
	void OnEnergyBeamObjectDestroyed(const AbilityEnergyBeamObject& destroyBeamObject);

private:
	/* 에너지 빔 가로 길이 */
	int maxEnergyBeamWidth = 20;

	/* 에너지 빔 세로 길이 */
	int maxEnergyBeamHeight = 10;

	/* 생성된 에너지 빔 액터 */
	std::unordered_map<int, std::shared_ptr<AbilityEnergyBeamObject>> spawnedBeamActors;
};

