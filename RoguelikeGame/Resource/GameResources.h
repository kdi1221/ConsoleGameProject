#pragma once

#include <Resource/ResourceManager.h>

class GameResources : public Craft::ResourceManager
{
public:
	GameResources();
	virtual ~GameResources() = default;

private:
	/* 게임에 필요한 리소스들 로드 */
	virtual bool LoadResources() override;
};

