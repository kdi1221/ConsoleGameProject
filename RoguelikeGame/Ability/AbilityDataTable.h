#pragma once

#include "Types/Defines.h"
#include <unordered_map>
#include <Math/Color.h>
#include <string>

struct FAbilityData
{
	ABILITY_ID_TYPE abilityID = 0;

	std::wstring name = L"";

	std::wstring iconImage = L"";

	Craft::Color iconColor = Craft::Color::White;

	int maxLevel = 0;
};


//Ability 임시 데이터 모음..
class AbilityDataTable
{
public:
	static const FAbilityData& GetAbilityData(int abilityID)
	{
		//임시 하드코딩 테이블
		static std::unordered_map<int, FAbilityData> abilities =
		{
			{1, {1, L"Spirit Ball", L"B", Craft::Color::LightGreen, 3}},
			{2, {2, L"Frozen Orb", L"F", Craft::Color::LightBlue, 3}},
			{3, {3, L"Nova", L"N", Craft::Color::BrightYellow, 3}},
			{4, {4, L"Teleport", L"T", Craft::Color::Purple, 3}},
		};

		//더미
		static const FAbilityData dummyAbility{ 0, L"Invalid", L"X", Craft::Color::White, 1 };

		const auto findAbility = abilities.find(abilityID);

		return findAbility != abilities.end() ? findAbility->second : dummyAbility;
	}
};