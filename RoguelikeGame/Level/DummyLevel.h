#pragma once

#include <Level/Level.h>

class TextBlockFPS;

/* 더미 레벨, 프레임 체크용 */
class DummyLevel : public Craft::Level
{
	TYPE_DECLARATIONS(DummyLevel, Level)

public:
	DummyLevel();
	virtual ~DummyLevel();

protected:
	virtual void OnInitialized() override;

private:
	std::weak_ptr<TextBlockFPS> textBlockFPS;
};

