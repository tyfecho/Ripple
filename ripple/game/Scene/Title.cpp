#include "Title.h"

#include "GameLevel.h"


#include "../../NW_Lib/System/NW_Lib_SceneManager.h"

#include "../../NW_Lib/Graphics/NW_Lib_SpriteDrawer.h"
#include "../../NW_Lib/Graphics/NW_Lib_BillboardDrawer.h"
#include "../../NW_Lib/Graphics/NW_Lib_TextureManager.h"
#include "../../NW_Lib/Graphics/NW_Lib_Billboard.h"

#include "../../NW_Lib/Graphics/NW_Lib_ModelManager.h"
#include "../../NW_Lib/Graphics/NW_Lib_ModelControler.h"

#include "../../NW_Lib/Utility/NW_Lib_DebugFontManager.h"
#include "../../NW_Lib/System/NW_Lib_InputManager.h"
#include "../../NW_Lib/System/NW_Lib_RomFileStream.h"

// �T�E���h
#include "../../NW_Lib/Sound/NW_Lib_Sound.h"

// �J����
#include "../../NW_Lib/System/NW_Lib_CameraControler.h"

// ���C�g
#include "../../NW_Lib/System/NW_Lib_LightControler.h"

// �p�[�e�B�N��
#include "../../NW_Lib/Graphics/NW_Lib_ParticleControler.h"
#include "../../NW_Lib/Graphics/NW_Lib_ParticleManager.h"

//Sound Data
#ifdef _DirectX

	//#include SoundData
	//#include "../../works_romfiles/sound/RippleSoundOther.h"

#endif
#ifdef _3DS

	#include "../../NW_Lib/Graphics/NW_Lib_ResTextureManager.h"

	//#include SoundData

	// �T�E���h
#endif

using namespace NW_Lib;


namespace Game
{

	/**
	*	  �R���X�g���N�^      
	*/
	Title::Title()
	{
	}

	/**
	*	  �f�X�g���N�^      
	*/
	Title::~Title()
	{
		TextureManager::AllRemove();
		
	}
		static VECTOR2 ball = VECTOR2(0.0f,0.0f);
		static VECTOR2 bvel = VECTOR2(0.3f,0.3f);
		static VECTOR2 pos1 = VECTOR2(-5.0f,0.0f);
		static VECTOR2 pos2 = VECTOR2(5.0f,0.0f);
		static int point1 = 0;
		static int point2 = 0;

		static VECTOR2 posLogo = VECTOR2();
		static VECTOR2 velLogo = VECTOR2();
		static VECTOR2 posTexLogo = VECTOR2(200.0f, 240.0f);

	/**
	*	����������
	*/
	void Title::Initialize()
	{
		TextureManager::Entry(0 , "obj");
		TextureManager::Entry(1 , "LogoScreen");

		//Sound::GetInstance().Initialize("RippleSoundOther");
		//Sound::GetInstance().SE_Entry(SE_YOSHI);

		posLogo.x = UPPER_SCREEN_WIDTH/2 - 100.0f;
		posLogo.y = 0 - 80.0f;

	}
	/**
	*	�X�V
	*/
	void Title::Update()
	{
		// ����
		if( System::InputManager::GetButtonPush(BUTTON_UP) )
		{
			pos1.y += 0.2f;
		}
		if( System::InputManager::GetButtonPush(BUTTON_DOWN) )
		{
			pos1.y -= 0.2f;
		}
		
		if( System::InputManager::GetButtonPush(BUTTON_A) )
		{
			System::SceneManager::GetInstance().ChangeScene(new GameLevel(1));
		}
		// ����
		pos2.y = ((1.0f - System::InputManager::GetTouchPosition().y / 240.0f) * 6.4f - 3.2f );

		// ���˕Ԃ�
		if (ball.y < -3.3f || ball.y > 3.3f)
		{
			bvel.y = -bvel.y;
		}

		// ������
		if (bvel.x < 0 &&
			pos1.x - 0.25f <= ball.x + 0.25f && pos1.x + 0.25f >= ball.x - 0.25f &&
			pos1.y - 1.0f  <= ball.y + 0.25f && pos1.y + 1.0f  >= ball.y - 0.25f)
		{
			bvel.x = -bvel.x;
		}
		if (bvel.x > 0 &&
			pos2.x - 0.25f <= ball.x + 0.25f && pos2.x + 0.25f >= ball.x - 0.25f &&
			pos2.y - 1.0f  <= ball.y + 0.25f && pos2.y + 1.0f  >= ball.y - 0.25f)
		{
			bvel.x = -bvel.x;
		}

		// �_��
		if (ball.x < -5.3f)
		{
			++point2;
			ball = VECTOR2(0.0f,0.0f);
			//Sound::GetInstance().SE_Play(SE_YOSHI);
		}
		if (ball.x > 5.3f)
		{
			++point1;
			ball = VECTOR2(0.0f,0.0f);
			//Sound::GetInstance().SE_Play(SE_YOSHI);
		}

		ball += bvel;
		
		/*
		DebugFontManager::Printf(UPPER_SCREEN_DRAWING,
						100, 100, "%d - %d", point1, point2);
		DebugFontManager::Printf(UPPER_SCREEN_DRAWING,
			100, 50, "mouse %.0f,%.0f %d %d %d", System::InputManager::GetTouchPosition().x, System::InputManager::GetTouchPosition().y,System::InputManager::GetTouchTrigger(),System::InputManager::GetTouchPush() , System::InputManager::GetTouchRelease());
			*/
		velLogo.y += 0.75f;
		posLogo.y += velLogo.y;
		if (posLogo.y > 80.0f)
		{
			posLogo.y = 80.0f;
			posTexLogo.x = 0;
		}
						
	}

	/**
	*	���ʕ`��
	*/
	void Title::DrawUpper()
	{

		SpriteDrawer::Draw(
			TextureManager::Get(1),
			0.0f,0.0f,400.0f,240.0f,
			0.0f,0.0f,400.0f,240.0f);
		/*
		SpriteDrawer::Draw(
			TextureManager::Get(1),
			posLogo.x,posLogo.y,200.0f,80.0f,
			0.0f,240.0f,200.0f,80.0f);
			*/
		SpriteDrawer::Draw(
			TextureManager::Get(1),
			posLogo.x,posLogo.y,200.0f,80.0f,
			posTexLogo.x,posTexLogo.y,200.0f,80.0f);
	}

	/**
	*	����ʕ`��
	*/
	void Title::DrawLower()
	{
		//billboard->DrawAll(TextureManager::Get(1),pos.x,pos.y,0.0f,0.0f,0.0f,128.0f,128.0f,0.0f);


		BillboardDrawer::Draw(TextureManager::Get(0),pos1.x,pos1.y,0.0f,0.5f,2.0f,0.0f,0.0f,64.0f,256.0f);
		BillboardDrawer::Draw(TextureManager::Get(0),pos2.x,pos2.y,0.0f,0.5f,2.0f,0.0f,0.0f,64.0f,256.0f);
		BillboardDrawer::Draw(TextureManager::Get(0),ball.x,ball.y,0.0f,0.5f,0.5f,64.0f,0.0f,64.0f,64.0f);
	}
} 
