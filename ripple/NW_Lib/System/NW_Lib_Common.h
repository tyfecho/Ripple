/**
*	@file	Common.h
*
*	@brief	�S�ʓI�ɕK�v�ȏ��̓��������N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
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
			// �E�C���h�E�n���h��
			static HWND m_hwnd;
			// �f�o�C�X
			static LPDIRECT3DDEVICE9 m_pD3DDevice;
			// DirectX�p�����[�^
			static D3DPRESENT_PARAMETERS* m_pD3Dpp;

			// ���͗p�}�l�[�W���[
			static DirectXInputManager* m_pInput;
			// �X�v���C�g
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



