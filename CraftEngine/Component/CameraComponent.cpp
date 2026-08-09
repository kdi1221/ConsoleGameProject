#include "CameraComponent.h"
#include "Engine/Engine.h"
#include "Camera/CameraManager.h"

namespace Craft
{
	CameraComponent::CameraComponent()
	{

	}
	
	void CameraComponent::SetViewPosition(const Vector2Int& viewPosition)
	{
		CameraManager& cameraManager = Engine::Get().GetCameraManager();
		cameraManager.SetViewPosition(viewPosition);
	}
}