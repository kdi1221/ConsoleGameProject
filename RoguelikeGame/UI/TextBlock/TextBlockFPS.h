#pragma once

#include <UI/Widget/TextBlock/TextBlock.h>

class TextBlockFPS : public Craft::TextBlock
{
	TYPE_DECLARATIONS(TextBlockFPS, TextBlock)

public:
	TextBlockFPS(const Craft::Vector2Int& positionLT, int widgetWidth);
	virtual ~TextBlockFPS() = default;

private:
	virtual void Tick(float deltaTime) override;

private:
	void FormatFPSString(const float deltaTime);
};

