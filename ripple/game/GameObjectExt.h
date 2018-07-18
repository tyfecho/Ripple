/*
* GameObjectExt.h
*	Additional classes derived from the GameObject class, which require functionalities that are not provided in the base class
*	-Player Class
*	-Lily Pad Class
*	-Frog Class
*	-Log Pivot Class
*	-Piranha Class
*	-Whirlpool Class
*	-Splash Class
*/

#pragma once

/***************************************************
*	File includes
***************************************************/

#include "GameObject.h"

/***************************************************
*	Macro Definitions
***************************************************/

#define LEVEL_TILE_SIZE		32

#define LILYPAD_SIZE		LEVEL_TILE_SIZE
#define	LILYPAD_ROT_SPEED	60.f

#define FROG_MOVESPEED		150.f
#define FROG_DOWN_TIME		0.f//0.5f

#define LOG_LENGTH			LEVEL_TILE_SIZE*3
#define LOG_WIDTH			LEVEL_TILE_SIZE

#define PIRANHA_SIZE		30.f
#define PIRANHA_MOVESPEED	100.f

#define WPOOL_SIZE			LEVEL_TILE_SIZE*1.5f
#define WPOOL_ROT_SPEED		70.f

#define SPLASH_SIZE			LEVEL_TILE_SIZE

/***************************************************
*	Game namespace
***************************************************/

namespace Game
{
	/*
	*	Forward declarations
	*/
	class Frog;
	class LilyPad;

	/*
	*	Texture enumerator
	*		Used for texture ID
	*/
	enum ETEXTURE
	{
		ETEXTURE_WATER,
		ETEXTURE_RIPPLE,
		ETEXTURE_WATERBED,
		ETEXTURE_PLAYER,
		ETEXTURE_GOAL,
		ETEXTURE_TILEMAP,
		ETEXTURE_LOG,
		ETEXTURE_FROG,
		ETEXTURE_GAMEELEMENTS,
		ETEXTURE_PIRANHA,
		ETEXTURE_UI,
		ETEXTURE_NUMBER,
		ETEXTURE_FONT,
		ETEXTURE_BOARD,
		ETEXTURE_BACK,
		ETEXTURE_TUTORIAL,
		ETEXTURE_DEBUG_SQUARE,
		ETEXTURE_COUNT
	};

	/*
	*	Player Class
	*/
	class Player : public GameObject
	{
		private:
			LilyPad*		m_LilyPadPtr;				// Pointer to the lilypad that the player is occupying, if any
		public:
			// Constructor
			Player();
			// Destructor
			~Player();

			// Get the pointer to the lilypad that the player is occupying, if any
			LilyPad* GetLilyPad() {return m_LilyPadPtr;}

			// Set the pointer to the lilypad that the player is occupying, if any
			void SetLilyPad(LilyPad* lilyPad) {m_LilyPadPtr = lilyPad;}
	};

	/*
	*	Lily Pad Class
	*/
	class LilyPad : public GameObject
	{
		private:
			bool			m_Occupied;					// Boolean of whether the lily pad is occupied by another object
			Node<Frog*>*	m_FrogNodePtr;				// Pointer to the node of the frog occupying the lily pad, if any
			GameObject*		m_PlayerPtr;				// Pointer to the player occupying the lily pad, if any
			float			m_Disturbance;				// Amount of disturbance caused by ripples hitting the lily pad - Decays over time
														// -Added so that it is easier for a frog to re-activate itself with the ripples it has generated
														//  when it is jumping to another lily pad, since ripples would be deleted when it contacts a lily pad
		public:
			// Constructor
			LilyPad();
			// Destructor
			~LilyPad();

			// Returns true if an object (frog/flower/player) is occupying the lily pad
			bool IsOccupied() {return m_Occupied;}
			// Get the pointer to the frog occupying the lily pad, if any
			Frog* GetFrog() {return (m_FrogNodePtr) ? m_FrogNodePtr->data : NULL;}
			// Get the pointer to the node of the frog occupying the lily pad, if any
			Node<Frog*>* GetFrogNode() {return m_FrogNodePtr;}
			// Get the pointer to the player occupying the lily pad, if any
			GameObject* GetPlayer() {return m_PlayerPtr;}
			// Get the disturbance of the lilypad caused by ripples
			float GetDisturbance() {return m_Disturbance;}

			// Set the boolean of whether the lily pad is occupied
			void SetOccupied(bool state) {m_Occupied = state;}
			// Set the pointer to the node of the frog occupying the lily pad, if any
			void SetFrogNode(Node<Frog*>* frogNode) {m_FrogNodePtr = frogNode;}
			// Set the pointer to the player occupying the lily pad, if any
			void SetPlayer(GameObject* player) {m_PlayerPtr = player;}
			// Add distrubance by the ripples to the lily pad
			void AddDisturbance(float ripple_strength) {m_Disturbance += ripple_strength;}

			// Overloaded update function that rotates the lily pad and player if a player is occupying it
			void Update (float deltaTime);
	};

