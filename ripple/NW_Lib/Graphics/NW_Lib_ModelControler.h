/**
*	@file	ModelControler.h
*
*	@brief	���f���`��V�X�e���N���X
*
*	@date	2013/09/03
*
*	@author	�g�c�@����
*	@author	�F���@����
*/

#pragma once

#include "../Utility/NW_Lib_Constant.h"
#include "../Graphics/NW_Lib_texture.h"
#include "NW_Lib_Drawer.h"

namespace NW_Lib
{

	/*
		���f���`��V�X�e���N���X
	*/
	class ModelControler : public Drawer
	{
		public:
			/// ���f���̕`��
			static void Draw(int handle);
			/// ���f���̈ʒu�̐ݒ�
			static void SetPosision(int handle,VECTOR3 position);
			/// ���f���̉�]�̐ݒ�
			static void SetRotate(int handle,VECTOR3 rotation);
			/// ���f���̃T�C�Y�̐ݒ�
			static void SetScale(int handle,VECTOR3 scale);
			/// ���f���ɍs���ݒ�
			static void SetWorldMatrix(int handle,MATRIX* matrix);
	};
}
