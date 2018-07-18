/*
* SpatialPartition.cpp
*/

/***************************************************
*	File includes
***************************************************/

#include "SpatialPartition.h"

/***************************************************
*	Game namespace
***************************************************/

namespace Game
{
	/***************************************************
	*	Quad Tree Class
	***************************************************/
	
	/*
	*	Constructor
	*/
	QuadTree::QuadTree()
#ifdef QUADTREE_DEBUG_DRAW
		: GameObject()
#endif
	{
		m_Nodes = NULL;
	};

	/*
	*	Destructor
	*/
	QuadTree::~QuadTree()
	{
		Clear();
	}

	/*
	*	Initialize the quad tree values
	*/
	void QuadTree::Initialize(int level, VECTOR2 pos, VECTOR2 size)
	{
		m_Level = level;
		m_Position = pos;
		m_Size = size;
#ifdef QUADTREE_DEBUG_DRAW
		SetTexture(ETEXTURE_DEBUG_SQUARE);
#endif
	}

	/*
	*	Clear all objects in the quad tree
	*/
	void QuadTree::Clear()
	{
		m_Objects.clear();
 
		if (m_Nodes)
		{
			for (int i = 0; i < 4; ++ i) 
				m_Nodes[i].Clear();

			delete[] m_Nodes;
			m_Nodes = NULL;
		}
	}

	/*
	*	Split the parent node into 4 sub nodes
	*/
	void QuadTree::Split()
	{
		VECTOR2 halfSize = m_Size * 0.5f;
		VECTOR2 quarSize = m_Size * 0.25f;
		m_Nodes = new QuadTree[4];
		m_Nodes[0].Initialize(m_Level+1, VECTOR2(m_Position.x - quarSize.x, m_Position.y - quarSize.y), halfSize);
		m_Nodes[1].Initialize(m_Level+1, VECTOR2(m_Position.x + quarSize.x, m_Position.y - quarSize.y), halfSize);
		m_Nodes[2].Initialize(m_Level+1, VECTOR2(m_Position.x - quarSize.x, m_Position.y + quarSize.y), halfSize);
		m_Nodes[3].Initialize(m_Level+1, VECTOR2(m_Position.x + quarSize.x, m_Position.y + quarSize.y), halfSize);
	}

	/*
	*	Determine which child node the object belongs to
	*	-1 means that the object cannot completely fit within a child node and is part of the parent node
	*	Else, the index will be in the order of ((0, 1), (2, 3))
	*/
	int QuadTree::GetIndex(VECTOR2 rect_pos, VECTOR2 rect_size) 
	{
		int index = -1;
		VECTOR2 rect_min = rect_pos - rect_size*0.5f;
		VECTOR2 rect_max = rect_pos + rect_size*0.5f;
 
		// Object can completely fit within the top half
		bool topQuadrant = (rect_max.y < m_Position.y);
		// Object can completely fit within the bottom half
		bool bottomQuadrant = (rect_min.y > m_Position.y);
 
		// Object can completely fit within the left half
		if (rect_max.x < m_Position.x) 
		{
			//Top left quadrant
			if (topQuadrant)
				index = 0;
			//Btm left quadrant
			else if (bottomQuadrant)
				index = 2;
		}
		// Object can completely fit within the right half
		else if (rect_min.x > m_Position.x) 
		{
			//Top right quadrant
			if (topQuadrant)
				index = 1;
			//Btm right quadrant
			else if (bottomQuadrant)
				index = 3;
		}
 
		// Object cannot completely fit within any of the quadrants
		return index;
	}

