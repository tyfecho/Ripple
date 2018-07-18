/*
* Water.cpp
*/

/***************************************************
*	File includes
***************************************************/

#include "../NW_Lib/Graphics/NW_Lib_SpriteDrawer.h"
#include "../NW_Lib/Graphics/NW_Lib_TextureManager.h"
#include "../NW_Lib/Graphics/NW_Lib_texture.h"
#ifdef _3DS
	#include "../../NW_Lib/Graphics/NW_Lib_ResTextureManager.h"
#endif

#include "../NW_Lib/Utility/NW_Lib_DebugFontManager.h"

#include "GameObject.h"
#include "Water.h"
#include <math.h>
#include <iostream>

using namespace NW_Lib;

/***************************************************
*	Game namespace
***************************************************/

namespace Game
{
	/***************************************************
	*	Water Class
	***************************************************/
	//	-Handles the drawing and updating of the water texture and the creation and updating of ripples
	//	-Ripples are drawn by updating the water texture overlay's alpha channel depending on its current position and strength
	//	-Game object lists from the game level must be passed to the water in order for the ripple to detect them using line-collisions

	/*
	*	コンストラクタ
	*/
	Water::Water(int waterTexID, int rippleTexID)
		: m_Player(NULL)
#ifdef SPATIAL_PARTITIONING
		, m_QuadTree(NULL)
#else
		, m_SquareWall(NULL)
		, m_AngledWall(NULL)
		, m_LilyPad(NULL)
		, m_Log(NULL)
#endif
#ifdef _DirectX
		, m_RippleTextureID(rippleTexID)
#endif
#ifdef _3DS
		, m_RippleTextureIndex(NULL)
#endif
	{

		//Calculate the nearest power of 2 for the width and height of the texture file
		//e.g. 70 -> 128, 240 -> 256
		m_RippleTextureWidth = GetNextPower(2, WATER_TILE_COL);
		m_RippleTextureHeight = GetNextPower(2, WATER_TILE_ROW);

		//Allocate memory for the dynamic arrays
		//-Create a water level 2D array of water tiles
		m_WaterLevel = new float*[m_RippleTextureWidth];
		for (int i = 0; i < m_RippleTextureWidth; ++ i)
		{
			m_WaterLevel[i] = new float[m_RippleTextureHeight];
			for (int j = 0; j < m_RippleTextureHeight; ++ j)
				m_WaterLevel[i][j] = 0;
		}

		//-Create a shallow water 2D array of level tiles
		m_ShallowWater = new bool*[LEVEL_TILE_COL];
		for (int i = 0; i < LEVEL_TILE_COL; ++ i)
		{
			m_ShallowWater[i] = new bool[LEVEL_TILE_ROW];
			for (int j = 0; j < LEVEL_TILE_ROW; ++ j)
				m_ShallowWater[i][j] = false;
		}

		//Initialize game objects
		TextureManager::Entry(ETEXTURE_WATERBED, "water_bed");
		m_WaterBed = new GameObject();
		m_WaterBed->SetTexture(ETEXTURE_WATERBED, GAME_SCREEN_WIDTH, TOTAL_SCREEN_HEIGHT);
		m_WaterBed->m_Size = VECTOR2(GAME_SCREEN_WIDTH, TOTAL_SCREEN_HEIGHT);
		m_WaterBed->m_Position = m_WaterBed->m_Size*0.5f;

		m_WaterBody = new GameObject();
		m_WaterBody->SetTexture(waterTexID, GAME_SCREEN_WIDTH, TOTAL_SCREEN_HEIGHT, SCREEN_WIDTH_DIFF*0.5f);
		m_WaterBody->m_Size = VECTOR2(GAME_SCREEN_WIDTH, TOTAL_SCREEN_HEIGHT);
		m_WaterBody->m_Position = m_WaterBody->m_Size*0.5f;

		m_WaterRipple = new GameObject();
		m_WaterRipple->SetTexture(rippleTexID, WATER_TILE_COL, WATER_TILE_ROW);
		m_WaterRipple->m_Size = VECTOR2(GAME_SCREEN_WIDTH, TOTAL_SCREEN_HEIGHT);
		m_WaterRipple->m_Position = m_WaterRipple->m_Size*0.5f;

#ifdef _3DS
		/*
		*	Initialize ripple texture
		*/

		m_RippleTextureData = TextureManager::Get(m_RippleTextureID)->GetImage();

		//Calculate the nearest power of 2 for the width and height of the texture file
		int width = GetNextPower(2, WATER_TILE_COL);
		int height = GetNextPower(2, WATER_TILE_ROW);

		m_RippleTextureData.SetWidth(width);
		m_RippleTextureData.SetHeight(height);

		//Pre-calculate the indexes of the ripple texture and initialize the alpha values to 0
		m_RippleTextureIndex = new int*[WATER_TILE_COL];
		for (int i = 0; i < WATER_TILE_COL; ++ i)
		{
			m_RippleTextureIndex[i] = new int[WATER_TILE_ROW];
			for (int j = 0; j < WATER_TILE_ROW; ++ j)
			{
				int index = (i%4)*2 + ((i/4)%2)*16 + (i/8)*64 
							+ (j%2) + ((j%4)/2)*32 + (j/4)*512;
					
				m_RippleTextureData.SetImageData(index, 0x00);
				m_RippleTextureIndex[i][j] = index;
			}
		}
		if( m_RippleTextureData.CheckMemoryLocation() )
		{
			nngxAddVramDmaCommand	(m_RippleTextureData.GetImageData()
									,(GLvoid*)m_RippleTextureData.GetLocationAddress()
									,m_RippleTextureData.GetImageDataCount() 
									);
		}
#endif
	}

