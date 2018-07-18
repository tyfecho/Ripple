/**
*	@file	SceneManager.cpp
*
*	@brief	�V�[���Ǘ��p�}�l�[�W���N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#include "NW_Lib_SceneManager.h"

#include <cstdlib>
#include "NW_Lib_IScene.h"
#include "../../Game/LoadAnimation/LoadAnimation.h"

namespace NW_Lib
{
namespace System
{

	/**
	*	�R���X�g���N�^
	*/
	SceneManager::SceneManager()
	: m_pCurrentScene(NULL)
	, m_pNextScene(NULL)
	, m_loadingHandle(NULL)
	, m_loadAnimation(NULL)
	{
		m_loadFlag = false;
		m_loadAnimation = new Game::LoadAnimation();
		m_loadAnimation->SetLoadAnimationTime(45);
		m_loadAnimation->SetBeforeLoadAnimationMode(LOADOUT);
		m_loadAnimation->LoadAnimationModeChanger(LOADOUT);
	}


	/**
	*	�f�X�g���N�^
	*/
	SceneManager::~SceneManager()
	{
		SafeDelete( m_pCurrentScene );
		SafeDelete( m_pNextScene);

		SafeDelete( m_loadAnimation );

	}

	/**
	*	���t���[���X�V
	*/
	void SceneManager::Update()
	{
		// �V�[���ύX
		if ( m_pNextScene && m_loadAnimation->isLoadAnimationEnd())
		{
			// ���݃V�[���̃C���X�^���X�����
			SafeDelete( m_pCurrentScene);

			// ���݃V�[������������
			m_pCurrentScene = m_pNextScene;
			
			//���[�f�B���O�X���b�h�N��
			m_loadingHandle = (HANDLE)_beginthreadex(NULL, 0, &(SceneManager::Loading), m_pNextScene, 0, &m_threadID);
			
			//���[�h�J�n
			m_loadFlag = true;
			
			// �V�[���ύX�I��
			m_pNextScene = NULL;
		}

		// �V�[���̖��t���[���X�V
		if ( m_pCurrentScene && ( !(m_loadAnimation->isLoadAnimationNow()) || m_loadAnimation->GetLoadAnimationMode() == LOADIN ) && !m_loadFlag )
		{
			m_pCurrentScene->Update();
		}

		//���[�h�X���b�h�̏�ԃ`�F�b�N
		if( m_loadingHandle != NULL )
		{
			DWORD dwCode;
			GetExitCodeThread( m_loadingHandle, &dwCode);
			if (!( dwCode == STILL_ACTIVE ) ) 
			{
				WaitForSingleObject(m_loadingHandle, INFINITE);
				CloseHandle(m_loadingHandle);
				m_loadingHandle = NULL;
			
				//���[�h�I��
				m_loadFlag = false;

				// �t�F�[�h�C���J�n
				m_loadAnimation->LoadAnimationModeChanger( LOADIN );
			}
		}
	}


	///////////���ʂ̕`��////////////
	/**
	*	���ʕ`��
	*/
	void SceneManager::DrawUpper()
	{
		Drawer::SetDrawUpper();

		if( !m_loadFlag )
		{
			if ( m_pCurrentScene )
			{
				m_pCurrentScene->DrawUpper();
			}
		}
		else
		{

		}
		
		m_loadAnimation->LoadAnimationDraw(UPPER_SCREEN_DRAWING);

	}
	/////////////////////////////////

	///////////����ʂ̕`��////////////
	/**
	*	����ʕ`��
	*/
	void SceneManager::DrawLower()
	{
		Drawer::SetDrawLower();

		if( !m_loadFlag )
		{
			if ( m_pCurrentScene )
			{
				m_pCurrentScene->DrawLower();
			}
		}
		else
		{

		}
		
		m_loadAnimation->LoadAnimationDraw(LOWER_SCREEN_DRAWING);
	}

	/////////////////////////////////

	/**
	*	�V�[���ύX���N�G�X�g
	*/
	void SceneManager::ChangeScene( IScene* pNextScene )
	{
		m_pNextScene = pNextScene;

		m_loadAnimation->LoadAnimationModeChanger( LOADOUT );
	}

	/**
	*	 ���[�h�A�j���[�V�������邩
	*/
	void SceneManager::SetLoadAnimation(bool flag)
	{
		m_loadAnimation->SetEnable(flag);
	}

	/**
	*	���[�f�B���O
	*/
	unsigned __stdcall SceneManager::Loading(void *lpx)
	{
		IScene* pScene = static_cast<IScene*>(lpx);

		pScene->Initialize();

		return 0;
	}

}
}