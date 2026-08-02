#include "Component.h"

namespace Craft
{
	Component::Component()
	{

	}

	Component::~Component()
	{

	}

	void Component::BeginPlay()
	{
		//중복 호출 방지를 위해 설정.
		hasBeganPlay = true;
	}

	void Component::Tick(float deltaTime)
	{

	}

	void Component::Draw()
	{

	}

	void Component::OnCollision(const std::shared_ptr<Actor>& other)
	{

	}
}