	/*
	*	デストラクタ
	*/
	Water::~Water()
	{
		//Delete dynamic arrays
		for (int i = 0; i < m_RippleTextureWidth; ++ i)
		{
			delete[] m_WaterLevel[i];
			m_WaterLevel[i] = NULL;
		}
		delete[] m_WaterLevel;
		m_WaterLevel = NULL;

		for (int i = 0; i < LEVEL_TILE_COL; ++ i)
		{
			delete[] m_ShallowWater[i];
			m_ShallowWater[i] = NULL;
		}
		delete[] m_ShallowWater;
		m_ShallowWater = NULL;

		//Delete game objects
		if (m_WaterBed)
		{
			delete m_WaterBed;
			m_WaterBed = NULL;
		}

		if (m_WaterBody)
		{
			delete m_WaterBody;
			m_WaterBody = NULL;
		}

		if (m_WaterRipple)
		{
			delete m_WaterRipple;
			m_WaterRipple = NULL;
		}

#ifdef _3DS
		//Delete ripple texture index array
		for (int i = 0; i < WATER_TILE_COL; ++ i)
		{
			delete[] m_RippleTextureIndex[i];
		}
		delete[] m_RippleTextureIndex;
		m_RippleTextureIndex = NULL;
#endif

		//Set the object pointers to NULL
		//No need to delete as they will be deleted in the GameLevel class destructor
		m_Player = NULL;
#ifdef SPATIAL_PARTITIONING
		m_QuadTree = NULL;
		m_CollisionIsland = NULL;
#else
		m_SquareWall = NULL;
		m_AngledWall = NULL;
		m_LilyPad = NULL;
		m_Log = NULL;
#endif
		
		//Delete the list of ripples
		ClearRipple();
	}

