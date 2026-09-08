#include "PS_Roguelike.h"
#include "UI/HUD/HUDPlayer.h"
#include "Actor/Pawn/Player/PlayerPawn.h"
#include "Actor/Pawn/NPC/Boss/BossOneEye.h"
#include "PlayerAbilityInfo.h"
#include "Component/AbilitySystemComponent.h"
#include "Ability/AbilityDataTable.h"

#include "Item/ItemBase.h"
#include "Item/ItemData/ItemDataTable.h"
#include <Level/Level.h>
#include <cassert>

using namespace Craft;

PS_Roguelike::PS_Roguelike()
	:startPlayTime()
	,lastPauseTime()
{
	/* 플레이어 Ability 입력키 예약 키코드 */
	reserveAbilityKeycode.push('1');
	reserveAbilityKeycode.push('2');
	reserveAbilityKeycode.push('3');
	reserveAbilityKeycode.push('4');
}

PS_Roguelike::~PS_Roguelike()
{

}

void PS_Roguelike::InitializeSessionData()
{
	killMonsterNum = 0;
	playerMaxHealth = 100.f;
	playerCurrentHealth = playerMaxHealth;
	playerMaxMana = 100.f;
	playerCurrentMana = playerMaxMana;

	/* 초기 Ability */
	GrantAbilityToPlayer(1, 3, VK_RBUTTON);
	//GrantAbilityToPlayer(2, 1, '1');
	//GrantAbilityToPlayer(3, 1, '2');
	//GrantAbilityToPlayer(4, 1, '3');
	//GrantAbilityToPlayer(5, 1, '4');
}

void PS_Roguelike::OnInitializeLevel(std::weak_ptr<Level> level)
{
	super::OnInitializeLevel(level);

	if (std::shared_ptr<Level> currentLevel = level.lock())
	{
		currentLevel->SetOnGamePause(std::bind(&PS_Roguelike::OnSetGamePaused, this, std::placeholders::_1));
	}
}

void PS_Roguelike::OnDestroyedCurrentLevel()
{
	super::OnDestroyedCurrentLevel();

	/* PlayerPawn의 Ability Cooltime 저장 */
	std::shared_ptr<PlayerPawn> currentPlayerPawn = playerPawn.lock();
	if (currentPlayerPawn)
	{
		std::shared_ptr<AbilitySystemComponent> playerPawnASC = currentPlayerPawn->GetComponent<AbilitySystemComponent>();
		assert(playerPawnASC && "Invalid PlayerPawnASC");

		for (auto& grantedAbility : mapGrantedAbilities)
		{
			AbilityObject* findAbilityObject = playerPawnASC->GetAbility<AbilityObject>(grantedAbility.first);
			assert(findAbilityObject && "Invalid abilityObject");

			std::unique_ptr<PlayerAbilityInfo>& playerAbilityInfo = grantedAbility.second;
			assert(playerAbilityInfo && "Invalid playerAbilityInfo");

			if (findAbilityObject->IsCooldown())
			{
				playerAbilityInfo->SaveCooldownElapsedTime(findAbilityObject->GetCooldownElapsedTime());
			}
			else
			{
				playerAbilityInfo->ResetCooldownElapsedTime();
			}
		}

		playerPawn.reset();
	}	
}

