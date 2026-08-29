#include "MultiSpriteRendererComponent.h"
#include "Render/Renderer.h"
#include "Actor/Actor.h"

namespace Craft
{
	MultiSpriteRendererComponent::MultiSpriteRendererComponent(Color color, int sortingOrder)
		:color(color)
		,sortingOrder(sortingOrder)
	{

	}

	void MultiSpriteRendererComponent::Draw()
	{
		super::Draw();

		// 컴포넌트가 소속된 액터.
		std::shared_ptr<Actor> actor = GetOwner();
		if (!actor || !actor->IsActive())
		{
			return;
		}

		/* 여러 줄에 걸쳐 Draw */
		Renderer& renderer = Renderer::Get();
		const Vector2Int& drawCenterPos = actor->GetWorldPosition();
		for (const FSpriteInfo& spriteInfo : drawImages)
		{
			renderer.Submit(spriteInfo.image, drawCenterPos + spriteInfo.imageOffset, color, sortingOrder);
		}
	}

	void MultiSpriteRendererComponent::AppendImage(const Vector2Int& offset, const std::wstring& image)
	{
		drawImages.emplace_back(FSpriteInfo(offset, image));
	}
}