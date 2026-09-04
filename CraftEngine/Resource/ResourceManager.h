#pragma once

#include "Core/Core.h"
#include <unordered_map>
#include <vector>
#include <memory>

namespace Craft
{
	class ResourceBase;

	class CRAFT_API ResourceManager
	{
	public:
		enum class eResourceCategory
		{
			Image = 0,
			MaxNum
		};

	public:
		/* 리소스 종류별 타입 */
		using ResourceCategoryListType = std::unordered_map<int, std::shared_ptr<ResourceBase>>;

	public:
		ResourceManager();
		virtual ~ResourceManager();

	public:
		/* 게임에 필요한 리소스들 로드 */
		virtual bool LoadResources();

	public:
		/* 특정 리소스 반환 */
		std::shared_ptr<ResourceBase> FindGameResource(const eResourceCategory category, int resourceIndex) const;

	protected:
		bool AddResource(const eResourceCategory category, int resourceIndex, std::shared_ptr<ResourceBase> resource);

	private:
		/* 로드된 리소스들 */
		std::vector<ResourceCategoryListType> resourceMaps;
	};
}

