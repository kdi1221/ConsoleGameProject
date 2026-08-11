#pragma once

#include <Core/Core.h>
#include <Math/Vector2Int.h>
#include <vector>

class CRAFT_API StaticFunctionLibrary
{
public:
	static void GetBresenhamPath(const Craft::Vector2Int& startPos, 
								const Craft::Vector2Int& endPos,
								std::vector<Craft::Vector2Int>& outPaths);

private:
	StaticFunctionLibrary() = default;
};