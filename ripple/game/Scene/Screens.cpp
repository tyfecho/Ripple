#include "Screens.h"
#include "OptionScreen.h"
#include "LevelSelect.h"
#include "GameLevel.h"

#include "../../NW_Lib/System/NW_Lib_IScene.h"

#include "../../NW_Lib/Utility/NW_Lib_CommonData.h"
#include "../../NW_Lib/Graphics/NW_Lib_TextureManager.h"
#include "../../NW_Lib/Graphics/NW_Lib_SpriteDrawer.h"
#include "../../NW_Lib/System/NW_Lib_InputManager.h"
#include "../../NW_Lib/Sound/NW_Lib_Sound.h"

#include "../../NW_Lib/System/NW_Lib_SceneManager.h"
#include "../../NW_Lib/Utility/NW_Lib_CommonData.h"

// Sound Data
#ifdef _DirectX
	//#include SoundData
	#include "../../works_romfiles/sound/RippleSoundOther.h"
#endif
#ifdef _3DS
	#include "../../NW_Lib/Graphics/NW_Lib_ResTextureManager.h"
	//#include SoundData
	// ƒTƒEƒ“ƒh
	#include"../../NW_Lib/Sound/RippleSoundOther.h"
#endif

using namespace NW_Lib;

namespace Game
{

	//	Credits Declaration
	Credits::Credits()
	{
	};

	Credits::~Credits()
	{
		Sound::GetInstance().Finalize();
		TextureManager::AllRemove();
	};

	ButtonType Credits::GetPushButtonType()
	{
		
		if ( this->theButton->DetectCollision() )
		{
			return this->theButton->GetCurrentButtonType();
		}
		
		return NONE;
	};

	void Credits::Update()
	{
		if ( System::InputManager::GetTouchRelease() )
		{
			if ( this->ButtonPressed == BACK )
			{
				Sound::GetInstance().SE_Play(SE_CANCEL);
				System::SceneManager::GetInstance().ChangeScene(new OptionScreen());
			}
		}

		if ( this->GetPushButtonType() == BACK )
		{
			this->theButton->SetPosX(50.0f-10.0f);
			this->ButtonPressed = BACK;
			//System::SceneManager::GetInstance().ChangeScene(new MainMenu());
		}
		else if ( this->GetPushButtonType() == NONE )
		{
			this->ButtonPressed = NONE;
			this->theButton->SetPosX(50.0f);
			//System::SceneManager::GetInstance().ChangeScene(new Credits());
		}
	};

	void Credits::Initialize()
	{
		if ( CommonData::GetInstance().GetOptionData()->language == 1 )
			TextureManager::Entry(0,"OptionScreenSprite_EN");	//	For the Screens
		else
			TextureManager::Entry(0,"OptionScreenSprite_JP");	//	For the Screens
		TextureManager::Entry(1,"CreditsScreenSprite");	//	Need this for 
		this->ButtonPressed = NONE;

		// Load sound
		Sound::GetInstance().Initialize("RippleSoundOther");
		Sound::GetInstance().STRM_SetVolume(CommonData::GetInstance().GetOptionData()->bgmVolume);
		Sound::GetInstance().SE_SetVolumeAll(CommonData::GetInstance().GetOptionData()->sfxVolume);
		Sound::GetInstance().SE_Entry(SE_CANCEL);

		// Play BGM
		Sound::GetInstance().STRM_Play(BGM_TITLE);

		this->theButton = new Button();
		this->theButton->Init(50.0f,220.0f,100.0f,50.0f,BACK,0.0f,0.0f);
	};

	void Credits::DrawUpper()
	{
		SpriteDrawer::Draw(TextureManager::Get(1),0.0f,0.0f,400.0f,240.0f,
			0.0f,0.0f,400.0f,240.0f);
	};

	void Credits::DrawLower()
	{
		SpriteDrawer::Draw(TextureManager::Get(1),0.0f,0.0f,320.0f,240.0f,
			0.0f,512.0f-240.0f,320.0f,240.0f);

		//	Draw The BACK Button
		SpriteDrawer::Draw(TextureManager::Get(0), this->theButton->GetPosX(), this->theButton->GetPosY(), this->theButton->GetWidth(), this->theButton->GetHeight(),
			150.0f,50.0f,this->theButton->GetWidth(),this->theButton->GetHeight(),this->theButton->GetWidth()*0.5,this->theButton->GetHeight()*0.5);
	};


	//	Help Declaration
	Help::Help()
	{
	};

	Help::~Help()
	{
		Sound::GetInstance().Finalize();
		TextureManager::AllRemove();
	};

	ButtonType Help::GetPushButtonType()
	{
		
		if ( this->theButton->DetectCollision() )
		{
			return this->theButton->GetCurrentButtonType();
		}
		return NONE;
	};


