#pragma once
#include "node.h"
#include <math/vec.h>

namespace scene
{
	class Node3D : public Node
	{
	public:
		math::vec3 pos;
	};
}