	/*
	*	Frog Class
	*/
	class Frog : public GameObject
	{
		private:
			bool					m_Active;			// Boolean of whether the frog is currently jumping and generating ripples
			float					m_DownTime;			// Amount of time before the frog can be activated by ripples
			Node<Frog*>*			m_ParentNode;		// Pointer to the parent node of the frog
			Node<LilyPad*>*			m_LilyPadNodePtr;	// Pointer to the node of the lily pad that the frog is on
			static LinkedList							// Static pointer to the list of lily pads
					<LilyPad*>*		m_LilyPadListPtr;	// -Set the pointer when entering the level state the first time

			// Get the node of the next available lily pad that is not occupied, if any
			Node<LilyPad*>*			GetNextLilyPadNode();
		public:
			// Constructor
			Frog(Node<LilyPad*>* lilyPadNode);
			// Destructor
			~Frog();

			// Returns true if the frog has reached the lily pad
			bool IsAtTarget(void) {return (m_Position == m_LilyPadNodePtr->data->m_Position);}
			// Returns true if the down time is 0
			bool IsReady(void) {return (m_DownTime <= 0.f) ? true : false;}
			// Get the pointer to the lily pad that the frog is standing on
			LilyPad* GetLilyPad() {return (m_LilyPadNodePtr) ? m_LilyPadNodePtr->data : NULL;}
			// Get the pointer to node of the lily pad that the frog is standing on
			Node<LilyPad*>* GetLilyPadNode() {return m_LilyPadNodePtr;}
	
			// Set the pointer to the list of lily pads - Do this only once in the level state initialization
			static void SetLilyPadListPtr(LinkedList<LilyPad*>* lilyPadList) {m_LilyPadListPtr = lilyPadList;}
			// Set the pointer to the parent node of the frog
			void SetParentNode(Node<Frog*>* parentNode) {m_ParentNode = parentNode;}
			// Set the pointer to the node of the lily pad that the frog is on
			void SetLilyPadNode(Node<LilyPad*>* lilyPadNode) {m_LilyPadNodePtr = lilyPadNode;}
			// Set the frog to start or stop jumping and generating ripples
			void SetActive(bool active) {m_Active = active; PlayAnimation(6.f, -1);}

			// Order the frog to stop jumping and move to the next available lily pad - Works only if the frog has already reached its target lily pad
			bool MoveToNext();
			// Order the frog to look at the next lily pad it will jump to - Re-run for all other frogs whenever MoveToNext is successful
			void LookToNext();

			// Overloaded update function which handles the periodical ripple generation
			// Returns true if the frog has completed one cycle of jump (Signal to generate ripples)
			bool Update(float deltaTime);
	};

	/*
	*	Log Pivot Class
	*/
	class LogPivot : public GameObject
	{
		private:
			GameObject* m_LogPtr;						// Pointer to the log that it is a pivot of
		public:
			// Constructor
			LogPivot(GameObject* logPtr);
			// Destructor
			~LogPivot();

			// Get the pointer to the log that it is a pivot of
			GameObject* GetLog() {return m_LogPtr;}
	};

	/*
	*	Piranha Class
	*/
	class Piranha : public GameObject
	{
		private:
			LinkedList<VECTOR2> m_Waypoint;				// List of points to move to. When piranha reaches last point in the list, go back to the first point
			Node<VECTOR2>* m_CurrentWaypointNode;		// Pointer to the node of the point that it last reached
		public:
			// Constructor
			Piranha();
			// Destructor
			~Piranha();

			// Initialize the piranha's position and velocity values and also turn the waypoint list into a circular linked list
			void Initialize();

			// Add a new waypoint to the list - Use both functions together! Separated in 2 functions due to how the level data file is loaded
			// Always add the X-coordinate of the waypoint before the Y as the former function pushes back a new vector, while the latter edits the y-value only
			void AddWaypointX(float x) {m_Waypoint.push_back(VECTOR2(x, 0.f));}
			void AddWaypointY(float y) {m_Waypoint.back_node()->data.y = y;}

			// Overloaded update function which handles the piranha's waypoint movements and smooths it
			void Update(float deltaTime);
	};

	/*
	*	Whirlpool Class
	*/
	class Whirlpool : public GameObject
	{
		private:
			bool m_Clockwise;							// Direction in which the whirlpool is rotating (CW or CCW)
		public:
			static VECTOR2 m_TotalForce;				// Sum of all the forces by the whirlpool acting on the player. Reset this value every frame

			// Constructor
			Whirlpool();
			// Destructor
			~Whirlpool();

			// Set the direction which the whirlpool rotates
			void SetClockwise(bool cw) {m_Clockwise = cw; m_FlipHorizontal = !cw;}

			// Returns true if the whirlpool is rotating clockwise
			bool IsClockwise(void) {return m_Clockwise;}

			// Overloaded update function which rotates the whirlpool
			void Update(float deltaTime);
	};

	/*
	*	Splash Class
	*/
	class Splash : public GameObject
	{
		private:
			bool m_Startup;
		public:
			// Constructor
			Splash(float x, float y);
			// Destructor
			~Splash();

			//
			void Update(float deltaTime);
	};
};