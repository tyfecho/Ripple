/***
*	@file	DirectXInputManager.cpp
*
*	@brief	���͏����p�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#include "NW_Lib_DirectXInputManager.h"
#include "NW_Lib_GamePad.h"
#include <Windows.h>
#include "../Utility/NW_Lib_utility.h"
#include "NW_Lib_window.h"
#include "../Utility/NW_Lib_HardwareDefine.h"


namespace NW_Lib
{
	/**
	*	�R���X�g���N�^
	*	@param �E�C���h�E���
	*/
	DirectXInputManager::DirectXInputManager(Window* pWindow)
	:m_pWindow(pWindow)
	,m_pGamePad(NULL)
	,m_NoGamePad(false)
	{
		ZeroMemory(keyState,sizeof(keyState));

		m_pGamePad = new GamePad();
		if(FAILED(m_pGamePad->Initialize())){
			// �Q�[���p�b�h�ŉ����ُ킪�������̂ŁA�Q�[���p�b�h���g��Ȃ�
			m_NoGamePad = true;
		}
	}
	/**
	*	�f�X�g���N�^
	*/
	DirectXInputManager::~DirectXInputManager()
	{
		SafeDelete(m_pGamePad);
	}
	/**
	*	�X�V�����i���t���[���j
	*/
	void DirectXInputManager::Update()
	{
		// �L�[�{�[�h
		memcpy(keyState_mae,keyState,sizeof(keyState_mae));
		GetKeyboardState(keyState);

		// �}�E�X
		m_mousePos[1] = m_mousePos[0];
		GetCursorPos( &m_mousePos[0] );
		ScreenToClient(m_pWindow->GetWindowHandle(), &m_mousePos[0]);

		// Scale the touch position according to the window size
		RECT rect;
		GetClientRect(m_pWindow->GetWindowHandle(), &rect);
		LONG width = rect.right - rect.left;
		LONG height = rect.bottom - rect.top;
		m_mousePos[0].x *= (float)UPPER_SCREEN_WIDTH/width;
		m_mousePos[0].y *= (float)(LOWER_SCREEN_HEIGHT+UPPER_SCREEN_HEIGHT)/height;

		// �Q�[���p�b�h
		if(!m_NoGamePad){
			m_pGamePad->Update();
		}
	}
	/**
	*	�����L�[�������ꂽ�Ƃ��̏���
	*	@param keyCode  �L�[�R�[�h
	*/
	bool DirectXInputManager::PushKey(BYTE keyCode)
	{
		if(keyState[keyCode] & 0x80){
			return true;
		}else{
			return false;
		}
	}
	/**
	*	�����L�[�������ꂽ�u�Ԃ̏���
	*	@param keyCode  �L�[�R�[�h
	*/
	bool DirectXInputManager::TrigerKey(BYTE keyCode)
	{
		if(keyState[keyCode] & 0x80 && !(keyState_mae[keyCode] & 0x80)){
			return true;
		}else{
			return false;
		}
	}
	/**
	*	�����L�[�������ꂽ�u�Ԃ̏���
	*	@param keyCode  �L�[�R�[�h
	*/
	bool DirectXInputManager::PushOffKey(BYTE keyCode)
	{
		if(!(keyState[keyCode] & 0x80) && keyState_mae[keyCode] & 0x80){
			return true;
		}else{
			return false;
		}
	}

	/**
	*	�����Ŏw�肳�ꂽ�Q�[���p�b�h�̃{�^������������ςȂ����ǂ���
	*	@param button  �{�^���ԍ�
	*/
	bool DirectXInputManager::PushButton(int button)
	{
		if(m_NoGamePad){
			// �Q�[���p�b�h���Ȃ��Ƃ���g���Ȃ��Ƃ�
			return false;
		}
		if( button >= 37 && button <= 40 )
		{
			return m_pGamePad->PushButton(button);
		}
		else
		{
			return m_pGamePad->PushButton(button-1);
		}
	}

	/**
	*	�����Ŏw�肳�ꂽ�Q�[���p�b�h�̃{�^���������ꂽ�u�Ԃ��ǂ���
	*	@param button  �{�^���ԍ�	
	*/
	bool DirectXInputManager::TrigerButton(int button)
	{
		if(m_NoGamePad){
			// �Q�[���p�b�h���Ȃ��Ƃ���g���Ȃ��Ƃ�
			return false;
		}
		if( button >= 37 && button <= 40 )
		{
			return m_pGamePad->TrigerButton(button);
		}
		else
		{
			return m_pGamePad->TrigerButton(button-1);
		}
	}
	
	/**
	*	�����Ŏw�肳�ꂽ�Q�[���p�b�h�̃{�^���������ꂽ�u�Ԃ��ǂ���
	*	@param button  �{�^���ԍ�	
	*/
	bool DirectXInputManager::PushOffButton(int button)
	{
		if(m_NoGamePad){
			// �Q�[���p�b�h���Ȃ��Ƃ���g���Ȃ��Ƃ�
			return false;
		}
		if( button >= 37 && button <= 40 )
		{
			return m_pGamePad->PushOffButton(button);
		}
		else
		{
			return m_pGamePad->PushOffButton(button-1);
		}
	}

	/**
	*	�\���L�[�̏�Ԃ�Ԃ�
	*/
	D3DXVECTOR3 DirectXInputManager::GetAllow()
	{
		if(m_NoGamePad){
			// �Q�[���p�b�h���Ȃ��Ƃ���g���Ȃ��Ƃ�
			return D3DXVECTOR3(-8.0f,-8.0f,0.0f);
		}

		return m_pGamePad->GetAllow();
	}


	/**
	*	�����I�ɃL�[���������鏈��
	*	@param keyCode  �L�[�R�[�h
	*/
	void DirectXInputManager::OnKey(BYTE keyCode)
	{
		if(!PushKey(keyCode)){
			keyState[keyCode] += 0x80;
		}
	}

	/**
	*	�����I�ɃL�[���������Ȃ�����
	*	@param keyCode  �L�[�R�[�h
	*/
	void DirectXInputManager::OffKey(BYTE keyCode)
	{
		keyState[keyCode] = 0x00;
	}

	/**
	*	�}�E�X�̃|�W�V�����̐���
	*	@param left    ������
	*	@param top     �㐧��
	*	@param right   �E����
	*	@param bottom  ������
	*/
	void DirectXInputManager::MouseCliping(long left, long top, long right, long bottom)
	{
		RECT rect,windowRect;
		GetWindowRect(m_pWindow->GetWindowHandle(),&windowRect);

		rect.left   = windowRect.left    + left;
		rect.top    = windowRect.top     + top;
		rect.right  = windowRect.right   + right;
		rect.bottom = windowRect.bottom  + bottom;

		ClipCursor( &rect );
	}
	/**
	*	�}�E�X�̃|�W�V�����̐�������������
	*/
	void DirectXInputManager::MouseClipingOff()
	{
		ClipCursor( NULL );
	}

	/**
	*	�}�E�X�̈ړ��x�N�g���̎擾
	*/
	D3DXVECTOR2 DirectXInputManager::GetMouseVec()
	{
		return D3DXVECTOR2(static_cast<float>(m_mousePos[0].x - m_mousePos[1].x), static_cast<float>(m_mousePos[0].y - m_mousePos[1].y));
	}
}

