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
	AttributeComponent(int initialHealth);
	virtual ~AttributeComponent() = default;

public:
	void AddOutofHealthCallback(const OnOutOfHealth& inCallback);
	void AddOutofHealthCallback(OnOutOfHealth&& inCallback);
	void DecreaseCurrrentHealth(int decreaseValue);

public:
	inline int GetCurrentHealth() const { return currentHealth; }
	inline int GetMaxHealth() const { return maxHealth; }

private:
	std::function<void()> onOutofHealth;

private:
	/* 현재 체력 */
	int currentHealth = 0;

	/* 최대 체력 */
	int maxHealth = 0;
};

