/**
*	@file	CameraManager.h
*
*	@brief	�J�����̊Ǘ�
*
*	@date	92013/09/1
*
*	@author	�g�c�@����
*	@author	�F���@����
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
				 * @brief   �R���X�g���N�^.
				 */
				CameraManager();
				CameraManager(const CameraManager& val){(void)val;}
				CameraManager& operator=(const CameraManager& val){(void)val;return *this;}

				/**
				 * @brief   �f�X�g���N�^.
				 */
				~CameraManager();
			public:

				/// �V���O���g����������
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

