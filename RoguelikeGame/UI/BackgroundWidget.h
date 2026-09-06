#pragma once

#include <UI/Widget/Widget.h>
#include <Math/Color.h>
#include <vector>
#include <string>

class BackgroundWidget : public Craft::Widget
{
	TYPE_DECLARATIONS(BackgroundWidget, Widget)

public:
	BackgroundWidget(const Craft::Vector2Int& positionLT, int widgetWidth, int widgetHeight, Craft::Color color);
	virtual ~BackgroundWidget() = default;

public:
	virtual void Draw() override;

private:
	Craft::Color drawColor = Craft::Color::White;

	std::vector<std::wstring> drawImageBuffer;
};

