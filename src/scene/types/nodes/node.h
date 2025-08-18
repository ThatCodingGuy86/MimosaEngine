#pragma once
#include <functional>
#include <array>
#include <string>
#include <memory>
#include <functional>
#include <scene/types/node_path.h>
#include <scene/types/node_type.h>

namespace scene
{

	//! @brief The internal representation of a node in the scene tree
	class Node : public std::enable_shared_from_this<Node>
	{
	public:
		
		Node(std::string name, std::shared_ptr<Node> parent);

		/*!
		 * @brief Runs the update function of this node and all child nodes recursively
		 * @param delta The delta time value to use
		 */
		void Update(double delta);

		/*!
		 * @brief Runs the physics update function of this node and all child nodes recursively
		 * @param delta The delta time value to use
		 */
		void PhysUpdate(double delta);

		/*!
		 * @brief Adds a child to this node
		 * @param child The child node
		 * @return The index of the child node
		 */
		size_t AddChild(std::shared_ptr<Node> child);

		/*!
		 * @brief Moves a child of this node to a new parent node
		 * @param childIndex The child to move
		 * @param newParent The new parent node
		 */
		void MoveChild(size_t childIndex, std::shared_ptr<Node> newParent);

		/*!
		 * @brief Removes a child from this node
		 * @param child The index of the child node to remove
		 */
		void RemoveChild(size_t child);

		/*!
		 * @brief Finds a node
		 * @param path The node path
		 * @returns A pointer to the node, or nullptr if it couldn't be found
		 */
		std::shared_ptr<Node> FindNode(NodePath& path);

		/*!
		 * @brief Finds a child with a given name
		 * @param childName The name to search for
		 * @returns The child index, or -1 if it couldn't be found
		 */
		size_t FindChild(std::string childName);

		/*!
		 * @brief Finds a child with a given name
		 * @param childName The name to search for
		 * @returns A pointer to the child, or nullptr if it couldn't be found
		 */
		std::shared_ptr<Node> FindChildPtr(std::string childName);

		/*!
		 * @brief Finds all children that meet a given filter function
		 * @param filterFunc The filter function to use
		 * @returns A list of the children that met the filter
		 */
		const std::vector<std::shared_ptr<Node>> FindChildren(std::function<bool(std::shared_ptr<Node>)> filterFunc);

		const std::vector<std::shared_ptr<Node>>& GetChildren();

		std::shared_ptr<Node> GetParent();

		/*!
		 * @brief Sets the parent of this node, does not update the children of the parent. Call AddChild or MoveChild to add or move a child properly.
		 * @param newParent The parent node to set
		 */
		void SetParent(std::shared_ptr<Node> newParent);

		//! @brief Gets the node type
		NodeType GetType();

		/*!
		 * @brief Updates whether this node and its descendants are in the scene tree.
		 * May also call the initFunc when applicable.
		 * @param newValue The new value to update to
		 */
		void UpdateIsInSceneTree(bool newValue);

		//! @brief Sets the init function. Will fail if it has already been set.
		void SetInitFunc(std::function<void(std::shared_ptr<Node>)> newInitFunc);

		//! @brief Sets the update function. Will fail if it has already been set.
		void SetUpdateFunc(std::function<void(std::shared_ptr<Node>, double)> newUpdateFunc);

		//! @brief Sets the physics update function. Will fail if it has already been set.
		void SetPhysUpdateFunc(std::function<void(std::shared_ptr<Node>, double)> newPhysUpdateFunc);

		std::string name = "New Node";

		bool isRoot = false;
	protected:
		std::shared_ptr<Node> parent = nullptr;
		std::vector<std::shared_ptr<Node>> children = {};

		NodeType type = Type_NONE;

		bool isInSceneTree = false;
		bool hasBeenInitalized = false;

		std::function<void(std::shared_ptr<Node>)> initFunc = nullptr;
		std::function<void(std::shared_ptr<Node>, double)> updateFunc = nullptr;
		std::function<void(std::shared_ptr<Node>, double)> physUpdateFunc = nullptr;
	};
}