#pragma once
#include "node.h"
#include <math/vec.h>

namespace scene
{
	class Node2D : public Node
	{
	public:
		math::vec2 pos;
	};
}