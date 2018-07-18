#include "LevelSelect.h"

// Lib
#include "../../NW_Lib/System/NW_Lib_SceneManager.h"
#include "../../NW_Lib/System/NW_Lib_InputManager.h"
#include "../../NW_Lib/System/NW_Lib_RomFileStream.h"
#include "../../NW_Lib/Sound/NW_Lib_Sound.h"
#include "../../NW_Lib/Graphics/NW_Lib_SpriteDrawer.h"
#include "../../NW_Lib/Graphics/NW_Lib_TextureManager.h"
#include "../../NW_Lib/Utility/NW_Lib_DebugFontManager.h"
#include "../../NW_Lib/Utility/NW_Lib_CommonData.h"


#include "../GameObject.h"
// Scene
#include "GameLevel.h"
#include "MainMenu.h"

// Sound Data
#ifdef _DirectX
	//#include SoundData
	#include "../../works_romfiles/sound/RippleSoundOther.h"
#endif
#ifdef _3DS
	#include "../../NW_Lib/Graphics/NW_Lib_ResTextureManager.h"
	//#include SoundData
	// サウンド
#endif

using namespace NW_Lib;

namespace Game
{

	/**
	*	  コンストラクタ      
	*/
	LevelSelect::LevelSelect(int stage, int level)
	: m_elapsedFrame(0)
	, m_selectedStage(stage)
	, m_selectedLevel(level)
	, m_carpPosX(200)
	{
	}

	/**
	*	  デストラクタ      
	*/
	LevelSelect::~LevelSelect()
	{

		for (int i=0; i<NUM_OF_BUTTON_TYPE; i++)
		{
			delete m_buttons[i];
		}
		for (int i=0; i<NUM_OF_LEVELS_INLEVEL; i++)
		{
			delete m_levels[i];
		}
		delete[] m_buttons;
		delete[] m_levels;
		delete m_lowerBG;
		delete m_upperBG;
		m_buttons = NULL;
		m_levels = NULL;
		m_lowerBG = NULL;
		m_upperBG = NULL;

		Sound::GetInstance().Finalize();
		TextureManager::AllRemove();
	}

