/**
*	@file	DirectXInputManager.h
*
*	@brief	���͏����p�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#pragma once


#include <Windows.h>

#include <d3dx9.h>


namespace NW_Lib
{
	class Window;
	class GamePad;

	class DirectXInputManager
	{
		private:
			// �L�[�{�[�h�̏��
			BYTE keyState[256];
			BYTE keyState_mae[256];
			// �}�E�X�̃|�W�V����
			POINT m_mousePos[2];
			// �Q�[���p�b�h
			GamePad* m_pGamePad;
			bool m_NoGamePad;

			// �E�C���h�E���
			Window* m_pWindow;
		public:
			DirectXInputManager(Window* pWindow);
			~DirectXInputManager();
			// �S�̂̍X�V
			void Update();
			// �L�[�{�[�h
			bool PushKey(BYTE keyCode);
			bool TrigerKey(BYTE keyCode);
			bool PushOffKey(BYTE keyCode);
			// �Q�[���p�b�h
			bool PushButton(int button);
			bool TrigerButton(int button);
			bool PushOffButton(int button);
			D3DXVECTOR3 GetAllow();

			// �}�E�X�̃|�W�V�����̎擾
			POINT GetMousePos()
			{
				return m_mousePos[0]; 
			}
			// �}�E�X�̃|�W�V�����̐���
			void MouseCliping(long left = 0, long top = 0, long right = 0, long bottom = 0);
			void MouseClipingOff();
			// �}�E�X�̈ړ��x�N�g���̎擾
			D3DXVECTOR2 GetMouseVec();
			// �v���O������ł̃L�[�{�[�h����p�֐�
			void OnKey(BYTE keyCode);
			void OffKey(BYTE keyCode);
	};
}



