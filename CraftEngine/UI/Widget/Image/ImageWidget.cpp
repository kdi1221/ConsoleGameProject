#include "ImageWidget.h"
#include "Render/Renderer.h"

namespace Craft
{
	ImageWidget::ImageWidget(const Vector2Int& positionLT, 
							const std::wstring& newImage, 
							const Color newColor)
		:super(positionLT, static_cast<int>(newImage.length()), 1)
		,imageValue(newImage)
		,drawColor(newColor)
	{

	}

	void ImageWidget::Draw()
	{
		//비활성화 상태라면 처리 안함.
		if (!IsActive())
		{
			return;
		}

		if (!imageValue.empty())
		{
			Renderer::Get().SubmitUI(imageValue, GetPosition(), drawColor, GetRenderSortingOrder());
		}
	}
}