	/*
	*	Initialize pointers to game objects
	*/
	void Water::InitPointers(GameObject* player
#ifdef SPATIAL_PARTITIONING
							,QuadTree* quadTree
							,LinkedList<GameObject*>* collisionIsland
#else
							,LinkedList<GameObject*>* squareWall
							,LinkedList<GameObject*>* angledWall
							,LinkedList<LilyPad*>* lilyPad
							,LinkedList<GameObject*>* log
#endif
							)
	{
		m_Player = player;
#ifdef SPATIAL_PARTITIONING
		m_QuadTree = quadTree;
		m_CollisionIsland = collisionIsland;
#else
		m_SquareWall = squareWall;
		m_AngledWall = angledWall;
		m_LilyPad = lilyPad;
		m_Log = log;
#endif
	}

	/*
	*	Initialize the water bed texture by changing the alpha values of the texture depending on whether the tile is shallow water
	*/
	void Water::InitWaterBedTexture()
	{
#ifdef _DirectX
		//Open the file for writing
		FILE *fp = NULL;
		fopen_s(&fp, ".\\works_romfiles\\texture\\water_bed_temp.tga", "w");
		if (fp == NULL) return;
		
		TextureManager::Remove(ETEXTURE_WATERBED);

		//Set and write image header values
		char header[ 18 ] = { 0 };
		header[ 2 ] = 2; // truecolor
		header[ 12 ] = m_WaterBed->GetTexture().width & 0xFF;
		header[ 13 ] = (m_WaterBed->GetTexture().width >> 8) & 0xFF;
		header[ 14 ] = m_WaterBed->GetTexture().height & 0xFF;
		header[ 15 ] = (m_WaterBed->GetTexture().height >> 8) & 0xFF;
		header[ 16 ] = 32; // bits per pixel
		fwrite((const char*)&header, 1, sizeof(header), fp);
 
		//Image data is stored bottom-to-top, left-to-right
		//Loop from bottom to top
		for (int j = m_WaterBed->GetTexture().height-1; j >= 0; --j)
		{
			//Loop from left to right
			for (int i = 0; i < m_WaterBed->GetTexture().width; ++i)
			{
				int alpha = 0;

				//If writing a water tile
				if (j < TOTAL_SCREEN_HEIGHT && i < GAME_SCREEN_WIDTH)
				{
					if (IsShallowWater(i, j))
						alpha = 64;
				}
				//endif writing water tile

				//Write the color and alpha values
				putc(0xFF,fp);
				putc(0xFF,fp);
				putc(0xFF,fp);
				putc((int)(alpha & 0xFF),fp);
			}
			//end loop left to right
		}
		//end loop bottom to top

		//Set and write image footer values
		static const char footer[ 26 ] =
		"\0\0\0\0" // no extension area
		"\0\0\0\0" // no developer directory
		"TRUEVISION-XFILE" // tga file
		".";
		fwrite((const char*)&footer, 1, sizeof(footer), fp);
 
		//Close the file
		fclose(fp);
		
		TextureManager::Entry(ETEXTURE_WATERBED, "water_bed_temp");
#endif
#ifdef _3DS
		u8 alpha;
		int temp;

		//Loop through all columns of water
		for (int i = 0; i < GAME_SCREEN_WIDTH; ++ i)
		{
			//Loop through all rows of water
			for (int j = 0; j < TOTAL_SCREEN_HEIGHT; ++ j)
			{
				//
				if (IsShallowWater(i, j))
					alpha = 0x88;
				else
					alpha = 0x00;
				
				int index = ;

				//Set the texture data
				m_WaterBedTextureData.SetImageData(index, alpha);
			}
		}
		//DMA the updated texture data
		if( m_WaterBedTextureData.CheckMemoryLocation() )
		{
			nngxAddVramDmaCommand	(m_WaterBedTextureData.GetImageData()
									,(GLvoid*)m_WaterBedTextureData.GetLocationAddress()
									,m_WaterBedTextureData.GetImageDataCount() 
									);
		}
#endif
	}

