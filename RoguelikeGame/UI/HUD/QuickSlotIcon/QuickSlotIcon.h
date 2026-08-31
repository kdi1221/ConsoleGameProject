#pragma once

#include <Math/Vector2Int.h>
#include <Math/Color.h>
#include <UI/Widget/Widget.h>
#include <string>

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
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

public:
	void SetIconImage(const std::wstring& image);
	void SetIconImageColor(const Craft::Color& color);
	void StartCooldown(const float cooldownTime);
	void StopCooldown();
	void SetGamePause(bool bPause);

private:
	//현재 게임 일시정지 여부
	bool bGamePause = false;

	//현재 쿨다운 상태 여부
	bool bCooldownState = false;

	//아이콘 이미지
	std::wstring iconImage = L"";

	//아이콘 이미지 색상
	Craft::Color iconImageColorBase = Craft::Color::White;

	//아이콘 이미지 색상(Cooldown 진행시 색상)
	Craft::Color iconImageColorCooldown = Craft::Color::DarkGray;

	//아이콘 이미지 출력 위치
	Craft::Vector2Int iconImaeDrawPos = Craft::Vector2Int::Zero;

	//외곽 모서리 Cooldown 만족시 색상
	Craft::Color outlineColorBase = Craft::Color::Yellow;

	//외곽 모서리 Cooldown 진행시 색상
	Craft::Color outlineColorCooldown = Craft::Color::DarkGray;
};

