/**
*	@file	SceneManager.h
*
*	@brief	�V�[���Ǘ��p�}�l�[�W���N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#pragma once

#include "../System/NW_Lib_Common.h"
#include "../System/NW_Lib_ResourceManager.h"
#include "../Graphics/NW_Lib_texture.h"
#include <windows.h>
#include <process.h>

namespace Game
{
	class LoadAnimation;
}

namespace NW_Lib
{

	class SceneManager;
	class Timer;

	class Texture;
	class Sprite;
	class Sprite3D;
	class G3DObj;

	class IScene;


namespace System
{

	/*
		�V�[���Ǘ��N���X
	*/
	class SceneManager : public Common
	{
		private:
			// ���݂̃V�[���C���X�^���X
			IScene* m_pCurrentScene;

			// ���̃V�[���C���X�^���X
			IScene* m_pNextScene;

			//�X���b�h�h�c
			unsigned int m_threadID;

			DWORD a;
			//NowLoading�p�n���h��
			HANDLE m_loadingHandle;

			//���[�h���
			//Game::Load* m_load;

			//���[�h���̃A�j���[�V����
			Game::LoadAnimation* m_loadAnimation;

			//���[�h����
			bool m_loadFlag;



			// �R���X�g���N�^
			SceneManager();
			SceneManager(const SceneManager& val){(void)val;}
			SceneManager& operator=(const SceneManager& val){(void)val;return *this;}

			// �f�X�g���N�^
			virtual ~SceneManager();

		public:
			/// �V���O���g����������
			static SceneManager& GetInstance()
			{
				static SceneManager instance;
				return instance;
			}


			// ���t���[���X�V
			virtual void Update();
	
			// ���ʕ`��
			void DrawUpper();

			// ����ʕ`��
			void DrawLower();
			
			// �V�[���ύX���N�G�X�g
			void ChangeScene(IScene* pNextScene);

			// ���[�h�A�j���[�V�������邩
			void SetLoadAnimation(bool flag);
			
			IScene* GetCurrentScene()
			{
				return m_pCurrentScene;
			}

			// ���[�f�B���O
			static unsigned __stdcall Loading(void *);

			// �e�N�X�`���}�l�[�W���[
			NW_Lib::ResourceManager<NW_Lib::Texture*> m_textureMnager;

	};

}
}