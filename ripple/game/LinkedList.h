/*
* LinkedList.h
*	Used as a replacement when the stl vector class is not available or supported
*	Doubly-linked list which can push or pop nodes at the front or back and traverse either direction
*	Linked list can access either the nodes or the data itself
*	To optimize loops through the linked list, use node iterators along with ->next rather than the get() function
*	Can be turned into a circular list by linking the first and last node manually - Make sure to do this everytime a new node is added!
*/

#pragma once

#define NULL 0

/*
*	Node Struct
*/
template <class T>
struct Node
{
	T data;		// Data of the current node
	Node* next;	// Pointer to the next node in the list
	Node* prev;	// Pointer to the prev node in the list

	// Constructor
	Node()
	{
	}

	Node(T inValue)
	{
		data = inValue;
	}
};

/*
*	Linked List Class
*/
template <class T>
class LinkedList
{
private:
    Node<T>* 	m_First;	// First node in the list
	Node<T>* 	m_Last;		// Last node in the list
	int			m_Size;		// Number of nodes in the list
public:
	// Constructor
				LinkedList		()					{m_First = NULL; m_Last = NULL; m_Size = 0;}
	// Destructor
				~LinkedList		()					{clear();}
	// Add a new node to the front of the list
	void		push_front		();
	void		push_front		(T value);
	// Add a new node to the back of the list
	void 		push_back		();
	void 		push_back		(T value);
	// Remove the first node
	void		pop_front		();
	// Remove the last node
	void 		pop_back		();
	// Remove the node and data at the specified index
	void		erase			(int index);
	// Remove all nodes and data in the list
	void		clear			();
	// Get the node and data at the specified index
	Node<T>*	get_node		(int index);
	// Get the first node of the list
	Node<T>*	front_node		()					{return m_First;}
	// Get the last node of the list
	Node<T>*	back_node		()					{return m_Last;}
	// Get the data at the specified index
	T			get				(int index)			{return get_node(index)->data;}
	// Get the first data of the list
	T			front			()					{return m_First->data;}
	// Get the last data of the list
	T			back			()					{return m_Last->data;}
	// Get whether the list is empty
	bool		empty			()					{return (m_Size == 0) ? true : false;}
	// Get the size of the list
	int			size			()					{return m_Size;}
	// Resizes the linked list - use to pre-allocate memory
	void		resize			(int size);
	// Replace the data at the specified index
	void		set				(int index, T data);	
};

/*
*	Add a new element to the front of the list
*/
template <class T>
void LinkedList<T>::push_front ()
{
	//If there is a maximum capacity set and the current number of nodes has reached or exceeded this capacity
	if (m_Capacity >= 0 && m_Size >= m_Capacity)
		//Do not add the new node
		return;

	Node<T>* newNode = new Node<T>();

	//If the list is empty
	if (m_First == NULL)
		//Set the last node to the new node
		m_Last = newNode;
	//Else if the list is not empty
	else
	{
		//Forward link of new node to first node
		newNode->next = m_First;

		//Backward link of first node to the new node
		m_First->prev = newNode;
	}

	//Set first node to the new node
	m_First = newNode;

	//Increase the size counter
	++m_Size;
}

template <class T>
void LinkedList<T>::push_front (T value)
{
	//If there is a maximum capacity set and the current number of nodes has reached or exceeded this capacity
	if (m_Capacity >= 0 && m_Size >= m_Capacity)
		//Do not add the new node
		return;

	Node<T>* newNode = new Node<T>(value);

	//If the list is empty
	if (m_First == NULL)
		//Set the last node to the new node
		m_Last = newNode;
	//Else if the list is not empty
	else
	{
		//Forward link of new node to first node
		newNode->next = m_First;

		//Backward link of first node to the new node
		m_First->prev = newNode;
	}

	//Set first node to the new node
	m_First = newNode;

	//Increase the size counter
	++m_Size;
}

/*
*	Add a new element to the back of the list
*/
template <class T>
void LinkedList<T>::push_back ()
{
	Node<T>* newNode = new Node<T>();

	//If the list is empty
	if (m_First == NULL)
		//Set the first node to the new node
		m_First = newNode;
	//Else if the list is not empty
	else
	{
		//Backward link of new node to last node
		newNode->prev = m_Last;

		//Forward link of last node to the new node
		m_Last->next = newNode;
	}

	//Set last node to the new node
	m_Last = newNode;

	//Increase the size counter
	++m_Size;
}

