#pragma once

#include <UI/Widget/TextBlock/TextBlock.h>

class TextBlockMousePos : public Craft::TextBlock
{
	TYPE_DECLARATIONS(TextBlockMousePos, TextBlock)

public:
	TextBlockMousePos(const Craft::Vector2Int& positionLT, int widgetWidth);
	virtual ~TextBlockMousePos() = default;

private:
	virtual void Tick(float deltaTime) override;

private:
	void FormatMousePosString();
};

