/*
* Water.h
*	Water and ripple information, put in a separate class/file so that it can also be implemented in other parts of the game (e.g. main menu) if desired
*	-Collision Group Enumerator
*		The order in which the collision checks are performed depends on the order of the enums
*	-Level file data emunerator
*		Data group that is being loaded
*		The enums after COUNT are used to set object ID only, which are used to determine what type of object it is
*	-Level tilemap enumerator
*		Type of wall tile depending on the ID
*	-Ripple Struct
*		Stores the previous and current position values (For line-collision), direction of movement and the current strength
*		Ripples are created as multiple lines in different directions
*		Increasing the number of slices will make the ripple appear more accurate, but will be more expensive for calculation
*		Velocity values passed to the player may have to be tweaked due to the increased number of ripple slices
*		Decay of ripple strength has both an exponential and linear factor. Linear factor has to have a minimum value (Recommended at least 0.1) for it to be properly deleted
*	-Water Class
*		Handles the drawing and updating of the water texture and the creation and updating of ripples
*		Ripples are drawn by updating the water texture overlay's alpha channel depending on its current position and strength
*		Game object lists from the game level must be passed to the water in order for the ripple to detect them using line-collisions
*/

#pragma once

/***************************************************
*	File includes
***************************************************/

#include "../NW_Lib/Utility/NW_Lib_HardwareDefine.h"
#include "LinkedList.h"
#include "GameObjectExt.h"
#include "SpatialPartition.h"

/***************************************************
*	Macro Definitions
***************************************************/

#define SPATIAL_PARTITIONING
#define SPECULATIVE_CONTACT

#define WATER_TILE_SLICE	4											// Times to slice a level tile to form water tiles
#define WATER_TILE_SIZE		((float)LEVEL_TILE_SIZE / WATER_TILE_SLICE)	// Size of a water tile

#define LEVEL_TILE_COL		(LOWER_SCREEN_WIDTH / LEVEL_TILE_SIZE)		// No. of level tile columns (x-axis)
#define LEVEL_TILE_ROW		(TOTAL_SCREEN_HEIGHT / LEVEL_TILE_SIZE)		// No. of level tile rows (y-axis)
#define WATER_TILE_COL		(LEVEL_TILE_COL * WATER_TILE_SLICE)			// No. of water tile columns (x-axis)
#define WATER_TILE_ROW		(LEVEL_TILE_ROW * WATER_TILE_SLICE)			// No. of water tile rows (y-axis)

#define RIPPLE_SPEED		90.f										// Speed of the ripple
#define RIPPLE_DECAY_LINEAR	18.f										// Linear decay of ripple strength
#define RIPPLE_DECAY_EXP	1.5f										// Exponential (multiplicative) decay of ripple strength

#define RIPPLE_SLICE		60											// Number of slices in a circle. Higher values will create more accurate ripples but requires more computations
#define RIPPLE_DEG_STEP		(360.f / RIPPLE_SLICE)						// Angle in degrees between each slices

/***************************************************
*	Game namespace
***************************************************/

namespace Game
{
	/*
	*	Collision group enumerator
	*		The order in which the collision checks are performed depends on the order of the enums
	*/
	enum ECOLLISIONGROUP
	{
		ECOLLISIONGROUP_START,
		ECOLLISIONGROUP_LOGPIVOT = ECOLLISIONGROUP_START,
		ECOLLISIONGROUP_LILYPAD,
		ECOLLISIONGROUP_SQUAREWALL,
		ECOLLISIONGROUP_ANGLEDWALL,
		ECOLLISIONGROUP_LOG,
		ECOLLISIONGROUP_COUNT
	};

	/*
	*	Level file data emunerator
	*		Data group that is being loaded
	*		The enums after COUNT are used to set object ID only, which are used to determine what type of object it is
	*/
	enum ELEVELDATA
	{
		ELEVELDATA_WALL,
		ELEVELDATA_DROPLET,
		ELEVELDATA_PLAYER,
		ELEVELDATA_GOAL,
		ELEVELDATA_LILYPAD,
		ELEVELDATA_LOG,
		ELEVELDATA_PIRANHA,
		ELEVELDATA_WPOOL,
		ELEVELDATA_COUNT,
		ELEVELDATA_NONE = ELEVELDATA_COUNT,
		ELEVELDATA_FROG,
		ELEVELDATA_LOGPIVOT
	};

	/*
	*	Level tilemap enumerator
	*		Type of wall tile depending on the ID
	*/
	enum ELEVELTILE
	{
		ELEVELTILE_WATER,
		ELEVELTILE_SQRWALL,
		ELEVELTILE_TRIWALL0,
		ELEVELTILE_TRIWALL1,
		ELEVELTILE_TRIWALL2,
		ELEVELTILE_TRIWALL3,
		ELEVELTILE_COUNT
	};