	void Help::Update()
	{


		if ( System::InputManager::GetTouchRelease() )
		{
			if ( this->ButtonPressed == BACK )
			{
				Sound::GetInstance().SE_Play(SE_CANCEL);
				System::SceneManager::GetInstance().ChangeScene(new OptionScreen());
			}
		}
		

		if ( this->GetPushButtonType() == BACK )
		{
			this->theButton->SetPosX(320.0f-50.0f+10.0f);
			this->ButtonPressed = BACK;
			//System::SceneManager::GetInstance().ChangeScene(new MainMenu());
		}
		else if ( this->GetPushButtonType() == NONE )
		{
			this->ButtonPressed = NONE;
			this->theButton->SetPosX(320.0f-50.0f);
			//System::SceneManager::GetInstance().ChangeScene(new Credits());
		}
	};

	void Help::Initialize()
	{
		if ( CommonData::GetInstance().GetOptionData()->language == 1 )
			TextureManager::Entry(0,"HelpScreenSprite_EN");	//	For the Screens
		else
			TextureManager::Entry(0,"HelpScreenSprite_JP");	//	For the Screens
		this->ButtonPressed = NONE;
		
		// Load sound
		Sound::GetInstance().Initialize("RippleSoundOther");
		Sound::GetInstance().STRM_SetVolume(CommonData::GetInstance().GetOptionData()->bgmVolume);
		Sound::GetInstance().SE_SetVolumeAll(CommonData::GetInstance().GetOptionData()->sfxVolume);
		Sound::GetInstance().SE_Entry(SE_CANCEL);

		// Play BGM
		Sound::GetInstance().STRM_Play(BGM_TITLE);
		
		theButton = new Button();
		theButton->Init(320.0f-50.0f,220.0f,100.0f,30.0f,BACK,0.0f,0.0f);
	};

	void Help::DrawUpper()
	{
		SpriteDrawer::Draw(TextureManager::Get(0),0.0f,0.0f,400.0f,240.0f,
			0.0f,0.0f,400.0f,240.0f);
	};

	void Help::DrawLower()
	{
		SpriteDrawer::Draw(TextureManager::Get(0),0.0f,0.0f,320.0f,240.0f,
			0.0f,512.0f-240.0f,320.0f,240.0f);

		//	Back  button
		SpriteDrawer::Draw(TextureManager::Get(0),theButton->GetPosX(),theButton->GetPosY(),theButton->GetWidth(),theButton->GetHeight(),
			512.0f-100.0f,0.0f,theButton->GetWidth(),theButton->GetHeight(),theButton->GetWidth()*0.5,theButton->GetHeight()*0.5);
	}


	//	Level Cleared Declaration
	LevelCleared::LevelCleared(int ID = 0, int ClearTime = 0, int DropletsLeft = 0)
	{
		this->ClearTime = ClearTime;
		this->Score = Score;
		this->DropletsLeft = DropletsLeft;
		this->ID = ID;
		
		CommonData::GetInstance().GetLevelInfo(this->ID)->unlocked = true;
	};

	LevelCleared::~LevelCleared()
	{
		Sound::GetInstance().Finalize();
		TextureManager::AllRemove();
	};

	void LevelCleared::Initialize()
	{
		TextureManager::Entry(0,"TitleScreenSprite");	//	For the Screens
		if ( CommonData::GetInstance().GetOptionData()->language == 1 )
		{
			TextureManager::Entry(1,"ScoreScreenSprite_EN");
			TextureManager::Entry(2,"OptionScreenSprite_EN");
			TextureManager::Entry(3,"GameScreenSprite_EN");
		}
		else
		{
			TextureManager::Entry(1,"ScoreScreenSprite_JP");
			TextureManager::Entry(2,"OptionScreenSprite_JP");
			TextureManager::Entry(3,"GameScreenSprite_JP");
		}

		
		// Load sound
		Sound::GetInstance().Initialize("RippleSoundOther");
		Sound::GetInstance().STRM_SetVolume(CommonData::GetInstance().GetOptionData()->bgmVolume);
		Sound::GetInstance().SE_SetVolumeAll(CommonData::GetInstance().GetOptionData()->sfxVolume);
		Sound::GetInstance().SE_Entry(SE_DECIDE);
		Sound::GetInstance().SE_Entry(SE_CANCEL);
		Sound::GetInstance().SE_Entry(SE_SELECT);
		Sound::GetInstance().SE_Entry(SE_FANFARE2);

		// Play Fanfare
		Sound::GetInstance().SE_Play(SE_FANFARE2);

		this->AnimIter = 0.0f;
		this->topSegment = this->TopAnimFrame = this->topTextAlpha = 0.0f;
		this->NumToSprite();

		this->ButtonPressed = NONE;


		this->numOfButtons = 3;
		theButton = new Button[this->numOfButtons];

		//	Main Menu Button
		this->theButton[0].Init(50.0f,220.0f,100.0f,44.0f,MENU,0.0f,0.0f);
		this->theButton[1].Init(320.0f-50.0f,220.0f,100.0f,44.0f,NEXT,0.0f,0.0f);
		this->theButton[2].Init(320.0f/2,220.0f,32.0f,32.0f,RESTART,0.0f,0.0f);


	};

