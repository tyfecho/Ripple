#pragma once

#include "../../NW_Lib/Utility/NW_Lib_CommonData.h"
#include "../GameObject/SpriteObject.h"

#include "../../NW_Lib/System/NW_Lib_IScene.h"

namespace Game
{
	// 使用するテクスチャのハンドル用
	enum LSTEXTURE
	{
		WATER = 0,
		LOG_FRAME,
		BOARD,
		BABY_SWAN,
		LEAF,
		DECORATION,
		SCORE_STAR,
		LOTUS_FLOWERS,
		PLAY_BUTTON,
		BACK_BUTTON,
		CARP_ANIME,
		PIRANIA_ANIME,
		FONT,

		NUM_OF_LEVEL_SELECT_TEX,
	};
	enum LSBUTTON
	{
		LSBUTTON_BACK_STAGE = 0,
		LSBUTTON_NEXT_STAGE,
		LSBUTTON_PLAY_BG,
		LSBUTTON_PLAY,
		LSBUTTON_BACK_TO_TITLE,

		NUM_OF_BUTTON_TYPE,
	};

	class LevelSelect : public NW_Lib::IScene
	{
		private:
			int m_elapsedFrame;
			int m_selectedStage;
			int m_selectedLevel;		
			float m_carpPosX;
			
			// 上画面
			SpriteObject* m_upperBG;
			// 下画面、背景画像の座標サイズと、UIの座標等
			SpriteObject* m_lowerBG;
			SpriteObject** m_levels;
			SpriteObject** m_buttons;

		public:
			// コンストラクタ
			LevelSelect(int stage=1, int level=1);
			// デストラクタ
			~LevelSelect();
			// 初期化
			void Initialize();
			// 更新
			void Update();
			// 入力まとめ
			void ProccessInput();

			// 上画面描画
			virtual void DrawUpper();
			// 下画面描画
			virtual void DrawLower();


	};

	// 
	struct XY {
		float x;
		float y;
	};
	// レベルスプライトの配置場所のデータ[Stage][Level][x,y]の様に格納
	const XY LEVEL_SPRITE_POSDATA[][NW_Lib::NUM_OF_LEVELS_INLEVEL] =
	{
		{
			{60.0f,140.0f},{110.0f,40.0f},{160.0f,120.0f},{220.0f,60.0f},{290.0f,150.0f},
		},
		{
			{60.0f,60.0f},{110.0f,130.0f},{160.0f,40.0f},{220.0f,160.0f},{290.0f,55.0f},
		},
		{
			{60.0f,70.0f},{115.0f,110.0f},{170.0f,150.0f},{225.0f,110.0f},{280.0f, 70.0f},
		},
		{
			{60.0f,150.0f},{115.0f,110.0f},{170.0f,70.0f},{225.0f,110.0f},{280.0f,150.0f},
		},
		{
			{70.0f,70.0f},{110.0f,150.0f},{160.0f,40.0f},{220.0f,150.0f},{290.0f,70.0f},
		},
		{
			{70.0f,120.0f},{120.0f,50.0f},{170.0f,150.0f},{220.0f,40.0f},{270.0f,120.0f},
		},
		{
			{70.0f,60.0f},{160.0f,40.0f},{140.0f,150.0f},{250.0f,130.0f},{280.0f,60.0f},
		}/*,
		{
			{70.0f,150.0f},{110.0f,85.0f},{170.0f,40.0f},{230.0f,85.0f},{270.0f, 150.0f},
		}*/
	};

}

