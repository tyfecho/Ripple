/**
*	@file	GamePad.h
*
*	@brief	�Q�[���p�b�h�p�֐�
*
*	@date	2012/08/25
*
*	@author	�g�c�@����
*/
#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <d3dx9.h>
#include "../System/NW_Lib_Common.h"

namespace NW_Lib
{
	// �R���g���[���[�̃{�^���ꗗ
	enum
	{
		BUTTON1,
		BUTTON2,
		BUTTON3,
		BUTTON4,
		BUTTON5,
		BUTTON6,
		BUTTON7,
		BUTTON8,
		BUTTON9,
		BUTTON10,
		BUTTON11,
		BUTTON12,
		BUTTON13,
		BUTTON14,
		BUTTON15,
		BUTTON16,
		BUTTON17,
		BUTTON18,
		BUTTON19,
		BUTTON20,
	};

	class GamePad:public Common
	{
		private:
			// DirectInput
			LPDIRECTINPUT8 m_pDinput;
			// DirectInput�̃f�o�C�X
			LPDIRECTINPUTDEVICE8 m_pDinputDevice;
			// �{�^���̏��
			DIJOYSTATE2 m_GamePadState;
			DIJOYSTATE2 m_GamePadStateBefore;
		public:
			// �R���X�g���N�^
			GamePad();
			// �f�X�g���N�^
			~GamePad();

			// ������
			HRESULT Initialize();

			// �X�V
			void Update();

			// �������ςȂ����ǂ���
			bool PushButton(int buttonnum);
			// �����ꂽ�u�Ԃ��ǂ���
			bool TrigerButton(int buttonnum);
			// �����ꂽ�u��
			bool PushOffButton(int buttonnum);
			// �\���L�[�̏�Ԃ�Ԃ�
			D3DXVECTOR3 GetAllow();

			// ���p�\�ȃW���C�X�e�B�b�N��񋓂���R�[���o�b�N�֐�
			static BOOL CALLBACK EnumJoysticksCBfunc(const DIDEVICEINSTANCE* pdidInstance, LPVOID pContext);
			// �A�v���P�[�V�����Ŏg�p����R���g���[���[�̃v���p�e�B��񋓂��Đݒ肷��
			static BOOL CALLBACK EnumObjectsCBfunc(const DIDEVICEOBJECTINSTANCE* pdidoi,LPVOID pContext);

			LPDIRECTINPUT8 GetDInput()
			{
				return m_pDinput;
			}
			LPDIRECTINPUTDEVICE8* GetDInputDevice()
			{
				return &m_pDinputDevice;
			}
	};
}