	ButtonType LevelCleared::GetPushButtonType()
	{
		for ( int button = 0; button < this->numOfButtons; button++ )
		{
			if ( this->theButton[button].DetectCollision() )
			{
				return this->theButton[button].GetCurrentButtonType();
			}
		}
		return NONE;
	};


	void LevelCleared::NumToSprite()
	{

		int seconds = this->ClearTime / 1000;	//	Milliseconds to Seconds
		int minutes = seconds / 60; //	Convert to minutes	00:XX
		int Seconds2 = seconds % 60;	//	find remainder	XX:00

		this->timeData[0] = minutes / 10;
		this->timeData[1] = minutes - (this->timeData[0]*10);
		this->timeData[2] = Seconds2 / 10;
		this->timeData[3] = Seconds2 - (this->timeData[2]*10);


		//	HighScore
		int thousand2 = CommonData::GetInstance().GetLevelInfo(ID)->highScore / 1000;
		int hundred2 = (CommonData::GetInstance().GetLevelInfo(ID)->highScore - (thousand2*1000)) / 100;
		int ten2 = (CommonData::GetInstance().GetLevelInfo(ID)->highScore - (thousand2*1000) - (hundred2*100)) / 10;
		int one2 = CommonData::GetInstance().GetLevelInfo(ID)->highScore - (thousand2*1000) - (hundred2*100) - (ten2*10);
		
		this->HighScoreData[0] = thousand2;
		this->HighScoreData[1] = hundred2;
		this->HighScoreData[2] = ten2;
		this->HighScoreData[3] = one2;

		//	Shortest Time

		int seconds2 = CommonData::GetInstance().GetLevelInfo(ID)->shortestTime / 1000;	//	Milliseconds to Seconds
		int minutes2 = seconds2 / 60; //	Convert to minutes	00:XX
		int Seconds22 = seconds2 % 60;	//	find remainder	XX:00

		this->ShortTimeData[0] = minutes2 / 10;
		this->ShortTimeData[1] = minutes2 - (this->ShortTimeData[0]*10);
		this->ShortTimeData[2] = Seconds22 / 10;
		this->ShortTimeData[3] = Seconds22 - (this->ShortTimeData[2]*10);

		this->Score = (240 - seconds) + ( this->DropletsLeft * 100 );

		int thousand = this->Score / 1000;
		int hundred = (this->Score - (thousand*1000)) / 100;
		int ten = (this->Score - (thousand*1000) - (hundred*100)) / 10;
		int one = this->Score - (thousand*1000) - (hundred*100) - (ten*10);

		this->ScoreData[0] = thousand;	//	Current Score
		this->ScoreData[1] = hundred;
		this->ScoreData[2] = ten;
		this->ScoreData[3] = one;

		if ( this->Score > CommonData::GetInstance().GetLevelInfo(ID)->highScore )
			CommonData::GetInstance().GetLevelInfo(ID)->highScore = this->Score;
		if ( this->ClearTime < CommonData::GetInstance().GetLevelInfo(ID)->shortestTime )
			CommonData::GetInstance().GetLevelInfo(ID)->shortestTime = this->ClearTime;
		
	};

