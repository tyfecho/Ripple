/**
*	@file	CameraManager.cpp
*
*	@brief	�J�����̊Ǘ�
*
*	@date	2013/09/19
*
*	@author	�g�c�@����
*	@author	�F���@����
*/
#include "NW_Lib_CameraManager.h"
#include "../Utility/NW_Lib_utility.h"

namespace NW_Lib
{
	
	namespace System
	{
		/**
		 * @brief   �R���X�g���N�^.
		 */
		CameraManager::CameraManager()
		{
			m_pCamera[UPPER_SCREEN_DRAWING] = NULL;
			m_pCamera[LOWER_SCREEN_DRAWING] = NULL;
		}

		/**
		 * @brief   �f�X�g���N�^.
		 */
		CameraManager::~CameraManager()
		{
			SafeDelete(m_pCamera[0]);
			SafeDelete(m_pCamera[1]);
		}

		/**
		 * @brief   ����������
		 *
		 * @param   pGraphicDevice �O���t�B�b�N�f�o�C�X.
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
		 * @brief   ����������
		 */
		void CameraManager::Finalize()
		{
			SafeDelete(m_pCamera[0]);
			SafeDelete(m_pCamera[1]);
		}

		/**
		 * @brief   �J�������擾����
		 *
		 * @return  �J����
		 */
		Camera* CameraManager::GetCamera(DrawingScreen drawingScreen)
		{
			return m_pCamera[drawingScreen];
		}
	}
}

