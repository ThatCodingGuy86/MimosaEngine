#pragma once
#include "node.h"
#include <math/vec.h>

namespace scene
{
	class UINode : public Node
	{
	public:
		math::vec2 pos;
		math::vec2 dim;
		
		//! @brief UI node constructor
		UINode(std::string name, std::shared_ptr<Node> parent);

		/*!
		 * @brief Recursively calculates the position and dimensions of every child UI node and the current node
		 * @param direction 
		 */
		virtual void Layout(bool direction);

		/*!
		 * @brief Calls the Layout function on all the child UI nodes, but doesn't calculate this node's layout
		 * @param direction Passed into the Layout function on the child nodes
		 */
		void LayoutChildren(bool direction);
	};
}