	void LevelCleared::Update()
	{
		float rateOfUpdate = 0.25f;
		if ( System::InputManager::GetTouchPush() )
			rateOfUpdate = 1.0f;
		if ( this->AnimIter == 0 )
		{
			this->TopAnimFrame += rateOfUpdate;	//	add frame
			if ( this->topSegment == 0.0f && this->TopAnimFrame >= 3.0f )	//	if top row
			{
				this->TopAnimFrame = 0.0f;
				this->topSegment = 240.0f;
			}
			else if ( this->topSegment == 240.0f && this->TopAnimFrame >= 3.0f )	//	if bottom row
			{
				this->TopAnimFrame = 2.0f;
				this->AnimIter++;
			}
		}

		else if ( (int)this->AnimIter == 1 )
		{
			if ( this->topTextAlpha < 0.75f )
				this->topTextAlpha += 0.15;
			else
				this->AnimIter+=0.1f;	//	Delay Time
		}
		else if ( (int)this->AnimIter == 10 )
		{

			if ( System::InputManager::GetTouchRelease() )
				if ( this->ButtonPressed == NEXT )
				{
					Sound::GetInstance().SE_Play(SE_DECIDE);
					System::SceneManager::GetInstance().ChangeScene(new GameLevel(this->ID+1));
				}
				else if ( this->ButtonPressed == MENU )
				{
					Sound::GetInstance().SE_Play(SE_CANCEL);
					System::SceneManager::GetInstance().ChangeScene(new LevelSelect((this->ID)/5+1, (this->ID)%5+1));
				}
				else if ( this->ButtonPressed == RESTART )
				{
					Sound::GetInstance().SE_Play(SE_SELECT);
					System::SceneManager::GetInstance().ChangeScene(new GameLevel(this->ID));
				}


			if ( this->GetPushButtonType() == NEXT )
			{
				if (ID != NUM_OF_STAGES*NUM_OF_LEVELS_INLEVEL)
				{
					this->theButton[1].SetPosX(320.0f-50.0f+10.0f);
					this->ButtonPressed = NEXT;
				}
			}
			else if ( this->GetPushButtonType() == MENU )
			{
				this->theButton[0].SetPosX(50.0f-10.0f);
				this->ButtonPressed = MENU;
			}
			else if ( this->GetPushButtonType() == RESTART )
			{
				this->ButtonPressed = RESTART;
			}
			else if ( this->GetPushButtonType() == NONE )
			{
				this->theButton[0].SetPosX(50.0f);
				this->theButton[1].SetPosX(320.0f-50.0f);
				this->ButtonPressed = NONE;
			}
		}

		

			//System::SceneManager::GetInstance().ChangeScene(new LevelCleared(1,12000,4500,2));

	};

		/*if ( this->ButtonPressed == NONE )
		{
			this->ButtonPressed = NONE;
			if ( this->ButtonPressed == NEXT )
				this->theButton[button].SetPosX(320.0f-50.0f);
			else if ( this->ButtonPressed == MENU )
				this->theButton[button].SetPosX(50.0f);
		}
*/
		/*if ( System::InputManager::GetTouchRelease() )
			if ( this->ButtonPressed == NEXT )
				System::SceneManager::GetInstance().ChangeScene(new GameLevel(this->ID+1));
			else if ( this->ButtonPressed == MENU )
				System::SceneManager::GetInstance().ChangeScene(new LevelSelect((this->ID)/5+1, (this->ID)%5+1));
			else if ( this->ButtonPressed == RESTART )
				System::SceneManager::GetInstance().ChangeScene(new GameLevel(this->ID));*/

	
	void LevelCleared::DrawUpper()
	{
		//	First draw the Overall layout
		SpriteDrawer::Draw(TextureManager::Get(0),0.0f,0.0f,400.0f,240.0f,0.0f,0.0f,400.0f,240.0f);
		//	Then, draw over it
		SpriteDrawer::Draw(TextureManager::Get(1),400.0f/2,240.0f/2,(1024.0f-64.0f)/3,240.0f,
			(1024.0f-64.0f)/3*(int)this->TopAnimFrame,this->topSegment,(1024.0f-64.0f)/3,240.0f,(1024.0f-64.0f)/3*0.5, 240.0f*0.5);

		for ( int digit = 0; digit < 4; digit++ )
			SpriteDrawer::Draw(TextureManager::Get(1),230.0f+11.0f*digit,155.0f,12.0f,16.0f,1024.0f-12.0f,16.0f*this->HighScoreData[digit],12.0f,16.0f,0.0f,0.0f,0.0f,this->topTextAlpha);

		for ( int time = 0; time < 2; time++ )
			SpriteDrawer::Draw(TextureManager::Get(1),230.0f+11.0f*time,180.0f,12.0f,16.0f,1024.0f-12.0f,16.0f*this->ShortTimeData[time],12.0f,16.0f,0.0f,0.0f,0.0f,this->topTextAlpha);

		SpriteDrawer::Draw(TextureManager::Get(1),230.0f+11.0f*2,180.0f,12.0f,16.0f,1024.0f-12.0f,16.0f*10,12.0f,16.0f,0.0f,0.0f,0.0f,this->topTextAlpha);

		for ( int time2 = 2; time2 < 4; time2++ )
			SpriteDrawer::Draw(TextureManager::Get(1),230.0f+11.0f*(time2+1),180.0f,12.0f,16.0f,1024.0f-12.0f,16.0f*this->ShortTimeData[time2],12.0f,16.0f,0.0f,0.0f,0.0f,this->topTextAlpha);



	}
	
