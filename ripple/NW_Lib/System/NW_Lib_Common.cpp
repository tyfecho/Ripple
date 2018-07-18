/**
*	@file	Common.cpp
*
*	@brief	全般的に必要な情報の入った基底クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#include "../System/NW_Lib_Common.h"

namespace NW_Lib
{
	HWND				   Common::m_hwnd;
	LPDIRECT3DDEVICE9      Common::m_pD3DDevice;
	D3DPRESENT_PARAMETERS* Common::m_pD3Dpp;
	DirectXInputManager*   Common::m_pInput;
	Sprite*				   Common::m_pSprite;
}
