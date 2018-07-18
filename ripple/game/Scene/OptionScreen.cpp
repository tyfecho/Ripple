#include "OptionScreen.h"
#include "Screens.h"

#include "../../NW_Lib/Utility/NW_Lib_CommonData.h"
#include "../../NW_Lib/Graphics/NW_Lib_TextureManager.h"
#include "../../NW_Lib/Graphics/NW_Lib_BillboardDrawer.h"
#include "../../NW_Lib/Graphics/NW_Lib_SpriteDrawer.h"
#include "../../NW_Lib/System/NW_Lib_InputManager.h"
#include "../../NW_Lib/Sound/NW_Lib_Sound.h"
#include "../../NW_Lib/Utility/NW_Lib_CommonData.h"

#include "../../NW_Lib/System/NW_Lib_SceneManager.h"

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
	//	Declarations for Option Screen

	OptionScreen::OptionScreen()
	{
	//	TextureManager::AllRemove();
	}

	OptionScreen::~OptionScreen()
	{
		Sound::GetInstance().Finalize();
		TextureManager::AllRemove();
	};

	// ‰Šú‰»
	void OptionScreen::Initialize()
	{
		TextureManager::Entry(0,"TitleScreenSprite");
		if ( CommonData::GetInstance().GetOptionData()->language == 1 )
			TextureManager::Entry(1,"OptionScreenSprite_EN");
		else
			TextureManager::Entry(1,"OptionScreenSprite_JP");

		// Load sound
		Sound::GetInstance().Initialize("RippleSoundOther");
		Sound::GetInstance().STRM_SetVolume(CommonData::GetInstance().GetOptionData()->bgmVolume);
		Sound::GetInstance().SE_SetVolumeAll(CommonData::GetInstance().GetOptionData()->sfxVolume);
		Sound::GetInstance().SE_Entry(SE_DECIDE);
		Sound::GetInstance().SE_Entry(SE_CANCEL);
		Sound::GetInstance().SE_Entry(SE_SELECT);
		
		// Play BGM
		Sound::GetInstance().STRM_Play(BGM_TITLE);
		
		//	New the buttons
		this->numOfButton = 3;
		this->OptionButton = new Button[this->numOfButton];
		this->ButtonPressed = NONE;

		//	Help Button
		this->OptionButton[0].Init(320.0f/2,155.0f,210.0f,30.0f,HELP,0.0f,0.0f);
		//	Credit Button
		this->OptionButton[1].Init(320.0f/2,185.0f,210.0f,30.0f,CREDITS,0.0f,0.0f);
		//	Back Button
		this->OptionButton[2].Init(50.0f,220.0f,100.0f,50.0f,BACK,0.0f,0.0f);

		//	Sliders
		this->numOfSlider = 2;
		this->Slider = new Button[this->numOfSlider];

		//	SFX slider
		this->Slider[0].Init(170.0f,95.0f,150.0f,20.0f,SFX,0.0f,0.0f);	//	SFX Slider
		//	BGM slider
		this->Slider[1].Init(170.0f,125.0f,150.0f,20.0f,BGM,0.0f,0.0f);	//	BGM Slider


	};

	ButtonType OptionScreen::GetPushButtonType()
	{
		for ( int button = 0; button < this->numOfButton; button++ )
		{
			if ( this->OptionButton[button].DetectCollision() )
			{
				return this->OptionButton[button].GetCurrentButtonType();
			}
		}
		return NONE;
	};

	void OptionScreen::Update()
	{
		for ( int num = 0; num < this->numOfSlider; num++ )
		{
			if ( this->Slider[num].DetectCollision() )
			{					
				if ( this->Slider[num].GetCurrentButtonType() == SFX )
				{
					// SE Play
					if (CommonData::GetInstance().GetOptionData()->sfxVolume !=  static_cast<int>(((System::InputManager::GetTouchPosition().x - (this->Slider[num].GetPosX()*0.5f)) / (this->Slider[num].GetWidth()/8.0f))))
						Sound::GetInstance().SE_Play(SE_SELECT);
					CommonData::GetInstance().GetOptionData()->sfxVolume =  ((System::InputManager::GetTouchPosition().x - (this->Slider[num].GetPosX()*0.5f)) / (this->Slider[num].GetWidth()/8));
					Sound::GetInstance().SE_SetVolumeAll(CommonData::GetInstance().GetOptionData()->sfxVolume / 8.0f);
				}
				if ( this->Slider[num].GetCurrentButtonType() == BGM )
				{
					// SE Play
					if (CommonData::GetInstance().GetOptionData()->bgmVolume !=  static_cast<int>(((System::InputManager::GetTouchPosition().x - (this->Slider[num].GetPosX()*0.5f)) / (this->Slider[num].GetWidth()/8.0f))))
						Sound::GetInstance().SE_Play(SE_SELECT);
					CommonData::GetInstance().GetOptionData()->bgmVolume =  ((System::InputManager::GetTouchPosition().x - (this->Slider[num].GetPosX()*0.5f)) / (this->Slider[num].GetWidth()/8));
					Sound::GetInstance().STRM_SetVolume(CommonData::GetInstance().GetOptionData()->bgmVolume / 8.0f);
				}
			}
				
		}

		if ( System::InputManager::GetTouchRelease() )
			if ( this->ButtonPressed == BACK )
			{
				Sound::GetInstance().SE_Play(SE_CANCEL);
				System::SceneManager::GetInstance().ChangeScene(new MainMenu());
			}
			else if ( this->ButtonPressed == CREDITS )
			{
				Sound::GetInstance().SE_Play(SE_DECIDE);
				System::SceneManager::GetInstance().ChangeScene(new Credits());
			}
			else if ( this->ButtonPressed == HELP )
			{
				Sound::GetInstance().SE_Play(SE_DECIDE);
				System::SceneManager::GetInstance().ChangeScene(new Help());
			}

		if ( this->GetPushButtonType() == BACK )
		{
			this->OptionButton[2].SetPosX(50.0f-10.0f);
			this->ButtonPressed = BACK;
			//System::SceneManager::GetInstance().ChangeScene(new MainMenu());
		}
		else if ( this->GetPushButtonType() == CREDITS )
		{
			this->ButtonPressed = CREDITS;
			//System::SceneManager::GetInstance().ChangeScene(new Credits());
		}
		else if ( this->GetPushButtonType() == HELP )
		{
			this->ButtonPressed = HELP;
			//System::SceneManager::GetInstance().ChangeScene(new Help());
		}
		else if ( this->GetPushButtonType() == NONE )
		{
			this->OptionButton[2].SetPosX(50.0f);
			this->ButtonPressed = NONE;
		}
	

		/*for ( int num2 = 0; num2 < this->numOfButton; num2++ )
		{
			if ( this->OptionButton[num2].DetectCollision() )
			{
				{
					if ( this->OptionButton[num2].GetCurrentButtonType() == BACK )
						this->ButtonPressed = BACK;
					if ( this->OptionButton[num2].GetCurrentButtonType() == CREDITS )
						this->ButtonPressed = CREDITS;
					if ( this->OptionButton[num2].GetCurrentButtonType() == HELP )
						this->ButtonPressed = HELP;
				}

			}
		}

		if ( System::InputManager::GetTouchRelease() && this->ButtonPressed != NONE )
		{
			if ( this->ButtonPressed == BACK )
			{
				Sound::GetInstance().SE_Play(SE_CANCEL);
				System::SceneManager::GetInstance().ChangeScene(new MainMenu());
			}
			else if ( this->ButtonPressed == CREDITS )
			{
				Sound::GetInstance().SE_Play(SE_DECIDE);
				System::SceneManager::GetInstance().ChangeScene(new Credits());
			}
			else if ( this->ButtonPressed == HELP )
			{
				Sound::GetInstance().SE_Play(SE_DECIDE);
				System::SceneManager::GetInstance().ChangeScene(new Help());
			}
		}*/
	};


	void OptionScreen::DrawLower()
	{


		//	BackGround
		SpriteDrawer::Draw(TextureManager::Get(1),0.0f,0.0f,320.0f,240.0f,0.0f,512.0f-240.0f,320.0f,240.0f);

		//	Option Word
		SpriteDrawer::Draw(TextureManager::Get(1),(320.0f-120.0f)/2,40.0f,120.0f,40.0f,0.0f,512.0f-240.0f-40.0f,120.0f,40.0f);

		//	Draw the Help Button
		if ( !this->OptionButton[0].DetectCollision() )
			SpriteDrawer::Draw(TextureManager::Get(1), this->OptionButton[0].GetPosX(), this->OptionButton[0].GetPosY(), this->OptionButton[0].GetWidth(), this->OptionButton[0].GetHeight(),
				250.0f,0.0f,this->OptionButton[0].GetWidth(),this->OptionButton[0].GetHeight(),this->OptionButton[0].GetWidth()*0.5,this->OptionButton[0].GetHeight()*0.5);
		else
			SpriteDrawer::Draw(TextureManager::Get(1), this->OptionButton[0].GetPosX(), this->OptionButton[0].GetPosY(), this->OptionButton[0].GetWidth(), this->OptionButton[0].GetHeight(),
				250.0f,0.0f+60.0f,this->OptionButton[0].GetWidth(),this->OptionButton[0].GetHeight(),this->OptionButton[0].GetWidth()*0.5,this->OptionButton[0].GetHeight()*0.5);

		//	Draw the Credits Button
		if (!this->OptionButton[1].DetectCollision())
			SpriteDrawer::Draw(TextureManager::Get(1), this->OptionButton[1].GetPosX(), this->OptionButton[1].GetPosY(), this->OptionButton[1].GetWidth(), this->OptionButton[1].GetHeight(),
				250.0f,30.0f,this->OptionButton[1].GetWidth(),this->OptionButton[1].GetHeight(),this->OptionButton[1].GetWidth()*0.5,this->OptionButton[1].GetHeight()*0.5);
		else
			SpriteDrawer::Draw(TextureManager::Get(1), this->OptionButton[1].GetPosX(), this->OptionButton[1].GetPosY(), this->OptionButton[1].GetWidth(), this->OptionButton[1].GetHeight(),
				250.0f,30.0+60.0f,this->OptionButton[1].GetWidth(),this->OptionButton[1].GetHeight(),this->OptionButton[1].GetWidth()*0.5,this->OptionButton[1].GetHeight()*0.5);

		//	Draw The BACK Button
		SpriteDrawer::Draw(TextureManager::Get(1), this->OptionButton[2].GetPosX(), this->OptionButton[2].GetPosY(), this->OptionButton[2].GetWidth(), this->OptionButton[2].GetHeight(),
			150.0f,50.0f,this->OptionButton[2].GetWidth(),this->OptionButton[2].GetHeight(),this->OptionButton[2].GetWidth()*0.5,this->OptionButton[2].GetHeight()*0.5);

		//	Draw SFX bar
		SpriteDrawer::Draw(TextureManager::Get(1),this->Slider[0].GetPosX(),this->Slider[0].GetPosY(),this->Slider[0].GetWidth(),this->Slider[0].GetHeight(),
			0.0f,(CommonData::GetInstance().GetOptionData()->sfxVolume*20.0f),this->Slider[0].GetWidth(),this->Slider[0].GetHeight(),this->Slider[0].GetWidth()*0.5,this->Slider[0].GetHeight()*0.5);
		//	Draw SFX word
		SpriteDrawer::Draw(TextureManager::Get(1),25.0f,70.0f,100.0f,50.0f,150.0f,150.0f,100.0f,50.0f);

		//	Draw BGM bar
		SpriteDrawer::Draw(TextureManager::Get(1),this->Slider[1].GetPosX(),this->Slider[1].GetPosY(),this->Slider[1].GetWidth(),this->Slider[1].GetHeight(),
			0.0f,(CommonData::GetInstance().GetOptionData()->bgmVolume*20.0f),this->Slider[1].GetWidth(),this->Slider[1].GetHeight(),this->Slider[1].GetWidth()*0.5,this->Slider[1].GetHeight()*0.5);
		//	Draw BGM word
		SpriteDrawer::Draw(TextureManager::Get(1),25.0f,100.0f,100.0f,50.0f,150.0f,100.0f,100.0f,50.0f);
	};

	void OptionScreen::DrawUpper()
	{
		//	Draw the Title Screen
		SpriteDrawer::Draw(TextureManager::Get(0),0.0f,0.0f,400.0f,240.0f,0.0f,0.0f,400.0f,240.0f);

	};
}