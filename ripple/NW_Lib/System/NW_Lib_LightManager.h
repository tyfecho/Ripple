/**
*	@file	LightControler.h
*
*	@brief	ライトを管理するクラス
*
*	@date	2013/09/19
*
*	@author	吉田　将也
*	@author	皆光　健太
*/

#include "NW_Lib_Light.h"
#include "../Utility/NW_Lib_Constant.h"
#include "../Graphics/NW_Lib_texture.h"

#pragma once

namespace NW_Lib
{
	namespace System
	{
		class LightManager 
		{
			private:
				Light* m_pLight[NUM_SCREEN_DRAWING]; // ライト情報
				/**
				 * @brief   コンストラクタ.
				 */
				LightManager();
				LightManager(const LightManager& val){(void)val;}
				LightManager& operator=(const LightManager& val){(void)val;return *this;}


				/**
				 * @brief   デストラクタ.
				 */
				~LightManager();
			public:
				
				/**
				 * @brief   シングルトン化させる
				 */
				static LightManager& GetInstance()
				{
					static LightManager instance;
					return instance;
				}

				/**
				 * @brief   初期化
				 */
				void Initialize();

				/**
				 * @brief   解放
				 */
				void Finalize();

				/**
				 * @brief   ライト情報の取得
				 *
				 * @param drawingscreen 上下のどちらのライトかを指定
				 *
				 * @return ライト情報
				 */
				Light* GetLight(DrawingScreen drawingscreen);
		};
	}
}

