#include "AttributeComponent.h"

AttributeComponent::AttributeComponent(float initialHealth)
	:currentHealth(initialHealth)
	,maxHealth(initialHealth)
{
	
}

void AttributeComponent::SetOutofHealthCallback(OnOutOfHealth inCallback)
{
	onOutofHealth = inCallback;
}

void AttributeComponent::SetChangeHealthValueCallback(OnChangeHeatlhValue inCallback)
{
	onChangeHealth = inCallback;
}

void AttributeComponent::InitializeHealthValue(float currentValue, float maxValue)
{
	currentHealth = currentValue;
	maxHealth = maxValue;

	isDeath = currentHealth <= 0.f;
}

void AttributeComponent::DecreaseCurrrentHealth(float decreaseValue)
{
	if (isDeath)
	{
		return;
	}

	currentHealth = std::max(0.f, currentHealth - decreaseValue);
	if (onChangeHealth)
	{
		onChangeHealth(currentHealth, maxHealth);
	}

	if (currentHealth <= 0.f)
	{
		isDeath = true;

		//체력이 모두 감소되었으므로 이벤트 발생
		if (onOutofHealth)
		{
			onOutofHealth();
		}
	}
}

void AttributeComponent::IncreaseCurrrentHealth(float increaseValue)
{
	if (isDeath)
	{
		return;
	}

	currentHealth = std::min(maxHealth, currentHealth + increaseValue);
	if (onChangeHealth)
	{
		onChangeHealth(currentHealth, maxHealth);
	}
}
