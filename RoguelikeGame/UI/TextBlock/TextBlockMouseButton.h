#pragma once

#include <UI/Widget/TextBlock/TextBlock.h>

class TextBlockMouseButton : public Craft::TextBlock
{
	TYPE_DECLARATIONS(TextBlockMouseButton, TextBlock)

public:
	TextBlockMouseButton(const Craft::Vector2Int& positionLT, int widgetWidth);
	virtual ~TextBlockMouseButton() = default;

private:
	virtual void Tick(float deltaTime) override;

private:
	void FormatMouseButton();
};

