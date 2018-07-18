/**
*	@file	LightControler.h
*
*	@brief	���C�g���Ǘ�����N���X
*
*	@date	2013/09/19
*
*	@author	�g�c�@����
*	@author	�F���@����
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
				Light* m_pLight[NUM_SCREEN_DRAWING]; // ���C�g���
				/**
				 * @brief   �R���X�g���N�^.
				 */
				LightManager();
				LightManager(const LightManager& val){(void)val;}
				LightManager& operator=(const LightManager& val){(void)val;return *this;}


				/**
				 * @brief   �f�X�g���N�^.
				 */
				~LightManager();
			public:
				
				/**
				 * @brief   �V���O���g����������
				 */
				static LightManager& GetInstance()
				{
					static LightManager instance;
					return instance;
				}

				/**
				 * @brief   ������
				 */
				void Initialize();

				/**
				 * @brief   ���
				 */
				void Finalize();

				/**
				 * @brief   ���C�g���̎擾
				 *
				 * @param drawingscreen �㉺�̂ǂ���̃��C�g�����w��
				 *
				 * @return ���C�g���
				 */
				Light* GetLight(DrawingScreen drawingscreen);
		};
	}
}