	/*
	*	Clear all the ripples
	*/
	void Water::ClearRipple()
	{
		Node<Ripple*> *it = m_Ripple.front_node();
		for (int i = 0; i < m_Ripple.size(); ++ i)
		{
			delete it->data;
			it->data = NULL;

			it = it->next;
		}
		m_Ripple.clear();
	}

	/*
	*	Create a ripple at the specified position
	*/
	void Water::CreateRipple(int x, int y, float strength, LilyPad* lilyPad)
	{
		for (int i = 0; i < RIPPLE_SLICE; ++ i)
		{
			m_Ripple.push_back(new Ripple(VECTOR2(x, y), VECTOR2(fast_sin_deg(i*RIPPLE_DEG_STEP), fast_cos_deg(i*RIPPLE_DEG_STEP)), strength, lilyPad));
		}
	}

	/*
	*	Update the water ripples
	*/
	void Water::UpdateWaterRipple(float deltaTime)
	{
		//Reset the water level for recalculation purposes
		for (int i = 0; i < m_RippleTextureWidth; ++ i)
			for (int j = 0; j < m_RippleTextureHeight; ++ j)
				m_WaterLevel[i][j] = 0.f;

		//Loop through all ripple vectors
		Node<Ripple*> *it = m_Ripple.front_node();
		for (int i = 0; i < m_Ripple.size(); ++ i)
		{
			//Update the ripple vector position
			it->data->prevPos = it->data->currPos;
			float factor = it->data->strength * 0.005f;
			factor = clamp(factor, 1.f, 2.f);
			it->data->currPos += it->data->dir*RIPPLE_SPEED*deltaTime*factor;

			//Update the ripple strength
			it->data->strength -= it->data->strength * RIPPLE_DECAY_EXP * deltaTime;
			it->data->strength -= RIPPLE_DECAY_LINEAR * deltaTime;

			//If the ripple has subsided
			if (it->data->strength <= 0.f)
			{
				//Delete the ripple and continue to the next iteration
				it = it->next;
				m_Ripple.erase(i--);
				continue;
			}

			//Check collision with the various game objects
			//If a collision was found
			if (CheckRippleCollision(it->data))
			{
				//Delete the ripple and continue to the next iteration
				it = it->next;
				m_Ripple.erase(i--);
				continue;
			}
			//end collision with game objects

			//If a pointer to the player is set (Game scene)
			//-Pointer will/should be null if implemented in non-game scenes, e.g. main menu
			if (m_Player)
			{
				//If the ripple vector collides with the player
				if (m_Player->CollideLine(it->data->prevPos, it->data->currPos))
				{
					//Add player velocity based on the ripple direction and strength
					m_Player->m_Velocity += it->data->dir*it->data->strength*it->data->multiplier*RIPPLE_SPEED*0.001f;
				}
				//endif ripple collide player
			}
			//endif player pointer

			//If the ripple vector has gone out of the screen
			if (	it->data->currPos.x < 0.f || it->data->currPos.x > GAME_SCREEN_WIDTH
				||	it->data->currPos.y < 0.f || it->data->currPos.y > TOTAL_SCREEN_HEIGHT
				)
			{
				//Delete the ripple and continue to the next iteration
				it = it->next;
				m_Ripple.erase(i--);
				continue;
			}
			//endif ripple out of screen

			//Add the strength of the ripple to the water tile it is currently in
			m_WaterLevel[(int)(it->data->currPos.x/WATER_TILE_SIZE)][(int)(it->data->currPos.y/WATER_TILE_SIZE)] += it->data->strength;

			//Get the next iterator
			it = it->next;
		}
		//end loop ripple
	}
	
