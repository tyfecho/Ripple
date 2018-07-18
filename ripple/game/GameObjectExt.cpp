/*
* GameObjectExt.cpp
*/

#include "GameObjectExt.h"

namespace Game
{
	/***************************************************
	*	Player Class
	****************************************************/

	/*
	*	Constructor
	*/
	Player::Player() : GameObject()
	{
		m_LilyPadPtr = NULL;
	}

	/*
	*	Destructor
	*/
	Player::~Player()
	{
		m_LilyPadPtr = NULL;
	}
	
	/***************************************************
	*	Lily Pad Class
	****************************************************/

	/*
	*	Constructor
	*/
	LilyPad::LilyPad() : GameObject()
	{
		m_Occupied = false;
		m_FrogNodePtr = NULL;
		m_PlayerPtr = NULL;
		m_Disturbance = 0.f;

		//Randomize the direction so there is some variety in appearances of the lily pads
		m_Direction = Vec2RotateDeg(m_Direction, (Random() % 36) * 10);
	}

	/*
	*	Destructor
	*/
	LilyPad::~LilyPad() 
	{
		m_FrogNodePtr = NULL;
		m_PlayerPtr = NULL;
	}

	/*
	*	Overloaded update function that rotates the lily pad and player if a player is occupying it
	*/
	void LilyPad::Update(float deltaTime)
	{
		GameObject::Update(deltaTime);

		//If a player is occupying the lily pad
		if (m_PlayerPtr)
		{
			//Rotate both the lily pad and the player
			m_Direction = Vec2RotateDeg(m_Direction, LILYPAD_ROT_SPEED * deltaTime);
			m_PlayerPtr->m_Direction = Vec2RotateDeg(m_PlayerPtr->m_Direction, LILYPAD_ROT_SPEED * deltaTime);
		}
		//endif player occupying lily pad

		//If the disturbance in the lily pad has not yet dissipated
		if (m_Disturbance > 0.f)
		{
			//Apply linear decay
			m_Disturbance -= m_Disturbance * 3.f * deltaTime;

			//Apply exponential decay
			m_Disturbance -= 15.f * deltaTime;

			//Clamp the disturbance so that it doesn't shoot up to very high values when there are many ripples
			m_Disturbance = clamp(m_Disturbance, 0.f, 225.f);
		}
		//endif disturbance in lily pad
	}

	/***************************************************
	*	Frog Class
	****************************************************/

	LinkedList<LilyPad*>* Frog::m_LilyPadListPtr = NULL;

	/*
	*	Constructor
	*/
	Frog::Frog(Node<LilyPad*>* lilyPadNode) : GameObject()
	{
		m_Active = false;
		m_ParentNode = NULL;
		m_LilyPadNodePtr = lilyPadNode;
		m_Position = lilyPadNode->data->m_Position;
	}

	/*
	*	Destructor
	*/
	Frog::~Frog() 
	{
		m_ParentNode = NULL;
		m_LilyPadNodePtr = NULL;
	}

	/*
	*	Get the node of the next available lily pad that is not occupied, if any
	*/
	Node<LilyPad*>* Frog::GetNextLilyPadNode()
	{
		//Loop through the list of lily pads starting from the current one that the frog is on
		Node<LilyPad*> *it = m_LilyPadNodePtr;
		for (int j = 0; j < m_LilyPadListPtr->size()-1; ++ j)
		{
			it = it->next;

			//If the next lily pad is not occupied
			if (!it->data->IsOccupied())
				return it;
		}
		//end loop lily pad

		//Else return NULL if no available lily pads were found
		return NULL;
	}

	/*
	*	Order the frog to stop jumping and move to the next available lily pad - Works only if the frog has already reached its target lily pad
	*/
	bool Frog::MoveToNext()
	{
		//If the frog is still moving towards its current target
		if (!IsAtTarget())
			//Exit the function so that it doesn't move to the next lily pad
			return false;

		//Get the next available lily pad
		Node<LilyPad*>* nextNode = GetNextLilyPadNode();

		//If a lily pad is available
		if (nextNode)
		{
			//Un-occupy the current lily pad
			m_LilyPadNodePtr->data->SetFrogNode(NULL);
			m_LilyPadNodePtr->data->SetOccupied(false);

			//Set the frog to occupy the next lily pad
			nextNode->data->SetOccupied(true);
			nextNode->data->SetFrogNode(m_ParentNode);
			m_LilyPadNodePtr = nextNode;

			//Set the frog to move towards the next lily pad
			VECTOR2 vec = nextNode->data->m_Position - m_Position;
			m_Direction = Vec2Normalize(vec);
			m_Velocity = m_Direction * FROG_MOVESPEED;

			//Deactivate the frog's ripple generation and apply a down time
			SetActive(false);
			m_DownTime = FROG_DOWN_TIME;

			//Play the jump animation, with a speed depending on the distance so that it completes one cycle through the entire movement
			PlayAnimation(FROG_MOVESPEED / Vec2Length(vec) * 6.f);

			return true;
		}
		//endif lily pad available

		return false;
	}

