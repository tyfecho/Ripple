/*
* GameLevel.cpp
*/

/***************************************************
*	File includes
***************************************************/

#include "GameLevel.h"
#include "LevelSelect.h"
#include "Screens.h"

#include "../../NW_Lib/System/NW_Lib_SceneManager.h"

#include "../../NW_Lib/Graphics/NW_Lib_SpriteDrawer.h"
#include "../../NW_Lib/Graphics/NW_Lib_TextureManager.h"

#include "../../NW_Lib/Utility/NW_Lib_DebugFontManager.h"
#include "../../NW_Lib/System/NW_Lib_InputManager.h"
#include "../../NW_Lib/Utility/NW_Lib_CommonData.h"

#include "../../NW_Lib/Sound/NW_Lib_Sound.h"

#include <math.h>
#include <sstream>
#include <fstream>

//Sound Data
#ifdef _DirectX
	#include <time.h>
	float prevTime = timeGetTime();
	float currTime = timeGetTime();
	int frameCount = 0;
	float fps = 0.f;

	//#include SoundData
	#include "../../works_romfiles/sound/RippleSoundInGame.h"

#endif
#ifdef _3DS

	#include "../../NW_Lib/Graphics/NW_Lib_ResTextureManager.h"

	//#include SoundData

	// サウンド
	#include"../../NW_Lib/Sound/RippleSoundInGame.h"
#endif

// File Stream
#include "../../NW_Lib/System/NW_Lib_RomFileStream.h"
#include "../../NW_Lib/System/NW_Lib_SDFileStream.h"

using namespace NW_Lib;

/***************************************************
*	Game namespace
***************************************************/

namespace Game
{
	/***************************************************
	*	Initialization & de-initialization functions
	****************************************************/

	/*
	*	コンストラクタ      
	*/
	GameLevel::GameLevel(int level)
		: m_Water(NULL)
		, m_Player(NULL)
		, m_Goal(NULL)
		, m_Level(level)
	{
		
	}

	/*
	*	デストラクタ      
	*/
	GameLevel::~GameLevel()
	{
		//Delete textures
		TextureManager::AllRemove();

		//Delete water
		if (m_Water)
		{
			delete m_Water;
			m_Water = NULL;
		}

		//Delete player
		if (m_Player)
		{
			delete m_Player;
			m_Player = NULL;
		}

		//Delete goal
		if (m_Goal)
		{
			delete m_Goal;
			m_Goal = NULL;
		}

		//Delete game objects
		ClearLevel();

#ifdef SPATIAL_PARTITIONING
		//Delete spatial partitioning
		if (m_QuadTree)
		{
			delete m_QuadTree;
			m_QuadTree = NULL;
		}
#endif

		//Reset the lily pad list static pointer
		Frog::SetLilyPadListPtr(NULL);

		//Delete Tutorial
		if (theTutorial)
		{
			delete theTutorial;
			theTutorial = NULL;
		}

		//Delete MenuObj
		if (m_MenuObj)
		{
			delete m_MenuObj;
			m_MenuObj = NULL;
		}

		Sound::GetInstance().Finalize();
	}

	/*
	*	初期化処理
	*/
	void GameLevel::Initialize()
	{
		//Load textures
		TextureManager::Entry(ETEXTURE_WATER		, "water");
		TextureManager::Entry(ETEXTURE_RIPPLE		, "ripple");
		TextureManager::Entry(ETEXTURE_PLAYER		, "babyswan");
		TextureManager::Entry(ETEXTURE_GOAL			, "motherswan");
		TextureManager::Entry(ETEXTURE_TILEMAP		, "tilemap");
		TextureManager::Entry(ETEXTURE_LOG			, "Log");
		TextureManager::Entry(ETEXTURE_FROG			, "32x32 frog sprite sheet");
		TextureManager::Entry(ETEXTURE_GAMEELEMENTS	, "64ElementsSprite");
		TextureManager::Entry(ETEXTURE_PIRANHA		, "PiraniaAnime");
		if ( CommonData::GetInstance().GetOptionData()->language == 1 )
			TextureManager::Entry(ETEXTURE_UI		, "GameScreenSprite_EN");
		else
		{
			TextureManager::Entry(ETEXTURE_UI		, "GameScreenSprite_JP");
		}
		TextureManager::Entry(ETEXTURE_NUMBER		, "Number");
		TextureManager::Entry(ETEXTURE_FONT			, "LevelSelectScene/Font");
		TextureManager::Entry(ETEXTURE_BOARD		, "LevelSelectScene/Board");
		TextureManager::Entry(ETEXTURE_DEBUG_SQUARE	, "debug_square");

		//Initialize sounds
		Sound::GetInstance().Initialize("RippleSoundInGame");
		Sound::GetInstance().SE_Entry(SE_CANCEL);
		Sound::GetInstance().SE_Entry(SE_DECIDE);
		Sound::GetInstance().SE_Entry(SE_FROG);
		Sound::GetInstance().SE_Entry(SE_FROGJUMP2);
		Sound::GetInstance().SE_Entry(SE_LOG);
		Sound::GetInstance().SE_Entry(SE_LOTASLEAF2);
		Sound::GetInstance().SE_Entry(SE_PIRANHA);
		Sound::GetInstance().SE_Entry(SE_RIPPLE_MIN);
		Sound::GetInstance().SE_Entry(SE_VOLTEX_MID);

		//Play BGM
		Sound::GetInstance().STRM_Play(BGM_GAMEPLAYINGRIPLLE);

		//Initialize water
		m_Water = new Water(ETEXTURE_WATER, ETEXTURE_RIPPLE);

		//Initialize game objects
		m_Player = new Player();
		m_Player->SetTexture(ETEXTURE_PLAYER);
		m_Player->m_Size = VECTOR2(30.f, 30.f);
		m_Player->CalcCollisionRadius();
		m_Player->SetCollisionRadius(m_Player->GetCollisionRadius() * 0.7f);
		m_Player->SetCollisionShape(ECOLLISIONSHAPE_CIRCLE);
#ifndef SPATIAL_PARTITIONING
		m_Water->InitPointers(m_Player, &m_SquareWall, &m_AngledWall, &m_LilyPad, &m_Log);
#endif

		m_Goal = new GameObject();
		m_Goal->SetTexture(ETEXTURE_GOAL);
		m_Goal->m_Size = VECTOR2(50.f, 50.f);
		m_Goal->CalcCollisionRadius();
		m_Goal->SetCollisionShape(ECOLLISIONSHAPE_CIRCLE);

#ifdef SPATIAL_PARTITIONING
		//Initialize spatial partitioning
		m_QuadTree = new QuadTree();
		m_QuadTree->Initialize(0, VECTOR2(GAME_SCREEN_WIDTH*0.5f, TOTAL_SCREEN_HEIGHT*0.5f), VECTOR2(GAME_SCREEN_WIDTH, TOTAL_SCREEN_HEIGHT));
		m_Water->InitPointers(m_Player, m_QuadTree, &m_CollisionIsland);
#endif

		//Intialize gameplay variables
		m_SwapScreen = false;
		m_Paused = false;
		m_ElapsedFrame = 0;
		m_DropsRemainingAlpha = 0.0f;
		m_ElapsedTime = 0.f;

		//Set the pointer to the list of lily pads
		Frog::SetLilyPadListPtr(&m_LilyPad);

		// Menu
		m_MenuObj = new MenuInGame();
		m_MenuObj->Initialize();

		// Tutorial
		theTutorial = new Tutorial();
		theTutorial->Init(this->m_Level);

		//Load the level file data
		LoadLevel();

		//Update the water texture at init to blank everything out
		m_Water->UpdateWaterTexture();
	}

	/*
	*	Clear the current level data
	*/
	void GameLevel::ClearLevel()
	{
#ifdef SPATIAL_PARTITIONING
		//Clear the spatial partitioning
		if (m_QuadTree)
			m_QuadTree->Clear();
#endif

		m_CollisionIsland.clear();

		//Delete square walls
		{
			Node<GameObject*> *it = m_SquareWall.front_node();
			for (int i = 0; i < m_SquareWall.size(); ++ i)
			{
				delete it->data;
				it->data = NULL;

				it = it->next;
			}
			m_SquareWall.clear();
		}

		//Delete angled walls
		{
			Node<GameObject*> *it = m_AngledWall.front_node();
			for (int i = 0; i < m_AngledWall.size(); ++ i)
			{
				delete it->data;
				it->data = NULL;

				it = it->next;
			}
			m_AngledWall.clear();
		}

		//Delete lily pads
		{
			Node<LilyPad*> *it = m_LilyPad.front_node();
			for (int i = 0; i < m_LilyPad.size(); ++ i)
			{
				delete it->data;
				it->data = NULL;

				it = it->next;
			}
			m_LilyPad.clear();
		}

		//Delete frogs
		{
			Node<Frog*> *it = m_Frog.front_node();
			for (int i = 0; i < m_Frog.size(); ++ i)
			{
				delete it->data;
				it->data = NULL;

				it = it->next;
			}
			m_Frog.clear();
		}

		//Delete logs
		{
			Node<GameObject*> *it = m_Log.front_node();
			for (int i = 0; i < m_Log.size(); ++ i)
			{
				delete it->data;
				it->data = NULL;

				it = it->next;
			}
			m_Log.clear();
		}

		//Delete log pivots
		{
			Node<LogPivot*> *it = m_LogPivot.front_node();
			for (int i = 0; i < m_LogPivot.size(); ++ i)
			{
				delete it->data;
				it->data = NULL;

				it = it->next;
			}
			m_LogPivot.clear();
		}

		//Delete piranhas
		{
			Node<Piranha*> *it = m_Piranha.front_node();
			for (int i = 0; i < m_Piranha.size(); ++ i)
			{
				delete it->data;
				it->data = NULL;

				it = it->next;
			}
			m_Piranha.clear();
		}

		//Delete whirlpools
		{
			Node<Whirlpool*> *it = m_Whirlpool.front_node();
			for (int i = 0; i < m_Whirlpool.size(); ++ i)
			{
				delete it->data;
				it->data = NULL;

				it = it->next;
			}
			m_Whirlpool.clear();
		}
		//m_CollisionObjects.clear();
	}

