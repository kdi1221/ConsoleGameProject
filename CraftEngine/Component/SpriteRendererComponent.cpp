#include "SpriteRendererComponent.h"
#include "TransformComponent.h"
#include "Render/Renderer.h"
#include "Actor/Actor.h"

namespace Craft
{
	SpriteRendererComponent::SpriteRendererComponent(const std::string& image, Color color, int sortingOrder)
		:image(image)
		,color(color)
		,sortingOrder(sortingOrder)
	{

	}

	void SpriteRendererComponent::Draw()
	{
		super::Draw();

		// 컴포넌트가 소속된 액터.
		std::shared_ptr<Actor> actor = GetOwner();
		if (!actor || !actor->IsActive())
		{
			return;
		}

		// TransformComponent 가져오기.
		std::shared_ptr<TransformComponent> transform = actor->GetTransform();
		if (!transform)
		{
			return;
		}

		// 렌더러에 필요한 데이터를 제출(Submit)
		Renderer::Get().Submit(image, transform->GetWorldPosition(), color, sortingOrder);
	}
}