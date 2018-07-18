/**
*	@file	CameraManager.h
*
*	@brief	カメラの管理
*
*	@date	92013/09/1
*
*	@author	吉田　将也
*	@author	皆光　健太
*/

#pragma once

#include "NW_Lib_Camera.h"

namespace NW_Lib
{
	namespace System
	{
		class CameraManager
		{
			private:
				Camera* m_pCamera[NUM_SCREEN_DRAWING];

				/**
				 * @brief   コンストラクタ.
				 */
				CameraManager();
				CameraManager(const CameraManager& val){(void)val;}
				CameraManager& operator=(const CameraManager& val){(void)val;return *this;}

				/**
				 * @brief   デストラクタ.
				 */
				~CameraManager();
			public:

				/// シングルトン化させる
				static CameraManager& GetInstance()
				{
					static CameraManager instance;
					return instance;
				}

				void Initialize();
				void Finalize();


				Camera* GetCamera(DrawingScreen drawingScreen);
		};
	}
}

