/**
 * @file    InputManager.h
 * @brief   �C���v�b�g��S������փb�_�ł�
 * 
 * @author  Masaya Yoshida
 * @author  Kenta Minamitsu
 * @date    2013/09/18
 * @version 1.0
 */

#include "NW_Lib_InputManager.h"

#include "../Utility/NW_Lib_utility.h"

#include "NW_Lib_Touch.h"

namespace NW_Lib
{
	namespace System
	{
		Touch*			InputManager::m_pTouch			= NULL;

		/**
		 * @brief   ����������
		 */
		void InputManager::Initialize()
		{
			//// �^�b�`�̐���
			m_pTouch = new Touch();
			m_pTouch->Initialize();
		}
		/**
		 * @brief   �X�V����
		 */
		void InputManager::Update()
		{
			//// �^�b�`�̍X�V
			m_pTouch->Update();
		}
		/**
		 * @brief   �������
		 */
		void InputManager::Finalize()
		{
			//// �^�b�`�̉��
			m_pTouch->Finalize();
			SafeDelete(m_pTouch);
		}


	//////////////////////////  Touch  //////////////////////////
		/**
		 * @brief   �^�b�`�����ꏊ��Ԃ�����
		 *
		 * @return �^�b�`�����ꏊ��Ԃ�VECTOR2
		 */
		VECTOR2 InputManager::GetTouchPosition()
		{
			return VECTOR2(m_pTouch->GetTouchX(),m_pTouch->GetTouchY());
		}

		/**
		 * @brief   �^�b�`�������Ă��邩�̔��菈��
		 *
		 * @return true:�^�b�`�������Ă��� false:�^�b�`����Ă��Ȃ�
		 */
		bool InputManager::GetTouchPush()
		{
			return m_pTouch->IsTouch();
		}

		/**
		 * @brief   �^�b�`�����u�Ԃ��̔��菈��
		 *
		 * @return true:�^�b�`�����u�� false:�^�b�`���ꂽ�u�Ԃł͂Ȃ�
		 */
		bool InputManager::GetTouchTrigger()
		{
			return m_pTouch->IsTrigger();
		}

		/**
		 * @brief   �^�b�`���I�����u�Ԃ��̔��菈��
		 *
		 * @return true:�^�b�`�𗣂��� false:�^�b�`�𗣂����u�Ԃł͂Ȃ�
		 */
		bool InputManager::GetTouchRelease()
		{
			return m_pTouch->IsRelease();
		}

	//////////////////////////  Button  //////////////////////////
		/**
		 * @brief   �{�^�������������Ă��邩�̔��菈��
		 *
		 * @param  key �L�[�R�[�h
		 *
		 * @return true:�{�^�������������Ă��� false:�{�^����������Ă��Ȃ�
		 */
		bool InputManager::GetButtonPush(Button_Key key)
		{
			return m_pInput->PushButton(key);
		}

		/**
		 * @brief   �{�^�����������u�Ԃ��̔��菈��
		 *
		 * @param  key �L�[�R�[�h
		 *
		 * @return true:�{�^���������ꂽ�u�� false:�{�^���������ꂽ�u�ԂłȂ�
		 */
		bool InputManager::GetButtonTrigger(Button_Key key)
		{
			return m_pInput->TrigerButton(key);
		}

		/**
		 * @brief   �{�^���𗣂����u�Ԃ��̔��菈��
		 *
		 * @param  key �L�[�R�[�h
		 *
		 * @return true:�{�^���𗣂����u�� false:�{�^���𗣂����u�ԂłȂ�
		 */
		bool InputManager::GetButtonRelease(Button_Key key)
		{
			return m_pInput->PushOffButton(key);
		}

	//////////////////////////  Mic  //////////////////////////
		/**
		 * @brief   �}�C�N�ɔ������Ă鉹�ʂ��擾
		 *
		 * @return �}�C�N�̉���
		 */		
		int InputManager::GetMicLoudness()
		{
			return 0;//m_pMic->GetLoudness();
		}
	//////////////////////////  Accel  //////////////////////////
		/**
		 * @brief   �����x�Z���T�[�̎擾
		 *
		 * @return �eXYZ�����̉����x���i�[����VECTOR3�^
		 */
		VECTOR3* InputManager::GetAccel()
		{
			return &VECTOR3(0.0f,0.0f,0.0f);//m_pGyro->GetAccele();
		}

	//////////////////////////  SlidePad  //////////////////////////
		/**
		 * @brief   �X���C�h�p�b�h�̎擾
		 *
		 * @return �eXY�����̃X�e�B�b�N�����i�[����VECTOR2�^
		 */		
		VECTOR2 InputManager::GetSlidePad()
		{
			return VECTOR2(m_pInput->GetAllow().x,m_pInput->GetAllow().y);
		}

	//////////////////////////  Gyro  //////////////////////////
		/**
		 * @brief   �W���C���̎p�����擾
		 *
		 * @return �W���C���̎p�����i�[������]�s��
		 */		
		MATRIX33 InputManager::GetGyroDirection()
		{
			static D3DXMATRIX mat;
			D3DXMatrixIdentity(&mat);

			return &mat;
		}
		/**
		 * @brief   �W���C���̊p���x�̐ώZ���擾
		 *
		 * @return �W���C���̊p���x�̐ώZ
		 */	
		VECTOR3 InputManager::GetGyroAngle()
		{
			return VECTOR3(0.0f,0.0f,0.0f);
		}
		/**
		 * @brief   �W���C���̊p���x���擾
		 *
		 * @return �W���C���̊p���x
		 */	
		VECTOR3 InputManager::GetGyroSpeed()
		{
			return VECTOR3(0.0f,0.0f,0.0f);
		}
	}
}