	/*
	*	Check for ripple's collision with other game objects and process a response according to the collision group
	*/
	bool Water::CheckRippleCollision(Ripple* ripple)
	{
		//Point and the normal vector of the collision
		VECTOR2 collisionPoint;
		VECTOR2 collisionNormal;

#ifdef SPATIAL_PARTITIONING
		//Calculate the swept bounding box of the ripple's movement this frame
		VECTOR2 vec = ripple->currPos - ripple->prevPos;
		VECTOR2 swept_bounding_box_pos = ripple->prevPos + vec*0.5f;
		VECTOR2 swept_bounding_box_size;
		swept_bounding_box_size.x = abs(vec.x);
		swept_bounding_box_size.y = abs(vec.y);

		//Retrieve the list of potential collision candidates from the spatial partitioning
		Node<GameObject*>* node = m_CollisionIsland->front_node();
		int size = m_QuadTree->Retrieve(&node, swept_bounding_box_pos, swept_bounding_box_size);

		//Loop through all collision candidates in the collision island
		Node<GameObject*> *it = m_CollisionIsland->front_node();
		for (int i = 0; i < size; ++ i)
		{
			if (it->data->CollideLine(ripple->prevPos, ripple->currPos, &collisionPoint, &collisionNormal))
			{
				switch (it->data->GetObjectID())
				{
					case ELEVELDATA_LILYPAD:	{	//If a lily pad pointer was set
													//i.e. Ripple was generated by a frog occupying this lily pad
													if (ripple->lilyPad == it->data)
													{
														//Don't check against this lily pad so that ripple isn't deleted!
														it = it->next;
														continue;
													}
													//endif lily pad pointer set

													//Add the ripple strength to the lily pad
													static_cast<LilyPad*>(it->data)->AddDisturbance(ripple->strength);
														
													//Exit the function and signal for the ripple to be deleted
													return true;
												} break;
					case ELEVELDATA_WALL:		{	//Diminish the strength
													ripple->strength *= 0.4f;

													//Reset the ripple multiplier
													ripple->multiplier = 1.f;

													//Reflect the direction of the ripple along the normal
													ripple->dir = Vec2Reflect(ripple->dir, collisionNormal);

													//Calculate the new position of the vector based on the new direction and excess distance it has penetrated the AABB
													ripple->currPos = collisionPoint + ripple->dir*Vec2Length(ripple->currPos - collisionPoint);
												} break;
					case ELEVELDATA_LOG:		{	//Increase the ripple multiplier so that the player will be moved by the reflected vector more easily
													ripple->multiplier = 4.f;

													//Reflect the direction of the ripple along the normal
													ripple->dir = Vec2Reflect(ripple->dir, collisionNormal);

													//Calculate the new position of the vector based on the new direction and excess distance it has penetrated the AABB
													ripple->currPos = collisionPoint + ripple->dir*Vec2Length(ripple->currPos - collisionPoint);
												} break;
				}
			}
			it = it->next;
		}
#else
		//Loop through the different object types
		for (int grp = 0; grp < ECOLLISIONGROUP_COUNT; ++ grp)
		{
			switch (grp)
			{
				case ECOLLISIONGROUP_LILYPAD:	{	//Loop through the list of lilypad
													Node<LilyPad*> *it = m_LilyPad->front_node();
													for (int i = 0; i < m_LilyPad->size(); ++ i)
													{
														//If a lily pad pointer was set
														//i.e. Ripple was generated by a frog occupying this lily pad
														if (ripple->lilyPad == it->data)
														{
															//Don't check against this lily pad so that ripple isn't deleted!
															it = it->next;
															continue;
														}
														//endif lily pad pointer set

														//If the ripple vector colldes
														if (CollideLineCircle(ripple->prevPos, ripple->currPos, it->data->m_Position, it->data->GetCollisionRadius()))
														{
															//Add the ripple strength to the lily pad
															it->data->AddDisturbance(ripple->strength);

															//Exit the function and signal for the ripple to be deleted
															return true;
														}
														//endif ripple collide

														//Get the next iterator
														it = it->next;
													}
													//end loop lilypad
												} break;
				case ECOLLISIONGROUP_SQUAREWALL:{	//Loop through the list of square wall
													Node<GameObject*> *it = m_SquareWall->front_node();
													for (int i = 0; i < m_SquareWall->size(); ++ i)
													{

														//If the ripple vector collides
														if (CollideLineAABB(ripple->prevPos, ripple->currPos, it->data->m_Position, it->data->m_Size, &collisionPoint, &collisionNormal))
														{
															//Diminish the strength
															ripple->strength *= 0.2f;

															//Reset the ripple multiplier
															ripple->multiplier = 1.f;

															//Reflect the direction of the ripple along the normal
															ripple->dir = Vec2Reflect(ripple->dir, collisionNormal);

															//Calculate the new position of the vector based on the new direction and excess distance it has penetrated the AABB
															ripple->currPos = collisionPoint + ripple->dir*Vec2Length(ripple->currPos - collisionPoint);
														}
														//endif ripple collide

														//Get the next iterator
														it = it->next;
													}
													//end loop
												} break;
				case ECOLLISIONGROUP_ANGLEDWALL:{	//Loop through the list of angled wall
													Node<GameObject*> *it = m_AngledWall->front_node();
													for (int i = 0; i < m_AngledWall->size(); ++ i)
													{

														//If the ripple vector collides
														if (CollideLineTriangle(ripple->prevPos, ripple->currPos, it->data->m_Vertex.get(0), it->data->m_Vertex.get(1), it->data->m_Vertex.get(2), &collisionPoint, &collisionNormal))
														{
															//Diminish the strength
															ripple->strength *= 0.2f;

															//Reset the ripple multiplier
															ripple->multiplier = 1.f;

															//Reflect the direction of the ripple along the normal
															ripple->dir = Vec2Reflect(ripple->dir, collisionNormal);

															//Calculate the new position of the vector based on the new direction and excess distance it has penetrated the AABB
															ripple->currPos = collisionPoint + ripple->dir*Vec2Length(ripple->currPos - collisionPoint);
														}
														//endif ripple collide

														//Get the next iterator
														it = it->next;
													}
													//end loop
												} break;
				case ECOLLISIONGROUP_LOG:		{	//Loop through the list of log
													Node<GameObject*> *it = m_Log->front_node();
													for (int i = 0; i < m_Log->size(); ++ i)
													{
														//If the ripple vector collides
														if (CollideLineOBB(ripple->prevPos, ripple->currPos, it->data->m_Position, it->data->m_Size, it->data->m_Direction, &collisionPoint, &collisionNormal))
														{
															//Increase the ripple multiplier so that the player will be moved by the reflected vector more easily
															ripple->multiplier = 2.f;

															//Reflect the direction of the ripple along the normal
															ripple->dir = Vec2Reflect(ripple->dir, collisionNormal);

															//Calculate the new position of the vector based on the new direction and excess distance it has penetrated the AABB
															ripple->currPos = collisionPoint + ripple->dir*Vec2Length(ripple->currPos - collisionPoint);
														}
														//endif ripple collide

														//Get the next iterator
														it = it->next;
													}
													//end loop
												} break;
				default:						{
												} break;
			}
		}
		//end loop different object types
#endif

		return false;
	}

