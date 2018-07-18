/**
*	@file	Fade.h
*
*	@brief	フェードクラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#pragma once

#include <d3dx9.h>
#include "../System/NW_Lib_common.h"

namespace NW_Lib
{
	class Texture;

	class Fade:public Common
	{
		private:
			// テクスチャ
			Texture* m_pTexture;

			int fade_cnt;
		public:
			//Fade();
			//~Fade();

			//void Initialize();

			//// 普通のフェード
			//bool FadeIn(float speed = 1.0f,bool black = true);
			//bool FadeOut(float speed = 1.0f,bool black = true);
			//bool FadeInOut(float speed = 1.0f,bool black = true);

			//void AllRender(bool black = true);
	};
}
