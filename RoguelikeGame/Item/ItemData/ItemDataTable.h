#pragma once

#include <string>
#include <vector>
#include <Math/Color.h>

struct ItemData
{
	int itemID = 0;

	std::wstring itemName = L"";

	std::wstring itemIconImage = L"";

	std::wstring fieldItemImage = L"";

	Craft::Color color = Craft::Color::White;

	int maxNum = 1;

	int abilityID = 0;
};

//아이템 임시 데이터 모음..
class ItemDataTable
{
public:
	static const ItemData& GetItemData(int itemId)
	{
		//임시 하드코딩 테이블
		static std::vector<ItemData> items =
		{
			{1, L"Spirit Ball", L"○", L"A", Craft::Color::LightGreen, 3, 1},
			{2, L"Hand Blast", L"♧", L"A", Craft::Color::LightRed, 3, 2}
		};

		//더미
		static const ItemData dummy{ 0, L"Invalid", L"X", L"X", Craft::Color::White, 1, 0 };

		for (const auto& item : items)
		{
			if (item.abilityID == itemId)
			{
				return item;
			}
		}

		return dummy;
	}
};
