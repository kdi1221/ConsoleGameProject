#pragma once

#include <UI/Widget/Widget.h>
#include <string>

namespace Craft
{
	class TextBlock;
	class ProgressBar;
}

//보스 상태 패널
class BossStatusPanel : public Craft::Widget
{
	TYPE_DECLARATIONS(BossStatusPanel, Widget)

public:
	BossStatusPanel(const Craft::Vector2Int& positionLT, int width, int height);
	virtual ~BossStatusPanel();

public:
	/* 체력 설정 */
	void SetHealthValue(float currentValue, float maxValue);

	/* 보스 이름 지정 */
	void SetBossNameText(const std::wstring& bossNameText);

private:
	/* 보스 이름 */
	std::shared_ptr<Craft::TextBlock> textBlockBossName;

	/* 보스 체력바 */
	std::shared_ptr<Craft::ProgressBar> healthBar;
};