	/*
	*	Order the frog to look at the next lily pad it will jump to - Re-run for all other frogs whenever MoveToNext is successful
	*/
	void Frog::LookToNext()
	{
		//Get the next available lily pad
		Node<LilyPad*>* nextNode = GetNextLilyPadNode();

		//If a lily pad is available
		if (nextNode)
			m_Direction = Vec2Normalize(nextNode->data->m_Position - m_Position);
	}

	/*
	*	Overloaded update function which handles the periodical ripple generation
	*	Returns true if the frog has completed one cycle of jump (Signal to generate ripples)
	*/
	bool Frog::Update(float deltaTime)
	{
		//If there is enough disturbance to the lily pad
		if (m_LilyPadNodePtr->data->GetDisturbance() >= 100.f)
		{
			//If the frog is not active already, has reached the target lily pad and down-time is over
			if (!m_Active && IsAtTarget() && IsReady())
			{
				//Activate the frog to make it periodically generate ripples
				SetActive(true);
			}
			//endif reach target and no down-time
		}
		//endif enough disturbance

		//If the frog has not yet reached the target lily pad
		if (!IsAtTarget())
		{
			//If the frog is within range to reach the lily pad this frame
			float distSq = Vec2LengthSq(m_LilyPadNodePtr->data->m_Position - m_Position);
			if (distSq <= pow(FROG_MOVESPEED*deltaTime, 2))
			{
				//Set the frog to be exactly on the lily pad and zero the velocity
				m_Position = m_LilyPadNodePtr->data->m_Position;
				m_Velocity = VECTOR2(0.f, 0.f);

				//Order the frog to look at the next lily pad available
				LookToNext();

				//Signal for a ripple to be created
				return true;
			}
			//endif frog within range to lily pad
		}
		//Else if frog has reached the target lily pad and there is still down time before it can be reactivated
		else if (m_DownTime > 0.f)
		{
			//Update the down time
			m_DownTime -= deltaTime;
		}
		//endif reached lily pad

		//Store the frame of the sprite animation before updating for this update frame
		//-For use so that we can control which frame of the sprite animation we want the signal for ripple generation to be sent
		int frameThen = m_TexInfo.currentFrame;
		
		GameObject::Update(deltaTime);

		//If the frog is active and the sprite animation frame just reached 4 in this update call
		int frameNow = (int)m_TexInfo.currentFrame;
		if (m_Active && frameThen != frameNow && frameNow == 4)
			//Signal for a ripple to be created
			return true;

		return false;
	}
	
	/***************************************************
	*	Log Pivot Class
	****************************************************/

	/*
	*	Constructor
	*/
	LogPivot::LogPivot(GameObject* logPtr) : GameObject()
	{
		m_LogPtr = logPtr;
	}

	/*
	*	Destructor
	*/
	LogPivot::~LogPivot() 
	{
		m_LogPtr = NULL;
	}

	/***************************************************
	*	Piranha Class
	****************************************************/
	
	/*
	*	Constructor
	*/
	Piranha::Piranha() : GameObject()
	{
		m_CurrentWaypointNode = NULL;
	}

	/*
	*	Destructor
	*/
	Piranha::~Piranha() 
	{
		m_Waypoint.clear();
		m_CurrentWaypointNode = NULL;
	}

	/*
	*	Initialize the piranha's position and velocity values and also turn the waypoint list into a circular linked list
	*/
	void Piranha::Initialize() 
	{
		//Set the piranha's initial position to the first waypoint
		m_CurrentWaypointNode = m_Waypoint.front_node();
		m_Position = m_CurrentWaypointNode->data;

		//If there is more than 1 waypoint
		if (m_Waypoint.size() > 1)
		{
			//Set the piranha to face and move towards the next waypoint
			m_Direction = Vec2Normalize(m_CurrentWaypointNode->next->data - m_Position);
			m_Velocity = m_Direction*PIRANHA_MOVESPEED;
		}
		//endif more than 1 waypoint

		//Convert the waypoint list to a circular linked list
		m_Waypoint.back_node()->next = m_Waypoint.front_node();
		m_Waypoint.front_node()->prev = m_Waypoint.back_node();
	}