	/*
	*	Load the current level file
	*/
	void GameLevel::LoadLevel()
	{
		//Reset the player variables
		m_Player->m_Velocity = VECTOR2(0.f, 0.f);
		m_Player->m_Scale = VECTOR2(1.f, 1.f);
		m_Player->SetActive(true);
		m_Player->SetLilyPad(NULL);

		//Reset the gameplay variables
		m_SwapScreen = false;
		Whirlpool::m_TotalForce = VECTOR2(0.f, 0.f);
		m_ElapsedTime = 0.f;

		//Clear all the ripples currently on the screen
		m_Water->ClearRipple();

#ifdef _DirectX
		ostringstream filename; 
		filename << ".\\works_romfiles\\levels\\lv_"; 
		for (int divisor = 100; divisor >= 1; divisor /= 10) 
		{ 
			if (m_Level / divisor == 0) 
			{ 
				filename << 0;
			} 
			else
			{ 
				filename << m_Level; 
				break; 
			} 
		} 
		filename << ".txt"; 
		ifstream file(filename.str().c_str());		//Open the file

		if(file.is_open())							//If the file was successfully opened
		{ 
			int dataGroup = -1;
			int y = -1;

			while(file.good())							//While stream is good for i/o 
			{ 
				std::string LineOfText = "";				//String to store all the data in the current line 
				getline(file, LineOfText);					//Get the data from the current line 

				if (LineOfText.find("//") == NULL)			//New data group
				{
					++dataGroup;
				}											//endif new data group 
				else										//Else if still in the same data group
				{
					LoadLevelData((ELEVELDATA)dataGroup, LineOfText, y);
				}											//endif same data group

				++y;
			}											//End file good loop 
		}											//endif file successfully open 
  
		file.close();								//Close the file
#endif
#ifdef _3DS
		ostringstream filename; 
		filename << "levels//lv_"; 
		for (int divisor = 100; divisor >= 1; divisor /= 10) 
		{ 
			if (m_Level / divisor == 0) 
			{ 
				filename << 0;
			} 
			else
			{ 
				filename << m_Level; 
				break; 
			} 
		} 
		filename << ".txt"; 
		NW_Lib::System::RomFileStream file(filename.str().c_str());		//Open the file

		if(!file.IsEOF())							//If the file was successfully opened
		{ 
			int dataGroup = -1;
			int y = -1;

			while(!file.IsEOF())							//While stream is good for i/o 
			{ 
				std::string LineOfText = "";				//String to store all the data in the current line

				//getline(file, LineOfText);					//Get the data from the current line 
				file.GetLine(&LineOfText);

				if (LineOfText.find("//") == NULL)			//New data group
				{
					++dataGroup;
				}											//endif new data group 
				else										//Else if still in the same data group
				{
					LoadLevelData((ELEVELDATA)dataGroup, LineOfText, y);
				}											//endif same data group

				++y;
			}											//End file good loop 
		}											//endif file successfully open 
  
		file.Close();								//Close the file
#endif

		//If there is at least one lily pad in the list
		if (m_LilyPad.size() > 0)
		{
			//Convert the lily pad list to a circular linked list
			m_LilyPad.back_node()->next = m_LilyPad.front_node();
			m_LilyPad.front_node()->prev = m_LilyPad.back_node();
		}
		//endif at least one lily pad

		//If there is at least one frog in the list
		if (m_Frog.size() > 0)
		{
			//Convert the frog list to a circular linked list
			m_Frog.back_node()->next = m_Frog.front_node();
			m_Frog.front_node()->prev = m_Frog.back_node();
		}
		//endif at least one frog

		//Get all frogs to look to the next available lily pad
		{
			Node<Frog*> *it = m_Frog.front_node();
			for (int i = 0; i < m_Frog.size(); ++ i)
			{
				it->data->LookToNext();
				it = it->next;
			}
		}

		//Initialize piranhas positions and waypoints
		{
			Node<Piranha*> *it = m_Piranha.front_node();
			for (int i = 0; i < m_Piranha.size(); ++ i)
			{
				it->data->Initialize();
				it = it->next;
			}
		}

		//Initialize the water bed texture depending on which tiles are shallow water
		m_Water->InitWaterBedTexture();

		int size = 0;
		//Loop through the different object types
		for (int grp = 0; grp < ECOLLISIONGROUP_COUNT; ++ grp)
		{
			//Perform spatial partitioning on all objects that will be checked for collision
			switch (grp)
			{
				case ECOLLISIONGROUP_LILYPAD:	{	Node<LilyPad*> *it = m_LilyPad.front_node();
													for (int i = 0; i < m_LilyPad.size(); ++ i)
													{
#ifdef SPATIAL_PARTITIONING
														m_QuadTree->Insert(it->data);
#endif
														++size;
														it = it->next;
													}
												} break;
				case ECOLLISIONGROUP_SQUAREWALL:{	Node<GameObject*> *it = m_SquareWall.front_node();
													for (int i = 0; i < m_SquareWall.size(); ++ i)
													{
#ifdef SPATIAL_PARTITIONING
														m_QuadTree->Insert(it->data);
#endif
														++size;
														it = it->next;
													}
												} break;
				case ECOLLISIONGROUP_ANGLEDWALL:{	Node<GameObject*> *it = m_AngledWall.front_node();
													for (int i = 0; i < m_AngledWall.size(); ++ i)
													{
#ifdef SPATIAL_PARTITIONING
														m_QuadTree->Insert(it->data);
#endif
														++size;
														it = it->next;
													}
												} break;
				case ECOLLISIONGROUP_LOGPIVOT:	{
												} break;
				case ECOLLISIONGROUP_LOG:		{	Node<GameObject*> *it = m_Log.front_node();
													for (int i = 0; i < m_Log.size(); ++ i)
													{
#ifdef SPATIAL_PARTITIONING
														m_QuadTree->Insert(it->data);
#endif
														++size;
														it = it->next;
													}
												} break;
			}
		}
		//end loop different object types

		//Pre-allocate memory for the collision island list
		//We want to keep using the same list for the collision checks without deleting nodes and clearing the list as it is very slow
		//As such, we resize the list to be same as the total number of objects so that it will always be large enough to hold everything
		m_CollisionIsland.resize(size);
	}

