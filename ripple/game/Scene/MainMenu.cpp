#include "MainMenu.h"

#include "../../NW_Lib/Utility/NW_Lib_CommonData.h"
#include "../../NW_Lib/Graphics/NW_Lib_TextureManager.h"
#include "../../NW_Lib/Graphics/NW_Lib_BillboardDrawer.h"
#include "../../NW_Lib/Graphics/NW_Lib_SpriteDrawer.h"
#include "../../NW_Lib/System/NW_Lib_InputManager.h"
#include "../../NW_Lib/Sound/NW_Lib_Sound.h"
#include "../../NW_Lib/Utility/NW_Lib_DebugFontManager.h"
#include "../../NW_Lib/Utility/NW_Lib_DebugFont.h"

#include "../../NW_Lib/System/NW_Lib_SceneManager.h"
#include "OptionScreen.h"
#include "LevelSelect.h"

// Sound Data
#ifdef _DirectX
	//#include SoundData
	#include "../../works_romfiles/sound/RippleSoundOther.h"
#endif
#ifdef _3DS
	#include "../../NW_Lib/Graphics/NW_Lib_ResTextureManager.h"
	//#include SoundData
	// ƒTƒEƒ“ƒh
#endif

using namespace NW_Lib;

namespace Game
{

	//	Declarations for Buttons

	bool Button::DetectCollision()
	{

		if ( System::InputManager::GetTouchPush() )
		{
			float touch_x = System::InputManager::GetTouchPosition().x;
			float touch_y = System::InputManager::GetTouchPosition().y;

			/*DebugFontManager::Printf(UPPER_SCREEN_DRAWING,
							100, 100, "%f , %f", touch_x, touch_y);*/

			//	Magnitude
			

			if ( (touch_x > this->GetPosX()-(this->GetWidth()*0.5) && touch_x < this->GetPosX()+(this->GetWidth()*0.5))
				&&(touch_y > this->GetPosY()-(this->GetHeight()*0.5) && touch_y < this->GetPosY()+(this->GetHeight()*0.5)) )
					return true;
			else
				return false;
		}
		else
			return false;
	};
	//	Declarations for Buttons

	bool Button::DetectCollisionTouchRelease()
	{

		if ( System::InputManager::GetTouchRelease() )
		{
			float touch_x = System::InputManager::GetTouchPosition().x;
			float touch_y = System::InputManager::GetTouchPosition().y;

			if ( (touch_x > this->GetPosX()-(this->GetWidth()*0.5) && touch_x < this->GetPosX()+(this->GetWidth()*0.5))
				&&(touch_y > this->GetPosY()-(this->GetHeight()*0.5) && touch_y < this->GetPosY()+(this->GetHeight()*0.5)) )
					return true;
			else
				return false;
		}
		else
			return false;
	};


	//	Declarations for MainMenu

	MainMenu::MainMenu()
	{
	}

	MainMenu::~MainMenu()
	{
		Finalize();
	}

	void MainMenu::Initialize()
	{
		TextureManager::AllRemove();
		TextureManager::Entry(0,"TitleScreenSprite");	//	Top Screen
		TextureManager::Entry(1,"TitleScreenButtons_EN");	//	Menu Stuff
		TextureManager::Entry(2,"TitleScreenButtons_JP");	//	Menu Stuff

		// Load sound
		Sound::GetInstance().Initialize("RippleSoundOther");
		Sound::GetInstance().STRM_SetVolume(CommonData::GetInstance().GetOptionData()->bgmVolume);
		Sound::GetInstance().SE_SetVolumeAll(CommonData::GetInstance().GetOptionData()->sfxVolume);
		Sound::GetInstance().SE_Entry(SE_DECIDE);
		Sound::GetInstance().SE_Entry(SE_SELECT);

		// Play BGM
		Sound::GetInstance().STRM_Play(BGM_TITLE);

		//	Button Init
		this->ButtonPressed = NONE;
		this->numOfButtons = 4;
		this->MenuButton = new Button[this->numOfButtons];
		//	For the Play Button
		this->MenuButton[0].Init(104.0f,104.0f,128.0f,128.0f,PLAY, 0.0f, -1.0f);
		//	For the Option Button
		this->MenuButton[1].Init(232.0f,112.0f,128.0f/2,128.0f/2,OPTION, 0.0f, 1.0f); 
		//	For the Eng Button
		this->MenuButton[2].Init(285.0f,175.0f,50.0f,35.0f,ENG, 0.0f, 1.0f); 
		//	For the Jap Button
		this->MenuButton[3].Init(285.0f,210.0f,50.0f,35.0f,JAP, 0.0f, 1.0f); 

		this->LanButtonPressed = ENG;
		this->CurrentLanguage = 1;
	}

	void MainMenu::DrawUpper()
	{
		SpriteDrawer::Draw(TextureManager::Get(0), 0.0f,0.0f,400.0f,240.0f,0.0f,0.0f,400.0f,240.0f);
	}

