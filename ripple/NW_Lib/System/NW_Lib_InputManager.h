/**
 * @file    InputManager.h
 * @brief   �C���v�b�g��S������փb�_�ł�
 * 
 * @author  Masaya Yoshida
 * @author  Kenta Minamitsu
 * @date    2013/09/18
 * @version 1.0
 */

#include "../Utility/NW_Lib_Constant.h"
#include "NW_Lib_common.h"

#pragma once

namespace NW_Lib
{
	namespace System
	{
		class Touch;
		class ButtonInput;
		class Mic;
		class GyroInput;

		class InputManager : public Common
		{
			private:
				/// �^�b�`�֘A
				static Touch* m_pTouch;
				/// �{�^���֘A
				//static ButtonInput* m_pButton;
				/// �}�C�N�֘A
				//static Mic* m_pMic;
				/// �W���C���E�����x�Z���T�[�֘A
				//static GyroInput* m_pGyro;
			public:
				/// ������
				static void Initialize();
				/// �X�V
				static void Update();
				/// ���
				static void Finalize();


				/// Touch
				static VECTOR2 GetTouchPosition();
				static bool GetTouchPush();
				static bool GetTouchTrigger();
				static bool GetTouchRelease();

				/// Button
				static bool GetButtonPush(Button_Key key);
				static bool GetButtonTrigger(Button_Key key);
				static bool GetButtonRelease(Button_Key key);

				/// Mic
				static int GetMicLoudness();

				/// Accel
				static VECTOR3* GetAccel();

				/// SlidePad
				static VECTOR2 GetSlidePad();

				/// Gyro
				static MATRIX33 GetGyroDirection();
				static VECTOR3 GetGyroAngle();
				static VECTOR3 GetGyroSpeed();
				/**
				 * @brief   �W���C���Z���T�[�̗V�т̐ݒ�.
				 * 
				 * @param   play �V��(0 �` SENSITIVITY).
				 * 
				 * @param   sensitivity ���o���x(0 �` SENSITIVITY)
				 *                      0 ... �����Ȃ�
				 *                      SENSITIVITY ... �f�o�C�X����擾�����l��Ԃ�.
				 */
				static void SetGyroSensitivity(signed short int play,signed short int sensivity);
		};
	}
}
