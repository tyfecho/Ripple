#pragma once

#include <Windows.h>
#include "NW_Lib/System/NW_Lib_SceneManager.h"


#include "NW_Lib/System/NW_Lib_Camera.h"


#include <fstream>
#include "NW_Lib/System/NW_Lib_Common.h"

namespace NW_Lib
{
	class Sprite3D;
}

namespace Game
{
	class ParticleEffect;

	class GameMain : public NW_Lib::Common
	{
		private:
			// スレッドのハンドル
			HANDLE hTh;
			// スレッドID
			UINT thID;

			bool m_LoadFlg;
			bool m_IsDrawingLogo;
			bool m_IsUpdate;

		public:
			// コンストラクタ
			GameMain();
			// デストラクタ
			~GameMain();

			// 更新
			void Update();
			
			// 上画面の描画
			void DrawUpper();

			// 下画面の描画
			void DrawLower();

			static unsigned __stdcall LoadThread(void* v);

			void AllLoad();

	};
}


