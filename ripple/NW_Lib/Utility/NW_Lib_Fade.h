/**
*	@file	Fade.h
*
*	@brief	�t�F�[�h�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
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
			// �e�N�X�`��
			Texture* m_pTexture;

			int fade_cnt;
		public:
			//Fade();
			//~Fade();

			//void Initialize();

			//// ���ʂ̃t�F�[�h
			//bool FadeIn(float speed = 1.0f,bool black = true);
			//bool FadeOut(float speed = 1.0f,bool black = true);
			//bool FadeInOut(float speed = 1.0f,bool black = true);

			//void AllRender(bool black = true);
	};
}
