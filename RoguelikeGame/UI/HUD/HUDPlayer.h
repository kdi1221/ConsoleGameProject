#pragma once

#include <UI/HUD.h>
#include <unordered_map>
#include <windows.h>

namespace Craft
{
	class TextBlock;
	class ImageWidget;
	class ProgressBar;
}

class TextBlockFPS;
class TextBlockMousePos;
class TextBlockMouseButton;
class TextBlockElapsedTime;
class BackgroundWidget;
class ItemBase;

struct FItemWidget
{
	std::weak_ptr<Craft::ImageWidget> iconWidget;
	std::weak_ptr<Craft::TextBlock> nameWidget;
};

class HUDPlayer : public Craft::HUD
{
	TYPE_DECLARATIONS(HUDPlayer, HUD)

public:
	HUDPlayer();
	virtual ~HUDPlayer() = default;

public:
	virtual void InitializeHUD(std::weak_ptr<Craft::Level> activeLevel) override;

public:
	void ChangeFloorLevel(int newFloorLevel);
	void ChangeMonsterKillNum(int newKillNum);
	void ChangePlayerHealthValue(float current, float maxValue);
	void ChangePlayerManaValue(float current, float maxValue);
	void UpdateItemListIcon(const ItemBase& item);
	void SetStartPlayTime(LARGE_INTEGER startTime);
	void SetGamePause(bool bPause);

private:
	std::weak_ptr<TextBlockFPS> textBlockFPS;
	std::weak_ptr<TextBlockMousePos> textBlockMousePos;
	std::weak_ptr<TextBlockMouseButton> textBlockMouseBtn;
	std::weak_ptr<BackgroundWidget> hudBackground;
	std::weak_ptr<Craft::ImageWidget> imageTimeIcon;
	std::weak_ptr<TextBlockElapsedTime> textBlockTime;
	std::weak_ptr<Craft::ImageWidget> imageFloorIcon;
	std::weak_ptr<Craft::TextBlock> textBlockFloor;
	std::weak_ptr<Craft::ImageWidget> imageKillNumIcon;
	std::weak_ptr<Craft::TextBlock> textBlockKillNum;
	std::weak_ptr<Craft::ImageWidget> imageHealthIcon;
	std::weak_ptr<Craft::ProgressBar> healthProgressBar;
	std::weak_ptr<Craft::ImageWidget> imageManaIcon;
	std::weak_ptr<Craft::ProgressBar> manaProgressBar;
	std::weak_ptr<Craft::ImageWidget> imageItemIcon;
	std::weak_ptr<Craft::TextBlock> textBlockItemList;
	std::weak_ptr<Craft::ImageWidget> imageControlIcon;
	std::weak_ptr<Craft::TextBlock> textBlockControl;
	std::weak_ptr<Craft::TextBlock> textBlockControlMove;
	std::weak_ptr<Craft::TextBlock> textBlockControlAttack;
	
	std::unordered_map<int, FItemWidget> mapItemWidgets;

};

