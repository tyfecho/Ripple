/**
*	@file	system.cpp
*
*	@brief	根本的なシステムをまとめたクラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
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
	*	コンストラクタ
	*/
	DirectXSystem::DirectXSystem()
	:m_pWindow(NULL)
	,m_pDirectX(NULL)
	,m_pLight(NULL)
	,m_pTimer(NULL)
	{
	}

	/**
	*	デストラクタ
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
	*	初期化処理
	*/
	void DirectXSystem::Initialize()
	{
		srand(static_cast<unsigned int>(time(NULL)));
		// ウインドウの作成
		m_pWindow = new Window();
		m_pWindow->Create(400, 480, "Ripple");

		// ウインドウ
		m_hwnd = m_pWindow->GetWindowHandle();

		// DirectXオブジェクトの生成
		m_pDirectX = new DirectX();
		m_pDirectX->Initialize(m_hwnd,m_pWindow->IsFullScrean());

		// デバイス
		m_pD3DDevice = m_pDirectX->GetD3DDevice();
		// DirectXパラメータ
		m_pD3Dpp = m_pDirectX->GetParameters();

		// ライト
		m_pLight = new NW_Lib::Light();

		// Commonクラスのものを生成

		// スプライト
		m_pSprite = new Sprite(m_pD3DDevice);
		// 入力マネージャ
		m_pInput = new DirectXInputManager(m_pWindow);
		// タイマー
		m_pTimer = new Timer();

		NW_Lib::System::CameraManager::GetInstance().Initialize();
		
		// Resize and move the window
		MoveWindow(m_pWindow->GetWindowHandle(), 560, 60, 800, 960, false);

		// ウインドウの表示
		m_pWindow->Show();
	}

	/**
	*	更新処理
	*	@param pGamemain  ゲームメインの情報
	*/
	void DirectXSystem::Update(GameMain* pGamemain)
	{
		// メッセージループ
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
					// ライト
					m_pLight->ShowLight();

					//	画面をクリア
					m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

					// ゲームの更新
					pGamemain->Update();

					// マウスのクリッピング
					if( NW_Lib::Window::IsActive() && !m_pInput->PushKey(VK_LCONTROL) )
					{
						m_pInput->MouseCliping(0,0,0,0);
					}
					else
					{
						m_pInput->MouseClipingOff();
					}

					// ビューポート
					const D3DVIEWPORT9 viewData1 = { 0, 0, UPPER_SCREEN_WIDTH, UPPER_SCREEN_HEIGHT, 0.0f, 1.0f };
					const D3DVIEWPORT9 viewData2 = { (UPPER_SCREEN_WIDTH - LOWER_SCREEN_WIDTH) / 2 , UPPER_SCREEN_HEIGHT, LOWER_SCREEN_WIDTH, UPPER_SCREEN_HEIGHT, 0.0f, 1.0f };

					//上画面
					m_pD3DDevice->SetViewport(&viewData1);
					m_pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_ARGB(50, 50, 50, 50),1.0f,0);
					
					// カメラ
					NW_Lib::System::CameraManager::GetInstance().GetCamera(UPPER_SCREEN_DRAWING)->ShowCamera();
					
					// ゲームの描画
					pGamemain->DrawUpper();
					//pGamemain->Draw();
    
					//下画面
					m_pD3DDevice->SetViewport(&viewData2);
					m_pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_ARGB(50, 50, 50, 50),1.0f,0);
					
					// カメラ
					NW_Lib::System::CameraManager::GetInstance().GetCamera(LOWER_SCREEN_DRAWING)->ShowCamera();
					
					// ゲームの描画
					pGamemain->DrawLower();
					//pGamemain->Draw();

				}
				m_pD3DDevice->EndScene(); // 描画終わり
				m_pTimer->Wait(1.0/30.0);
				m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
			}
		}

	}
}