	/*
	*	Sub-function called by the main load to process the data inside the file
	*/
	void GameLevel::LoadLevelData(ELEVELDATA dataGroup, std::string LineOfText, int y)
	{
		int x = 0;

		//If currently loading tile map
		if (dataGroup == ELEVELDATA_WALL)
		{	
			//Erase the line number indicator 
			LineOfText.erase(LineOfText.begin()
							,LineOfText.begin()+4
							);
		}
		//endif tile map group

		//String to store the broken down parts of the data 
		string token;

		//Break the data into parts divided by ',' 
		istringstream iss(LineOfText);
		while(getline(iss, token, ','))
		{
			//Process data depending on data group
			switch (dataGroup)
			{
				case ELEVELDATA_WALL:	{	int tileID = atoi(token.c_str());
											if (tileID != ELEVELTILE_WATER)
												LoadLevelTile(tileID, x, y);
										}	break;
				case ELEVELDATA_DROPLET:{	m_DropsRemaining = atoi(token.c_str());
										}	break;
				case ELEVELDATA_PLAYER:	{	if (x == 0)											
												m_Player->m_Position.x = atof(token.c_str());
											else
												m_Player->m_Position.y = atof(token.c_str());
										}	break;
				case ELEVELDATA_GOAL:	{	if (x == 0)
												m_Goal->m_Position.x = atof(token.c_str());
											else
											{
												m_Goal->m_Position.y = atof(token.c_str());
												m_Player->m_Direction = Vec2Normalize(m_Goal->m_Position - m_Player->m_Position);
											}
										}	break;
				case ELEVELDATA_LILYPAD:{	if (x == 0)
											{
												m_LilyPad.push_back(new LilyPad());
												m_LilyPad.back()->SetObjectID(ELEVELDATA_LILYPAD);
												m_LilyPad.back()->m_Size = VECTOR2(LILYPAD_SIZE, LILYPAD_SIZE);
												m_LilyPad.back()->SetTexture(ETEXTURE_GAMEELEMENTS, 64, 64);
												m_LilyPad.back()->m_Scale = VECTOR2(1.15f, 1.15f);
												m_LilyPad.back()->SetCollisionRadius(LILYPAD_SIZE*0.5f);
												m_LilyPad.back()->SetCollisionShape(ECOLLISIONSHAPE_CIRCLE);
												m_LilyPad.back()->m_Position.x = atof(token.c_str());
											}
											else if (x == 1)
												m_LilyPad.back()->m_Position.y = atof(token.c_str());
											else
											{
												int occupyID = atoi(token.c_str());
												m_LilyPad.back()->SetCurrentFrame(occupyID%2);
												
												if (occupyID)
												{
													m_LilyPad.back()->SetOccupied(true);
													if (occupyID == 2)
													{
														m_Frog.push_back(new Frog(m_LilyPad.back_node()));
														m_Frog.back()->SetObjectID(ELEVELDATA_FROG);
														m_Frog.back()->SetParentNode(m_Frog.back_node());
														m_Frog.back()->m_Size = VECTOR2(LILYPAD_SIZE, LILYPAD_SIZE);
														m_Frog.back()->SetTexture(ETEXTURE_FROG, 32, 32, 0, 0, 6);
														m_Frog.back()->SetCollisionRadius(LILYPAD_SIZE*0.5f*0.8f);
														m_Frog.back()->SetCollisionShape(ECOLLISIONSHAPE_CIRCLE);
														m_Frog.back()->SetLilyPadNode(m_LilyPad.back_node());
														m_LilyPad.back()->SetFrogNode(m_Frog.back_node());
													}
												}
											}
										}	break;
				case ELEVELDATA_LOG:	{	if (x == 0)
											{
												m_Log.push_back(new GameObject());
												m_Log.back()->SetObjectID(ELEVELDATA_LOG);
												m_Log.back()->m_Size = VECTOR2(LOG_LENGTH, LOG_WIDTH);
												m_Log.back()->SetTexture(ETEXTURE_LOG, 128, 64);
												m_Log.back()->m_Scale = VECTOR2(1.2f, 1.7f);
												m_Log.back()->SetCollisionShape(ECOLLISIONSHAPE_OBB);
												m_Log.back()->m_Position.x = atof(token.c_str());
											}
											else if (x == 1)
												m_Log.back()->m_Position.y = atof(token.c_str());
											else if (x == 2)
												m_Log.back()->m_Direction = Vec2RotateDeg(VECTOR2(0.f, 1.f), atof(token.c_str()));
											else
											{
												if (atoi(token.c_str()) == 0)
												{
													m_Log.back()->SetCurrentFrame(0);
												}
												else
												{
													m_Log.back()->SetCurrentFrame(1);
													m_Log.back()->CalcCollisionRadius(true, -1);
													m_LogPivot.push_back(new LogPivot(m_Log.back()));
													m_LogPivot.back()->SetObjectID(ELEVELDATA_LOGPIVOT);
													m_LogPivot.back()->m_Position = m_Log.back()->m_Position;
													m_LogPivot.back()->SetCollisionRadius(m_Log.back()->GetCollisionRadius());;
												}
											}
										}	break;
				case ELEVELDATA_PIRANHA:{	if (x == 0)
											{
												m_Piranha.push_back(new Piranha());
												m_Piranha.back()->SetObjectID(ELEVELDATA_PIRANHA);
												m_Piranha.back()->m_Size = VECTOR2(PIRANHA_SIZE, PIRANHA_SIZE*2);
												m_Piranha.back()->SetTexture(ETEXTURE_PIRANHA, 32, 64, 0, 0, 6);
												m_Piranha.back()->SetAlpha(0.5f);
												m_Piranha.back()->SetCollisionRadius(PIRANHA_SIZE*0.4f);
												m_Piranha.back()->SetCollisionShape(ECOLLISIONSHAPE_CIRCLE);
												m_Piranha.back()->PlayAnimation(6.f, -1);
											}
											
											if (x%2 == 0)
												m_Piranha.back()->AddWaypointX(atof(token.c_str()));
											else
												m_Piranha.back()->AddWaypointY(atof(token.c_str()));
										}	break;
				case ELEVELDATA_WPOOL:	{	if (x == 0)
											{
												m_Whirlpool.push_back(new Whirlpool());
												m_Whirlpool.back()->SetObjectID(ELEVELDATA_WPOOL);
												m_Whirlpool.back()->m_Size = VECTOR2(WPOOL_SIZE, WPOOL_SIZE);
												m_Whirlpool.back()->SetTexture(ETEXTURE_GAMEELEMENTS, 64, 64);
												m_Whirlpool.back()->SetCurrentFrame(2);
												m_Whirlpool.back()->SetCollisionRadius(WPOOL_SIZE*0.5f);
												m_Whirlpool.back()->SetCollisionShape(ECOLLISIONSHAPE_CIRCLE);
												m_Whirlpool.back()->m_Position.x = atof(token.c_str());
											}
											else if (x == 1)
												m_Whirlpool.back()->m_Position.y = atof(token.c_str());
											else
												m_Whirlpool.back()->SetClockwise((atoi(token.c_str()) == 0) ? true : false);
										}	break;
			}
			++x;
		}
	}
	
	/*
	*	Sub-loaddata function to process the tilemap
	*/
	void GameLevel::LoadLevelTile(int tileID, int x, int y)
	{
		//If tile ID is negative, i.e. shallow water
		if (tileID < 0)
			m_Water->SetShallowWater(x, y);

		//If water tile
		if (tileID == -1 || tileID == 0)
			//Return out of the function as we don't need to create a wall object
			return;

		//Calculate the centre point of the tile
		VECTOR2 size (LEVEL_TILE_SIZE, LEVEL_TILE_SIZE);
		VECTOR2 pos (x, y);
		pos *= LEVEL_TILE_SIZE;
		pos += size*0.5f;

		//Create a new game object
		GameObject* wall = new GameObject();
		wall->SetObjectID(ELEVELDATA_WALL);
		wall->m_Position = pos;
		wall->m_Size = size;
		wall->SetTexture(ETEXTURE_TILEMAP, 32, 32);

		//Empty shallow water (Negative) or square wall (Positive)
		if (abs(tileID) == ELEVELTILE_SQRWALL)
		{
			//If square wall
			if (tileID > 0)
			{
				//If tile is on the left-most or right-most column
				if (x == 0 || x == LEVEL_TILE_COL - 1)
				{
					//If tile is exactly at the corners
					if (y == 0 || y == LEVEL_TILE_ROW - 1)
						wall->SetCurrentFrame(2);
					//Else if any other tiles along the columns
					else
					{
						wall->SetCurrentFrame(3);
						wall->m_Direction.x = (x == 0) ? 1.f : -1.f;
						wall->m_Direction.y = 0.f;
					}
				}
				//endif tile on left-most or right-most column
				//Else if tile is on top-most or bottom-most rows
				else if (y == 0 || y == LEVEL_TILE_ROW - 1)
				{
					//No need to check if tiles are exactly at corner as the previous 'if' statement will account for it already
					wall->SetCurrentFrame(3);
					wall->m_Direction.y = (y == 0) ? 1.f : -1.f;
				}
				//endif tile is on top-most or bottom-most rows
				//Else if tile is anywhere else (Not lining the border)
				else
					wall->SetCurrentFrame(0);
				//endif tile is anywhere else

				wall->SetCollisionShape(ECOLLISIONSHAPE_AABB);
				m_SquareWall.push_back(wall);
			}
			//Else if shallow water
			//Do nothing as the shallow water bool has been set above
		}
		//Angled wall with shallow (Negative) or deep (Positive) water
		else
		{
			wall->SetCurrentFrame(1);
			wall->SetCollisionShape(ECOLLISIONSHAPE_TRIANGLE);
			m_AngledWall.push_back(wall);
			VECTOR2 p1, p2, p3;
			VECTOR2 rect_min = pos - size*0.5f;
			VECTOR2 rect_max = pos + size*0.5f;

			//Calculate the 3 points of the triangles depending on the orientation, which is determined by the tile ID
			switch (abs(tileID))
			{
				// Upper left triangle
				case ELEVELTILE_TRIWALL0:{	p1 = VECTOR2(rect_min.x, rect_min.y); // Top-left
											p2 = VECTOR2(rect_min.x, rect_max.y); // Btm-left
											p3 = VECTOR2(rect_max.x, rect_min.y); // Top-right
										 }	break;
				// Upper right triangle
				case ELEVELTILE_TRIWALL1:{	wall->m_Direction = VECTOR2(-1.f, 0.f);
											p1 = VECTOR2 (rect_min.x, rect_min.y); // Top-left
											p2 = VECTOR2 (rect_max.x, rect_max.y); // Btm-right
											p3 = VECTOR2 (rect_max.x, rect_min.y); // Top-right
										 }	break;
				// Lower right triangle
				case ELEVELTILE_TRIWALL2:{	wall->m_Direction = VECTOR2(0.f, -1.f);
											p1 = VECTOR2 (rect_min.x, rect_max.y); // Btm-left
											p2 = VECTOR2 (rect_max.x, rect_max.y); // Btm-right
											p3 = VECTOR2 (rect_max.x, rect_min.y); // Top-right
										 }	break;
				// Lower left triangle
				case ELEVELTILE_TRIWALL3:{	wall->m_Direction = VECTOR2(1.f, 0.f);
											p1 = VECTOR2(rect_min.x, rect_min.y); // Top-left
											p2 = VECTOR2(rect_min.x, rect_max.y); // Btm-left
											p3 = VECTOR2(rect_max.x, rect_max.y); // Btm-right
										 }	break;
			}

			//Push the 3 points into the object's vertex list
			wall->m_Vertex.push_back(p1);
			wall->m_Vertex.push_back(p2);
			wall->m_Vertex.push_back(p3);
		}
		//Empty deep water tile
		//Do nothing
	}

