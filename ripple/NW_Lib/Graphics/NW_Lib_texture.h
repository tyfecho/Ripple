/**
*	@file	texture.h
*
*	@brief	�e�N�X�`���N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#pragma once

#include <d3dx9.h>
#include "../System/NW_Lib_Common.h"

namespace NW_Lib
{
	class Texture : public Common
	{
		private:
			LPDIRECT3DTEXTURE9 m_texture; // �e�N�X�`���I�u�W�F�N�g

			DWORD m_width;  // �e�N�X�`���̕�
			DWORD m_height; // �e�N�X�`���̕�
		public:
			Texture( const char* filename);
			~Texture();
			LPDIRECT3DTEXTURE9 GetTexture() const; // �e�N�X�`���I�u�W�F�N�g��Ԃ�
			DWORD GetWidth() const;                // �e�N�X�`���̕���Ԃ�
			DWORD GetHeight() const;               // �e�N�X�`���̍�����Ԃ�
	};
}
