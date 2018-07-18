/*
* GameLevel.h
*	Gameplay scene
*	Constructor takes in a level number, which then loads the level file based on the number, up to level 999
*	Entry: 
*		Level select screen by selecting a level
*		Level clear screen by choosing retry (To try for a higher score)
*		Game level screen itself by pressing the retry button
*	Exit:
*		Level clear screen by winning the level
*		Level select screen by pressing the quit button and confirming the action
*/

#pragma once

/***************************************************
*	File includes
***************************************************/

#include "../../NW_Lib/System/NW_Lib_IScene.h"
#include "../../NW_Lib/Utility/NW_Lib_HardwareDefine.h"

#include "../Water.h"
#include "Tutorial.h"
#include <string>

/***************************************************
*	Game namespace
***************************************************/

namespace Game
{
	class MenuInGame;

	class GameLevel : public NW_Lib::IScene
	{
		private:
			int						m_Level;			// Current level number
			//Clear the current level data
			void					ClearLevel();
			//Load the current level file
			void					LoadLevel();
			//Sub-load function to process the data inside the file
			void					LoadLevelData(ELEVELDATA dataGroup, std::string LineOfText, int y);
			//Sub-loaddata function to process the tilemap
			void					LoadLevelTile(int tileID, int x, int y);

			Water*					m_Water;			// Instance of the water class
			int						m_DropsRemaining;	// Number of drops remaining

			Player*					m_Player;			// Game objects
			GameObject*				m_Goal;				//
			LinkedList<GameObject*>	m_SquareWall;		//
			LinkedList<GameObject*>	m_AngledWall;		//
			LinkedList<LilyPad*>	m_LilyPad;			//
			LinkedList<Frog*>		m_Frog;				//
			LinkedList<GameObject*>	m_Log;				//
			LinkedList<LogPivot*>	m_LogPivot;			//
			LinkedList<Piranha*>	m_Piranha;			//
			LinkedList<Whirlpool*>	m_Whirlpool;		//
			LinkedList<Splash*>		m_Splash;			//

			LinkedList<GameObject*>	m_CollisionIsland;	// List of objects that are likely to collide - has a pre-determined size equal to the number of collision objects
														// We want to keep using the same list for the collision checks without deleting nodes and clearing the list as it is very slow

#ifdef SPATIAL_PARTITIONING
			QuadTree*				m_QuadTree;			// Spatial partitioning
#endif

			bool					m_SwapScreen;		// Boolean of whether the screen is swapped
			bool					m_Paused;			// Boolean of whether the game is paused
			float					m_ElapsedTime;		// Time elapsed since the game started
			
			// Variables of Ryugo
			int						m_ElapsedFrame;		
			MenuInGame*				m_MenuObj;
			Tutorial*				theTutorial;
			float					m_DropsRemainingAlpha;

			// Sub-update function for game objects
			void					UpdateObjects(float deltaTime);
			// Check for player's collision with other game objects and process a response according to the collision group
			bool					CheckPlayerCollision(VECTOR2 prevPos, VECTOR2 newPos, float distToMove, bool resolve = true);
			// Process player input
			void					ProcessInput();
			// Check for touch input's collision with game objects and process a response according to the collision group
			bool					CheckTouchCollision(VECTOR2 touch);
			
		public:
			// コンストラクタ
									GameLevel(int level);

			// デストラクタ
									~GameLevel();

			// 初期化
			void					Initialize();

			// 更新
			void					Update();

			// 上画面描画
			virtual void			DrawUpper();
			
			// 下画面描画
			virtual void			DrawLower();
	};
}