	/***************************************************
	*	Update functions
	****************************************************/

	/*
	*	更新
	*/
	void GameLevel::Update()
	{
		m_ElapsedFrame++;
#ifdef _DirectX
		//Calculate and print the FPS
		if (++frameCount >= 10)
		{
			prevTime = currTime;
			currTime = timeGetTime();
			fps = 10 / (currTime - prevTime) * 1000;
			frameCount = 0;
		}
		//DebugFontManager::Printf(UPPER_SCREEN_DRAWING, 300, 10, "%.2f", fps);
#endif


		if (this->theTutorial->getIsThereOne())
			this->theTutorial->Update();
		else
		{
			//Process user input
			ProcessInput();

			//If game is paused
			if (m_Paused)
			{

				m_MenuObj->Update();
				if (m_MenuObj->GetPushButtonType() == BACK)
				{
					m_Paused = !m_Paused;
					Sound::GetInstance().SE_Play(SE_CANCEL);
				}
				else if (m_MenuObj->GetPushButtonType() == RESTART)
				{
					Sound::GetInstance().SE_Play(SE_DECIDE);
					System::SceneManager::GetInstance().ChangeScene(new GameLevel(m_Level));
				}
				else if (m_MenuObj->GetPushButtonType() == MENU)
				{
					Sound::GetInstance().SE_Play(SE_DECIDE);
					System::SceneManager::GetInstance().ChangeScene(new LevelSelect((m_Level-1)/5+1, (m_Level-1)%5+1));
				}

				//Skip the game update functions
				return;
			}

			//Update other gameplay variables
			m_ElapsedTime += DELTA_TIME;

			//Update water
			m_Water->UpdateWaterRipple(DELTA_TIME);
			m_Water->UpdateWaterTexture();

			//Update objects
			UpdateObjects(DELTA_TIME);
		}
	}

	/*
	*	Sub-update function for game objects
	*/
	void GameLevel::UpdateObjects(float deltaTime)
	{
		/*
		*	Update player
		*/
		//If player is still alive
		if (m_Player->IsActive())
		{
			//Clamp the player velocity to the maximum speed of the ripple
			//So that it doesn't move faster than the ripple does
			float speed = Vec2Length(m_Player->m_Velocity);
			float clampedSpeed = 0.f;
			if (speed > 0.f)
			{
				clampedSpeed = clamp(speed, 0.f, RIPPLE_SPEED);
				m_Player->m_Velocity *= clampedSpeed / speed;
			}

			//Add the total force acting on the player by the whirlpools to the player's velocity
			m_Player->m_Velocity += Whirlpool::m_TotalForce;

			//Store and update the player's position
			VECTOR2 prevPos = m_Player->m_Position;
			m_Player->Update(deltaTime);
			VECTOR2 newPos = m_Player->m_Position;

			//If the player moved in this frame
			if (speed > 0.f || Vec2LengthSq(Whirlpool::m_TotalForce) > 0.f)
			{
				//Remove the whirlpool's force from the player's velocity
				//So that future calculations, e.g. the player rotation by ripples and dampening effect applied later isn't affected by it
				m_Player->m_Velocity -= Whirlpool::m_TotalForce;

				//Turn the player slightly in the direction of the movement caused by ripples
				m_Player->m_Direction += m_Player->m_Velocity*0.03f*deltaTime;
 				m_Player->m_Direction = Vec2Normalize(m_Player->m_Direction);

				//Check and resolve player's collision against game elements
				CheckPlayerCollision(prevPos, newPos, (clampedSpeed + Vec2Length(Whirlpool::m_TotalForce))*deltaTime, deltaTime);

				//If the player is being moved by ripples in this frame
				if (speed > 0.f)
				{
					//Dampen the speed so that it gradually comes to a stop
					float temp = clampedSpeed;
					temp -= temp * 1.5f * deltaTime;
					temp -= 5.f * deltaTime;
					if (temp < 0.f)
						temp = 0.f;
					m_Player->m_Velocity *= temp / clampedSpeed;
				}
				//endif player moved by ripples
			}
			//endif player moved in this frame

			//If player goes out of screen
			if	(	m_Player->m_Position.x < 0.f || m_Player->m_Position.x > GAME_SCREEN_WIDTH
				||	m_Player->m_Position.y < 0.f || m_Player->m_Position.y > TOTAL_SCREEN_HEIGHT
				)
				//Set the player as "dead"
				m_Player->SetActive(false);
		}
		//endif player alive
		//Else if player is dead
		else
		{
			//Restart the level
			System::SceneManager::GetInstance().ChangeScene(new GameLevel(m_Level));
		}
		//endif player dead

		/*
		*	Update goal
		*/
		//Turn the goal to face the player
		m_Goal->m_Direction = Vec2Normalize(m_Player->m_Position - m_Goal->m_Position);

		//If the player is in contact with the goal
		if (m_Player->CollideObject(m_Goal))
		{
			//Win!
			System::SceneManager::GetInstance().ChangeScene(new LevelCleared(++m_Level-1, (int)m_ElapsedTime*1000, m_DropsRemaining%10));
		}
		
		/*
		*	Update level elements
		*/
		//Update lily pads
		{
			Node<LilyPad*> *it = m_LilyPad.front_node();
			for (int i = 0; i < m_LilyPad.size(); ++ i)
			{
				it->data->Update(deltaTime);
				it = it->next;
			}
		}
		//end update lily pads

		//Update frogs
		{
			Node<Frog*> *it = m_Frog.front_node();
			for (int i = 0; i < m_Frog.size(); ++ i)
			{
				//If the update function returns true, i.e. signal to generate ripple
				if (it->data->Update(deltaTime))
				{
					//Create a ripple at the frog's position
					m_Water->CreateRipple(it->data->m_Position.x, it->data->m_Position.y, 140, it->data->GetLilyPad());
				
					Sound::GetInstance().SE_Play(SE_FROGJUMP2);
				}
				it = it->next;
			}
		}
		//end update frogs

		//Update piranhas
		{
			Node<Piranha*> *it = m_Piranha.front_node();
			for (int i = 0; i < m_Piranha.size(); ++ i)
			{
				bool skipCheck = false;

				it->data->Update(deltaTime);

				//If player is not dead already
				if (m_Player->IsActive())
				{
					//If player collides with the piranha
					if (m_Player->CollideObject(it->data))
					{
						//If the player is not on a lily pad
						if (!m_Player->GetLilyPad())
						{
							//Create a splash effect
							m_Splash.push_back(new Splash(m_Player->m_Position.x, m_Player->m_Position.y));

							//Set the player to be dead
							m_Player->SetActive(false);

							Sound::GetInstance().SE_Play(SE_PIRANHA);
						}
					}
					//endif player collides piranha
				}
				//endif player not dead already

				//Loop through all frogs
				Node<Frog*> *frog = m_Frog.front_node();
				for (int j = 0; j < m_Frog.size(); ++ j)
				{
					//If the frog is not on at the target lily pad (i.e. moving halfway) and it collides with a piranha
					if (!frog->data->IsAtTarget() && it->data->CollideObject(frog->data))
					{
						//If the frog is very near a lily pad
						//Frog does not die as piranha can't hit above lily pads
						Node<LilyPad*> *lilyPad = m_LilyPad.front_node();
						for (int a = 0; a < m_LilyPad.size(); ++ a)
						{
							if (CollideCircleCircle(frog->data->m_Position, frog->data->GetCollisionRadius(), lilyPad->data->m_Position, lilyPad->data->GetCollisionRadius()*0.9f))
							{
								skipCheck = true;
								break;
							}

							lilyPad = lilyPad->next;
						}
						if (skipCheck)
						{
							frog = frog->next;
							continue;
						}

						//Else if frog is not near a lily pad
						//Create a splash effect
						//Unoccupy the lily pad that the frog is on
						//Delete the frog and continue to the next iteration
						m_Splash.push_back(new Splash(frog->data->m_Position.x, frog->data->m_Position.y));
						frog->data->GetLilyPad()->SetFrogNode(NULL);
						frog->data->GetLilyPad()->SetOccupied(false);
						frog = frog->next;
						m_Frog.erase(j--);

						Sound::GetInstance().SE_Play(SE_PIRANHA);
						continue;
					}

					//Get the next frog iterator
					frog = frog->next;
				}

				//Get the next piranha iterator
				it = it->next;
			}
			//end loop piranha
		}
		//end update piranhas

		//Update whirlpools
		{
			//Reset the total force acting on the player by the whirlpools to recalculate for this frame
			Whirlpool::m_TotalForce = VECTOR2(0.f, 0.f);

			// (Either x or y is fine since they should both be same)
			float prevScale = m_Player->m_Scale.x;
			m_Player->m_Scale = VECTOR2(1.f, 1.f);

			//Loop through all whirlpools
			float AngleToRotate = 0.f;
			Node<Whirlpool*> *it = m_Whirlpool.front_node();
			for (int i = 0; i < m_Whirlpool.size(); ++ i)
			{
				//Update the whirlpool sprite texture (Rotation)
				it->data->Update(deltaTime);

				//If player is dead already
				if (!m_Player->IsActive())
				{
					//Continue to the next iterator
					//Do not use break as we still need to run the Update function for the other iterators
					it = it->next;
					continue;
				}
				//endif player dead already

				//If the player is within the range of the whirlpool's effect
				if (CollidePointCircle(m_Player->m_Position, it->data->m_Position, it->data->GetCollisionRadius()*3))
				{
					//Get a vector from the player to the centre of the whirlpool
					VECTOR2 dir = it->data->m_Position - m_Player->m_Position;

					//Calculate the squared distance of the player from the centre of the whirlpool
					float distSq = Vec2LengthSq(dir);

					//If the player is almost at the centre of the whirlpool and doesn't have enough speed to break out of the pulling force
					if (distSq < 10.f && Vec2LengthSq(m_Player->m_Velocity) < 100.f)
					{
						//Set the player to be dead
						m_Player->SetActive(false);

						//Break out of the loop since player is dead and it is not required to do the rest of the calculations
						break;
					}
					//endif player almost at centre of whirlpool

					//Else if the player has not reached centre of whirlpool yet

					//Calculate the actual distance of the player from the centre of the whirlpool
					//Since the squared distance has been calculated already, use that value rather than calling Vec2Length again to avoid unnecessary calculations
					float dist = sqrt(distSq);

					//Normalize the direction vector since the magnitude isn't required anymore and we'll need the unit vector for the next few calculations
					dir = Vec2Normalize(dir);

					//Factor of the whirlpool's effect on the player
					//The further the player, the weaker the whirlpool's effect
					float distFactor = 1.f;

					//If the player is very near the centre of the whirlpool
					if (dist < it->data->GetCollisionRadius()*0.5f)
					{
						//Scale the player inversely propotionate to the distance from the centre of whirlpool
						//To simulate the effect of the player being sucked into the whirlpool
						float scale = (dist / (it->data->GetCollisionRadius()*0.5f))*0.7f + 0.3f;

						//In the event that there are multiple whirlpools very near to each other (For whatever weird reason)
						//Use the lower value between the calculated scale and the player's current scale
						scale = min(scale, prevScale);
						
						//Apply the scale effect to the player
						m_Player->m_Scale = VECTOR2(scale, scale);

						//distFactor is kept at the max of 1.0
					}
					//endif player near whirlpool centre
					//Else if player is not too near the centre of the whirlpool
					else
					{
						//Calculate the factor of the whirlpool's effect on the player
						distFactor -= (dist - it->data->GetCollisionRadius()*0.5f) / (it->data->GetCollisionRadius()*2.5f);	
					}
					//endif player not near centre of whirlpool

					//Add a force perpendicular to the player-whirlpool direction to create a swirling effect
					//distFactor range is converted from (0.0, 1.0) to (0.4, 1.0) so that even at maximum distance, the swirling effect is still noticeable
					VECTOR2 force = ((it->data->IsClockwise()) ? Vec2PerpCW(dir) : Vec2PerpCCW(dir)) * 30.f * (0.4f + distFactor*0.6f);

					//Add a force towards the centre of the whirlpool
					force += dir * 5.f * distFactor;

					//Calculate where the tentative position of the player due to this force will be
					//Relative to the whirlpool centre
					VECTOR2 tentativePos = m_Player->m_Position + force*deltaTime - it->data->m_Position;

					//Use the previous vector and the player-whirlpool vector to calculate the angle to rotate the player
					//Such that the angle of the player from the palyer-whirlpool vector is always the same
					AngleToRotate += Vec2AngleDeg(-dir, tentativePos) * ((it->data->IsClockwise()) ? 1 : -1);
					
					//Add the force of this whirlpool to the total
					Whirlpool::m_TotalForce += force;
				}
				//endif player in whirlpool range

				//Get the next whirlpool iterator
				it = it->next;
			}
			//end loop whirlpool

			//Play whirlpool sfx depending on the strength of the force acting on the player
			float whirlpoolStrengthSq = Vec2LengthSq(Whirlpool::m_TotalForce);
			static float timer = 0.f;
			timer += deltaTime;
			
			if (whirlpoolStrengthSq > 0.f && timer >= 1.4f)
			{
				float vol = clamp(whirlpoolStrengthSq / 600.f, 0.4f, 1.f);
				Sound::GetInstance().SE_Play(SE_VOLTEX_MID);
				Sound::GetInstance().SE_SetVolume(SE_VOLTEX_MID, vol);

				timer = 0.f;
			}

			//Rotate the player depending on the whirlpool's force vectors
			m_Player->m_Direction = Vec2RotateDeg(m_Player->m_Direction, AngleToRotate, false);
		}
		//end update whirlpools

		//Update splash
		{
			Node<Splash*> *it = m_Splash.front_node();
			for (int i = 0; i < m_Splash.size(); ++ i)
			{
				it->data->Update(deltaTime);
				if (!it->data->IsActive())
				{
					it = it->next;
					m_Splash.erase(i--);
					continue;
				}

				it = it->next;
			}
		}
		//end update splash
	}

