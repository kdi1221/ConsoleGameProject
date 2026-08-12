#include "AttributeComponent.h"

AttributeComponent::AttributeComponent(int initialHealth)
	:currentHealth(initialHealth)
	,maxHealth(initialHealth)
{
	
}

void AttributeComponent::AddOutofHealthCallback(const OnOutOfHealth& inCallback)
{
	onOutofHealth = inCallback;
}

void AttributeComponent::AddOutofHealthCallback(OnOutOfHealth&& inCallback)
{
	onOutofHealth = std::move(inCallback);
}

void AttributeComponent::DecreaseCurrrentHealth(int decreaseValue)
{
	currentHealth = std::max(0, currentHealth - decreaseValue);
	if (currentHealth <= 0)
	{
		//체력이 모두 감소되었으므로 이벤트 발생
		onOutofHealth();
	}
}
