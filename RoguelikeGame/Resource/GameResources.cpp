#include "GameResources.h"
#include "Resource/ResourceBossEye.h"

GameResources::GameResources()
{

}

bool GameResources::LoadResources()
{
	std::shared_ptr<ResourceBossEye> resourceBossEye = std::make_shared<ResourceBossEye>();
	if (!resourceBossEye->Initialize())
	{
		return false;
	}

	if (!AddResource(eResourceCategory::Image, 1, resourceBossEye))
	{
		return false;
	}

	return true;
}