	/*
	*	Check for player's collision with other game objects and process a response according to the collision group
	*/
	bool GameLevel::CheckPlayerCollision(VECTOR2 prevPos, VECTOR2 newPos, float distToMove, bool resolve)
	{
		//Store the vector moved by the player this frame
		VECTOR2 scaledVelocity = newPos - prevPos;

		//Calculate the swept bounding box of the player's movement this frame
		VECTOR2 swept_bounding_box_pos = prevPos + scaledVelocity*0.5f;
		VECTOR2 swept_bounding_box_size;
		swept_bounding_box_size.x = abs(scaledVelocity.x) + m_Player->GetCollisionRadius()*2;
		swept_bounding_box_size.y = abs(scaledVelocity.y) + m_Player->GetCollisionRadius()*2;

#ifdef SPATIAL_PARTITIONING
		//Retrieve the list of potential collision candidates from the spatial partitioning
		Node<GameObject*>* node = m_CollisionIsland.front_node();
		int size = m_QuadTree->Retrieve(&node, swept_bounding_box_pos, swept_bounding_box_size);
#else
		//Loop through the different object types
		Node<GameObject*> *collisionNodeIt = m_CollisionIsland.front_node();
		int size = 0;
		for (int grp = 0; grp < ECOLLISIONGROUP_COUNT; ++ grp)
		{
			//Create a bounding box around all objects and check for AABB collision between the bounding box and the player's swept bounding box
			//To check for potential collision since AABB collisions are generally faster than others.
			//By doing so, we can avoid unnecessarily calculating the more expensive collision checks such as OBB
			VECTOR2 obj_bounding_box_size;
			switch (grp)
			{
				case ECOLLISIONGROUP_LILYPAD:	{	Node<LilyPad*> *it = m_LilyPad.front_node();
													for (int i = 0; i < m_LilyPad.size(); ++ i)
													{
														obj_bounding_box_size = it->data->GetBoundingSize();
														if (CollideAABBAABB(swept_bounding_box_pos, swept_bounding_box_size, it->data->m_Position, obj_bounding_box_size))
														{
															collisionNodeIt->data = it->data;
															collisionNodeIt = collisionNodeIt->next;
															++size;
														}
														it = it->next;
													}
												} break;
				case ECOLLISIONGROUP_SQUAREWALL:{	Node<GameObject*> *it = m_SquareWall.front_node();
													for (int i = 0; i < m_SquareWall.size(); ++ i)
													{
														obj_bounding_box_size = it->data->GetBoundingSize();
														if (CollideAABBAABB(swept_bounding_box_pos, swept_bounding_box_size, it->data->m_Position, obj_bounding_box_size))
														{
															collisionNodeIt->data = it->data;
															collisionNodeIt = collisionNodeIt->next;
															++size;
														}
														it = it->next;
													}
												} break;
				case ECOLLISIONGROUP_ANGLEDWALL:{	Node<GameObject*> *it = m_AngledWall.front_node();
													for (int i = 0; i < m_AngledWall.size(); ++ i)
													{
														obj_bounding_box_size = it->data->GetBoundingSize();
														if (CollideAABBAABB(swept_bounding_box_pos, swept_bounding_box_size, it->data->m_Position, obj_bounding_box_size))
														{
															collisionNodeIt->data = it->data;
															collisionNodeIt = collisionNodeIt->next;
															++size;
														}
														it = it->next;
													}
												} break;
				case ECOLLISIONGROUP_LOGPIVOT:	{
												} break;
				case ECOLLISIONGROUP_LOG:		{	Node<GameObject*> *it = m_Log.front_node();
													for (int i = 0; i < m_Log.size(); ++ i)
													{
														obj_bounding_box_size = it->data->GetBoundingSize();
														if (CollideAABBAABB(swept_bounding_box_pos, swept_bounding_box_size, it->data->m_Position, obj_bounding_box_size))
														{
															collisionNodeIt->data = it->data;
															collisionNodeIt = collisionNodeIt->next;
															++size;
														}
														it = it->next;
													}
												} break;
			}
		}
		//end loop different object types
#endif

#ifdef SPECULATIVE_CONTACT
		VECTOR2 remove (0.f, 0.f);

		//Loop through all collision candidates in the collision island
		Node<GameObject*> *it = m_CollisionIsland.front_node();
		for (int i = 0; i < size; ++ i)
		{
			//If the collision does not need to be resolved
			if (!resolve)
			{
				//Use a more specific collision check depending on the object's collision shape to check if it actually collides
				if (m_Player->CollideObject(it->data))
					//Return true as a collision has been found
					return true;
			}
			//endif collision does not need to be resolved
			//Else if collision need to be resolved
			else
			{
				//Find the pointer nearest to the player's previous position
				//-The point most likely to collide with the player
				VECTOR2 contactPoint = it->data->GetClosestPoint(prevPos);

				//Vector from the player position to the contact point
				VECTOR2 vec = contactPoint - prevPos;

				//Distance between the contact point and the edge of the player
				float dist = Vec2Length(vec) - m_Player->GetCollisionRadius();

				//Project the player velocity onto the player-point vector
				float projSpeed = abs(Vec2ProjComponent(scaledVelocity, vec));
				VECTOR2 projVelocity = Vec2Normalize(vec)*projSpeed;

				//If the distance to the contact point is less than the projected speed
				//i.e. player will hit the point in this frame
				if (dist <= projSpeed)
				{
					//Find the highest velocity to remove from the player
					//So that the player will stop just before colliding

					projVelocity *= 1.001f - dist/projSpeed;

					if (scaledVelocity.x > 0)
						remove.x = max(projVelocity.x, remove.x);
					else if (scaledVelocity.x < 0)
						remove.x = min(projVelocity.x, remove.x);

					if (scaledVelocity.y > 0)
						remove.y = max(projVelocity.y, remove.y);
					else if (scaledVelocity.y < 0)
						remove.y = min(projVelocity.y, remove.y);
				}
				//endif distance less than projected speed
			}
			//endif collision need to be resolved

			//Get the next collision candidate iterator
			it = it->next;
		}
		//end loop collision island

		//Apply the calculated remove vector to the player
		m_Player->m_Position -= remove;

		//Collision is found
		if (Vec2LengthSq(remove) > 0.f)
			return true;
#else
		//Loop through all collision candidates in the collision island
		LinkedList<VECTOR2> nearestPoint;
		Node<GameObject*> *it = m_CollisionIsland.front_node();
		for (int i = 0; i < size; ++ i)
		{
			//Use a more specific collision check depending on the object's collision shape to check if it actually collides
			if (m_Player->CollideObject(it->data))
			{
				//If the collision does not need to be resolved
				if (!resolve)
					//Return true as a collision has been found
					return true;

				//Else if collision need to be resolved
				//Push back the point on the object nearest to the player's previous position
				//-To use when trying to determine which object is the first to collide
				nearestPoint.push_back(it->data->GetClosestPoint(prevPos));
			}
			//endif player collide object

			it = it->next;
		}
		//end loop collision island

		//If the player collided with something
		if (nearestPoint.size() > 0)
		{
			//Get the closest point to the player (First collision in the time sequence)
			Node<VECTOR2> *it = nearestPoint.front_node();
			VECTOR2 p;
			float nearestDistSq = 0.f;
			for (int i = 0; i < nearestPoint.size(); ++ i)
			{
				float distSq = Vec2LengthSq(it->data - prevPos);
				if (nearestDistSq == 0.f || distSq < nearestDistSq)
				{
					nearestDistSq = distSq;
					p = it->data;
				}

				it = it->next;
			}

			//Find the time-step factor to the collision point
			float t = (Vec2Length(p - prevPos) - m_Player->GetCollisionRadius()) / Vec2ProjComponent(newPos - prevPos, p - prevPos);

			//Calculate a vector perpendicular to the collision normal
			VECTOR2 point = GetClosestPointOnLine(p, prevPos, newPos);
			VECTOR2 slideVec = Vec2Normalize(point - p)*distToMove + (newPos - prevPos);
			if (Vec2LengthSq(slideVec) > 1.f)
				slideVec = Vec2Normalize(slideVec);

			//Multiply the player new position by the time-step factor so that it stops just before collision
			m_Player->m_Position = prevPos + scaledVelocity * t;

			//Move the player along the slide vector by the amount of distance it had penetrated the object
			m_Player->m_Position += slideVec * distToMove * (1.f - t);

			//Collision is found
			return true;
		}
		//endif player collided
#endif

		//No collision
		return false;
	}

