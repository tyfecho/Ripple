/**
*	@file	CameraManager.cpp
*
*	@brief	カメラの管理
*
*	@date	2013/09/19
*
*	@author	吉田　将也
*	@author	皆光　健太
*/
#include "NW_Lib_CameraManager.h"
#include "../Utility/NW_Lib_utility.h"

namespace NW_Lib
{
	
	namespace System
	{
		/**
		 * @brief   コンストラクタ.
		 */
		CameraManager::CameraManager()
		{
			m_pCamera[UPPER_SCREEN_DRAWING] = NULL;
			m_pCamera[LOWER_SCREEN_DRAWING] = NULL;
		}

		/**
		 * @brief   デストラクタ.
		 */
		CameraManager::~CameraManager()
		{
			SafeDelete(m_pCamera[0]);
			SafeDelete(m_pCamera[1]);
		}

		/**
		 * @brief   初期化処理
		 *
		 * @param   pGraphicDevice グラフィックデバイス.
		 */
		void CameraManager::Initialize()
		{
			m_pCamera[0] = new Camera();
			m_pCamera[0]->SetPerspective();
			m_pCamera[0]->SetAspect(400.0f,240.0f);
			m_pCamera[1] = new Camera();
			m_pCamera[1]->SetPerspective();
			m_pCamera[1]->SetAspect(320.0f,240.0f);
		}

		/**
		 * @brief   初期化処理
		 */
		void CameraManager::Finalize()
		{
			SafeDelete(m_pCamera[0]);
			SafeDelete(m_pCamera[1]);
		}

		/**
		 * @brief   カメラを取得処理
		 *
		 * @return  カメラ
		 */
		Camera* CameraManager::GetCamera(DrawingScreen drawingScreen)
		{
			return m_pCamera[drawingScreen];
		}
	}
}

