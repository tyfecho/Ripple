/**
*	@file	LightControler.cpp
*
*	@brief	���C�g�𑀍삷��N���X
*
*	@date	2013/09/19
*
*	@author	�g�c�@����
*	@author	�F���@����
*/

#include "NW_Lib_LightControler.h"


namespace NW_Lib
{
	namespace System
	{
		/**
		 * @brief   ���C�g�̈ʒu��ݒ�
		 *
		 * @param drawingscreen ���ʂ�����ʂ��̎w��
		 *
		 * @param   x �V�����ݒ肷�鎋�_��X���W
		 * 
		 * @param   y �V�����ݒ肷�鎋�_��X���W
		 * 
		 * @param   z �V�����ݒ肷�鎋�_��X���W
		 */
		void LightControler::SetPosition(DrawingScreen drawingscreen,float x,float y,float z)
		{
			LightManager::GetInstance().GetLight(drawingscreen)->SetPos(x,y,z);
		}
		/**
		 * @brief   ���C�g�̈ʒu��ݒ�
		 *
		 * @param drawingscreen ���ʂ�����ʂ��̎w��
		 *
		 * @param pos			�V�����ݒ肷�鎋�_
		 */
		void LightControler::SetPosition(DrawingScreen drawingscreen,VECTOR3 pos)
		{
			LightManager::GetInstance().GetLight(drawingscreen)->SetPos(pos.x,pos.y,pos.z);
		}

		/**
		 * @brief   ���C�g�̌�����ݒ�
		 *
		 * @param drawingscreen ���ʂ�����ʂ��̎w��
		 *
		 * @param   x �V�����ݒ肷�鎋�_��X���W
		 * 
		 * @param   y �V�����ݒ肷�鎋�_��X���W
		 * 
		 * @param   z �V�����ݒ肷�鎋�_��X���W
		 */
		void LightControler::SetDirection(DrawingScreen drawingscreen,float x,float y,float z)
		{
			LightManager::GetInstance().GetLight(drawingscreen)->SetDirection(x,y,z);
		}
		/**
		 * @brief   ���C�g�̌�����ݒ�
		 *
		 * @param drawingscreen ���ʂ�����ʂ��̎w��
		 *
		 * @param pos			�V�����ݒ肷�鎋�_
		 */
		void LightControler::SetDirection(DrawingScreen drawingscreen,VECTOR3 pos)
		{
			LightManager::GetInstance().GetLight(drawingscreen)->SetDirection(pos.x,pos.y,pos.z);
		}

		/**
		 * @brief   ���C�g�̐F��ݒ�
		 *
		 * @param drawingscreen ���ʂ�����ʂ��̎w��
		 *
		 * @param   _r �ԐF�̗v�f
		 * 
		 * @param   _g �ΐF�̗v�f
		 * 
		 * @param   _b �F�̗v�f
		 */
		void LightControler::SetColor(DrawingScreen drawingscreen,float _r, float _g, float _b)
		{
			LightManager::GetInstance().GetLight(drawingscreen)->SetColor(_r,_g,_b);
		}
	}
}

