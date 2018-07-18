/**
*	@file	IScene.h
*
*	@brief	シーンのインターフェース
*
*	@date	2013/08/20
*
*	@author	吉田　将也
*	@author	皆光　健太
*/
#pragma once

#include <d3dx9.h>
#include "../System/NW_Lib_Common.h"

namespace NW_Lib
{

	class SceneManager;
	//class Timer;

	/*
		シーンのインターフェイスクラス
	*/
	class IScene
	{

		protected:

		public:
			// デストラクタ
			virtual ~IScene(){}

			// 初期化
			virtual void Initialize(){};

			// 毎フレーム更新
			virtual void Update(){};

			// 上画面の描画
			virtual void DrawUpper(){};

			// 下画面の描画
			virtual void DrawLower(){};

	};


}