	/*
	*	Update the water texture
	*/
	void Water::UpdateWaterTexture()
	{
#ifdef _DirectX
		//Open the file for writing
		FILE *fp = NULL;
		fopen_s(&fp, ".\\works_romfiles\\texture\\ripple.tga", "w");
		if (fp == NULL) return;
		
		TextureManager::Remove(m_RippleTextureID);

		//Set and write image header values
		char header[ 18 ] = { 0 };
		header[ 2 ] = 2; // truecolor
		header[ 12 ] = m_RippleTextureWidth & 0xFF;
		header[ 13 ] = (m_RippleTextureWidth >> 8) & 0xFF;
		header[ 14 ] = m_RippleTextureHeight & 0xFF;
		header[ 15 ] = (m_RippleTextureHeight >> 8) & 0xFF;
		header[ 16 ] = 32; // bits per pixel
		fwrite((const char*)&header, 1, sizeof(header), fp);
 
		//Image data is stored bottom-to-top, left-to-right
		//Loop from bottom to top
		for (int j = m_RippleTextureHeight-1; j >= 0; --j)
		{
			//Loop from left to right
			for (int i = 0; i < m_RippleTextureWidth; ++i)
			{
				int alpha = 0;

				//If writing a water tile
				if (j < WATER_TILE_ROW && i < WATER_TILE_COL)
				{
					float temp = m_WaterLevel[i][j]*0.005f;
					temp = clamp(temp, 0.f, 0.5f);
					alpha = temp*255;
				}
				//endif writing water tile

				//Write the color and alpha values
				putc(0x00,fp);
				putc(0x00,fp);
				putc(0x00,fp);
				putc((int)(alpha & 0xFF),fp);
			}
			//end loop left to right
		}
		//end loop bottom to top

		//Set and write image footer values
		static const char footer[ 26 ] =
		"\0\0\0\0" // no extension area
		"\0\0\0\0" // no developer directory
		"TRUEVISION-XFILE" // tga file
		".";
		fwrite((const char*)&footer, 1, sizeof(footer), fp);
 
		//Close the file
		fclose(fp);
		
		TextureManager::Entry(m_RippleTextureID, "ripple");
#endif
#ifdef _3DS
		u8 alpha;
		int temp;

		//Loop through all columns of water
		for (int i = 0; i < WATER_TILE_COL; ++ i)
		{
			//Loop through all rows of water
			for (int j = 0; j < WATER_TILE_ROW; ++ j)
			{
				//Calculate an alpha value based on the water level
				temp = clamp((int)m_CurrWaterHeight[i][j]*1.275f, 0, 128) / 16;
				alpha = 0x11 * temp;

				//Set the texture data
				m_RippleTextureData.SetImageData(m_RippleTextureIndex[i][j], alpha);
			}
		}
		//DMA the updated texture data
		if( m_RippleTextureData.CheckMemoryLocation() )
		{
			nngxAddVramDmaCommand	(m_RippleTextureData.GetImageData()
									,(GLvoid*)m_RippleTextureData.GetLocationAddress()
									,m_RippleTextureData.GetImageDataCount() 
									);
		}
#endif
	}