	/**
	*	初期化処理
	*/
	void LevelSelect::Initialize()
	{
		// load texture
		TextureManager::Entry(WATER, "water");
		TextureManager::Entry(LOG_FRAME, "LogFrame");
		TextureManager::Entry(BOARD, "LevelSelectScene/Board");
		TextureManager::Entry(BABY_SWAN, "babyswan");
		TextureManager::Entry(LEAF, "LevelSelectScene/Leaf");
		TextureManager::Entry(DECORATION, "LevelSelectScene/Decoration");
		TextureManager::Entry(SCORE_STAR, "ScoreStar");
		TextureManager::Entry(LOTUS_FLOWERS, "LevelSelectScene/Lotusflowers");
		if ( CommonData::GetInstance().GetOptionData()->language == 1 )
		{
			TextureManager::Entry(PLAY_BUTTON, "TitleScreenButtons_EN");
			TextureManager::Entry(BACK_BUTTON, "OptionScreenSprite_EN");
		}
		else
		{
			TextureManager::Entry(PLAY_BUTTON, "TitleScreenButtons_JP");
			TextureManager::Entry(BACK_BUTTON, "OptionScreenSprite_JP");
		}
		TextureManager::Entry(CARP_ANIME, "LevelSelectScene/carp anime");
		TextureManager::Entry(PIRANIA_ANIME, "LevelSelectScene/PiraniaAnime");
		TextureManager::Entry(FONT, "LevelSelectScene/Font");

		// load sound
		Sound::GetInstance().Initialize("RippleSoundOther");
		Sound::GetInstance().STRM_SetVolume(CommonData::GetInstance().GetOptionData()->bgmVolume/8.0f);
		Sound::GetInstance().SE_SetVolumeAll(CommonData::GetInstance().GetOptionData()->sfxVolume/8.0f);
		Sound::GetInstance().SE_Entry(SE_CANCEL);
		Sound::GetInstance().SE_Entry(SE_DECIDE);
		Sound::GetInstance().SE_Entry(SE_SELECT);
		Sound::GetInstance().SE_Entry(SE_WHISTLE);

		// Play BGM
		Sound::GetInstance().STRM_Play(BGM_TITLE);

		// 上画面用スプライト準備
		m_upperBG = new SpriteObject(LOWER_SCREEN_OFFSET_WIDTH, 0.0f, 320.0f, 240.0f, 40.0f, 240.0f, 320.0f, 240.0f);

		// 下画面用座標準備
		m_lowerBG = new SpriteObject(0.0f, 0.0f, 320.0f, 240.0f, 40.0f, 240.0f, 320.0f, 240.0f);
		m_levels = new SpriteObject*[NUM_OF_LEVELS_INLEVEL];
		// レベル情報の読み込み
		for (int i=0; i<NUM_OF_LEVELS_INLEVEL; i++)
		{
			VECTOR2 pos = VECTOR2(LEVEL_SPRITE_POSDATA[m_selectedStage-1][i].x, LEVEL_SPRITE_POSDATA[m_selectedStage-1][i].y);
			
			int highScore = CommonData::GetInstance().GetLevelInfo((m_selectedStage-1)*NUM_OF_LEVELS_INLEVEL+i+1)->highScore;
			if (highScore != 0)
			{
				m_levels[i] = new SpriteObject(pos.x-32.0f, pos.y-32.0f, 64.0f, 64.0f,  0.0f, 64.0f, 64.0f, 64.0f);
			}
			else 
			{
				m_levels[i] = new SpriteObject(pos.x-16.0f, pos.y-16.0f, 32.0f, 32.0f,  0.0f,  0.0f, 64.0f, 64.0f);

			}
		}
		// LSBUTTON::LSBUTTON_PLAYや、チェンジステージボタン
		m_buttons = new SpriteObject*[NUM_OF_BUTTON_TYPE];
		m_buttons[LSBUTTON_BACK_STAGE] = new SpriteObject(30.0f, LOWER_SCREEN_HEIGHT-64.0f, 64.0f, 64.0f, 64.0f, 0.0f, -64.0f, 64.0f, false, 64.0f, 40.0f);
		m_buttons[LSBUTTON_NEXT_STAGE] = new SpriteObject(LOWER_SCREEN_WIDTH-64.0f-30.0f, LOWER_SCREEN_HEIGHT-64.0f, 64.0f, 64.0f, 0.0f, 0.0f, 64.0f, 64.0f, false, 4.0f, 40.0f);
		m_buttons[LSBUTTON_PLAY_BG] = new SpriteObject(LOWER_SCREEN_WIDTH/2-32.0f, LOWER_SCREEN_HEIGHT-64.0f, 64.0f, 64.0f, 64.0f, 0.0f, 64.0f, 64.0f);
		m_buttons[LSBUTTON_PLAY] = new SpriteObject(LOWER_SCREEN_WIDTH/2-32.0f, LOWER_SCREEN_HEIGHT-64.0f, 64.0f, 64.0f, 0.0f, 0.0f, 128.0f, 128.0f);
		m_buttons[LSBUTTON_BACK_TO_TITLE] = new SpriteObject(0.0f, LOWER_SCREEN_HEIGHT/2-52.0f, 26.0f, 105.0f, 512.0f-52.0f, 0.0f, 26.0f, 105.0f);

		// ステージ１、最後のステージの場合、「前のレベルへ」「次のレベルへ」ボタンを隠す
		if (m_selectedStage == 1)
			m_buttons[LSBUTTON_BACK_STAGE]->SetPos(VECTOR2(-100, -100));
		if (m_selectedStage == NUM_OF_STAGES)
			m_buttons[LSBUTTON_NEXT_STAGE]->SetPos(VECTOR2(-100, -100));

	}
	/**
	*	更新
	*/
	void LevelSelect::Update()
	{
		ProccessInput();

		m_elapsedFrame++;
		m_carpPosX--;
	}

