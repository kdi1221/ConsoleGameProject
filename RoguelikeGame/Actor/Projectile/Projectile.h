#pragma once

#include <Actor/Actor.h>

namespace Craft
{
	class SpriteRendererComponent;
}

/* 게임 내 표시되는 모든 발사체들의 베이스 클래스 */
class Projectile : public Craft::Actor
{
	TYPE_DECLARATIONS(Projectile, Actor)

public:
	Projectile(const Craft::Vector2Int& inPosition,
				const std::wstring& inImage,
				Craft::Color inColor);
	virtual ~Projectile() = default;

private:
	/* 발사체 표시 Sprite Component */
	std::shared_ptr<Craft::SpriteRendererComponent> spriteComponent;
};