	/*
	*	上画面描画
	*/
	void Water::DrawUpperWater(bool SwapScreen)
	{
		m_WaterBody->Render(SCREEN_WIDTH_DIFF*0.5f, 0.f, SwapScreen);
		m_WaterBed->Render(SCREEN_WIDTH_DIFF*0.5f, 0.f, SwapScreen);
	}

	void Water::DrawUpperRipple(bool SwapScreen)
	{
		m_WaterRipple->Render(SCREEN_WIDTH_DIFF*0.5f, 0.f, SwapScreen);
	}

	/*
	*	下画面描画
	*/
	void Water::DrawLowerWater(bool SwapScreen)
	{
		m_WaterBody->Render(0.f, -UPPER_SCREEN_HEIGHT, SwapScreen);
		m_WaterBed->Render(0.f, -UPPER_SCREEN_HEIGHT, SwapScreen);
	}

	void Water::DrawLowerRipple(bool SwapScreen)
	{
		m_WaterRipple->Render(0.f, -UPPER_SCREEN_HEIGHT, SwapScreen);
	}

	/*
	*	Get the next power of the base number nearest to the compared number
	*/
	int Water::GetNextPower (int baseNum, int comparedNum)
	{
		int temp = 1;
		if (baseNum > 1)
		{
			while (temp < comparedNum)
				temp *= baseNum;
		}
		else if (baseNum < 1)
		{
			while (temp > comparedNum)
				temp *= baseNum;
		}
		return temp;
	}
}