	/**
	*	Inputはまとめてここに
	*/
	void LevelSelect::ProccessInput()
	{
		// sound test
		if (System::InputManager::GetButtonTrigger(BUTTON_L)) Sound::GetInstance().STRM_Play(BGM_TITLE, 60, 60);
		if (System::InputManager::GetButtonTrigger(BUTTON_R)) Sound::GetInstance().STRM_SetStop();
		if (System::InputManager::GetButtonPush(BUTTON_SELECT) && System::InputManager::GetButtonPush(BUTTON_START))		
			System::SceneManager::GetInstance().ChangeScene(new MainMenu());

		// コントローラでの選択
		// 十字キーでレベルセレクト
		if (m_selectedLevel<5 && System::InputManager::GetButtonTrigger(BUTTON_RIGHT))
		{
			Sound::GetInstance().SE_Play(SE_SELECT);
			m_selectedLevel++;
		}
		if (m_selectedLevel>1 && System::InputManager::GetButtonTrigger(BUTTON_LEFT))
		{
			Sound::GetInstance().SE_Play(SE_SELECT);
			m_selectedLevel--;
		}
		// Level Play
		if (System::InputManager::GetButtonTrigger(BUTTON_A))
		{
			Sound::GetInstance().SE_Play(SE_WHISTLE);
			System::SceneManager::GetInstance().ChangeScene(new GameLevel((m_selectedStage-1)*5 + m_selectedLevel));
		}
		// Back MainMenu
		if (System::InputManager::GetButtonTrigger(BUTTON_B))
		{
			Sound::GetInstance().SE_Play(SE_CANCEL);
			System::SceneManager::GetInstance().ChangeScene(new MainMenu());
		}

		VECTOR2 touch = System::InputManager::GetTouchPosition();
		// タッチダウン中
		if (System::InputManager::GetTouchPush())
		{
			VECTOR2 touch = System::InputManager::GetTouchPosition();

			// レベルの花との当たり判定
			for (int i=0; i<NUM_OF_LEVELS_INLEVEL; i++)
			{
				VECTOR2 posOfCenter = VECTOR2(m_levels[i]->GetPos().x+m_levels[i]->GetSize().x/2, m_levels[i]->GetPos().y+m_levels[i]->GetSize().y/2);

				if (CollideAABBAABB(touch, VECTOR2(0.0f, 0.0f), posOfCenter, m_levels[i]->GetSize()))
				{
					// SE Play
					if (m_selectedLevel != i+1)
						Sound::GetInstance().SE_Play(SE_SELECT);
					m_selectedLevel = i+1;
				}
			}
			// PLAY,矢印等、ボタン類との当たり判定とスプライト入れ替え
			for (int i=0; i<NUM_OF_BUTTON_TYPE; i++)
			{
				VECTOR2 posOfCenter = VECTOR2(m_buttons[i]->GetPos().x+m_buttons[i]->GetSize().x/2, m_buttons[i]->GetPos().y+m_buttons[i]->GetSize().y/2);
				if (CollideAABBAABB(touch, VECTOR2(0.0f, 0.0f), posOfCenter, m_buttons[i]->GetSize()))
				{
					switch (i)
					{
						case LSBUTTON_BACK_STAGE:
						case LSBUTTON_NEXT_STAGE:
							m_buttons[i]->SetAngle((i%2*2-1)*10.0f);
							break;

						case LSBUTTON_PLAY_BG:
						case LSBUTTON_PLAY:
							m_buttons[LSBUTTON_PLAY]->SetTexPos(VECTOR2(128.0f, 0.0f));
							break;
							
						case LSBUTTON_BACK_TO_TITLE:
							m_buttons[LSBUTTON_BACK_TO_TITLE]->SetTexPos(VECTOR2(512.0f-26.0f, 0.0f));
						default:
							break;
					}
				}
				else
				{
					switch (i)
					{
						case LSBUTTON_BACK_STAGE:
						case LSBUTTON_NEXT_STAGE:
							m_buttons[i]->SetAngle(0.0f);
							break;

						case LSBUTTON_PLAY_BG:
						case LSBUTTON_PLAY:
							m_buttons[LSBUTTON_PLAY]->SetTexPos(VECTOR2(0.0f, 0.0f));
							break;
							
						case LSBUTTON_BACK_TO_TITLE:
							m_buttons[LSBUTTON_BACK_TO_TITLE]->SetTexPos(VECTOR2(512.0f-52.0f, 0.0f));
						default:
							break;
					}
				}
			}

		}
		// タッチアップ時
		if (System::InputManager::GetTouchRelease())
		{
			VECTOR2 touch = System::InputManager::GetTouchPosition();
			// 矢印等、ボタン類との当たり判定とスプライト入れ替え
			for (int i=0; i<NUM_OF_BUTTON_TYPE; i++)
			{
				VECTOR2 posOfCenter = VECTOR2(m_buttons[i]->GetPos().x+m_buttons[i]->GetSize().x/2, m_buttons[i]->GetPos().y+m_buttons[i]->GetSize().y/2);

				if (CollideAABBAABB(touch, VECTOR2(0.0f, 0.0f), posOfCenter, m_buttons[i]->GetSize()))
				{
					switch (i)
					{
						case LSBUTTON_BACK_STAGE:
							m_buttons[i]->SetAngle(0.0f);
							m_selectedStage += (i*2-1);
							Sound::GetInstance().SE_Play(SE_DECIDE);
							System::SceneManager::GetInstance().SetLoadAnimation(true);
							System::SceneManager::GetInstance().ChangeScene(new LevelSelect(m_selectedStage, 5));
							break;

						case LSBUTTON_NEXT_STAGE:
							m_buttons[i]->SetAngle(0.0f);
							m_selectedStage += (i*2-1);
							Sound::GetInstance().SE_Play(SE_DECIDE);
							System::SceneManager::GetInstance().SetLoadAnimation(true);
							System::SceneManager::GetInstance().ChangeScene(new LevelSelect(m_selectedStage, 1));
							break;

						case LSBUTTON_PLAY_BG:
						case LSBUTTON_PLAY:
							Sound::GetInstance().SE_Play(SE_WHISTLE);
							System::SceneManager::GetInstance().SetLoadAnimation(true);
							System::SceneManager::GetInstance().ChangeScene(new GameLevel((m_selectedStage-1)*5 + m_selectedLevel));
							break;

						case LSBUTTON_BACK_TO_TITLE:
							Sound::GetInstance().SE_Play(SE_CANCEL);
							System::SceneManager::GetInstance().SetLoadAnimation(true);
							System::SceneManager::GetInstance().ChangeScene(new MainMenu());
							break;

						default:
							break;
					}
				}
			}
		}
	}

