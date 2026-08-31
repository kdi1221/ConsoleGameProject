#pragma once

#include <Math/Color.h>
#include <UI/Widget/Widget.h>

/* 스킬, 아이템 아이콘 */
class QuickSlotIcon : public Craft::Widget
{
	TYPE_DECLARATIONS(QuickSlotIcon, Widget)

public:
	static const int SLOT_ICON_WIDTH;
	static const int SLOT_ICON_HEIGHT;

public:
	QuickSlotIcon(const Craft::Vector2Int& positionLT);

private:
	virtual void Draw() override;

private:
	//외곽 모서리 색상
	Craft::Color outlineColor = Craft::Color::Yellow;
};

