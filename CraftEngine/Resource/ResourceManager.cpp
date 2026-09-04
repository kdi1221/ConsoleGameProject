#include "ResourceManager.h"
#include <cassert>

namespace Craft
{
	ResourceManager::ResourceManager()
		:resourceMaps(static_cast<int>(eResourceCategory::MaxNum))
	{
	
	}

	ResourceManager::~ResourceManager()
	{
		
	}

	bool ResourceManager::LoadResources()
	{
		return true;
	}

	std::shared_ptr<ResourceBase> ResourceManager::FindGameResource(const eResourceCategory category, int resourceIndex) const
	{
		const int resourceCategoryIndex = static_cast<int>(category);
		if (resourceCategoryIndex < 0 || resourceCategoryIndex >= static_cast<int>(resourceMaps.size()))
		{
			return nullptr;
		}

		const ResourceCategoryListType& resourceList = resourceMaps[resourceCategoryIndex];
		auto iterFindResource = resourceList.find(resourceIndex);

		if (iterFindResource == resourceList.end())
		{
			return nullptr;
		}

		return iterFindResource->second;
	}

	bool ResourceManager::AddResource(const eResourceCategory category,
										int resourceIndex, 
										std::shared_ptr<ResourceBase> resource)
	{
		const int resourceCategoryIndex = static_cast<int>(category);
		if (resourceCategoryIndex < 0 || resourceCategoryIndex >= static_cast<int>(resourceMaps.size()))
		{
			return false;
		}

		ResourceCategoryListType& resourceList = resourceMaps[resourceCategoryIndex];

		if (resourceList.find(resourceIndex) != resourceList.end())
		{
			return false;
		}

		resourceList.insert({ resourceIndex, resource});

		return true;
	}
}

