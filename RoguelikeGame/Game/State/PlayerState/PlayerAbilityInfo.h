#pragma once

/* 플레이어가 배운 스킬 정보 */
class PlayerAbilityInfo
{
public:
	PlayerAbilityInfo(int id, int level, int keyCode);
	~PlayerAbilityInfo() = default;

public:
	/* 스킬 레벨 설정 */
	void SetAbilityLevel(int newLevel);

	/* 스킬 사용 키 입력 바인딩 수정 */
	void SetBindingKeyCode(int newKeyCode);

public:
	inline int GetAbilityID() const { return abilityID; }
	inline int GetAbilityLevel() const { return currentLevel; }
	inline int GetBindingKeyCode() const { return bindingKeyCode; }

private:
	/* Ability ID */
	int abilityID = 0;

	/* Ability 현재 레벨 */
	int currentLevel = 0;

	/* 바인딩 된 입력 키 */
	int bindingKeyCode = 0;

};

