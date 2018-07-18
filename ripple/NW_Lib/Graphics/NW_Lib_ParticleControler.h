/**
*	@file	NW_Lib_ParticleControler.h
*
*	@brief	�p�[�e�B�N���`��V�X�e���N���X
*
*	@date	2013/09/24
*
*	@author	�g�c�@����
*/

#pragma once

#include "NW_Lib_Drawer.h"

namespace NW_Lib
{

	/*
		�X�v���C�g�`��V�X�e���N���X
	*/
	class ParticleControler : public Drawer
	{
		public:
			/// ���f���̕`��
			static void Draw(int handle){}
			/// ���f���̈ʒu�̐ݒ�
			static void SetPosision(int handle,VECTOR3 position){}
			/// ���f���̉�]�̐ݒ�
			static void SetRotate(int handle,VECTOR3 rotation){}
			/// ���f���̃T�C�Y�̐ݒ�
			static void SetScale(int handle,VECTOR3 scale){}
			/// ���o�ʂ̐ݒ�
			static void SetEmission(int handle,float emission){}
	};
}
