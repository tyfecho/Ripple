/**
*	@file	Common.cpp
*
*	@brief	�S�ʓI�ɕK�v�ȏ��̓��������N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
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
