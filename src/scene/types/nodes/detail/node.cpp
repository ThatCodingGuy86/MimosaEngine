#include <scene/types/nodes/node.h>
#include <core/globals.h>

namespace scene {
	Node::Node(std::string name, std::shared_ptr<Node> parent)
	{
		this->name = name;
		this->parent = parent;
		this->type = Type_Node;
	}

	void Node::Update(double delta)
	{
		for (auto& child : children)
		{
			if (!child)
			{
				logger.err("Tried to update a child UI node that doesn't exist! Scene tree is likely corrupted!");
                continue;
			}

			child->Update(delta);
		}

		if (updateFunc)
			updateFunc(this->shared_from_this(), delta);
	}

	void Node::PhysUpdate(double delta)
	{
		for (auto& child : children)
		{
			if (!child)
			{
				logger.err("Tried to update a child UI node that doesn't exist! Scene tree is likely corrupted!");
                continue;
			}

			child->PhysUpdate(delta);
		}

		if (physUpdateFunc)
			physUpdateFunc(this->shared_from_this(), delta);
	}

	size_t Node::AddChild(std::shared_ptr<Node> child)
	{
		size_t index = children.size();
		children.push_back(child);

		child->SetParent(this->shared_from_this());

		if (this->isRoot || this->isInSceneTree)
			child->UpdateIsInSceneTree(true);

		return index;
	}

	void Node::RemoveChild(size_t child)
	{
		children.erase(children.begin() + child, children.begin() + child);
	}

	void Node::MoveChild(size_t childIndex, std::shared_ptr<Node> newParent)
	{
		if (childIndex >= children.size())
		{
			logger.warn("Tried to move a child whose index is out of range");
			return;
		}

		std::shared_ptr<Node> child = children[childIndex];

		newParent->AddChild(child);
		this->RemoveChild(childIndex);
	}

	std::shared_ptr<Node> Node::FindNode(NodePath& path)
	{
		// Can't find a node that isn't a descendant of this node
		if (path.GetOrigin().get() != this)
		{
			logger.warn("Couldn't find node " + path.GetConcatenatedPath() + " because it is not a descendant of this node");
			return nullptr;
		}

		const std::vector<std::string> nodePath = path.GetPath();
		std::shared_ptr<Node> curNode = std::make_shared<Node>(*this);

		for (auto& pathNode : nodePath)
		{
			std::shared_ptr<Node> child = curNode->FindChildPtr(pathNode);

			if (child)
			{
				curNode = child;
			}
			else
			{
				logger.warn("Couldn't find node " + path.GetConcatenatedPath());
				return nullptr;
			}
		}

		return curNode;
	}

	size_t Node::FindChild(std::string childName)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->name == childName)
				return i;
		}

		logger.warn("Couldn't find child with name " + name);
		return (unsigned long long)-1;
	}

	std::shared_ptr<Node> Node::FindChildPtr(std::string childName)
	{
		size_t index = FindChild(childName);

		if (index == -1)
			return nullptr;

		return children[index];
	}

	const std::vector<std::shared_ptr<Node>> Node::FindChildren(std::function<bool(std::shared_ptr<Node>)> filterFunc)
	{
		if (!filterFunc)
		{
			logger.err("FindChildren(): `filterFunc` was empty!");
			return {};
		}

		std::vector<std::shared_ptr<Node>> matchedChildren = {};
		for (auto& node : children)
		{
			if (filterFunc(node))
				matchedChildren.push_back(node);
		}

		return matchedChildren;
	}

	const std::vector<std::shared_ptr<Node>>& Node::GetChildren()
	{
		return children;
	}

	std::shared_ptr<Node> Node::GetParent()
	{
		return parent;
	}

	void Node::SetParent(std::shared_ptr<Node> newParent)
	{
		this->parent = newParent;
	}

	NodeType Node::GetType()
	{
		return type;
	}

	void Node::UpdateIsInSceneTree(bool newValue)
	{
		// Update and maybe initialize the children first
		for (auto& child : children)
		{
			child->UpdateIsInSceneTree(newValue);
		}

		// Then update and maybe initialize this node

		isInSceneTree = newValue;

		if (!hasBeenInitalized && newValue == true)
		{
			if (initFunc)
				initFunc(this->shared_from_this());

			hasBeenInitalized = true;
		}
	}

	void Node::SetInitFunc(std::function<void(std::shared_ptr<Node>)> newInitFunc)
	{
		if (initFunc)
		{
			logger.err("Tried to set the init function of " + name + ", but it has already been set!");
			return;
		}

		initFunc = newInitFunc;
	}

	void Node::SetUpdateFunc(std::function<void(std::shared_ptr<Node>, double)> newUpdateFunc)
	{
		if (updateFunc)
		{
			logger.err("Tried to set the update function of " + name + ", but it has already been set!");
			return;
		}

		updateFunc = newUpdateFunc;
	}

	void Node::SetPhysUpdateFunc(std::function<void(std::shared_ptr<Node>, double)> newPhysUpdateFunc)
	{
		if (physUpdateFunc)
		{
			logger.err("Tried to set the physics update function of " + name + ", but it has already been set!");
			return;
		}

		physUpdateFunc = newPhysUpdateFunc;
	}
}