template <class T>
void LinkedList<T>::push_back (T value)
{
	Node<T>* newNode = new Node<T>(value);

	//If the list is empty
	if (m_First == NULL)
		//Set the first node to the new node
		m_First = newNode;
	//Else if the list is not empty
	else
	{
		//Backward link of new node to last node
		newNode->prev = m_Last;

		//Forward link of last node to the new node
		m_Last->next = newNode;
	}

	//Set last node to the new node
	m_Last = newNode;

	//Increase the size counter
	++m_Size;
}

/*
*	Remove the first node
*/
template <class T>
void LinkedList<T>::pop_front (void)
{
	//If the list is empty
	if (m_First == NULL)
		//Exit out of the function
		return;

	//Else if the list has only one element
	if (m_First == m_Last)
	{
		//Delete the first node (Which will also delete the last node since it's the same pointer)
		delete m_First;

		//Set the first and last node to null
		m_First = NULL;
		m_Last = NULL;
	}
	//Else if the list has more than one element
	else
	{
		//Set the first node to the 2nd node
		m_First = m_First->next;

		//Delete the original first node
		delete m_First->prev;
		m_First->prev = NULL;
	}

	//Decrease the size counter
	--m_Size;
}

/*
*	Remove the last node
*/
template <class T>
void LinkedList<T>::pop_back (void)
{
	//If the list is empty
	if (m_First == NULL)
		//Exit out of the function
		return;

	//Else if the list has only one element
	if (m_First == m_Last)
	{
		//Delete the first node (Which will also delete the last node since it's the same pointer)
		delete m_First;

		//Set the first and last node to null
		m_First = NULL;
		m_Last = NULL;
	}
	//Else if the list has more than one element
	else
	{
		//Set the last node to the 2nd last node
		m_Last = m_Last->prev;

		//Delete the original last node
		delete m_Last->next;
		m_Last->next = NULL;
	}

	//Decrease the size counter
	--m_Size;
}

/*
*	Remove the node at the specified index
*/
template <class T>
void LinkedList<T>::erase (int index)
{
	//If the index is invalid
	if (index < 0 || index >= m_Size)
		//Exit the function
		return;

	//If deleting the first node
	if (index == 0)
	{
		//Call the pop_front function
		pop_front();
	}
	//Else if deleting last node
	else if (index == m_Size - 1)
	{
		//Call the pop_back function
		pop_back();
	}
	//Else if deleting any nodes in between the first and last
	else
	{
		//Get the node at the specified index
		Node<T>* node = get_node(index);

		//Forward link from the index-1 to index+1
		node->prev->next = node->next;

		//Backward link from the index+1 to index-1
		node->next->prev = node->prev;

		//Delete the node
		delete node;

		//Decrease the size counter
		--m_Size;
	}
}

/*
*	Remove all nodes in the list
*/
template <class T>
void LinkedList<T>::clear (void)
{
	//Loop until the list is empty
	while (m_First != NULL)
		//Pop the last element
		pop_back();
}

/*
*	Get the node at the specified index
*/
template <class T>
Node<T>* LinkedList<T>::get_node (int index)
{
	//If the index is invalid
	if (index < 0 || index >= m_Size)
		//Return a null pointer
		return NULL;

	//Else if getting first element
	if (index == 0)
		//Return the pointer to the first element
		return m_First;

	//Else return the pointer to the desired element
	int counter = index;
	Node<T>* node = m_First;

	while (counter-- > 0)
		node = node->next;
	
	return node;
}

/*
*	Resizes the linked list - use to pre-allocate memory
*/
template <class T>
void LinkedList<T>::resize (int size)
{
	//If an invalid size is specified
	if (size < 0)
		//Return out of the function
		return;

	//If the new size is more than the current size of the list
	if (size > m_Size)
	{
		//Add nodes until the size of the list has reached the new size
		while (size > m_Size)
			push_back();
	}
	//Else if the new size is smaller than the current size of the list
	else if (size < m_Size)
	{
		//Delete nodes from the back until the size of the list falls within the new size
		while (size < m_Size)
			pop_back();
	}
}

/*
*	Replace the data at the specified index
*/
template <class T>
void LinkedList<T>::set (int index, T data)
{
	//If the index is invalid
	if (index < 0 || index >= m_Size)
		//Return out of the function
		return;

	//Get the node at the specified index
	Node<T>* node = get_node(index);

	//Replace the data of the node
	node->data = data;
}