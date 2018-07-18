/**
*	@file	LightControler.cpp
*
*	@brief	���C�g���Ǘ�����N���X
*
*	@date	2013/09/19
*
*	@author	�g�c�@����
*	@author	�F���@����
*/
#include "NW_Lib_LightManager.h"
#include "../Utility/NW_Lib_utility.h"

namespace NW_Lib
{
	namespace System
	{
		/**
		 * @brief   �R���X�g���N�^.
		 */
		LightManager::LightManager()
		{
			m_pLight[0] = NULL;
			m_pLight[1] = NULL;
		}

		/**
		 * @brief   �f�X�g���N�^.
		 */
		LightManager::~LightManager()
		{

		}

		/**
		 * @brief   ������
		 */
		void LightManager::Initialize()
		{
			m_pLight[UPPER_SCREEN_DRAWING] = new Light();

			m_pLight[LOWER_SCREEN_DRAWING] = new Light();
		}

		/**
		 * @brief   ���
		 */
		void LightManager::Finalize()
		{
			SafeDelete(m_pLight[UPPER_SCREEN_DRAWING]);

			SafeDelete(m_pLight[LOWER_SCREEN_DRAWING]);
		}

		/**
		 * @brief   ���C�g���̎擾
		 *
		 * @param drawingscreen �㉺�̂ǂ���̃��C�g�����w��
		 *
		 * @return ���C�g���
		 */
		Light* LightManager::GetLight(DrawingScreen drawingscreen)
		{
			return m_pLight[drawingscreen];
		}
	}
}

