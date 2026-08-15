#pragma once

#include <UI/HUD.h>
#include <unordered_map>

namespace Craft
{
	class TextBlock;
	class ImageWidget;
	class ProgressBar;
}

class TextBlockFPS;
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
	void UpdateItemListIcon(const ItemBase& item);

private:
	std::weak_ptr<TextBlockFPS> textBlockFPS;
	std::weak_ptr<BackgroundWidget> hudBackground;
	std::weak_ptr<Craft::ImageWidget> imageFloorIcon;
	std::weak_ptr<Craft::TextBlock> textBlockFloor;
	std::weak_ptr<Craft::ImageWidget> imageKillNumIcon;
	std::weak_ptr<Craft::TextBlock> textBlockKillNum;
	std::weak_ptr<Craft::ImageWidget> imageHealthIcon;
	std::weak_ptr<Craft::ProgressBar> healthProgressBar;
	std::weak_ptr<Craft::ImageWidget> imageItemIcon;
	std::weak_ptr<Craft::TextBlock> textBlockItemList;
	
	std::unordered_map<int, FItemWidget> mapItemWidgets;

};

