#include "HUDPlayer.h"
#include "UI/TextBlockFPS.h"
#include "UI/BackgroundWidget.h"
#include "Item/ItemBase.h"
#include "Item/ItemData/ItemDataTable.h"
#include <UI/Widget/Image/ImageWidget.h>
#include <UI/Widget/ProgressBar/ProgressBar.h>
#include <Engine/Engine.h>
#include <Level/Level.h>
#include <Engine/Config/ConfigBase.h>
#include <Math/Vector2Int.h>

using namespace Craft;

HUDPlayer::HUDPlayer()
{

}

void HUDPlayer::InitializeHUD(std::weak_ptr<Craft::Level> activeLevel)
{
	super::InitializeHUD(activeLevel);

	std::shared_ptr<Level> currentLevel = GetLevel();
	assert(currentLevel && "Invalid currentLevel");

	const ConfigBase& configBase = Engine::Get().GetConfig<Craft::ConfigBase>();
	const int ViewWidth = configBase.GetViewWidth();

	/* FPS 표시 위젯 생성 */
	const int widthWidgetFPS = 12;
	const Vector2Int positionWidgetFPS(configBase.GetDisplayWidth() - widthWidgetFPS - 1, 0);
	textBlockFPS = currentLevel->CreateWidget<TextBlockFPS>(positionWidgetFPS, widthWidgetFPS);

	/* Background 위젯 */
	const Vector2Int positionWidgetBackground(ViewWidth, 3);
	const int backgroundWidth = configBase.GetDisplayWidth() - ViewWidth;
	const int backgroundHeight = configBase.GetViewHeight() - positionWidgetBackground.y - 1;
	hudBackground = currentLevel->CreateWidget<BackgroundWidget>(positionWidgetBackground, backgroundWidth, backgroundHeight, Color::DarkYellow);

	/* 현재 층수 아이콘 표시 */
	const Vector2Int positionFloorIcon(ViewWidth + 2, 5);
	std::shared_ptr<ImageWidget> floorIcon = currentLevel->CreateWidget<ImageWidget>(positionFloorIcon, L"🏰", Color::LightGreen);
	assert(floorIcon && "floorIcon Invalid..");
	imageFloorIcon = floorIcon;

	/* 현재 층수 위젯 생성 */
	const int widthWidgetFloor = 16;
	const Vector2Int positionWidgetFloor(ViewWidth + 5, 5);
	std::shared_ptr<TextBlock> createdFloorText = currentLevel->CreateWidget<TextBlock>(positionWidgetFloor, widthWidgetFloor);
	assert(createdFloorText && "Floor Text Invalid..");
	createdFloorText->SetTextValue(L"CurrentFloor : 01");
	textBlockFloor = createdFloorText;

	/* 현재 킬수 아이콘 표시 */
	const Vector2Int positionKillIcon(ViewWidth + 2, 8);
	std::shared_ptr<ImageWidget> killIcon = currentLevel->CreateWidget<ImageWidget>(positionKillIcon, L"💀", Color::LightBlue);
	assert(killIcon && "killIcon Invalid..");
	imageKillNumIcon = killIcon;

	/* 현재 킬수 표시 위젯 생성 */
	const int widthWidgetKillNum = 21;
	const Vector2Int positionWidgetKillNum(ViewWidth + 5, 8);
	std::shared_ptr<TextBlock> createdKillNumText = currentLevel->CreateWidget<TextBlock>(positionWidgetKillNum, widthWidgetKillNum);
	assert(createdKillNumText && "Kill Num Text Invalid..");
	createdKillNumText->SetTextValue(L"Kill Count : 0");
	textBlockKillNum = createdKillNumText;

	/* 현재 플레이어 체력 아이콘 표시 */
	const Vector2Int positionHealthIcon(ViewWidth + 2, 11);
	std::shared_ptr<ImageWidget> healthIcon = currentLevel->CreateWidget<ImageWidget>(positionHealthIcon, L"💖", Color::LightRed);
	assert(healthIcon && "healthIcon Invalid..");
	imageHealthIcon = healthIcon;

	/* 현재 플레이어 체력 바 표시 */
	const Vector2Int positionHealthProgressbar(ViewWidth + 5, 11);
	const int ProgressbarWidth = configBase.GetDisplayWidth() - ViewWidth - 7;
	std::shared_ptr<ProgressBar> healthBar = currentLevel->CreateWidget<ProgressBar>(positionHealthProgressbar, ProgressbarWidth, Color::DarkGray, Color::LightRed);
	assert(healthBar && "healthBar invalid..");
	healthProgressBar = healthBar;

	/* 아이템 리스트 아이콘 */
	const Vector2Int positionItemIcon(ViewWidth + 2, 14);
	std::shared_ptr<ImageWidget> itemIcon = currentLevel->CreateWidget<ImageWidget>(positionItemIcon, L"🎒", Color::Purple);
	assert(itemIcon && "ItemIcon Invalid..");
	imageItemIcon = itemIcon;

	/* 아이템 리스트 이름 표시 */
	const int widthWidgetItemList = 9;
	const Vector2Int positionWidgetItemList(ViewWidth + 5, 14);
	std::shared_ptr<TextBlock> createdItemListText = currentLevel->CreateWidget<TextBlock>(positionWidgetItemList, widthWidgetItemList);
	assert(createdItemListText && "Item List Text Invalid..");
	createdItemListText->SetTextValue(L"Item List");
	textBlockItemList = createdItemListText;
}