void PS_Roguelike::OnSpawnedPlayerPawn(std::weak_ptr<PlayerPawn> pawn)
{
	playerPawn = pawn;

	/* 새로 스폰된 플레이어 폰의 체력및 체력 변경 콜백을 설정한다. */
	std::shared_ptr<PlayerPawn> currentPlayerPawn = playerPawn.lock();
	assert(currentPlayerPawn && "Invalid PlayerPawn");

	/* 플레이어 체력, 마나 설정(세션 데이터 유지) */
	currentPlayerPawn->InitializeHealthValue(playerCurrentHealth, playerMaxHealth);
	currentPlayerPawn->SetHealthChangeEventCallback(std::bind(&PS_Roguelike::OnUpdatePlayerHealth, this, std::placeholders::_1, std::placeholders::_2));
	currentPlayerPawn->InitializeManaValue(playerCurrentMana, playerMaxMana);
	currentPlayerPawn->SetManaChangeEventCallback(std::bind(&PS_Roguelike::OnUpdatePlayerMana, this, std::placeholders::_1, std::placeholders::_2));

	/* 플레이어가 아이템 주울때 호출되는 이벤트 */
	currentPlayerPawn->SetOnAbilityItemGainEvent(std::bind(&PS_Roguelike::OnPlayerAbilityItemGain, this, std::placeholders::_1));

	std::shared_ptr<AbilitySystemComponent> playerPawnASC = currentPlayerPawn->GetComponent<AbilitySystemComponent>();
	assert(playerPawnASC && "Invalid PlayerPawnASC");

	playerPawnASC->SetAbilityCooldownChangeCallback(std::bind(&PS_Roguelike::OnPlayerAbilityCooldownChange, this, std::placeholders::_1, std::placeholders::_2));

	InitializeHUD();

	/* 경과 시간 설정 */
	BeginGameElapsedTimeCount();

	/* 세션 데이터들을 새로 만든 위젯에 다시 갱신해준다. */
	OnUpdateMonsterKillNum();
	OnUpdatePlayerHealth(playerCurrentHealth, playerMaxHealth);
	OnUpdatePlayerMana(playerCurrentMana, playerMaxMana);

	/* 플레이어에게 부여된 스킬 업데이트 */
	for (const auto& iterAbility : mapGrantedAbilities)
	{
		const PlayerAbilityInfo* abilityInfo = iterAbility.second.get();
		if (!abilityInfo)
		{
			continue;
		}

		/* Ability Icon 먼저 업데이트 */
		UpdateAbilityIcon(*abilityInfo);

		/* Player Pawn에게 Ability 부여(이 과정에서 Cooltime등 Ability 정보들도 HUD에 업데이트) */
		currentPlayerPawn->GrantAbility(*abilityInfo);
	}
}

void PS_Roguelike::ChangeFloorLevel(int newFloorLevel)
{
	HUDPlayer* hudPlayer = GetHUD<HUDPlayer>();
	if (hudPlayer)
	{
		hudPlayer->ChangeFloorLevel(newFloorLevel);
	}
}

void PS_Roguelike::IncrementMonsterKillNum()
{
	int beforeKillNum = killMonsterNum;
	killMonsterNum = min(killMonsterNum + 1, MAX_KILL_NUM);

	if (beforeKillNum != killMonsterNum)
	{
		OnUpdateMonsterKillNum();
	}	
}

void PS_Roguelike::OnSpawnedBossMonster(std::shared_ptr<BossOneEye> spawnedBoss)
{
	if (!spawnedBoss)
	{
		return;
	}

	if (HUDPlayer* hudPlayer = GetHUD<HUDPlayer>())
	{
		hudPlayer->ShowBossStatusBar(spawnedBoss->GetBossName());
		hudPlayer->ChangeBossHealthValue(spawnedBoss->GetCurrentHealth(), spawnedBoss->GetMaxHealth());

		spawnedBoss->SetHealthChangeEventCallback(std::bind(&PS_Roguelike::OnUpdateBossHealth, this, std::placeholders::_1, std::placeholders::_2));
	}
}

void PS_Roguelike::OnDeathBossMonster()
{
	if (HUDPlayer* hudPlayer = GetHUD<HUDPlayer>())
	{
		hudPlayer->HideBossStatusBar();
	}
}

const PlayerAbilityInfo& PS_Roguelike::GrantAbilityToPlayer(int abilityID, int level, int keyCode)
{
	auto insertResult = mapGrantedAbilities.insert({ abilityID, std::make_unique<PlayerAbilityInfo>(abilityID, level, keyCode) });
	assert(insertResult.second && "Fail Grant Ability..");

	return *insertResult.first->second;
}

const PlayerAbilityInfo& PS_Roguelike::AddAbilityLevel(int abilityID, int addLevel)
{
	auto findGrantedAbility = mapGrantedAbilities.find(abilityID);

	assert(findGrantedAbility != mapGrantedAbilities.end() && findGrantedAbility->second && "Invalid Granted Ability");

	PlayerAbilityInfo& abilityInfo = *findGrantedAbility->second;
	const int currentLevel = abilityInfo.GetAbilityLevel();
	const int newLevel = min(AbilityDataTable::GetAbilityData(abilityID).maxLevel, currentLevel + 1);

	if (currentLevel != newLevel)
	{
		abilityInfo.SetAbilityLevel(newLevel);
	}

	return abilityInfo;
}

void PS_Roguelike::OnUpdateMonsterKillNum()
{
	HUDPlayer* hudPlayer = GetHUD<HUDPlayer>();
	if (hudPlayer)
	{
		hudPlayer->ChangeMonsterKillNum(killMonsterNum);
	}
}

void PS_Roguelike::OnUpdatePlayerHealth(float currentValue, float maxValue)
{
	playerCurrentHealth = currentValue;
	playerMaxHealth = maxValue;

	HUDPlayer* hudPlayer = GetHUD<HUDPlayer>();
	if (hudPlayer)
	{
		hudPlayer->ChangePlayerHealthValue(playerCurrentHealth, playerMaxHealth);
	}
}

