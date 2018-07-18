/**
*	@file	CameraControler.cpp
*
*	@brief	�J�����𑀍삷��N���X
*
*	@date	2013/09/19
*
*	@author	�g�c�@����
*	@author	�F���@����
*/
#include "NW_Lib_CameraControler.h"

namespace NW_Lib
{
	namespace System
	{
		/**
		 * @brief   �J�����̎��_��ݒ�
		 *
		 * @param drawingscreen ���ʂ�����ʂ��̎w��
		 *
		 * @param   x �V�����ݒ肷�鎋�_��X���W
		 * 
		 * @param   y �V�����ݒ肷�鎋�_��X���W
		 * 
		 * @param   z �V�����ݒ肷�鎋�_��X���W
		 */
		void CameraControler::SetPosition(DrawingScreen drawingscreen,float x,float y,float z)
		{
			CameraManager::GetInstance().GetCamera(drawingscreen)->SetCameraPos(x,y,z);
		}
		/**
		 * @brief   �J�����̎��_��ݒ�
		 *
		 * @param drawingscreen ���ʂ�����ʂ��̎w��
		 *
		 * @param pos			�V�����ݒ肷�鎋�_
		 */
		void CameraControler::SetPosition(DrawingScreen drawingscreen,VECTOR3 pos)
		{
			CameraManager::GetInstance().GetCamera(drawingscreen)->SetCameraPos(pos.x,pos.y,pos.z);
		}

		/**
		 * @brief   �J�����̒����_��ݒ�
		 *
		 * @param drawingscreen ���ʂ�����ʂ��̎w��
		 *
		 * @param   x �V�����ݒ肷�钍���_��X���W
		 * 
		 * @param   y �V�����ݒ肷�钍���_��X���W
		 * 
		 * @param   z �V�����ݒ肷�钍���_��X���W
		 */
		void CameraControler::SetTarget(DrawingScreen drawingscreen,float x,float y,float z)
		{
			CameraManager::GetInstance().GetCamera(drawingscreen)->SetCameraTarget(x,y,z);
		}
		/**
		 * @brief   �J�����̒����_��ݒ�
		 *
		 * @param drawingscreen ���ʂ�����ʂ��̎w��
		 *
		 * @param pos			�V�����ݒ肷�钍���_
		 */
		void CameraControler::SetTarget(DrawingScreen drawingscreen,VECTOR3 pos)
		{
			CameraManager::GetInstance().GetCamera(drawingscreen)->SetCameraTarget(pos.x,pos.y,pos.z);
		}

		/**
		 * @brief   �J�����̏�����x�N�g����ݒ�
		 *
		 * @param drawingscreen ���ʂ�����ʂ��̎w��
		 *
		 * @param   x �V�����ݒ肷�������x�N�g����X����
		 * 
		 * @param   y �V�����ݒ肷�������x�N�g����Y����
		 * 
		 * @param   z �V�����ݒ肷�������x�N�g����Z����
		 */
		void CameraControler::SetUpVec(DrawingScreen drawingscreen,float x,float y,float z)
		{
			CameraManager::GetInstance().GetCamera(drawingscreen)->SetVecUp(x,y,z);
		}
		/**
		 * @brief   �J�����̒����_��ݒ�
		 *
		 * @param drawingscreen ���ʂ�����ʂ��̎w��
		 *
		 * @param vec			�V�����ݒ肷�钍���_
		 */
		void CameraControler::SetUpVec(DrawingScreen drawingscreen,VECTOR3 vec)
		{
			CameraManager::GetInstance().GetCamera(drawingscreen)->SetCameraTarget(vec.x,vec.y,vec.z);
		}

		/**
		 * @brief   ���ˉe�������ˉe�̐؂�ւ�
		 *
		 * @param drawingscreen ���ʂ�����ʂ��̎w��
		 *
		 * @param cameramode	���ˉe�������ˉe����ݒ�
		 */
		void CameraControler::SetMode(DrawingScreen drawingscreen,MODE cameramode)
		{
			if(drawingscreen == UPPER_SCREEN_DRAWING){
				if(cameramode == MODE_PERSPECTIVE){
					CameraManager::GetInstance().GetCamera(drawingscreen)->SetPerspective();
				}else if(cameramode == MODE_ORTHO){
					CameraManager::GetInstance().GetCamera(drawingscreen)->SetOrtho(400.0f,240.0f);
				}
			}else if(drawingscreen == LOWER_SCREEN_DRAWING){
				if(cameramode == MODE_PERSPECTIVE){
					CameraManager::GetInstance().GetCamera(drawingscreen)->SetPerspective();
				}else if(cameramode == MODE_ORTHO){
					CameraManager::GetInstance().GetCamera(drawingscreen)->SetOrtho(320.0f,240.0f);
				}
			}
		}
	}
}
