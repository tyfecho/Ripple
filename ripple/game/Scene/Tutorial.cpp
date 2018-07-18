#include "Tutorial.h"

#include "../../NW_Lib/System/NW_Lib_SceneManager.h"
#include "../../NW_Lib/System/NW_Lib_InputManager.h"
#include "../../NW_Lib/Graphics/NW_Lib_TextureManager.h"
#include "../../NW_Lib/Graphics/NW_Lib_SpriteDrawer.h"
#include "../../NW_Lib/Utility/NW_Lib_CommonData.h"

#include "../GameObjectExt.h"

using namespace NW_Lib;

namespace Game
{

	Units::Units(float pos_x,float pos_y,float tex_x,float tex_y,float width,float height,int iter,int screen,int entryNum,bool animated,int frame)
	{
		this->pos_x = pos_x;
		this->pos_y = pos_y;
		this->tex_x = tex_x;
		this->tex_y = tex_y;
		this->width = width;
		this->height = height;
		this->screen = screen;
		this->iter = iter;
		this->entryNum = entryNum;
		this->animated = animated;
		this->frame = frame;
		this->currentFrame = 1.0f;
	}

	Units::~Units()
	{
	};

	void Units::Render()
	{
		if ( !this->animated )
			SpriteDrawer::Draw(TextureManager::Get(this->entryNum),GetPosX(),GetPosY(),GetWidth(),GetHeight(),
						GetTexX(),GetTexY(),GetWidth(),GetHeight(),GetWidth()/2,GetHeight()/2);
		else
			SpriteDrawer::Draw(TextureManager::Get(this->entryNum),GetPosX(),GetPosY(),GetWidth(),GetHeight(),
			GetTexX()+(GetWidth()*((int)this->currentFrame%this->frame)),GetTexY(),GetWidth(),GetHeight(),GetWidth()/2,GetHeight()/2);
	};

	void Units::Update()
	{
		if ( this->animated )
			this->currentFrame += 0.1f;
	};

	Tutorial::Tutorial()
	{
	};

	Tutorial::~Tutorial()
	{
		TextureManager::AllRemove();
		theUnits.clear();
	}

