/**
*	@file	IScene.h
*
*	@brief	�V�[���̃C���^�[�t�F�[�X
*
*	@date	2013/08/20
*
*	@author	�g�c�@����
*	@author	�F���@����
*/
#pragma once

#include <d3dx9.h>
#include "../System/NW_Lib_Common.h"

namespace NW_Lib
{

	class SceneManager;
	//class Timer;

	/*
		�V�[���̃C���^�[�t�F�C�X�N���X
	*/
	class IScene
	{

		protected:

		public:
			// �f�X�g���N�^
			virtual ~IScene(){}

			// ������
			virtual void Initialize(){};

			// ���t���[���X�V
			virtual void Update(){};

			// ���ʂ̕`��
			virtual void DrawUpper(){};

			// ����ʂ̕`��
			virtual void DrawLower(){};

	};


}