	/*
	*	Ripple Struct
	*		Stores the previous and current position values (For line-collision), direction of movement and the current strength
	*		Ripples are created as multiple lines in different directions
	*		Increasing the number of slices will make the ripple appear more accurate, but will be more expensive for calculation
	*		Velocity values passed to the player may have to be tweaked due to the increased number of ripple slices
	*		Decay of ripple strength has both an exponential and linear factor. Linear factor has to have a minimum value (Recommended at least 0.1) for it to be properly deleted
	*/
	struct Ripple
	{
		VECTOR2 prevPos;	// Position of the ripple in the previous frame
		VECTOR2 currPos;	// Position of the ripple in the current frame
		VECTOR2 dir;		// Direction in which the ripple is moving
		float strength;		// Strength of the ripple which decays over time. Delete from list when it reaches 0
		float multiplier;	// 
		LilyPad* lilyPad;	// Pointer to a lily pad which it was created from
							// -Used only for ripples created by frogs so that the ripples are not automatically deleted due to collision with lily pad

		// Constructor
		Ripple (VECTOR2 _pos, VECTOR2 _dir, float _strength, LilyPad* _lilyPad = NULL)
		{
			prevPos = _pos;
			currPos = _pos;
			dir = _dir;
			strength = _strength;
			multiplier = 1.f;
			lilyPad = _lilyPad;
		}
	};

	/*
	*	Water Class
	*		Handles the drawing and updating of the water texture and the creation and updating of ripples
	*		Ripples are drawn by updating the water texture overlay's alpha channel depending on its current position and strength
	*		Game object lists from the game level must be passed to the water in order for the ripple to detect them using line-collisions
	*/
	class Water
	{
		private:
#ifdef _DirectX
			int							m_RippleTextureID;		// ID of the ripple texture - Set separated as texture enums may differ for different states
#endif
			int							m_RippleTextureWidth;	// Width of the water texture
			int							m_RippleTextureHeight;	// Height of the water texture

			LinkedList<Ripple*>			m_Ripple;				// List of all the ripples that have yet to dissipate
			float**						m_WaterLevel;			// 2D array which stores the sum of the ripple strength in each water tile of the map
			bool**						m_ShallowWater;			// 2D array which stores whether a level tile is a shallow water tile
#ifdef _3DS
			int**						m_RippleTextureIndex;	// Pre-calculated index of the texture index
			--::---::---::										// Texture data of the ripple
			------------------			m_RippleTextureData;	// 
			--::---::---::										// Texture data of the water bed
			------------------			m_WaterBedTextureData;	// 
#endif
			GameObject*					m_WaterBed;				// Water bed texture to show shallow water - Update the alpha values dynamically on initialization
			GameObject*					m_WaterBody;			// Water body texture - Draw below everything else
			GameObject*					m_WaterRipple;			// Water ripple texture - Draw above the water bed and body

			GameObject*					m_Player;				// Pointer to player - Pass in the address using the Initialize function
#ifdef SPATIAL_PARTITIONING
			QuadTree*					m_QuadTree;				// Spatial partitioning
			LinkedList<GameObject*>*	m_CollisionIsland;		//
#else
			LinkedList<GameObject*>*	m_SquareWall;			// Pointer to the game objects - Pass in the addresses using the Initialize function
			LinkedList<GameObject*>*	m_AngledWall;			//
			LinkedList<LilyPad*>*		m_LilyPad;				//
			LinkedList<GameObject*>*	m_Log;					//
#endif

			// Check for ripple's collision with other game objects and process a response according to the collision group
			bool						CheckRippleCollision(Ripple* ripple);

			// Get the next power of the base number nearest to the compared number
			int							GetNextPower(int baseNum, int comparedNum);
		public:
			// コンストラクタ
										Water(int waterTexID, int rippleTexID);
			// デストラクタ
										~Water();
			// Initialize pointers to game objects
			void						InitPointers(GameObject* player
#ifdef SPATIAL_PARTITIONING
													,QuadTree* quadTree
													,LinkedList<GameObject*>* collisionIsland
#else
													,LinkedList<GameObject*>* squareWall
													,LinkedList<GameObject*>* angledWall
													,LinkedList<LilyPad*>* lilyPad
													,LinkedList<GameObject*>* log
#endif
													);

			// Returns true if the x and y position is on a shallow water tile (Note that this function takes in position values!)
			bool						IsShallowWater(int x, int y) {return m_ShallowWater[x/LEVEL_TILE_SIZE][y/LEVEL_TILE_SIZE];}
			// Sets a water tile to whether it is shallow water (Note that this function takes in tile index!)
			void						SetShallowWater(int tile_x, int tile_y, bool shallow = true) {m_ShallowWater[tile_x][tile_y] = shallow;}
			// Initialize the water bed texture by changing the alpha values of the texture depending on whether the tile is shallow water
			void						InitWaterBedTexture();
			
			// Clear all the ripples
			void						ClearRipple();
			// Create a ripple at the specified position
			void						CreateRipple(int x, int y, float strength, LilyPad* lilyPad = NULL);

			// Update the water ripple
			void						UpdateWaterRipple(float deltaTime);
			// Update the water texture
			void						UpdateWaterTexture();

			// 上画面描画
			virtual void				DrawUpperWater(bool SwapScreen = false);
			virtual void				DrawUpperRipple(bool SwapScreen = false);
			// 下画面描画
			virtual void				DrawLowerWater(bool SwapScreen = false);
			virtual void				DrawLowerRipple(bool SwapScreen = false);
	};
}