	/*
	*	Process Input
	*/
	void GameLevel::ProcessInput()
	{

		
		// Open Menu
		if (System::InputManager::GetButtonTrigger(BUTTON_SELECT))
		{
			m_Paused = !m_Paused;
		}
		// Open Menu
		if (System::InputManager::GetButtonTrigger(BUTTON_START))
		{
			m_Paused = !m_Paused;
		}
		//If game is paused
		if (m_Paused)
			//Skip the other game-related input functions
			return;
		
		//L and R bumpers - Swap screen
		if (System::InputManager::GetButtonTrigger(BUTTON_L) || System::InputManager::GetButtonTrigger(BUTTON_R))
			m_SwapScreen = !m_SwapScreen;

		//Single touch on screen
		if (System::InputManager::GetTouchTrigger())
		{
			//Convert the touch position to world position
			VECTOR2 touch = System::InputManager::GetTouchPosition();
			if (m_SwapScreen)
				touch.y = LOWER_SCREEN_HEIGHT - touch.y;
			else
				touch.y += GAME_SCREEN_HEIGHT;

			//Check touch collision against game objects and process an appropriate response
			//If the user did not click on any game elements and the water tile is not shallow water
			if (!CheckTouchCollision(touch) && !m_Water->IsShallowWater(touch.x, touch.y))
			{
				//If there are still water droplets remaining
				if (m_DropsRemaining > 0)
				{
					//Create a ripple at the touch location and update the drops remaining
					m_Water->CreateRipple(touch.x, touch.y, 100);
					--m_DropsRemaining;
					m_DropsRemainingAlpha = 1.0f;
					Sound::GetInstance().SE_Play(SE_RIPPLE_MIN);
				}
				//endif water dropelts remaining
			}
			//endif check touch collision & tile not shallow water
		}


		////D-pad - Debug swan movement
		////!!REMEMBER TO REMOVE ON PUBLIC RELEASE
		//if (System::InputManager::GetButtonPush(BUTTON_UP))		
		//	m_Player->m_Position.y -= 5;
		//if (System::InputManager::GetButtonPush(BUTTON_DOWN))		
		//	m_Player->m_Position.y += 5;
		//if (System::InputManager::GetButtonPush(BUTTON_LEFT))		
		//	m_Player->m_Position.x -= 5;
		//if (System::InputManager::GetButtonPush(BUTTON_RIGHT))		
		//	m_Player->m_Position.x += 5;
		////B button - Reset game
		//if (System::InputManager::GetButtonTrigger(BUTTON_B))
		//{
		//	ClearLevel();
		//	LoadLevel();
		//}
	}

