/**
*	@file	TextureGetter.h
*
*	@brief	�e�N�X�`�������Ǘ����邽�߂̃N���X
*
*	@date	2013/08/27
*
*	@author	�g�c  ����
*/
#pragma once

#include "../Utility/NW_Lib_Constant.h"
#include "../Graphics/NW_Lib_texture.h"

namespace NW_Lib
{
	class TextureManager
	{
		public:

			// �e�N�X�`���̎擾
			static TEXTURE* Get(int texID);

			// �e�N�X�`���̐���
			static void Entry(int texID , char* filename);

			// �e�N�X�`���̍폜
			static void Remove(int texID);

			// �S�Ẵe�N�X�`���̍폜
			static void AllRemove();
	};
}