	/*
	*	Insert the object into the QuadTree
	*	If the number of objects in the parent node exceeds the capacity, it will split and add all objects to the corresponding child nodes
	*	However, if parent node has been split but number of objects still exceeds the limit (Cannot fit into child nodes),
	*	-Objects will be left in the parent node as they are
	*/
	void QuadTree::Insert(GameObject* obj) 
	{
		//If parent node has child nodes, i.e. has been split already
		if (m_Nodes) 
		{
			//Determine which child node the object belongs to
			int index = GetIndex(obj->m_Position, obj->GetBoundingSize());
 
			//If the object can fit completely into any child node
			if (index != -1)
			{
				//Add the object to the child node
				m_Nodes[index].Insert(obj);
 
				//Return out of the function as the object has been added already
				return;
			}
			//endif object can fit completely in child node
		}
		//endif node has child nodes
 
		//Else if parent node has not been split yet
		//or if the object cannot fit completely into any child nodes

		//Add the object to the parent node
		m_Objects.push_back(obj);
 
		//If the number of objects in this node has exceeded the maximum capacity
		if (m_Objects.size() > QUADTREE_MAX_OBJECTS) 
		{
			//If the parent node has not been split already
			if (m_Nodes == NULL)
				//Split the parent node
				Split(); 
 
			//Loop through the parent node's list of objects
			Node<GameObject*>* it = m_Objects.front_node();
			for (int i = 0; i < m_Objects.size(); ++ i)
			{
				//Determine which child node the object belongs to
				int index = GetIndex(it->data->m_Position, it->data->GetBoundingSize());

				//If the object can fit completely into a child node
				if (index != -1) 
				{
					//Add the object to the child node
					m_Nodes[index].Insert(it->data);

					//Delete the object from the parent node and continue to the next object
					it = it->next;
					m_Objects.erase(i--);
					continue;
				}
				//endif object can fit completely into any child node
				
				//Else if object cannot fit completely into any child nodes
				//Leave it in the parent node and continue to the next object
				it = it->next;
			}
			//end loop parent node objects
		}
		//endif number of objects over the limit
	}

	/*
	*	Get all objects that could collide with the given object
	*	Returns the number of objects that could possibly collide
	*	First parameters returns either the list of objects that may collide or the node of the last object that may collide
	*/
	int QuadTree::Retrieve(LinkedList<GameObject*>* returnObjects, VECTOR2 rect_pos, VECTOR2 rect_size) 
	{
		//Determine which child node the object belongs to
		int index = GetIndex(rect_pos, rect_size);
		
		//If the parent node has child nodes
		if (m_Nodes)
		{
			//If the object cannot fit completely into any child node
			if (index == -1)
				//Check through all child nodes
				for (int i = 0; i < 4; ++ i)
					//Recursive "Retrieve" call to get objects in the child nodes
					m_Nodes[i].Retrieve(returnObjects, rect_pos, rect_size);
			//Else if the object can fit completely into a child node
			else
				//Check through that particular child node only
				m_Nodes[index].Retrieve(returnObjects, rect_pos, rect_size);
		}
		//endif has child nodes
 
		//Add all the objects in the parent node to the list
		Node<GameObject*>* it = m_Objects.front_node();
		for (int i = 0; i < m_Objects.size(); ++ i)
		{
			returnObjects->push_back(it->data);
			it = it->next;
		}

		return returnObjects->size();
	}

	int QuadTree::Retrieve(Node<GameObject*>* *collisionNodeIt, VECTOR2 rect_pos, VECTOR2 rect_size, int list_size) 
	{
		//Determine which child node the object belongs to
		int index = GetIndex(rect_pos, rect_size);
		
		//If the parent node has child nodes
		if (m_Nodes)
		{
			//If the object cannot fit completely into any child node
			if (index == -1)
				//Check through all child nodes
				for (int i = 0; i < 4; ++ i)
					//Recursive "Retrieve" call to get objects in the child nodes
					list_size = m_Nodes[i].Retrieve(collisionNodeIt, rect_pos, rect_size, list_size);
			//Else if the object can fit completely into a child node
			else
				//Check through that particular child node only
				list_size = m_Nodes[index].Retrieve(collisionNodeIt, rect_pos, rect_size, list_size);
		}
		//endif has child nodes
 
		//Add all the objects in the parent node to the list
		Node<GameObject*>* it = m_Objects.front_node();
		for (int i = 0; i < m_Objects.size(); ++ i)
		{
			(*collisionNodeIt)->data = it->data;
			(*collisionNodeIt) = (*collisionNodeIt)->next;
			it = it->next;
			++list_size;
		}

		return list_size;
	}

#ifdef QUADTREE_DEBUG_DRAW
	/*
	*	Render the partitioning squares
	*/
	void QuadTree::DrawUpper()
	{
		GameObject::Render(40.f, 0.f);
		if (m_Nodes)
		{
			for (int i = 0; i < 4; ++ i)
				m_Nodes[i].DrawUpper();
		}
	}

	void QuadTree::DrawLower()
	{
		GameObject::Render(0.f, -240.f);
		if (m_Nodes)
		{
			for (int i = 0; i < 4; ++ i)
				m_Nodes[i].DrawLower();
		}
	}
#endif
}