#include <scene/types/nodes/ui_node.h>
#include <core/globals.h>

namespace scene {

    UINode::UINode(std::string name, std::shared_ptr<Node> parent)
    {
        this->name = name;
		this->parent = parent;
        this->type = Type_UINode;
    }

    void UINode::Layout(bool direction)
    {
        // If direction == true, then start at the leaf nodes and go backwards
        if (direction)
        {
            LayoutChildren(direction);

            // Since this is a base UI node, it doesn't need to do any layout calculations

            // - Insert layout calculations here -
        }
        else
        {
            // Else, start at the current node and go forwards

            // Since this is a base UI node, it doesn't need to do any layout calculations

            // - Insert layout calculations here -

            LayoutChildren(direction);
        }
        
    }

    void UINode::LayoutChildren(bool direction)
    {
        for (auto& child : children)
        {
            if ((child->GetType() & NodeType::Type_UINode) == NodeType::Type_UINode)
            {
                std::shared_ptr<UINode> childUINode = std::dynamic_pointer_cast<UINode>(child);

                if (!childUINode)
                {
                    logger.err("Tried to layout a child UI node that doesn't exist! Scene tree is likely corrupted!");
                    continue;
                }

                childUINode->Layout(direction);
            }
        }
    }
}