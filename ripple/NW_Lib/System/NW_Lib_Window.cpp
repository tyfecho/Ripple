/***
*	@file	Window.cpp
*
*	@brief	�E�B���h�E�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#include "NW_Lib_window.h"
#include "../../resource.h"

POINT NW_Lib::Window::m_createPos;

bool NW_Lib::Window::m_windowActiveFlag = true;

NW_Lib::MOUSE NW_Lib::Window::m_mouse;

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            //Cleanup();
            PostQuitMessage( 0 );
            return 0;
		case WM_SETCURSOR:
			if(LOWORD(lParam) == HTCLIENT){
				HCURSOR hCursor;
				hCursor = LoadCursor( NULL, IDC_ARROW );
				SetCursor(hCursor);
				//m_pD3DDevice->ShowCursor(TRUE);
				return TRUE;
			}
			break;
		case WM_MOVING:
			NW_Lib::Window::m_createPos.x = reinterpret_cast<RECT*>(lParam)->left;
			NW_Lib::Window::m_createPos.y = reinterpret_cast<RECT*>(lParam)->top;
			break;
		case WM_ACTIVATE:

			//�E�B���h�E���A�N�e�B�u�ɂȂ�����
			if( (wParam & 0xFFFF) == WA_ACTIVE)
			{
				NW_Lib::Window::m_windowActiveFlag = true;
			}
			//�E�B���h�E����A�N�e�B�u�ɂȂ�����
			else if( (wParam & 0xFFFF) == WA_INACTIVE )
			{
				NW_Lib::Window::m_windowActiveFlag = false;
			}

			break;
		case WA_INACTIVE:
			
			NW_Lib::Window::m_mouse.L = false;
			NW_Lib::Window::m_mouse.R = false;
			break;
		case WM_LBUTTONDOWN:

			NW_Lib::Window::m_mouse.L = true;
			break;
		case WM_LBUTTONUP:

			NW_Lib::Window::m_mouse.L = false;
			break;
		case WM_RBUTTONDOWN:

			NW_Lib::Window::m_mouse.R = true;
			break;
		case WM_RBUTTONUP:

			NW_Lib::Window::m_mouse.R = false;
			break;

		case WM_MOUSEWHEEL:

			if((short)HIWORD(wParam) > 0)
			{
				// �z�C�[��������ɃX�N���[��
				NW_Lib::Window::m_mouse.wheel = (short)HIWORD(wParam);
			}
			else
			if((short)HIWORD(wParam) < 0)
			{
				// �z�C�[���������ɃX�N���[��
				NW_Lib::Window::m_mouse.wheel = (short)HIWORD(wParam);
			}

			break;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}


namespace NW_Lib
{

	/*
	*	�R���X�g���N�^
	*/
	Window::Window()
	:m_FullScrean(false)
	{
		wc.cbSize = sizeof(WNDCLASSEX); /**�\���̂̃T�C�Y*/
		wc.style = CS_CLASSDC;
		wc.lpfnWndProc = MsgProc;
		wc.cbClsExtra = 0L;                    /**�\���̂��w�肷��]���ȃ������T�C�Y�̎w��*/
		wc.cbWndExtra = 0L;					   /**�E�C���h�E���g�p����]���ȃ������T�C�Y�̎w��*/
		wc.hInstance = GetModuleHandle(NULL);  /**���W���[���̃C���X�^���X�n���h��*/
		//wc.hIcon = NULL;				       /**�A�C�R��*/
		wc.hIcon = LoadIcon(wc.hInstance, (LPCSTR)(IDI_ICON1));				       /**�A�C�R��*/
		wc.hCursor = NULL;				       /**�J�[�\��*/ 
		wc.hbrBackground = NULL;			   /**�w�i�F�̃u���V�w��*/
		wc.lpszMenuName = NULL;				   /**���j���[���\�[�X�ɂ���*/
		wc.lpszClassName = "D3D My_framework"; /**���O�i���R�ɂ�����j*/
		wc.hIconSm = NULL;                     /**�A�v���P�[�V��������A�z�����X���[���A�C�R��*/          

		RegisterClassEx( &wc );

		m_createPos.x = 10;
		m_createPos.y = 10;

	}

	/*
	*	�f�X�g���N�^
	*/
	Window::~Window()
	{
	}

	/*
	*	�E�C���h�E�쐬
	*	@param width   �X�N���[���̉���
	*	@param height  �X�N���[���̏c��
	*	@param title   �o�[�̖��O
	*/
	void Window::Create(int width, int height, char *title)
	{
		RECT rect = {0,0,width,height};
		AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,false);
		width = rect.right + (-rect.left);
		height = rect.bottom + (-rect.top);
		// Abe Debug
		//if(MessageBox(NULL, "�t���X�N���[���ɂ��܂����H", "CheckFullscrean", MB_YESNO) == IDYES){
		//	m_FullScrean = true;

		//	hWnd = CreateWindow(
		//							  "D3D My_framework", /**��ł������O*/
		//							  title, /**���R�ɂ����閼�O�i�Q�[�����Ƃ��j�B��ʏ�̐��Ƃ��ɕ\�������*/
		//							  WS_POPUPWINDOW,/**�E�C���h�E�̃^�C�v*/
		//							  /**��ʂ̈ʒu*/
		//							  100,                /**X�̈ʒu*/
		//							  100,                /**Y�̈ʒu*/
		//							  width,              /**Width�̒���*/
		//							  height,             /**Height�̒���*/
		//							  GetDesktopWindow(), /**�f�X�N�g�b�v�̃n���h��*/
		//							  NULL,               /**���j���[�n���h���܂��̓I�[�i�[�E�C���h�EID*/
		//							  wc.hInstance,       /**�A�v���P�[�V�����C���X�^���X�̃n���h��*/
		//							  NULL                /**�E�C���h�E�쐬�f�[�^*/
		//							  );
		//}else
		{
			m_FullScrean = false;

			hWnd = CreateWindow(
									  "D3D My_framework", /**��ł������O*/
									  title, /**���R�ɂ����閼�O�i�Q�[�����Ƃ��j�B��ʏ�̐��Ƃ��ɕ\�������*/
									  WS_OVERLAPPEDWINDOW,/**�E�C���h�E�̃^�C�v*/
									  /**��ʂ̈ʒu*/
									  100,                /**X�̈ʒu*/
									  100,                /**Y�̈ʒu*/
									  width,              /**Width�̒���*/
									  height,             /**Height�̒���*/
									  GetDesktopWindow(), /**�f�X�N�g�b�v�̃n���h��*/
									  NULL,               /**���j���[�n���h���܂��̓I�[�i�[�E�C���h�EID*/
									  wc.hInstance,       /**�A�v���P�[�V�����C���X�^���X�̃n���h��*/
									  NULL                /**�E�C���h�E�쐬�f�[�^*/
									  );
		}
	}

	/*
	*	�E�C���h�E��\��������
	*/
	void Window::Show()
	{
			ShowWindow( 
					hWnd, /**�E�C���h�E�̃n���h��*/
					SW_SHOWDEFAULT /**�\�����*/
					);

				UpdateWindow( hWnd );
	}
}
