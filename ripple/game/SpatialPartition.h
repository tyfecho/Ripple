/*
* SpatialPartition.h
*	Tree data structure where each internal node has exactly 4 children
*	Each node can hold up to a certain amount of objects before it is automatically split into 4 sub-nodes
*	-All objects in the parent node will then be re-allocated among the sub-nodes if possible
*/

#pragma once

/***************************************************
*	File includes
***************************************************/

#include "GameObject.h"

/***************************************************
*	Macro Definitions
***************************************************/

#define QUADTREE_MAX_OBJECTS 6
//#define QUADTREE_DEBUG_DRAW
#ifdef QUADTREE_DEBUG_DRAW
	#include "GameObjectExt.h"
#endif

/***************************************************
*	Game namespace
***************************************************/

namespace Game
{
	/*
	*	Quad Tree Class
	*/
	class QuadTree
#ifdef QUADTREE_DEBUG_DRAW
		: public GameObject
#endif
	{
	private:
		int m_Level;						// How deep the node is (Top-most node is 0)
		LinkedList<GameObject*> m_Objects;	// The list of objects in this node
		QuadTree* m_Nodes;					// Pointer to the child nodes. If there are no child nodes, this pointer will be null
#ifndef QUADTREE_DEBUG_DRAW
		VECTOR2 m_Position;					// Centre position of this node
		VECTOR2 m_Size;						// Size of this node
#endif
	public:
		// Constructor
		QuadTree();
		// Destructor
		~QuadTree();
		// Initialize the quad tree values
		void Initialize(int level, VECTOR2 pos, VECTOR2 size);
		// Clear all objects in the quad tree
		void Clear();
		// Split the parent node into 4 sub nodes
		void Split();
		// Determine which child node the object belongs to
		// -1 means that the object cannot completely fit within a child node and is part of the parent node
		// Else, the index will be in the order of ((0, 1), (2, 3))
		int GetIndex(VECTOR2 rect_pos, VECTOR2 rect_size) ;
		// Insert the object into the QuadTree
		// If the number of objects in the parent node exceeds the capacity, it will split and add all objects to the corresponding child nodes
		// However, if parent node has been split but number of objects still exceeds the limit (Cannot fit into child nodes),
		// -Objects will be left in the parent node as they are
		void Insert(GameObject* obj);
		// Get all objects that could collide with the given object
		// Returns the number of objects that could possibly collide
		// First parameters returns either the list of objects that may collide or the node of the last object that may collide
		int Retrieve(LinkedList<GameObject*>* returnObjects, VECTOR2 rect_pos, VECTOR2 rect_size);
		int Retrieve(Node<GameObject*>* *collisionNodeIt, VECTOR2 rect_pos, VECTOR2 rect_size, int list_size = 0);
#ifdef QUADTREE_DEBUG_DRAW
		// Render the partitioning squares
		void DrawUpper();
		void DrawLower();
#endif
	};
}