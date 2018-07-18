#include "GameMain.h"

#include "NW_Lib/Utility/NW_Lib_DebugFont.h"
#include "NW_Lib/System/NW_Lib_InputManager.h"

#include "Game/Scene/OpeningScene.h"
#include "Game/Scene/Screens.h"
#include "Game/Scene/LevelSelect.h"
#include "Game/Scene/GameLevel.h"


using namespace NW_Lib;

namespace Game
{
	/**
	*     �R���X�g���N�^      
	*/
	GameMain::GameMain()
	:m_LoadFlg(true)
	,m_IsDrawingLogo(false)
	,m_IsUpdate(false)
	,hTh(NULL)
	{
		System::InputManager::Initialize();
	}

	/**
	*	  �f�X�g���N�^      
	*/
	GameMain::~GameMain()
	{
		
		System::InputManager::Finalize();
	}

	/**
	*	�X�V
	*/
	void GameMain::Update()
	{
		m_pInput->Update();
		System::InputManager::Update();
		if(!m_IsDrawingLogo){

			// ���[�h���
			//hTh = (HANDLE)_beginthreadex(NULL, 0, &(GameMain::LoadThread), this, 0, &thID);
			m_LoadFlg = true;

			m_IsDrawingLogo = true;
		}

		if(m_LoadFlg){
			if(hTh != NULL){
				DWORD dwExCode;
				// �X���b�h�̏�Ԃ��m�F
				GetExitCodeThread(hTh, &dwExCode);
				if(!(dwExCode & STILL_ACTIVE)){
					// �X���b�h�̓��e���I�����
					m_LoadFlg = false;
					WaitForSingleObject( hTh, INFINITE );
					CloseHandle(hTh);
					hTh = NULL;
				}
			}
			else
			{
				AllLoad();
				m_LoadFlg = false;

			}
		}else{
			if(m_IsUpdate){

				NW_Lib::System::SceneManager::GetInstance().Update();

			}else{
				//if(m_pInput->TrigerKey(VK_SPACE))
				{
					// �V�[�����X�V������
					m_IsUpdate = true;
				}
			}
		}
	}

	
	/*
	*	���ʂ̕`��
	*/
	void GameMain::DrawUpper()
	{
		//���ʗp�R�}���h���X�g�̎擾
		NW_Lib::System::SceneManager::GetInstance().DrawUpper();

		//�t�H���g�̕`��
		NW_Lib::Utility::DebugFont::GetInstance().DrawUpper();
	}

	/*
	*	����ʂ̕`��
	*/
	void GameMain::DrawLower()
	{
		//����ʗp�R�}���h���X�g�̎擾
		NW_Lib::System::SceneManager::GetInstance().DrawLower();

		//�t�H���g�̕`��
		NW_Lib::Utility::DebugFont::GetInstance().DrawLower();
	}

	/**
	*	�S�I�u�W�F�N�g�̕K�v���̃��[�h
	*/
	void GameMain::AllLoad()
	{
		NW_Lib::System::SceneManager::GetInstance().ChangeScene(new OpeningScene());
	}

	unsigned __stdcall GameMain::LoadThread(void* v)
	{
		GameMain* scene = (GameMain*)v;

		scene->AllLoad();

		return 0;
	}
}