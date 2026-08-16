#pragma once

#include "GameState/PlayerState/PlayerState.h"
#include <unordered_map>
#include <windows.h>

class PlayerPawn;
class ItemBase;

/* 로그라이크 게임내에서 플레이어의 상태 값 저장 */
class PS_Roguelike : public Craft::PlayerState
{
	TYPE_DECLARATIONS(PS_Roguelike, PlayerState)

private:
	static constexpr int MAX_KILL_NUM = 100000;

public:
	PS_Roguelike();
	virtual ~PS_Roguelike();

public:
	/* 플레이어 세션 데이터 초기화 */
	virtual void InitializeSessionData() override;

	/* 레벨 초기화 과정에서 플레이어 Pawn이 생성된 후 호출 */
	virtual void OnInitializeLevel(std::weak_ptr<Craft::Level> level) override;

	/* 현재 활성화되어 있던 레벨이 지워지기전 호출됨 */
	virtual void OnDestroyedCurrentLevel() override;

public:
	/* 플레이어 폰이 스폰될때 호출 */
	void OnSpawnedPlayerPawn(std::weak_ptr<PlayerPawn> pawn);

	/* 던전 진행 층수가 변경될 때 호출 */
	void ChangeFloorLevel(int newFloorLevel);

	/* 플레이어가 몬스터를 잡을때 호출 */
	void IncrementMonsterKillNum();

private:
	/* 플레이어의 몬스터 킬수 업데이트 */
	void OnUpdateMonsterKillNum();

	/* 플레이어의 체력 업데이트 */
	void OnUpdatePlayerHealth(float currentValue, float maxValue);

	/* 플레이어가 특정 아이템을 획득할때 호출 */
	void OnPlayerItemGain(int itemID);

	/* 특정 아이템 아이콘 및 텍스트 업데이트 */
	void UpdateItemListIconText(const ItemBase& updateItem);

	/* 경과 시간 카운트 시작 */
	void BeginGameElapsedTimeCount();

private:
	/* HUD 객체 생성 */
	virtual std::unique_ptr<Craft::HUD> CreateHUD() const;

private:
	/* 현재 플레이중이 플레이어의 폰 */
	std::weak_ptr<PlayerPawn> playerPawn;

	/* 플레이어가 잡은 몬스터의 수 */
	int killMonsterNum = 0;

	/* 플레이어의 현재 체력 */
	float playerCurrentHealth = 0.f;

	/* 플레이어의 최대 체력 */
	float playerMaxHealth = 0.f;

	/* 플레이어가 소유중인 아이템 */
	std::unordered_map<int, std::unique_ptr<ItemBase>> mapItemlist;

	/* 경과 시간 누적 시작 여부 */
	bool isCountTime = false;

	/* 플레이어의 게임 시작 시간 */
	LARGE_INTEGER startPlayTime;
};

