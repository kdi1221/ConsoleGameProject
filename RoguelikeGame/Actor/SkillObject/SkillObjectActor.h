#pragma once

#include <Actor/Actor.h>

/* 스킬로 생성되는 모든 액터들의 부모 클래스 */
class SkillObject : public Craft::Actor
{
	TYPE_DECLARATIONS(SkillObject, Actor)

public:
	SkillObject(const Craft::Vector2Int& position);
	virtual ~SkillObject() = default;

};