	void LevelCleared::DrawLower()
	{
		SpriteDrawer::Draw(TextureManager::Get(2),0.0f,0.0f,320.0f,240.0f,
			0.0f,512.0f-240.0f,320.0f,240.0f);

		//	Menu Button
	//	SpriteDrawer::Draw(TextureManager::Get(2),theButton[0].GetPosX(),theButton[0].GetPosY(),theButton[0].GetWidth(),theButton[0].GetHeight(),

		int maxScore = 500;
		float rateOfUpdate = 0.1f;

		if ( System::InputManager::GetTouchPush() )
			rateOfUpdate = 1.0f;

		if ( (int)this->AnimIter >= 2 )
		{
			SpriteDrawer::Draw(TextureManager::Get(1),50.0f,55.0f,95.0f,25.0f,
				0.0f,512.0f-25.0f,95.0f,25.0f);	//	Droplet Left

			SpriteDrawer::Draw(TextureManager::Get(1),195.0f,57.5f,12.0f,16.0f,
				1024.0f-12.0f,this->DropletsLeft*16.0f,12.0f,16.0f);	//	Num of Droplets

			SpriteDrawer::Draw(TextureManager::Get(1),207.5f,57.5f,12.0f,16.0f,
				1024.0f-12.0f,11*16.0f,12.0f,16.0f);	//	Multiply

			//	100 points
			SpriteDrawer::Draw(TextureManager::Get(1),220.0f,57.5f,12.0f,16.0f,
				1024.0f-12.0f,1*16.0f,12.0f,16.0f);

			SpriteDrawer::Draw(TextureManager::Get(1),232.5f,57.5f,12.0f,16.0f,
				1024.0f-12.0f,0*16.0f,12.0f,16.0f);

			SpriteDrawer::Draw(TextureManager::Get(1),245.0f,57.5f,12.0f,16.0f,
				1024.0f-12.0f,0*16.0f,12.0f,16.0f);


			

			if ( (int)this->AnimIter == 2 )
				this->AnimIter += rateOfUpdate;	//	Delay Time
		}

		if ( (int)this->AnimIter >= 3 )
		{
			SpriteDrawer::Draw(TextureManager::Get(1),50.0f,80.0f,95.0f,25.0f,
				95.0f,512.0f-25.0f,95.0f,25.0f);	//	Time

			for ( int time = 0; time < 2; time++ )
				SpriteDrawer::Draw(TextureManager::Get(1),200.0f+11.0f*time,82.5f,12.0f,16.0f,1024.0f-12.0f,16.0f*this->timeData[time],12.0f,16.0f,0.0f,0.0f,0.0f);

			SpriteDrawer::Draw(TextureManager::Get(1),200.0f+11.0f*2,82.5f,12.0f,16.0f,1024.0f-12.0f,16.0f*10,12.0f,16.0f,0.0f,0.0f,0.0f);

			for ( int time2 = 2; time2 < 4; time2++ )
				SpriteDrawer::Draw(TextureManager::Get(1),200.0f+11.0f*(time2+1),82.5f,12.0f,16.0f,1024.0f-12.0f,16.0f*this->timeData[time2],12.0f,16.0f,0.0f,0.0f,0.0f);

			if ( (int)this->AnimIter == 3 )
				this->AnimIter += rateOfUpdate;	//	Delay Time
		}

		if ( (int)this->AnimIter >= 4 )
		{
			SpriteDrawer::Draw(TextureManager::Get(1),320.0f*0.5,210.0f*0.5,230.0f,25.0f,
				1024.0f-64.0f-230.0f,512.0f-25.0f,230.0f,25.0f, 230.0f*0.5,25.0f*0.25);	//	Line
			if ( (int)this->AnimIter == 4 )
				this->AnimIter += rateOfUpdate;	//	Delay Time
		}

		if ( (int)this->AnimIter >= 5 )
		{
			SpriteDrawer::Draw(TextureManager::Get(1),50.0f,120.0f,95.0f,25.0f,
				190.0f,512.0f-25.0f,95.0f,25.0f);	//	Score

			for ( int digit = 0; digit < 4; digit++ )
				SpriteDrawer::Draw(TextureManager::Get(1),195.0f+13.0f*digit,120.0f,15.0f,20.0f,1024.0f-12.0f,16.0f*this->ScoreData[digit],12.0f,16.0f);

			if ( (int)this->AnimIter == 5 )
				this->AnimIter += rateOfUpdate;	//	Delay Time
		}

		if ( (int)this->AnimIter >= 6 )
		{
			for ( int stars = 0; stars < 3; stars++ )
				SpriteDrawer::Draw(TextureManager::Get(1),96.0f+stars*64.0f,175.0f,64.0f,64.0f,
					1024.0f-64.0f,512.0f-64.0f,64.0f,64.0f,64.0f*0.5,64.0f*0.5);	//	Empty Stars
			if ( (int)this->AnimIter == 6 )
				this->AnimIter += rateOfUpdate;	//	Delay Time
		}

		if ( (int)this->AnimIter >= 7 )
		{
			if ( this->Score > maxScore / 3 )
				SpriteDrawer::Draw(TextureManager::Get(1),96.0f+0*64.0f,175.0f,64.0f,64.0f,
					1024.0f-64.0f,512.0f-(64.0f*2),64.0f,64.0f,64.0f*0.5,64.0f*0.5);	//	Filled Stars 1
			if ( (int)this->AnimIter == 7 )
				this->AnimIter += rateOfUpdate;	//	Delay Time
		}

		if ( (int)this->AnimIter >= 8 )
		{
			if ( this->Score > maxScore / 3 * 2 )
				SpriteDrawer::Draw(TextureManager::Get(1),96.0f+1*64.0f,175.0f,64.0f,64.0f,
					1024.0f-64.0f,512.0f-(64.0f*2),64.0f,64.0f,64.0f*0.5,64.0f*0.5);	//	Filled Stars 2
			if ( (int)this->AnimIter == 8 )
				this->AnimIter += rateOfUpdate;	//	Delay Time
		}

		if ( (int)this->AnimIter >= 9 )
		{
			if ( this->Score > maxScore )
				SpriteDrawer::Draw(TextureManager::Get(1),96.0f+2*64.0f,175.0f,64.0f,64.0f,
					1024.0f-64.0f,512.0f-(64.0f*2),64.0f,64.0f,64.0f*0.5,64.0f*0.5);	//	Filled Stars 3
			if ( (int)this->AnimIter == 9 )
				this->AnimIter += rateOfUpdate;	//	Delay Time
		}

		if ( (int)this->AnimIter >= 10 )
		{
			//	Draw The Main Menu Button
			SpriteDrawer::Draw(TextureManager::Get(3), this->theButton[0].GetPosX(), this->theButton[0].GetPosY(), this->theButton[0].GetWidth(), this->theButton[0].GetHeight(),
				0.0f,256.0f-44.0f,this->theButton[0].GetWidth(),this->theButton[0].GetHeight(),this->theButton[0].GetWidth()*0.5,this->theButton[0].GetHeight()*0.5);

			// NEXT
			if (ID != NUM_OF_STAGES*NUM_OF_LEVELS_INLEVEL)
			SpriteDrawer::Draw(TextureManager::Get(3), this->theButton[1].GetPosX(), this->theButton[1].GetPosY(), this->theButton[1].GetWidth(), this->theButton[1].GetHeight(),
				100.0f,256.0f-44.0f,this->theButton[1].GetWidth(),this->theButton[1].GetHeight(),this->theButton[1].GetWidth()*0.5,this->theButton[1].GetHeight()*0.5);

			// Restart
			SpriteDrawer::Draw(TextureManager::Get(3), this->theButton[2].GetPosX(), this->theButton[2].GetPosY(), this->theButton[2].GetWidth(), this->theButton[2].GetHeight(),
				512.0f-225.0f-32.0f,256.0f-32.0f,this->theButton[2].GetWidth(),this->theButton[2].GetHeight(),this->theButton[2].GetWidth()*0.5,this->theButton[2].GetHeight()*0.5);
		}
	}

