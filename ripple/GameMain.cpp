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
	*     コンストラクタ      
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
	*	  デストラクタ      
	*/
	GameMain::~GameMain()
	{
		
		System::InputManager::Finalize();
	}

	/**
	*	更新
	*/
	void GameMain::Update()
	{
		m_pInput->Update();
		System::InputManager::Update();
		if(!m_IsDrawingLogo){

			// ロード画面
			//hTh = (HANDLE)_beginthreadex(NULL, 0, &(GameMain::LoadThread), this, 0, &thID);
			m_LoadFlg = true;

			m_IsDrawingLogo = true;
		}

		if(m_LoadFlg){
			if(hTh != NULL){
				DWORD dwExCode;
				// スレッドの状態を確認
				GetExitCodeThread(hTh, &dwExCode);
				if(!(dwExCode & STILL_ACTIVE)){
					// スレッドの内容が終わった
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
					// シーンを更新させる
					m_IsUpdate = true;
				}
			}
		}
	}

	
	/*
	*	上画面の描画
	*/
	void GameMain::DrawUpper()
	{
		//上画面用コマンドリストの取得
		NW_Lib::System::SceneManager::GetInstance().DrawUpper();

		//フォントの描画
		NW_Lib::Utility::DebugFont::GetInstance().DrawUpper();
	}

	/*
	*	下画面の描画
	*/
	void GameMain::DrawLower()
	{
		//下画面用コマンドリストの取得
		NW_Lib::System::SceneManager::GetInstance().DrawLower();

		//フォントの描画
		NW_Lib::Utility::DebugFont::GetInstance().DrawLower();
	}

	/**
	*	全オブジェクトの必要情報のロード
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