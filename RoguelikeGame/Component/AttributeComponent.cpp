#include "AttributeComponent.h"

AttributeComponent::AttributeComponent(float initialHealth)
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

void AttributeComponent::DecreaseCurrrentHealth(float decreaseValue)
{
	if (isDeath)
	{
		return;
	}

	currentHealth = std::max(0.f, currentHealth - decreaseValue);
	if (currentHealth <= 0.f)
	{
		isDeath = true;

		//체력이 모두 감소되었으므로 이벤트 발생
		onOutofHealth();
	}
}