	/*
	*	Check for touch input's collision with game objects and process a response according to the collision group
	*/
	bool GameLevel::CheckTouchCollision(VECTOR2 touch)
	{
		//Loop through the different object types
		for (int grp = 0; grp < ECOLLISIONGROUP_COUNT; ++ grp)
		{
			//Check if the user touched a game element and process an appropriate response
			//e.g. clicking on a log's pivot will rotate it
			//Return true if a collision is found
			switch (grp)
			{
				case ECOLLISIONGROUP_LILYPAD:		{	//Loop through the list of lilypad
														Node<LilyPad*> *it = m_LilyPad.front_node();
														for (int i = 0; i < m_LilyPad.size(); ++ i)
														{
															//If lily pad is touched around the centre
															//Slightly smaller than the actual size so player doesn't accidentally click on it
															if (CollideCirclePoint(it->data->m_Position, it->data->GetCollisionRadius()*0.85f, touch))
															{
																//If the lily pad is occupied...
																if (it->data->IsOccupied())
																{
																	//...by a player
																	if (it->data->GetPlayer() != NULL)
																	{
																		//Move the player such that it will be exactly at the edge of the lily pad
																		float distToMove = m_Player->GetCollisionRadius() + it->data->GetCollisionRadius();
																		m_Player->m_Position += m_Player->m_Direction*distToMove;
																		
																		//Loop through the list of lily pads starting from the current one that the frog is on
																		LilyPad* nearestLilyPad = NULL;
																		float nearestDistSq = 0.f;
																		Node<LilyPad*> *it2 = it;
																		for (int j = 0; j < m_LilyPad.size()-1; ++ j)
																		{
																			//Get the next lily pad in the list
																			it2 = it2->next;

																			//If the centre of the player after dismounting the current lily pad falls on another lily pad that is not occupied
																			if (CollidePointCircle(m_Player->m_Position, it2->data->m_Position, it2->data->GetCollisionRadius()) && !it2->data->IsOccupied())
																			{
																				//If this is the first empty lily pad found or if that lily pad is nearer than another empty lily pad that is in the way
																				float distSq = Vec2LengthSq(m_Player->m_Position - it2->data->m_Position);
																				if (nearestDistSq == 0.f || distSq < nearestDistSq)
																				{
																					//Set it to be the nearest lily pad
																					nearestDistSq = distSq;
																					nearestLilyPad = it2->data;
																				}
																				//endif first empty lily pad or nearer lily pad
																			}
																			//endif player falls onto lily pad
																		}
																		//end loop lily pad

																		//If a empty lily pad was in the way
																		if (nearestLilyPad)
																		{
																			//Unoccupy the current lily pad
																			it->data->SetPlayer(NULL);
																			it->data->SetOccupied(false);

																			//Set the player to occupy the other lily pad
																			m_Player->m_Position = nearestLilyPad->m_Position;
																			m_Player->m_Velocity = VECTOR2(0.f, 0.f);
																			nearestLilyPad->SetPlayer(m_Player);
																			nearestLilyPad->SetOccupied(true);
																			m_Player->SetLilyPad(nearestLilyPad);

																			//Update all frogs to look at the next available lily pad
																			Node<Frog*> *frog = m_Frog.front_node();
																			for (int j = 0; j < m_Frog.size(); ++ j)
																			{
																				frog = frog->next;
																				if (frog == NULL)
																					frog = m_Frog.front_node();

																				frog->data->LookToNext();

																				Sound::GetInstance().SE_Play(SE_LOTASLEAF2);
																			}
																		}
																		//endif empty lily pad in the way
																		//Else if player will collide with an game object if it dismounts the lily pad
																		else if (CheckPlayerCollision(m_Player->m_Position, m_Player->m_Position, 0.f, false))
																		{
																			//Set the player position back onto the lily pad
																			m_Player->m_Position = it->data->m_Position;
																		}
																		//endif player collide with game object
																		//Else if it is free space
																		else
																		{
																			//Unoccupy the current lily pad
																			it->data->SetPlayer(NULL);
																			it->data->SetOccupied(false);
																			m_Player->SetLilyPad(NULL);


																			//Add some speed to make player slide a short distance away from the lily pad
																			m_Player->m_Velocity = m_Player->m_Direction*20.f;

																			//Update all frogs to look at the next available lily pad
																			Node<Frog*> *frog = m_Frog.front_node();

																			for (int j = 0; j < m_Frog.size(); ++ j)
																			{
																				frog = frog->next;
																				if (frog == NULL)
																					frog = m_Frog.front_node();

																				frog->data->LookToNext();
																			}

																			Sound::GetInstance().SE_Play(SE_LOTASLEAF2);
																		}
																		//endif free space
																	}
																	//endif lily pad occupied by player
																	//Else if lily pad is occupied by something else (frog or flower)
																	else
																	{
																		//Get the pointer to the frog on the lilypad, if any
																		Node<Frog*>* frog = it->data->GetFrogNode();

																		//If there is a frog on the lilypad (Not a null pointer)
																		if (frog)
																		{
																			//Order the frog to move to the next available lily pad
																			//If the frog was successfully ordered to move
																			if (frog->data->MoveToNext())
																			{
																				//Update all frogs to look at the next available lily pad
																				for (int j = 0; j < m_Frog.size()-1; ++ j)
																				{
																					frog = frog->next;
																					if (frog == NULL)
																						frog = m_Frog.front_node();

																					frog->data->LookToNext();
																				}

																				Sound::GetInstance().SE_Play(SE_FROG);
																			}
																			//endif frog successfully move
																		}
																		//endif there is frog

																		//Else if flower, do nothing
																	}
																	//endif lily pad occupied by something else
																}
																//endif lily pad is occupied
																//Else if lily pad is not occupied
																else
																{
																	//If player is not dead and is not on a lily pad already
																	//And the player is near the lily pad
																	if	(	m_Player->IsActive()
																		&&	!m_Player->GetLilyPad()
																		&&	CollideCircleCircle(it->data->m_Position, it->data->GetCollisionRadius()*1.2f, m_Player->m_Position, m_Player->GetCollisionRadius())
																		)
																	{
																		//Set the player to occupy the lily pad
																		m_Player->m_Position = it->data->m_Position;
																		m_Player->m_Velocity = VECTOR2(0.f, 0.f);
																		m_Player->SetLilyPad(it->data);
																		it->data->SetPlayer(m_Player);
																		it->data->SetOccupied(true);

																		//Update all frogs to look at the next available lily pad
																		Node<Frog*> *frog = m_Frog.front_node();
																		for (int j = 0; j < m_Frog.size(); ++ j)
																		{
																			frog = frog->next;
																			if (frog == NULL)
																				frog = m_Frog.front_node();

																			frog->data->LookToNext();
																		}

																		Sound::GetInstance().SE_Play(SE_LOTASLEAF2);
																	}
																	//endif lily pad mounting conditions met
																}
																//endif lily pad not occupied

																//Collision found!
																return true;
															}
															//endif touched around centre
															//Else if user touches the other parts of the lily pad
															else if (CollideCirclePoint(it->data->m_Position, it->data->GetCollisionRadius(), touch))
															{
																//Collision found!
																return true;
															}
															//endif touched other parts of lily pad

															//Get the next iterator
															it = it->next;
														}
														//end loop lily pad
													} break;
				case ECOLLISIONGROUP_SQUAREWALL:	{	//Loop through the list of square walls
														Node<GameObject*> *it = m_SquareWall.front_node();
														for (int i = 0; i < m_SquareWall.size(); ++ i)
														{
															//If square wall is touched
															if (CollideAABBPoint(it->data->m_Position, it->data->m_Size, touch))
																//Collision found!
																return true;

															//Get the next iterator
															it = it->next;
														}
														//end loop square wall
													} break;
				case ECOLLISIONGROUP_ANGLEDWALL:	{	//Loop through the list of angled walls
														Node<GameObject*> *it = m_AngledWall.front_node();
														for (int i = 0; i < m_AngledWall.size(); ++ i)
														{
															//If angled wall is touched
															if (CollideTrianglePoint(it->data->m_Vertex.get(0), it->data->m_Vertex.get(1), it->data->m_Vertex.get(2), touch))
																//Collision found!
																return true;
															
															//Get the next iterator
															it = it->next;
														}
														//end loop angled wall
													} break;
				case ECOLLISIONGROUP_LOGPIVOT:		{	//Loop through the list of log pivots
														Node<LogPivot*> *it = m_LogPivot.front_node();
														for (int i = 0; i < m_LogPivot.size(); ++ i)
														{
															//If log pivot is touched
															if (CollideCirclePoint(it->data->m_Position, it->data->GetCollisionRadius(), touch))
															{
																//Rotate the log which the pivot is linked to
																GameObject* log = it->data->GetLog();
																VECTOR2 prevDir = log->m_Direction;
																float rad = PI*0.25f;
																/*if (sign(touch, log->m_Position - log->m_Direction, log->m_Position) >= 0)
																	rad *= -1;*/
																log->m_Direction = Vec2RotateRad(log->m_Direction, rad);

																//If rotating the log will collide with the player
																if (m_Player->CollideObject(log))
																	//Reverse the log rotation
																	log->m_Direction = prevDir;
																else
																	Sound::GetInstance().SE_Play(SE_LOG);

																//Collision found!
																return true;
															}
															//endif touched log pivot

															//Get the next iterator
															it = it->next;
														}
														//end loop log pivot
													} break;
				case ECOLLISIONGROUP_LOG:			{	//Loop through the list of fixed logs
														Node<GameObject*> *it = m_Log.front_node();
														for (int i = 0; i < m_Log.size(); ++ i)
														{
															//If log is touched
															if (CollideOBBPoint(it->data->m_Position, it->data->m_Size, it->data->m_Direction, touch))
																//Collision found!
																return true;

															//Get the next iterator
															it = it->next;
														}
														//end loop fixed logs
													} break;
			}
		}
		//end loop different object types

		//No collision found
		return false;
	}

	/***************************************************
	*	Rendering functions
	****************************************************/

