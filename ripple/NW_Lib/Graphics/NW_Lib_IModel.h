/*-------------------------------------------------------------------------

	�N���X���@�F�@���f���̃C���^�[�t�F�[�X�N���X
	�쐬�ҁ@�@�F�@�F���@����
	�ŏI�X�V���F�@2013 / 09 / 17

-------------------------------------------------------------------------*/
#pragma once

#include "NW_Lib_AnimController.h"
#include "../System/NW_Lib_Common.h"
#include <d3dx9.h>
#include <d3d9.h>

#include "../Utility/NW_Lib_utility.h"

namespace NW_Lib
{

class Texture;

class IModel
{

	public:

		/*=======================================================
			�֐���	�F	Update
			����	�F	�X�V
			����	�F	�A�j���[�V�����R���g���[��
						���[���h�s��
			�߂�l	�F�@�Ȃ�
		========================================================*/
		virtual void Update(AnimController* pAnimController , LPD3DXMATRIX pWorld) = 0;

		/*=======================================================
			�֐���	�F	Draw
			����	�F	���b�V���̕`��
			����	�F	���[���h�}�g���b�N�X
						�A�j���[�V�����R���g���[��
						�A���t�@
			�߂�l	�F�@�Ȃ�
		========================================================*/
		virtual void Draw( D3DXMATRIX world , AnimController* pAnimController , float alpha = 1.0f) = 0;

};


}