	void MainMenu::Update()
	{
		// change the Scene
		if (System::InputManager::GetTouchRelease() && this->ButtonPressed != NONE)
		{
			if ( this->ButtonPressed == PLAY )
			{
				Sound::GetInstance().SE_Play(SE_DECIDE);
				System::SceneManager::GetInstance().SetLoadAnimation(true);
				System::SceneManager::GetInstance().ChangeScene(new LevelSelect());
			}
			if ( this->ButtonPressed == OPTION )
			{
				Sound::GetInstance().SE_Play(SE_DECIDE);
				System::SceneManager::GetInstance().SetLoadAnimation(false);
				System::SceneManager::GetInstance().ChangeScene(new OptionScreen());
			}
		}
		for ( int num = 2; num < this->numOfButtons; num ++ )
		{
			if ( this->MenuButton[num].DetectCollision() )
			{
				if ( this->MenuButton[num].GetCurrentButtonType() == ENG )
				{
					// Play SE
					if (CommonData::GetInstance().GetOptionData()->language == 2)
						Sound::GetInstance().SE_Play(SE_SELECT);
					CommonData::GetInstance().GetOptionData()->language = 1;
				}
				else
				{
					// Play SE
					if (CommonData::GetInstance().GetOptionData()->language == 1)
						Sound::GetInstance().SE_Play(SE_SELECT);
					CommonData::GetInstance().GetOptionData()->language = 2;
				}
			}
		}
		/*if ( this->LanButtonPressed == ENG )
			this->CurrentLanguage = 1;
		else
			this->CurrentLanguage = 2;*/
	}

	void MainMenu::DrawLower()
	{
		//	Draw Buttons
		//	Water background
		SpriteDrawer::Draw(TextureManager::Get(0),0.0f,0.0f,320.0f,240.0f,0.0f,512.0f-240.0f,320.0f,240.0f);
		for ( int num = 0; num < this->numOfButtons/2; num++ )
		{
			this->MenuButton[num].SetRotation(this->MenuButton[num].GetRotation()+this->MenuButton[num].GetRateOfRotation());
				
			if ( this->MenuButton[num].DetectCollision() )
			{
				float squeeze = 0.75f;
				SpriteDrawer::Draw(TextureManager::Get(CommonData::GetInstance().GetOptionData()->language),this->MenuButton[num].GetPosX(), this->MenuButton[num].GetPosY(),
					this->MenuButton[num].GetWidth()*squeeze,this->MenuButton[num].GetHeight()*squeeze,0.0f,128.0f,128.0f,128.0f,
						this->MenuButton[num].GetWidth()*0.5*squeeze,this->MenuButton[num].GetHeight()*0.5*squeeze, this->MenuButton[num].GetRotation() );
				SpriteDrawer::Draw(TextureManager::Get(CommonData::GetInstance().GetOptionData()->language),this->MenuButton[num].GetPosX(), this->MenuButton[num].GetPosY(),
					128.0f*squeeze,128.0f*squeeze,num*256.0f+128.0f,0.0f,128.0f,128.0f,64.f*squeeze,64.f*squeeze);
				this->ButtonPressed = this->MenuButton[num].GetCurrentButtonType();
			}
			else
			{
				SpriteDrawer::Draw(TextureManager::Get(CommonData::GetInstance().GetOptionData()->language),this->MenuButton[num].GetPosX(), this->MenuButton[num].GetPosY(),
					this->MenuButton[num].GetWidth(),this->MenuButton[num].GetHeight(),0.0f,128.0f,128.0f,128.0f,this->MenuButton[num].GetWidth()*0.5,this->MenuButton[num].GetHeight()*0.5, this->MenuButton[num].GetRotation() );
				SpriteDrawer::Draw(TextureManager::Get(CommonData::GetInstance().GetOptionData()->language),this->MenuButton[num].GetPosX(), this->MenuButton[num].GetPosY(),
					128.0f,128.0f,num*256.0f,0.0f,128.0f,128.0f,64.f,64.f);

			}

		}
		//	Draw Language Buttons
		int type[2];
		if ( CommonData::GetInstance().GetOptionData()->language == 1 )
		{
			type[0] = 4;
			type[1] = 1;
		}
		else
		{
			type[0] = 3;
			type[1] = 2;
		}

		for ( int lan = 2; lan < this->numOfButtons; lan++ )
		{
			SpriteDrawer::Draw(TextureManager::Get(0),this->MenuButton[lan].GetPosX(),this->MenuButton[lan].GetPosY(),50.0f,35.0f,
				512.0f-50.0f,(512.0f-(35.0f*type[lan-2])),50.0f,35.0f,50.0f*0.5f,35.0f*0.5);
		}
	}
	//	Final
	void MainMenu::Finalize()
	{
		Sound::GetInstance().Finalize();
		TextureManager::AllRemove();
	}

};

