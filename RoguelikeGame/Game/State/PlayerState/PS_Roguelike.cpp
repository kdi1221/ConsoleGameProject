#include "PS_Roguelike.h"
#include "UI/HUD/HUDPlayer.h"
#include "Actor/Pawn/Player/PlayerPawn.h"
#include "Item/ItemBase.h"
#include "Item/ItemData/ItemDataTable.h"
#include <cassert>

using namespace Craft;

PS_Roguelike::PS_Roguelike()
{

}

PS_Roguelike::~PS_Roguelike()
{

}

void PS_Roguelike::InitializeSessionData()
{
	killMonsterNum = 0;
	playerMaxHealth = 100.f;
	playerCurrentHealth = playerMaxHealth;

	/* 초기 아이템 */
	OnPlayerItemGain(1);
	OnPlayerItemGain(2);
}

void PS_Roguelike::OnInitializeLevel(std::weak_ptr<Level> level)
{
	super::OnInitializeLevel(level);
}

void PS_Roguelike::OnDestroyedCurrentLevel()
{
	super::OnDestroyedCurrentLevel();

	playerPawn.reset();
}

void PS_Roguelike::OnSpawnedPlayerPawn(std::weak_ptr<PlayerPawn> pawn)
{
	playerPawn = pawn;

	/* 새로 스폰된 플레이어 폰의 체력및 체력 변경 콜백을 설정한다. */
	std::shared_ptr<PlayerPawn> currentPlayerPawn = playerPawn.lock();
	assert(currentPlayerPawn && "Invalid PlayerPawn");
	currentPlayerPawn->InitializeHealthValue(playerCurrentHealth, playerMaxHealth);
	currentPlayerPawn->SetHealthChangeEventCallback(std::bind(&PS_Roguelike::OnUpdatePlayerHealth, this, std::placeholders::_1, std::placeholders::_2));
	currentPlayerPawn->SetOnItemGainEvent(std::bind(&PS_Roguelike::OnPlayerItemGain, this, std::placeholders::_1));

	InitializeHUD();

	/* 경과 시간 설정 */
	BeginGameElapsedTimeCount();

	/* 세션 데이터들을 새로 만든 위젯에 다시 갱신해준다. */
	OnUpdateMonsterKillNum();
	OnUpdatePlayerHealth(playerCurrentHealth, playerMaxHealth);

	/* 세션 데이터 - 스킬 아이템 리스트를 돌면서 플레이어 스킬 및 HUD를 갱신한다. */
	for (const auto& iterItem : mapItemlist)
	{
		const int itemID = iterItem.second->GetItemID();
		const ItemData& currentItemData = ItemDataTable::GetItemData(itemID);

		currentPlayerPawn->GrantAbility(currentItemData.abilityID, iterItem.second->GetItemNum());

		UpdateItemListIconText(*iterItem.second);
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

void PS_Roguelike::OnPlayerItemGain(int itemID)
{
	auto finditerItem = mapItemlist.find(itemID);
	if (finditerItem != mapItemlist.end())
	{
		ItemBase* findItem = finditerItem->second.get();
		assert(findItem && "Invalid findItem");
		findItem->SetItemNum(min(ItemDataTable::GetItemData(itemID).maxNum, findItem->GetItemNum() + 1));
		UpdateItemListIconText(*findItem);
	}
	else
	{
		auto insertResult = mapItemlist.insert(std::pair<int, std::unique_ptr<ItemBase>>(itemID, std::make_unique<ItemBase>(itemID, 1)));
		ItemBase* insertItem = insertResult.first->second.get();
		assert(insertItem && "Invalid insertItem");

		UpdateItemListIconText(*insertItem);
	}
}

void PS_Roguelike::UpdateItemListIconText(const ItemBase& updateItem)
{
	HUDPlayer* hudPlayer = GetHUD<HUDPlayer>();
	if (hudPlayer)
	{
		hudPlayer->UpdateItemListIcon(updateItem);
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

/* HUD 객체 생성 */
std::unique_ptr<HUD> PS_Roguelike::CreateHUD() const
{
	return std::make_unique<HUDPlayer>();
}
