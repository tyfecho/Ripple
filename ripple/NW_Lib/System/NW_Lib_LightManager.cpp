/**
*	@file	LightControler.cpp
*
*	@brief	ライトを管理するクラス
*
*	@date	2013/09/19
*
*	@author	吉田　将也
*	@author	皆光　健太
*/
#include "NW_Lib_LightManager.h"
#include "../Utility/NW_Lib_utility.h"

namespace NW_Lib
{
	namespace System
	{
		/**
		 * @brief   コンストラクタ.
		 */
		LightManager::LightManager()
		{
			m_pLight[0] = NULL;
			m_pLight[1] = NULL;
		}

		/**
		 * @brief   デストラクタ.
		 */
		LightManager::~LightManager()
		{

		}

		/**
		 * @brief   初期化
		 */
		void LightManager::Initialize()
		{
			m_pLight[UPPER_SCREEN_DRAWING] = new Light();

			m_pLight[LOWER_SCREEN_DRAWING] = new Light();
		}

		/**
		 * @brief   解放
		 */
		void LightManager::Finalize()
		{
			SafeDelete(m_pLight[UPPER_SCREEN_DRAWING]);

			SafeDelete(m_pLight[LOWER_SCREEN_DRAWING]);
		}

		/**
		 * @brief   ライト情報の取得
		 *
		 * @param drawingscreen 上下のどちらのライトかを指定
		 *
		 * @return ライト情報
		 */
		Light* LightManager::GetLight(DrawingScreen drawingscreen)
		{
			return m_pLight[drawingscreen];
		}
	}
}