void HUDPlayer::ChangeFloorLevel(int newFloorLevel)
{
	std::shared_ptr<TextBlock> textBlock = textBlockFloor.lock();
	if (!textBlock)
	{
		return;
	}

	wchar_t szTextValue[64] = { 0 };
	swprintf_s(szTextValue, L"CurrentFloor : %02d", newFloorLevel);
	textBlock->SetTextValue(szTextValue);
}

void HUDPlayer::ChangeMonsterKillNum(int newKillNum)
{
	std::shared_ptr<TextBlock> textBlock = textBlockKillNum.lock();
	if (!textBlock)
	{
		return;
	}

	wchar_t szTextValue[64] = { 0 };
	swprintf_s(szTextValue, L"Kill Count : %d", newKillNum);
	textBlock->SetTextValue(szTextValue);
}

void HUDPlayer::ChangePlayerHealthValue(float current, float maxValue)
{
	std::shared_ptr<ProgressBar> progressbar = healthProgressBar.lock();
	if (!progressbar)
	{
		return;
	}

	progressbar->SetValue(current, maxValue);
}

void HUDPlayer::UpdateItemListIcon(const ItemBase& item)
{
	std::shared_ptr<Level> currentLevel = GetLevel();
	if (!currentLevel)
	{
		return;
	}

	const int itemID = item.GetItemID();
	const ItemData& findItemData = ItemDataTable::GetItemData(itemID);

	auto iterFindItemWidget = mapItemWidgets.find(itemID);
	if (iterFindItemWidget != mapItemWidgets.end())
	{
		std::shared_ptr<TextBlock> itemNameText = iterFindItemWidget->second.nameWidget.lock();
		if (itemNameText)
		{
			wchar_t itemNumText[16] = { 0 };
			_itow_s(item.GetItemNum(), itemNumText, _countof(itemNumText), 10);
			itemNameText->SetTextValue(findItemData.itemName + L" - Lv" + itemNumText);
		}
	}
	else
	{
		const ConfigBase& configBase = Engine::Get().GetConfig<Craft::ConfigBase>();
		const int ViewWidth = configBase.GetViewWidth();
		const int currentItemIconNum = static_cast<int>(mapItemWidgets.size());

		const int addYPos = 17 + (currentItemIconNum * 2);
		const Vector2Int positionItemIcon(ViewWidth + 5, addYPos);
		std::shared_ptr<ImageWidget> itemIcon = currentLevel->CreateWidget<ImageWidget>(positionItemIcon, findItemData.itemIconImage, Color::White);
		assert(itemIcon && "ItemIcon Invalid..");

		const Vector2Int positionWidgetItemName(ViewWidth + 8, addYPos);
		std::shared_ptr<TextBlock> itemNameText = currentLevel->CreateWidget<TextBlock>(positionWidgetItemName, static_cast<int>(findItemData.itemName.length()) + 5);
		assert(itemNameText && "Item Name Text Invalid..");

		wchar_t itemNumText[16] = { 0 };
		_itow_s(item.GetItemNum(), itemNumText, _countof(itemNumText), 10);
		itemNameText->SetTextValue(findItemData.itemName + L" - Lv" + itemNumText);

		mapItemWidgets.insert(std::pair<int, FItemWidget>(itemID, { itemIcon, itemNameText }));
	}
}
