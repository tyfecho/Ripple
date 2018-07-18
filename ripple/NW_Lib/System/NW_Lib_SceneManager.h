/**
*	@file	SceneManager.h
*
*	@brief	シーン管理用マネージャクラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
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
		シーン管理クラス
	*/
	class SceneManager : public Common
	{
		private:
			// 現在のシーンインスタンス
			IScene* m_pCurrentScene;

			// 次のシーンインスタンス
			IScene* m_pNextScene;

			//スレッドＩＤ
			unsigned int m_threadID;

			DWORD a;
			//NowLoading用ハンドル
			HANDLE m_loadingHandle;

			//ロード画面
			//Game::Load* m_load;

			//ロード時のアニメーション
			Game::LoadAnimation* m_loadAnimation;

			//ロード中か
			bool m_loadFlag;



			// コンストラクタ
			SceneManager();
			SceneManager(const SceneManager& val){(void)val;}
			SceneManager& operator=(const SceneManager& val){(void)val;return *this;}

			// デストラクタ
			virtual ~SceneManager();

		public:
			/// シングルトン化させる
			static SceneManager& GetInstance()
			{
				static SceneManager instance;
				return instance;
			}


			// 毎フレーム更新
			virtual void Update();
	
			// 上画面描画
			void DrawUpper();

			// 下画面描画
			void DrawLower();
			
			// シーン変更リクエスト
			void ChangeScene(IScene* pNextScene);

			// ロードアニメーションするか
			void SetLoadAnimation(bool flag);
			
			IScene* GetCurrentScene()
			{
				return m_pCurrentScene;
			}

			// ローディング
			static unsigned __stdcall Loading(void *);

			// テクスチャマネージャー
			NW_Lib::ResourceManager<NW_Lib::Texture*> m_textureMnager;

	};

}
}