	//	Confirmation
	Confirmation::Confirmation()
	{
	};

	Confirmation::~Confirmation()
	{
		Sound::GetInstance().Finalize();
		TextureManager::AllRemove();
	};

	void Confirmation::Initialize()
	{
		//TextureManager::Entry(20, "water");
		if ( CommonData::GetInstance().GetOptionData()->language == 1 )
		{
			TextureManager::Entry(21, "GameScreenSprite_EN");
		}
		else
		{
			TextureManager::Entry(21, "GameScreenSprite_JP");
		}
		this->numOfButton = 2;

		// Load sound
		Sound::GetInstance().Initialize("RippleSoundOther");
		Sound::GetInstance().STRM_SetVolume(CommonData::GetInstance().GetOptionData()->bgmVolume);
		Sound::GetInstance().SE_SetVolumeAll(CommonData::GetInstance().GetOptionData()->sfxVolume);

		//	percentage for ripple
		this->percentage = 0.75f;
		this->currentAlpha = 1.0f;

		//	Init Buttons
		this->theButton = new Button[this->numOfButton];
		this->theButton[0].Init(320.0f/2-50.0f,240.0f/2+15.0f,64.0f,44.0f,ButtonType::YES,0.0f,0.0f);
		this->theButton[1].Init(320.0f/2+50.0f,240.0f/2+15.0f,64.0f,44.0f,ButtonType::NO,0.0f,0.0f);
	};


	void Confirmation::Update()
	{
		if ( this->percentage < 1.0f )
			this->percentage += 0.025f;
		else
		{
			if ( this->currentAlpha - 0.025f > 0.0f )
				this->currentAlpha -= 0.025f;
			else
			{
				this->currentAlpha = 1.0f;
				this->percentage = 0.75f;
			}
		}
	};

