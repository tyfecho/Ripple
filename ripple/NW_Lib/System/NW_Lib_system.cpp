/**
*	@file	system.cpp
*
*	@brief	���{�I�ȃV�X�e�����܂Ƃ߂��N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/


#include "NW_Lib_system.h"

#include "NW_Lib_Window.h"
#include "NW_Lib_DirectX.h"
#include "NW_Lib_light.h"
#include "NW_Lib_Timer.h"

#include "NW_Lib_CameraControler.h"
#include "NW_Lib_CameraManager.h"

#include <stdio.h>
#include <windows.h>
#include <process.h>

#include "../Utility/NW_Lib_HardwareDefine.h"
#include "../../GameMain.h"

using namespace Game;

namespace NW_Lib
{
	/**
	*	�R���X�g���N�^
	*/
	DirectXSystem::DirectXSystem()
	:m_pWindow(NULL)
	,m_pDirectX(NULL)
	,m_pLight(NULL)
	,m_pTimer(NULL)
	{
	}

	/**
	*	�f�X�g���N�^
	*/
	DirectXSystem::~DirectXSystem()
	{
		SafeDelete(m_pLight);
		SafeDelete(m_pDirectX);
		SafeDelete(m_pWindow);
		SafeDelete(m_pSprite);
		SafeDelete(m_pInput);
		SafeDelete(m_pTimer);
		
		NW_Lib::System::CameraManager::GetInstance().Finalize();
	}

	/**
	*	����������
	*/
	void DirectXSystem::Initialize()
	{
		srand(static_cast<unsigned int>(time(NULL)));
		// �E�C���h�E�̍쐬
		m_pWindow = new Window();
		m_pWindow->Create(400, 480, "Ripple");

		// �E�C���h�E
		m_hwnd = m_pWindow->GetWindowHandle();

		// DirectX�I�u�W�F�N�g�̐���
		m_pDirectX = new DirectX();
		m_pDirectX->Initialize(m_hwnd,m_pWindow->IsFullScrean());

		// �f�o�C�X
		m_pD3DDevice = m_pDirectX->GetD3DDevice();
		// DirectX�p�����[�^
		m_pD3Dpp = m_pDirectX->GetParameters();

		// ���C�g
		m_pLight = new NW_Lib::Light();

		// Common�N���X�̂��̂𐶐�

		// �X�v���C�g
		m_pSprite = new Sprite(m_pD3DDevice);
		// ���̓}�l�[�W��
		m_pInput = new DirectXInputManager(m_pWindow);
		// �^�C�}�[
		m_pTimer = new Timer();

		NW_Lib::System::CameraManager::GetInstance().Initialize();
		
		// Resize and move the window
		MoveWindow(m_pWindow->GetWindowHandle(), 560, 60, 800, 960, false);

		// �E�C���h�E�̕\��
		m_pWindow->Show();
	}

	/**
	*	�X�V����
	*	@param pGamemain  �Q�[�����C���̏��
	*/
	void DirectXSystem::Update(GameMain* pGamemain)
	{
		// ���b�Z�[�W���[�v
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				if (SUCCEEDED(m_pD3DDevice->BeginScene()))
				{
					// ���C�g
					m_pLight->ShowLight();

					//	��ʂ��N���A
					m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

					// �Q�[���̍X�V
					pGamemain->Update();

					// �}�E�X�̃N���b�s���O
					if( NW_Lib::Window::IsActive() && !m_pInput->PushKey(VK_LCONTROL) )
					{
						m_pInput->MouseCliping(0,0,0,0);
					}
					else
					{
						m_pInput->MouseClipingOff();
					}

					// �r���[�|�[�g
					const D3DVIEWPORT9 viewData1 = { 0, 0, UPPER_SCREEN_WIDTH, UPPER_SCREEN_HEIGHT, 0.0f, 1.0f };
					const D3DVIEWPORT9 viewData2 = { (UPPER_SCREEN_WIDTH - LOWER_SCREEN_WIDTH) / 2 , UPPER_SCREEN_HEIGHT, LOWER_SCREEN_WIDTH, UPPER_SCREEN_HEIGHT, 0.0f, 1.0f };

					//����
					m_pD3DDevice->SetViewport(&viewData1);
					m_pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_ARGB(50, 50, 50, 50),1.0f,0);
					
					// �J����
					NW_Lib::System::CameraManager::GetInstance().GetCamera(UPPER_SCREEN_DRAWING)->ShowCamera();
					
					// �Q�[���̕`��
					pGamemain->DrawUpper();
					//pGamemain->Draw();
    
					//�����
					m_pD3DDevice->SetViewport(&viewData2);
					m_pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_ARGB(50, 50, 50, 50),1.0f,0);
					
					// �J����
					NW_Lib::System::CameraManager::GetInstance().GetCamera(LOWER_SCREEN_DRAWING)->ShowCamera();
					
					// �Q�[���̕`��
					pGamemain->DrawLower();
					//pGamemain->Draw();

				}
				m_pD3DDevice->EndScene(); // �`��I���
				m_pTimer->Wait(1.0/30.0);
				m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
			}
		}

	}
}

