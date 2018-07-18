/**
*	@file	Common.h
*
*	@brief	全般的に必要な情報の入った基底クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#pragma once
#include <d3dx9.h>
#include "NW_Lib_DirectXInputManager.h"
#include "../Graphics/NW_Lib_sprite.h"
#include "../Utility/NW_Lib_Constant.h"


namespace NW_Lib
{
	class Common
	{
		protected:
			// ウインドウハンドル
			static HWND m_hwnd;
			// デバイス
			static LPDIRECT3DDEVICE9 m_pD3DDevice;
			// DirectXパラメータ
			static D3DPRESENT_PARAMETERS* m_pD3Dpp;

			// 入力用マネージャー
			static DirectXInputManager* m_pInput;
			// スプライト
			static Sprite* m_pSprite;
		public:
			LPDIRECT3DDEVICE9 GetDevice()
			{
				return m_pD3DDevice;
			}
			Sprite* GetSprite()
			{
				return m_pSprite;
			}
			
	};
}



