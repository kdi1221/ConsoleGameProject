#pragma once

#include "Core/Core.h"
#include "Core/CraftObject.h"
#include "Math/Vector2Int.h"

namespace Craft
{
	/* 게임에 표시되는 모든 리소스(이미지 등)들의 베이스 클래스 */
	class CRAFT_API ResourceBase : public CraftObject
	{
		// 타입 정보 설정을 위한 매크로 추가.
		TYPE_DECLARATIONS(ResourceBase, CraftObject)

	public:
		ResourceBase();
		virtual ~ResourceBase();

	public:
		/* 초기화 */
		virtual bool Initialize();
	};
}

