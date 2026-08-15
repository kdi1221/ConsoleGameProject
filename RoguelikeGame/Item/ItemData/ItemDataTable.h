#pragma once

#include <string>
#include <vector>

struct ItemData
{
	int itemID = 0;

	std::wstring itemName = L"";

	std::wstring itemIconImage = L"";

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
			{1, L"Sprit Ball", L"🔮", 1}
		};

		//더미
		static const ItemData dummy{ 0, L"Invalid", L"X", 0 };

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