void PS_Roguelike::OnUpdatePlayerMana(float currentValue, float maxValue)
{
	playerCurrentMana = currentValue;
	playerMaxMana = maxValue;

	HUDPlayer* hudPlayer = GetHUD<HUDPlayer>();
	if (hudPlayer)
	{
		hudPlayer->ChangePlayerManaValue(playerCurrentMana, playerMaxMana);
	}
}

void PS_Roguelike::OnPlayerAbilityItemGain(int abilityID)
{
	/* 새로 스폰된 플레이어 폰의 체력및 체력 변경 콜백을 설정한다. */
	std::shared_ptr<PlayerPawn> currentPlayerPawn = playerPawn.lock();
	assert(currentPlayerPawn && "Invalid PlayerPawn");

	auto iterGrantedAbility = mapGrantedAbilities.find(abilityID);	
	if (iterGrantedAbility == mapGrantedAbilities.end())
	{
		/* 부여되지 않은 스킬이면 부여 */
		const PlayerAbilityInfo& grantedAbilityInfo = GrantAbilityToPlayer(abilityID, 1, reserveAbilityKeycode.front());

		/* 예약 키코드 제거 */
		reserveAbilityKeycode.pop();

		/* Ability Icon 먼저 업데이트 */
		UpdateAbilityIcon(grantedAbilityInfo);

		/* Player Pawn에게 Ability 부여(이 과정에서 Cooltime등 Ability 정보들도 HUD에 업데이트) */
		currentPlayerPawn->GrantAbility(grantedAbilityInfo);
	}
	else
	{
		/* 부여된 스킬이면 스킬 레벨업 */
		const PlayerAbilityInfo& grantedAbilityInfo = AddAbilityLevel(abilityID, 1);

		/* Ability Icon 먼저 업데이트 */
		UpdateAbilityIcon(grantedAbilityInfo);

		/* 플레이어한테 알려야 함 */
		currentPlayerPawn->SetGrantedAbilityLevel(grantedAbilityInfo);
	}
}

void PS_Roguelike::OnPlayerAbilityCooldownChange(const AbilityObject& ability, bool bCooldown)
{
	auto iterGrantedAbility = mapGrantedAbilities.find(ability.GetAbilityID());
	if (iterGrantedAbility == mapGrantedAbilities.end())
	{
		return;
	}

	HUDPlayer* hudPlayer = GetHUD<HUDPlayer>();
	if (hudPlayer)
	{
		hudPlayer->AbilityCooldownChange(ability, *iterGrantedAbility->second, bCooldown);
	}
}

void PS_Roguelike::UpdateAbilityIcon(const PlayerAbilityInfo& abilityInfo)
{
	HUDPlayer* hudPlayer = GetHUD<HUDPlayer>();
	if (hudPlayer)
	{
		hudPlayer->UpdateAbilityIcon(abilityInfo);
	}
}

void PS_Roguelike::BeginGameElapsedTimeCount()
{
	//(최초일때는 StartPlayTime 지정)
	if (!isCountTime)
	{
		QueryPerformanceCounter(&startPlayTime);    // 시작 시점의 카운터 값 저장

		isCountTime = true;
	}
	
	HUDPlayer* hudPlayer = GetHUD<HUDPlayer>();
	if (hudPlayer)
	{
		hudPlayer->SetStartPlayTime(startPlayTime);
	}
}

void PS_Roguelike::OnSetGamePaused(bool bPause)
{
	HUDPlayer* hudPlayer = GetHUD<HUDPlayer>();
	if (hudPlayer)
	{
		hudPlayer->SetGamePause(bPause);
	}

	if (bPause)
	{
		//일시정지 시간 저장
		QueryPerformanceCounter(&lastPauseTime);
	}
	else
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);

		startPlayTime.QuadPart += currentTime.QuadPart - lastPauseTime.QuadPart;

		if (hudPlayer)
		{
			hudPlayer->SetStartPlayTime(startPlayTime);
		}
	}
}

void PS_Roguelike::OnUpdateBossHealth(float currentValue, float maxValue)
{
	if (HUDPlayer* hudPlayer = GetHUD<HUDPlayer>())
	{
		hudPlayer->ChangeBossHealthValue(currentValue, maxValue);
	}
}

/* HUD 객체 생성 */
std::unique_ptr<HUD> PS_Roguelike::CreateHUD() const
{
	return std::make_unique<HUDPlayer>();
}