	void Tutorial::Init(int id)
	{
		this->id = id;
		if ( this->id == 1 )	//	touch tutorial
		{
			if ( CommonData::GetInstance().GetOptionData()->language == 1 )
				TextureManager::Entry(ETEXTURE_TUTORIAL, "TutorialScreenSprite_EN");
			else
				TextureManager::Entry(ETEXTURE_TUTORIAL, "TutorialScreenSprite_JP");
			//	First is to touch the water text
			this->theUnits.push_back(new Units(320.0f/2-50.0f,240.0f/2-20.0f,0.0f,0.0f,256.0f,40.0f,0,2,ETEXTURE_TUTORIAL));
			this->theUnits.push_back(new Units(50.0f,150.0f,0.0f,512.0f-40.0f,40.0f,40.0f,0,2,ETEXTURE_TUTORIAL,true,2));
			this->theUnits.push_back(new Units(120.0f,240.0f/2,0.0f,40.0f,256.0f,40.0f,1,1,ETEXTURE_TUTORIAL));

			this->theUnits.push_back(new Units(320.0f/2-50.0f,200.0f,0.0f,80.0f,256.0f,55.0f,2,2,ETEXTURE_TUTORIAL));
			this->theUnits.push_back(new Units(320.0f/2+110.0f,220.0f,80.0f,512.0f-40.0f,40.0f,40.0f,2,2,ETEXTURE_TUTORIAL));

			this->theUnits.push_back(new Units(320.0f/2-40.0f,240.0f/2-15.0f,0.0f,135.0f,256.0f,40.0f,3,2,ETEXTURE_TUTORIAL));
			this->theUnits.push_back(new Units(320.0f/2-75.0f,240.0f/2+65.0f,0.0f,512.0f-40.0f-20.0f,80.0f,20.0f,3,2,ETEXTURE_TUTORIAL));
			this->theUnits.push_back(new Units(320.0f/2-30.0f,240.0f/2-60.0f,0.0f,175.0f,256.0f,35.0f,4,2,ETEXTURE_TUTORIAL));
			this->theUnits.push_back(new Units(320.0f/2+75.0f,240.0f/2-20.0f,80.0f,512.0f-40.0f-20.0f,80.0f,20.0f,4,2,ETEXTURE_TUTORIAL));
			this->maxIter = 4;
		}
		else if ( this->id == 2 )
		{
			if ( CommonData::GetInstance().GetOptionData()->language == 1 )
				TextureManager::Entry(ETEXTURE_TUTORIAL, "TutorialScreenSprite_EN");
			else
				TextureManager::Entry(ETEXTURE_TUTORIAL, "TutorialScreenSprite_JP");

			this->theUnits.push_back(new Units(400.0f/2,240.0f/2+30.0f,0.0f,210.0f,256.0f,55.0f,0,1,ETEXTURE_TUTORIAL));
			this->maxIter = 0;
			
		}
		else if ( this->id == 3 )
		{
			if ( CommonData::GetInstance().GetOptionData()->language == 1 )
				TextureManager::Entry(ETEXTURE_TUTORIAL, "ElementsTutorialSprite_EN");
			else
				TextureManager::Entry(ETEXTURE_TUTORIAL, "ElementsTutorialSprite_JP");

			this->theUnits.push_back(new Units(400.0f/2,240.0f/2,0.0f,0.0f,320.0f,240.0f,0,1,ETEXTURE_TUTORIAL));
			this->maxIter = 0;
			
		}
		else if ( this->id == 6 )
		{
			if ( CommonData::GetInstance().GetOptionData()->language == 1 )
				TextureManager::Entry(ETEXTURE_TUTORIAL, "ElementsTutorialSprite_EN");
			else
				TextureManager::Entry(ETEXTURE_TUTORIAL, "ElementsTutorialSprite_JP");

			this->theUnits.push_back(new Units(400.0f/2,240.0f/2,320.0f,0.0f,320.0f,240.0f,0,1,ETEXTURE_TUTORIAL));
			this->theUnits.push_back(new Units(400.0f/2,240.0f/2,640.0f,0.0f,320.0f,240.0f,1,1,ETEXTURE_TUTORIAL));
			this->maxIter = 1;
			
		}
		else if ( this->id == 13 )
		{
			if ( CommonData::GetInstance().GetOptionData()->language == 1 )
				TextureManager::Entry(ETEXTURE_TUTORIAL, "ElementsTutorialSprite_EN");
			else
				TextureManager::Entry(ETEXTURE_TUTORIAL, "ElementsTutorialSprite_JP");

			this->theUnits.push_back(new Units(400.0f/2,240.0f/2,0.0f,240.0f,320.0f,240.0f,0,1,ETEXTURE_TUTORIAL));
			this->maxIter = 0;
			
		}
		else if ( this->id == 22 )
		{
			if ( CommonData::GetInstance().GetOptionData()->language == 1 )
				TextureManager::Entry(ETEXTURE_TUTORIAL, "ElementsTutorialSprite_EN");
			else
				TextureManager::Entry(ETEXTURE_TUTORIAL, "ElementsTutorialSprite_JP");

			this->theUnits.push_back(new Units(400.0f/2,240.0f/2,320.0f,240.0f,320.0f,240.0f,0,1,ETEXTURE_TUTORIAL));
			this->maxIter = 0;
			
		}
		else
			this->isThereOne = false;

		this->currentIter = 0.0f;
	};

	void Tutorial::Update()
	{
		if (System::InputManager::GetTouchRelease())
			this->currentIter ++;
		if ( this->currentIter > this->maxIter )
			this->isThereOne = false;

		Node<Units*> *unit = this->theUnits.front_node();
		for ( int num = 0; num < this->theUnits.size(); num++ )
		{
			if ( unit->data->GetIter() == this->currentIter )
				unit->data->Update();
			unit = unit->next;
		}
	};

	void Tutorial::RenderLower()
	{
		Node<Units*> *unit = this->theUnits.front_node();
		for ( int num = 0; num < this->theUnits.size(); num++ )
		{
			if ( unit->data->GetScreen() == 2 && unit->data->GetIter() == this->currentIter )
				unit->data->Render();
			unit = unit->next;
		}
	};

	void Tutorial::RenderUpper()
	{
		Node<Units*> *unit = this->theUnits.front_node();
		for ( int num = 0; num < this->theUnits.size(); num++ )
		{
			if ( unit->data->GetScreen() == 1 && unit->data->GetIter() == this->currentIter )
				unit->data->Render();
			unit = unit->next;
		}
	};

}