	/*
	*	上画面描画
	*/
	void GameLevel::DrawUpper()
	{
		if (m_Paused)
		{
			m_MenuObj->DrawUpper();
			// Stage Board
			SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_BOARD),UPPER_SCREEN_WIDTH/2-128.0f, 0.f,256.0f, 128.0f, 0.f, 0.0f, 256.0f, 128.0f);
			// Stage, StageNum, -, LevelNum
			SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_FONT),130.f          , 55.f, 64.f, 32.f,  0.f,  0.f, 64.f, 32.f);
			SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_FONT),130.f+70.f     , 55.f, 25.f, 32.f, 25.f*((m_Level-1)/5+1), 32.f, 25.f, 32.f);
			SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_FONT),130.f+70.f+25.f, 55.f, 25.f, 32.f,160.f,  0.f, 25.f, 32.f);
			SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_FONT),130.f+70.f+50.f, 55.f, 25.f, 32.f, 25.f*((m_Level-1)%5+1), 32.f, 25.f, 32.f);
			return;
		}

		
		/*
		*	Draw water
		*/
		m_Water->DrawUpperWater(m_SwapScreen);

		//Whirlpools
		{
			Node<Whirlpool*> *it = m_Whirlpool.front_node();
			for (int i = 0; i < m_Whirlpool.size(); ++ i)
			{
				it->data->Render(SCREEN_WIDTH_DIFF*0.5f, 0.f, m_SwapScreen);
				it = it->next;
			}
		}

		//Ripple
		m_Water->DrawUpperRipple(m_SwapScreen);

		//Piranhas
		{
			Node<Piranha*> *it = m_Piranha.front_node();
			for (int i = 0; i < m_Piranha.size(); ++ i)
			{
				it->data->Render(SCREEN_WIDTH_DIFF*0.5f, 0.f, m_SwapScreen);
				it = it->next;
			}
		}

		//Square walls
		{
			Node<GameObject*> *it = m_SquareWall.front_node();
			for (int i = 0; i < m_SquareWall.size(); ++ i)
			{
				it->data->Render(SCREEN_WIDTH_DIFF*0.5f, 0.f, m_SwapScreen);
				it = it->next;
			}
		}


		//Angled walls
		{
			Node<GameObject*> *it = m_AngledWall.front_node();
			for (int i = 0; i < m_AngledWall.size(); ++ i)
			{
				it->data->Render(SCREEN_WIDTH_DIFF*0.5f, 0.f, m_SwapScreen);
				it = it->next;
			}
		}

		//Logs
		{
			Node<GameObject*> *it = m_Log.front_node();
			for (int i = 0; i < m_Log.size(); ++ i)
			{
				it->data->Render(SCREEN_WIDTH_DIFF*0.5f, 0.f, m_SwapScreen);
				it = it->next;
			}
		}

		//Lily pads
		{
			Node<LilyPad*> *it = m_LilyPad.front_node();
			for (int i = 0; i < m_LilyPad.size(); ++ i)
			{
				it->data->Render(SCREEN_WIDTH_DIFF*0.5f, 0.f, m_SwapScreen);
				it = it->next;
			}
		}

		//Frogs
		{
			Node<Frog*> *it = m_Frog.front_node();
			for (int i = 0; i < m_Frog.size(); ++ i)
			{
				it->data->Render(SCREEN_WIDTH_DIFF*0.5f, 0.f, m_SwapScreen);
				it = it->next;
			}
		}

		//If player is not dead
		if (m_Player->IsActive())
			//Draw the player
			m_Player->Render(SCREEN_WIDTH_DIFF*0.5f, 0.f, m_SwapScreen);
		//Draw the goal
		m_Goal->Render(SCREEN_WIDTH_DIFF*0.5f, 0.f, m_SwapScreen);

		//Splash
		{
			Node<Splash*> *it = m_Splash.front_node();
			for (int i = 0; i < m_Splash.size(); ++ i)
			{
				it->data->Render(SCREEN_WIDTH_DIFF*0.5f, 0.f, m_SwapScreen);
				it = it->next;
			}
		}

		/*
		*	Draw HUD
		*/
		SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_WATER),
							0.f, 0.f,
							SCREEN_WIDTH_DIFF*0.5f, GAME_SCREEN_HEIGHT,
							0.f, 0.f, SCREEN_WIDTH_DIFF*0.5f, GAME_SCREEN_HEIGHT
						);
		SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_WATER),
							GAME_SCREEN_WIDTH + SCREEN_WIDTH_DIFF*0.5f, 0.f,
							SCREEN_WIDTH_DIFF*0.5f, GAME_SCREEN_HEIGHT,
							GAME_SCREEN_WIDTH + SCREEN_WIDTH_DIFF*0.5f, 0.f, SCREEN_WIDTH_DIFF*0.5f, GAME_SCREEN_HEIGHT
						);

		// Scale
		static int addScale = 0;

		// Left Wood frame
		addScale = 0;
		SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_UI),
							3.f-addScale*0.5, 60.f-addScale*0.5,
							35.f+addScale, 50.f+addScale,
							143.f, 0.f, 35.f, 50.f
						);
		// X
		SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_NUMBER),
							14.0f+addScale*0.5, 110.f-addScale*0.5,
							12.f+addScale, 16.f+addScale,
							12.f, 16.f, 12.f, 16.f
						);
		// Num of m_DropsRemaining
		if (m_DropsRemaining >= 10)
		{
			SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_NUMBER),
								8.f-addScale*0.5, 135.f-addScale*0.5,
								12.f+addScale, 16.f+addScale,
								m_DropsRemaining/10*12.f, 0.f, 12.f, 16.f
							);
		}
		SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_NUMBER),
							8.f+12.0f-addScale*0.5, 135.f-addScale*0.5,
							12.f+addScale, 16.f+addScale,
							m_DropsRemaining%10*12.f, 0.f, 12.f, 16.f
						);
		
		// Right Wood frame
		// Switch Screen Icon addScale
		addScale = 0;
		if ((m_Player->m_Position.y < 220 && !m_SwapScreen) || (m_Player->m_Position.y > 260 && m_SwapScreen) )
		{
			addScale = sinf(static_cast<float>(m_ElapsedFrame % 30) *PI/15) * 10.0f;
			if (addScale < 0) addScale = 0;
		}
		// L/R
		SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_UI),
							364.f-addScale*0.5, 70.f-addScale*0.5,
							35.f+addScale, 50.f+addScale,
							283.f, 0.f, 35.f, 50.f
						);
		// FLIP
		SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_UI),
							364.f-addScale*0.5, 140.f-addScale*0.5,
							35.f+addScale, 50.f+addScale,
							248.f-(m_SwapScreen*35.0f), 0.f, 35.f, 50.f
						);		
		
		//	Draw Tutorial Upper
		if ( this->theTutorial->getIsThereOne() )
			this->theTutorial->RenderUpper();
#ifdef QUADTREE_DEBUG_DRAW
		m_QuadTree->DrawUpper();
#endif
	}

	/*
	*	下画面描画
	*/
	void GameLevel::DrawLower()
	{
		if (m_Paused)
		{
			m_MenuObj->DrawLower();
			return;
		}
		/*
		*	Draw water
		*/
		m_Water->DrawLowerWater(m_SwapScreen);
		
		//Whirlpools
		{
			Node<Whirlpool*> *it = m_Whirlpool.front_node();
			for (int i = 0; i < m_Whirlpool.size(); ++ i)
			{
				it->data->Render(0.f, -UPPER_SCREEN_HEIGHT, m_SwapScreen);
				it = it->next;
			}
		}

		//Ripple
		m_Water->DrawLowerRipple(m_SwapScreen);

		//Piranhas
		{
			Node<Piranha*> *it = m_Piranha.front_node();
			for (int i = 0; i < m_Piranha.size(); ++ i)
			{
				it->data->Render(0.f, -UPPER_SCREEN_HEIGHT, m_SwapScreen);
				it = it->next;
			}
		}

		//Square walls
		{
			Node<GameObject*> *it = m_SquareWall.front_node();
			for (int i = 0; i < m_SquareWall.size(); ++ i)
			{
				it->data->Render(0.f, -UPPER_SCREEN_HEIGHT, m_SwapScreen);
				it = it->next;
			}
		}

		//Angled walls
		{
			Node<GameObject*> *it = m_AngledWall.front_node();
			for (int i = 0; i < m_AngledWall.size(); ++ i)
			{
				it->data->Render(0.f, -UPPER_SCREEN_HEIGHT, m_SwapScreen);
				it = it->next;
			}
		}

		//Logs
		{
			Node<GameObject*> *it = m_Log.front_node();
			for (int i = 0; i < m_Log.size(); ++ i)
			{
				it->data->Render(0.f, -UPPER_SCREEN_HEIGHT, m_SwapScreen);
				it = it->next;
			}
		}

		//Lily pads
		{
			Node<LilyPad*> *it = m_LilyPad.front_node();
			for (int i = 0; i < m_LilyPad.size(); ++ i)
			{
				it->data->Render(0.f, -UPPER_SCREEN_HEIGHT, m_SwapScreen);
				it = it->next;
			}
		}

		//Frogs
		{
			Node<Frog*> *it = m_Frog.front_node();
			for (int i = 0; i < m_Frog.size(); ++ i)
			{
				it->data->Render(0.f, -UPPER_SCREEN_HEIGHT, m_SwapScreen);
				it = it->next;
			}
		}

		// Ryugo. When generating the ripple to show the m_DropsRemaining.
		if (m_DropsRemainingAlpha > 0.0f) m_DropsRemainingAlpha -= 0.05f;
		if (m_DropsRemaining >= 10)
		{
			SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_NUMBER),
								System::InputManager::GetTouchPosition().x-12.f, System::InputManager::GetTouchPosition().y-16.f*0.5f,
								12.f, 16.f,
								m_DropsRemaining/10*12.f, 0.f, 12.f, 16.f,
								0.f, 0.f, 0.f, m_DropsRemainingAlpha
							);
		}		
		SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_NUMBER),
							System::InputManager::GetTouchPosition().x, System::InputManager::GetTouchPosition().y-16.f*0.5f,
							12.f, 16.f,
							m_DropsRemaining%10*12.f, 0.f, 12.f, 16.f,
							0.f, 0.f, 0.f, m_DropsRemainingAlpha
						);

		// DEBUG Ryugo Screen Border
		SpriteDrawer::Draw(	TextureManager::Get(ETEXTURE_WATER),
							0.f, 0.f,
							320.f, 1.f,
							0.f, 0.f, 1.f, 1.f
						);
		//If player is not dead
		if (m_Player->IsActive())
			//Draw the player
			m_Player->Render(0.f, -UPPER_SCREEN_HEIGHT, m_SwapScreen);
		//Draw the goal
		m_Goal->Render(0.f, -UPPER_SCREEN_HEIGHT, m_SwapScreen);

		//Splash
		{
			Node<Splash*> *it = m_Splash.front_node();
			for (int i = 0; i < m_Splash.size(); ++ i)
			{
				it->data->Render(0.f, -UPPER_SCREEN_HEIGHT, m_SwapScreen);
				it = it->next;
			}
		}

		//	Draw Tutorial Lower
		if ( this->theTutorial->getIsThereOne() )
			this->theTutorial->RenderLower();

#ifdef QUADTREE_DEBUG_DRAW
		m_QuadTree->DrawLower();
#endif
	}
}