	/**
	*	上画面描画
	*/
	void LevelSelect::DrawUpper()
	{
		// BG
		m_upperBG->Draw(TextureManager::Get(WATER));
		// 鯉アニメーション
		static const int CarpAnimeIndex[16] = { 0,1,2,3,4,3,2,1,0,5,6,7,8,7,6,5 };
		SpriteDrawer::Draw(TextureManager::Get(CARP_ANIME),
			m_carpPosX, 80.0f, 84.0f, 42.0f,
			84.0f*(CarpAnimeIndex[m_elapsedFrame/5%15]%3), 42.0f*(CarpAnimeIndex[m_elapsedFrame/5%15]/3), 84.0f, 42.0f,
			0.0f, 0.0f, 0.0f, 0.5f
		);
		SpriteDrawer::Draw(TextureManager::Get(CARP_ANIME),
			m_carpPosX+85.0f, 110.0f, 84.0f, 42.0f,
			84.0f*(CarpAnimeIndex[(m_elapsedFrame/5+4)%15]%3), 42.0f*(CarpAnimeIndex[(m_elapsedFrame/5+4)%15]/3), 84.0f, 42.0f,
			0.0f, 0.0f, 0.0f, 0.5f
		);
		// leafs
		SpriteDrawer::Draw(TextureManager::Get(DECORATION),
			LOWER_SCREEN_OFFSET_WIDTH, 0.0f, 64.0f, 64.0f,
			0.0f, 64.0f, 64.0f, 64.0f,
			0.0f, 0.0f, 0.0f, 0.5f
		);
		SpriteDrawer::Draw(TextureManager::Get(DECORATION),
			UPPER_SCREEN_WIDTH-LOWER_SCREEN_OFFSET_WIDTH-64.0f, UPPER_SCREEN_HEIGHT-64.0f, 64.0f, 64.0f,
			64.0f, 128.0f, -64.0f, -64.0f,
			0.0f, 0.0f, 0.0f, 0.5f
		);
		// stone
		SpriteDrawer::Draw(TextureManager::Get(DECORATION),
			UPPER_SCREEN_WIDTH-LOWER_SCREEN_OFFSET_WIDTH-64.0f, UPPER_SCREEN_HEIGHT/3, 64.0f, 64.0f,
			64.0f, 64.0f, 64.0f, 64.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		// StageName Board & Font
		SpriteDrawer::Draw(TextureManager::Get(BOARD),
			UPPER_SCREEN_WIDTH-LOWER_SCREEN_OFFSET_WIDTH-220.0f, 0.0f, 220.0f, 110.0f,
			0.0f, 0.0f, 256.0f, 128.0f
		);
		SpriteDrawer::Draw(TextureManager::Get(FONT),
			200.0, 43.0f, 64.0f, 32.0f,
			0.0f, 0.0f, 64.0f, 32.0f
		);
		SpriteDrawer::Draw(TextureManager::Get(FONT),
			200.0f+64.0f+16.0f, 43.0f, 25.0f, 32.0f,
			25.0f*m_selectedStage, 32.0f, 25.0f, 32.0f
		);
		// LevelName Board & Font
		SpriteDrawer::Draw(TextureManager::Get(BOARD),
			UPPER_SCREEN_WIDTH/2-128.0f, UPPER_SCREEN_HEIGHT-96.0f, 256.0f, 96.0f,
			0.0f, 144.0f, 256.0f, 96.0f
		);
		SpriteDrawer::Draw(TextureManager::Get(FONT),
			100.0f, 182.0f, 64.0f*0.75f, 32.0f*0.75f,
			64.0f, 0.0f, 64.0f, 32.0f
		);
		SpriteDrawer::Draw(TextureManager::Get(FONT),
			100.0f+(64.0f*0.75f), 182.0f, 25.0f*0.75f, 32.0f*0.75f,
			25.0f*m_selectedLevel, 32.0f, 25.0f, 32.0f
		);
		// Score_Star
		for (int i=0; i<3; i++)
		{
			SpriteDrawer::Draw(TextureManager::Get(SCORE_STAR),
				200.0f+i*32.0f, 175.0f, 32.0f, 32.0f,
				0.0f, 64.0f, 64.0f, 64.0f
			);
		}
		int numOfStar = CommonData::GetInstance().GetLevelInfo((m_selectedStage-1)*NUM_OF_LEVELS_INLEVEL+m_selectedLevel)->highScore / (500/3);
		if (numOfStar >= 3)
			numOfStar = 3;
		for (int i=0; i<numOfStar; i++)
		{
			SpriteDrawer::Draw(TextureManager::Get(SCORE_STAR),
				200.0f+i*32.0f, 175.0f, 32.0f, 32.0f,
				0.0f, 0.0f, 64.0f, 64.0f
			);
		}

		// LogFrame
		NW_Lib::SpriteDrawer::Draw(TextureManager::Get(LOG_FRAME),          0.0f,0.0f,40.0f,240.0f, 0.0f,0.0f, 40.0f,240.0f);
		NW_Lib::SpriteDrawer::Draw(TextureManager::Get(LOG_FRAME),40.0f + 320.0f,0.0f,40.0f,240.0f,40.0f,0.0f,-40.0f,240.0f);

	}

	/**
	*	下画面描画
	*/
	void LevelSelect::DrawLower()
	{
		// BG & Level Button
		m_lowerBG->Draw(TextureManager::Get(WATER));
		// 次のレベルへの道描画
		for (int i=0; i<NUM_OF_LEVELS_INLEVEL-1; i++)
		{
			//if (CommonData::GetInstance().GetLevelData()[(m_selectedStage-1)*NUM_OF_LEVELS_INLEVEL+i].unlocked)
			{
				VECTOR2 vec = m_levels[i+1]->GetCenterPos() - m_levels[i]->GetCenterPos();
				int numOfDot = static_cast<int>(Vec2Length(vec)/20);
				for (int j=0; j<numOfDot; j++)
				{
					VECTOR2 pos =  m_levels[i]->GetCenterPos() + vec * (j+1.0f)/(numOfDot+1.0f);
					SpriteDrawer::Draw(TextureManager::Get(WATER),
						pos.x-3.0f, pos.y-3.0f, 6.0f, 6.0f,
						0.0f, 0.0f, 0.0f, 0.0f
					);
				}
			}
		}
		// 各レベル画像の描画
		for (int i=0; i<NUM_OF_LEVELS_INLEVEL; i++)
		{
			m_levels[i]->Draw(TextureManager::Get(LOTUS_FLOWERS));
		}
		// 選択しているレベルに赤ちゃん白鳥を置く
		SpriteDrawer::Draw(TextureManager::Get(BABY_SWAN),
			m_levels[m_selectedLevel-1]->GetCenterPos().x, m_levels[m_selectedLevel-1]->GetCenterPos().y, 32.0f, 32.0f,
			0.0f,  0.0f, 64.0f, 64.0f,
			16.0f, 16.0f, 180.0f+(sinf(m_elapsedFrame*PI/30)*10.0f), 1.0f
		);
		// leafs
		SpriteDrawer::Draw(TextureManager::Get(DECORATION),
			LOWER_SCREEN_WIDTH-64.0f, 0.0f, 64.0f, 64.0f,
			64.0f, 64.0f, -64.0f, 64.0f,
			0.0f,  0.0f, 0.0f, 0.5f
		);
		SpriteDrawer::Draw(TextureManager::Get(DECORATION),
			-10.0f, 20.0f, 64.0f, 64.0f,
			0.0f,  0.0f, 64.0f, 64.0f,
			0.0f,  0.0f, 0.0f, 0.5f
		);
		// stone
		SpriteDrawer::Draw(TextureManager::Get(DECORATION),
			LOWER_SCREEN_WIDTH*2/3, LOWER_SCREEN_HEIGHT/2, 16.0f, 16.0f,
			64, 0.0f, 64.0f, 64.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		// Button & Arrow
		for (int i=0; i<NUM_OF_BUTTON_TYPE; i++)
		{
			TEXTURE* tex;
			switch (i)
			{
				case LSBUTTON_BACK_STAGE:
				case LSBUTTON_NEXT_STAGE:
				case LSBUTTON_PLAY_BG:
					tex = TextureManager::Get(LEAF);
					break;

				case LSBUTTON_PLAY:
					tex = TextureManager::Get(PLAY_BUTTON);
					break;
					
				case LSBUTTON_BACK_TO_TITLE:
					tex = TextureManager::Get(BACK_BUTTON);
					break;

				default:
					tex = TextureManager::Get(LEAF);
					break;
			}

			m_buttons[i]->Draw(tex);
		}

	}
} 
