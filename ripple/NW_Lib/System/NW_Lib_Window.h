/**
*	@file	Window.h
*
*	@brief	�E�B���h�E�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#pragma once

#include <Windows.h>


namespace NW_Lib
{	
	//�}�E�X�\����
	typedef struct
	{
		bool R , prevR;
		bool L , prevL;
		short wheel;

	}MOUSE;

	class Window
	{
		private:
			WNDCLASSEX wc;
			HWND hWnd;

			// �t���X�N���[�����ǂ������i�[
			bool m_FullScrean;

		public:
			static POINT m_createPos;
			static MOUSE m_mouse;
			static bool m_windowActiveFlag;

		public:
			Window();
			~Window();
			void Create(int width,int height,char* title);
			void Show();

			// �E�B���h�E�n���h����n��
			HWND Window::GetWindowHandle()
			{
				return hWnd;
			}

			// �������W�̎擾
			POINT GetCreatePos()
			{
				return m_createPos; 
			}

			// �t���X�N���[����Ԃ��ǂ�����Ԃ�
			bool IsFullScrean()
			{
				return m_FullScrean;
			}

			// �E�B���h�E�A�N�e�B�u
			static bool IsActive()
			{
				return m_windowActiveFlag;
			}

	};
}
