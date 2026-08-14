#pragma once

#include "Component/Component.h"
#include <functional>

/* 체력 등의 속성값 관리 */
class AttributeComponent : public Craft::Component
{
	TYPE_DECLARATIONS(AttributeComponent, Component)

public:
	using OnOutOfHealth = std::function<void()>;

public:
	AttributeComponent(float initialHealth);
	virtual ~AttributeComponent() = default;

public:
	void AddOutofHealthCallback(const OnOutOfHealth& inCallback);
	void AddOutofHealthCallback(OnOutOfHealth&& inCallback);
	void DecreaseCurrrentHealth(float decreaseValue);

public:
	inline float GetCurrentHealth() const { return currentHealth; }
	inline float GetMaxHealth() const { return maxHealth; }
	inline bool IsDeath() const { return isDeath; }

private:
	std::function<void()> onOutofHealth;

private:
	/* 현재 체력 */
	float currentHealth = 0.f;

	/* 최대 체력 */
	float maxHealth = 0.f;

	/* 사망 여부 */
	bool isDeath = false;
};

