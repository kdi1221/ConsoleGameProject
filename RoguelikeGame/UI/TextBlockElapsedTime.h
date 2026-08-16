#pragma once

#include <UI/Widget/TextBlock/TextBlock.h>

class TextBlockElapsedTime : public Craft::TextBlock
{
	TYPE_DECLARATIONS(TextBlockElapsedTime, TextBlock)

public:
	TextBlockElapsedTime(const Craft::Vector2Int& positionLT);
	virtual ~TextBlockElapsedTime() = default;

private:
	virtual void Tick(float deltaTime) override;

public:
	void SetStartTime(LARGE_INTEGER time);

private:
	void FormatTimeString();

private:
	LARGE_INTEGER frequency;
	LARGE_INTEGER startTime;
};