	ButtonType Confirmation::GetPushButtonType()
	{
		for ( int button = 0; button < this->numOfButton; button++ )
		{
			if ( this->theButton[button].DetectCollisionTouchRelease() )
			{
				return this->theButton[button].GetCurrentButtonType();
			}
		}
		return NONE;
	};

	void Confirmation::DrawUpper()
	{
		//	Draw the Water BG
		SpriteDrawer::Draw(TextureManager::Get(ETEXTURE_WATER),0.0f,0.0f,400.0f,240.0f,0.0f,0.0f,400.0f,240.0f);
	}

	void Confirmation::DrawLower()
	{
		//	Draw the Water BG
		SpriteDrawer::Draw(TextureManager::Get(ETEXTURE_WATER),0.0f,0.0f,320.0f,240.0f,(400.0f-320.0f)/2,240.0f,320.0f,240.0f);

		//	Animation in Background ( Ripples )
		for ( int ripple = 0; ripple < 3; ripple++ )
		{
			if ( ripple == 0 )
				SpriteDrawer::Draw(TextureManager::Get(21),320.0f/2+60.0f,240.0f/2-30.0f,128.0f*this->percentage,128.0f*this->percentage,
					512.0f-128.0f,0.0f,128.0f,128.0f,128.0f*this->percentage/2,128.0f*this->percentage/2,0.0f,this->currentAlpha);
			else if ( ripple == 1 )
				SpriteDrawer::Draw(TextureManager::Get(21),320.0f/2-20.0f,240.0f/2+25.0f,(128.0f*0.75f)*this->percentage,(128.0f*0.75f)*this->percentage,
					512.0f-128.0f,0.0f,128.0f,128.0f,(128.0f*0.75f)*this->percentage/2,(128.0f*0.75f)*this->percentage/2,0.0f,this->currentAlpha);
			else if ( ripple == 2 )
				SpriteDrawer::Draw(TextureManager::Get(21),320.0f/2-90.0f,240.0f/2-25.0f,(128.0f*0.5f)*this->percentage,(128.0f*0.5f)*this->percentage,
					512.0f-128.0f,0.0f,128.0f,128.0f,(128.0f*0.5f)*this->percentage/2,(128.0f*0.5f)*this->percentage/2,0.0f,this->currentAlpha);
		}

		//	Draw the words
		//SpriteDrawer::Draw(TextureManager::Get(21),320.0f/2,240.0f/2-30.0f,146.0f,26.0f,512.0f-146.0f,256.0f-26.0f,146.0f,26.0f,146.0f/2,26.0f/2);

		//	Draw Buttons
		for ( int button = 0; button < this->numOfButton; button++ )
		{
			if ( !this->theButton[button].DetectCollision() )
				SpriteDrawer::Draw(TextureManager::Get(21),this->theButton[button].GetPosX(),this->theButton[button].GetPosY(),this->theButton[button].GetWidth(),this->theButton[button].GetHeight(),
					512.0f-128.0f-64.0f,button*88.0f,this->theButton[button].GetWidth(),this->theButton[button].GetHeight(),this->theButton[button].GetWidth()/2,this->theButton[button].GetHeight()/2);
			else
				SpriteDrawer::Draw(TextureManager::Get(21),this->theButton[button].GetPosX(),this->theButton[button].GetPosY(),this->theButton[button].GetWidth(),this->theButton[button].GetHeight(),
					512.0f-128.0f-64.0f,44.0f+button*88.0f,this->theButton[button].GetWidth(),this->theButton[button].GetHeight(),this->theButton[button].GetWidth()/2,this->theButton[button].GetHeight()/2);
		}
	}

	//	MenuInGame
	MenuInGame::MenuInGame()
	{
	};

	MenuInGame::~MenuInGame()
	{
		TextureManager::AllRemove();
	};

	void MenuInGame::Initialize()
	{
		//TextureManager::Entry(20, "water");
		if ( CommonData::GetInstance().GetOptionData()->language == 1 )
		{
			TextureManager::Entry(ETEXTURE_BACK, "Back_EN");
		}
		else
		{
			TextureManager::Entry(ETEXTURE_BACK, "Back_JP");
		}
		this->numOfButton = 3;

		//	percentage for ripple
		this->percentage = 0.75f;
		this->currentAlpha = 1.0f;

		//	Init Buttons
		this->theButton = new Button[this->numOfButton];
		this->theButton[0].Init(320.0f/2,240.0f/2-25.0f, 225.0f,35.0f,RESTART,0.0f,0.0f);
		this->theButton[1].Init(320.0f/2,240.0f/2+25.0f, 225.0f,35.0f,MENU,0.0f,0.0f);
		this->theButton[2].Init(100.0f/2,240.0f-15.0f-10.0f,100.0f,30.0f,BACK,0.0f,0.0f);
	};


