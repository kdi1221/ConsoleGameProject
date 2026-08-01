#include "Box.h"

Box::Box(const Craft::Vector2& position)
	:Actor("B", position, Craft::Color::Red)
{
	// Ground/Target 액터와 겹쳤을때 Box 액터가 위에 그려지도록 정렬 순서 설정.
	sortingOrder = 3;
}