	/*
	*	Overloaded update function which handles the piranha's waypoint movements and smooths it
	*/
	void Piranha::Update(float deltaTime)
	{
		bool isNearWaypoint = false;

		//Get the next waypoint and the current distance to it
		Node<VECTOR2>* nextWaypointNode = m_CurrentWaypointNode->next;
		float dist = Vec2Length(nextWaypointNode->data - m_Position);

		//If the piranha is within range to reach the next waypoint this frame
		if (dist <= PIRANHA_MOVESPEED*deltaTime)
		{
			//Set the piranha position to be exactly at the next waypoint
			m_Position = nextWaypointNode->data;

			//Update the waypoint nodes to the next ones
			m_CurrentWaypointNode = nextWaypointNode;
			nextWaypointNode = nextWaypointNode->next;

			//Set the piranha to face and the next waypoint
			m_Direction = Vec2Normalize(nextWaypointNode->data - m_Position);

			//Move the piranha in the new direction by the remaining distance it has yet to move
			m_Position += m_Direction*(PIRANHA_MOVESPEED*deltaTime - dist);

			//Set the velocity to 0 so that the game object update function doesn't move the piranha as it has been accounted for
			m_Velocity = VECTOR2(0.f, 0.f);
			isNearWaypoint = true;
		}
		//endif piranha within range to next waypoint

		GameObject::Update(deltaTime);

		//If the piranha was within range to the next waypoint
		if (isNearWaypoint)
			//Set the velocity of the piranha after the game object update function
			m_Velocity = m_Direction*PIRANHA_MOVESPEED;
	}

	/***************************************************
	*	Whirlpool Class
	****************************************************/
	
	VECTOR2 Whirlpool::m_TotalForce (0.f, 0.f);

	/*
	*	Constructor
	*/
	Whirlpool::Whirlpool()
	{
	}

	/*
	*	Destructor
	*/
	Whirlpool::~Whirlpool()
	{
	}

	/*
	*	Overloaded update function which rotates the whirlpool
	*/
	void Whirlpool::Update(float deltaTime)
	{
		GameObject::Update(deltaTime);

		//Rotate the whirlpool
		m_Direction = Vec2RotateDeg(m_Direction, ((m_Clockwise) ? WPOOL_ROT_SPEED : -WPOOL_ROT_SPEED) * deltaTime);
	}

	/***************************************************
	*	Splash Class
	****************************************************/

	/*
	*	Constructor
	*/
	Splash::Splash(float x, float y)
	{
		m_Position = VECTOR2(x, y);
		m_Startup = true;
		m_Size = VECTOR2(SPLASH_SIZE, SPLASH_SIZE);
		m_Scale = VECTOR2(0.f, 0.f);
		m_Alpha = 0.f;
		SetTexture(ETEXTURE_GAMEELEMENTS, 64, 64);
		SetCurrentFrame(3);
	}

	/*
	*	Destructor
	*/
	Splash::~Splash()
	{

	}

	/*
	*
	*/
	void Splash::Update(float deltaTime)
	{
		GameObject::Update(deltaTime);

		//Splash effect building up
		if (m_Startup)
		{
			//Increase scale and alpha
			m_Scale.x += 10.f * deltaTime;
			m_Scale.y += 10.f * deltaTime;
			m_Alpha += 6.f * deltaTime;
			
			//If scale and alpha reaches a cap
			if (m_Scale.x > 1.f)
			{
				//Set the scale and alpha to the exact capped values
				m_Scale.x = 1.f;
				m_Scale.y = 1.f;
				m_Alpha = 0.6f;

				//Stop the build up effect and play the fade-off effect
				m_Startup = false;
			}
		}
		//Fade off effect
		else
		{
			//Decrease the alpha
			m_Alpha -= 4.f * deltaTime;

			//If alpha has reached 0 (Has faded out)
			if (m_Alpha < 0.f)
			{
				//Set the splash to inactive - use active state as a signal to delete the object from the list
				m_Alpha = 0.f;
				m_Active = false;
			}
		}
	}
};