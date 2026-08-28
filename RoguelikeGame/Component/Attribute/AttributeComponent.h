#pragma once

#include "Component/Component.h"
#include <functional>

using ChangeAttributeValueEvent = std::function<void(float, float)>;

/* 속성값 정의를 위한 매크로 */
#define ATTRIBUTE_DECLARATIONS(AttributeName)													\
private:																						\
	float current##AttributeName = 0.f;															\
	float max##AttributeName = 0.f;																\
																								\
private:																						\
	ChangeAttributeValueEvent onChange##AttributeName;											\
																								\
public:																							\
	inline float GetCurrent##AttributeName() const {return current##AttributeName;}				\
	inline float GetMax##AttributeName() const {return max##AttributeName;}						\
																								\
public:																							\
	void Initial##AttributeName(float currentValue, float maxValue)								\
	{																							\
		max##AttributeName = maxValue;															\
		current##AttributeName = (std::max)(0.f, (std::min)(currentValue, max##AttributeName));	\
	}																							\
																								\
	void SetCurrent##AttributeName(float newValue)												\
	{																							\
		current##AttributeName = (std::max)(0.f, (std::min)(newValue, max##AttributeName));		\
		if(onChange##AttributeName)																\
		{																						\
			onChange##AttributeName(current##AttributeName, max##AttributeName);				\
		}																						\
	}																							\
																								\
	void SetMax##AttributeName(float newValue)													\
	{																							\
		max##AttributeName = newValue;															\
		if(onChange##AttributeName)																\
		{																						\
			onChange##AttributeName(current##AttributeName, max##AttributeName);				\
		}																						\
	}																							\
																								\
	void SetChange##AttributeName##EventCallback(ChangeAttributeValueEvent callback)			\
	{																							\
		onChange##AttributeName = callback;														\
	}


/* 체력 등의 속성값 관리 */
class AttributeComponent : public Craft::Component
{
	TYPE_DECLARATIONS(AttributeComponent, Component)

public:
	AttributeComponent() = default;
	virtual ~AttributeComponent() = default;

	/* Health 속성 */
	ATTRIBUTE_DECLARATIONS(Health)
};

