/**
*	@file	SceneManager.cpp
*
*	@brief	シーン管理用マネージャクラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
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
	*	コンストラクタ
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
	*	デストラクタ
	*/
	SceneManager::~SceneManager()
	{
		SafeDelete( m_pCurrentScene );
		SafeDelete( m_pNextScene);

		SafeDelete( m_loadAnimation );

	}

	/**
	*	毎フレーム更新
	*/
	void SceneManager::Update()
	{
		// シーン変更
		if ( m_pNextScene && m_loadAnimation->isLoadAnimationEnd())
		{
			// 現在シーンのインスタンスを解放
			SafeDelete( m_pCurrentScene);

			// 現在シーンを書き換え
			m_pCurrentScene = m_pNextScene;
			
			//ローディングスレッド起動
			m_loadingHandle = (HANDLE)_beginthreadex(NULL, 0, &(SceneManager::Loading), m_pNextScene, 0, &m_threadID);
			
			//ロード開始
			m_loadFlag = true;
			
			// シーン変更終了
			m_pNextScene = NULL;
		}

		// シーンの毎フレーム更新
		if ( m_pCurrentScene && ( !(m_loadAnimation->isLoadAnimationNow()) || m_loadAnimation->GetLoadAnimationMode() == LOADIN ) && !m_loadFlag )
		{
			m_pCurrentScene->Update();
		}

		//ロードスレッドの状態チェック
		if( m_loadingHandle != NULL )
		{
			DWORD dwCode;
			GetExitCodeThread( m_loadingHandle, &dwCode);
			if (!( dwCode == STILL_ACTIVE ) ) 
			{
				WaitForSingleObject(m_loadingHandle, INFINITE);
				CloseHandle(m_loadingHandle);
				m_loadingHandle = NULL;
			
				//ロード終了
				m_loadFlag = false;

				// フェードイン開始
				m_loadAnimation->LoadAnimationModeChanger( LOADIN );
			}
		}
	}


	///////////上画面の描画////////////
	/**
	*	上画面描画
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

	///////////下画面の描画////////////
	/**
	*	下画面描画
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
	*	シーン変更リクエスト
	*/
	void SceneManager::ChangeScene( IScene* pNextScene )
	{
		m_pNextScene = pNextScene;

		m_loadAnimation->LoadAnimationModeChanger( LOADOUT );
	}

	/**
	*	 ロードアニメーションするか
	*/
	void SceneManager::SetLoadAnimation(bool flag)
	{
		m_loadAnimation->SetEnable(flag);
	}

	/**
	*	ローディング
	*/
	unsigned __stdcall SceneManager::Loading(void *lpx)
	{
		IScene* pScene = static_cast<IScene*>(lpx);

		pScene->Initialize();

		return 0;
	}

}
}