	void MenuInGame::Update()
	{
		if ( this->percentage < 1.0f )
			this->percentage += 0.025f;
		else
		{
			if ( this->currentAlpha - 0.025f > 0.0f )
				this->currentAlpha -= 0.025f;
			else
			{
				this->currentAlpha = 1.0f;
				this->percentage = 0.75f;
			}
		}
	};

	ButtonType MenuInGame::GetPushButtonType()
	{
		for ( int button = 0; button < this->numOfButton; button++ )
		{
			if ( this->theButton[button].DetectCollisionTouchRelease() )
			{
				return this->theButton[button].GetCurrentButtonType();
			}
		}
		return NONE;
	};

	void MenuInGame::DrawUpper()
	{
		//	Draw the Water BG
		SpriteDrawer::Draw(TextureManager::Get(ETEXTURE_WATER),0.0f,0.0f,400.0f,240.0f,0.0f,0.0f,400.0f,240.0f);
	}

	void MenuInGame::DrawLower()
	{
		//	Draw the Water BG
		SpriteDrawer::Draw(TextureManager::Get(ETEXTURE_WATER),0.0f,0.0f,320.0f,240.0f,(400.0f-320.0f)/2,240.0f,320.0f,240.0f);

		//	Animation in Background ( Ripples )
		for ( int ripple = 0; ripple < 3; ripple++ )
		{
			if ( ripple == 0 )
				SpriteDrawer::Draw(TextureManager::Get(ETEXTURE_UI),320.0f/2+60.0f,240.0f/2-30.0f,128.0f*this->percentage,128.0f*this->percentage,
					512.0f-128.0f,0.0f,128.0f,128.0f,128.0f*this->percentage/2,128.0f*this->percentage/2,0.0f,this->currentAlpha);
			else if ( ripple == 1 )
				SpriteDrawer::Draw(TextureManager::Get(ETEXTURE_UI),320.0f/2-20.0f,240.0f/2+25.0f,(128.0f*0.75f)*this->percentage,(128.0f*0.75f)*this->percentage,
					512.0f-128.0f,0.0f,128.0f,128.0f,(128.0f*0.75f)*this->percentage/2,(128.0f*0.75f)*this->percentage/2,0.0f,this->currentAlpha);
			else if ( ripple == 2 )
				SpriteDrawer::Draw(TextureManager::Get(ETEXTURE_UI),320.0f/2-90.0f,240.0f/2-25.0f,(128.0f*0.5f)*this->percentage,(128.0f*0.5f)*this->percentage,
					512.0f-128.0f,0.0f,128.0f,128.0f,(128.0f*0.5f)*this->percentage/2,(128.0f*0.5f)*this->percentage/2,0.0f,this->currentAlpha);
		}
		//	Draw Buttons
		for ( int button = 0; button < this->numOfButton; button++ )
		{
			if (this->theButton[button].GetCurrentButtonType() == BACK)
			{
				if ( !this->theButton[button].DetectCollision() )
					SpriteDrawer::Draw(TextureManager::Get(ETEXTURE_BACK),this->theButton[button].GetPosX(),this->theButton[button].GetPosY(),this->theButton[button].GetWidth(),this->theButton[button].GetHeight(),
						0.0f,0.0f,this->theButton[button].GetWidth(),this->theButton[button].GetHeight(),this->theButton[button].GetWidth()/2,this->theButton[button].GetHeight()/2);
				else
					SpriteDrawer::Draw(TextureManager::Get(ETEXTURE_BACK),this->theButton[button].GetPosX()-10.0f,this->theButton[button].GetPosY(),this->theButton[button].GetWidth(),this->theButton[button].GetHeight(),
						0.0f,0.0f,this->theButton[button].GetWidth(),this->theButton[button].GetHeight(),this->theButton[button].GetWidth()/2,this->theButton[button].GetHeight()/2);
			}
			else 
			{
				if ( !this->theButton[button].DetectCollision() )
					SpriteDrawer::Draw(TextureManager::Get(ETEXTURE_UI),this->theButton[button].GetPosX(),this->theButton[button].GetPosY(),this->theButton[button].GetWidth(),this->theButton[button].GetHeight(),
						0.0f,256.0f-44.0f-140.0f+button*70.0f,this->theButton[button].GetWidth(),this->theButton[button].GetHeight(),this->theButton[button].GetWidth()/2,this->theButton[button].GetHeight()/2);
				else
					SpriteDrawer::Draw(TextureManager::Get(ETEXTURE_UI),this->theButton[button].GetPosX(),this->theButton[button].GetPosY(),this->theButton[button].GetWidth(),this->theButton[button].GetHeight(),
						0.0f,256.0f-44.0f-140.0f+35.0f+button*70.0f,this->theButton[button].GetWidth(),this->theButton[button].GetHeight(),this->theButton[button].GetWidth()/2,this->theButton[button].GetHeight()/2);
			}
		}
	}
};	