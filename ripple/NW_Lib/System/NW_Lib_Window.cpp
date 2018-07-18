/***
*	@file	Window.cpp
*
*	@brief	ウィンドウクラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
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

			//ウィンドウがアクティブになった時
			if( (wParam & 0xFFFF) == WA_ACTIVE)
			{
				NW_Lib::Window::m_windowActiveFlag = true;
			}
			//ウィンドウが非アクティブになった時
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
				// ホイール上方向にスクロール
				NW_Lib::Window::m_mouse.wheel = (short)HIWORD(wParam);
			}
			else
			if((short)HIWORD(wParam) < 0)
			{
				// ホイール下方向にスクロール
				NW_Lib::Window::m_mouse.wheel = (short)HIWORD(wParam);
			}

			break;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}


namespace NW_Lib
{

	/*
	*	コンストラクタ
	*/
	Window::Window()
	:m_FullScrean(false)
	{
		wc.cbSize = sizeof(WNDCLASSEX); /**構造体のサイズ*/
		wc.style = CS_CLASSDC;
		wc.lpfnWndProc = MsgProc;
		wc.cbClsExtra = 0L;                    /**構造体が指定する余分なメモリサイズの指定*/
		wc.cbWndExtra = 0L;					   /**ウインドウが使用する余分なメモリサイズの指定*/
		wc.hInstance = GetModuleHandle(NULL);  /**モジュールのインスタンスハンドル*/
		//wc.hIcon = NULL;				       /**アイコン*/
		wc.hIcon = LoadIcon(wc.hInstance, (LPCSTR)(IDI_ICON1));				       /**アイコン*/
		wc.hCursor = NULL;				       /**カーソル*/ 
		wc.hbrBackground = NULL;			   /**背景色のブラシ指定*/
		wc.lpszMenuName = NULL;				   /**メニューリソースについて*/
		wc.lpszClassName = "D3D My_framework"; /**名前（自由につけられる）*/
		wc.hIconSm = NULL;                     /**アプリケーションから連想されるスモールアイコン*/          

		RegisterClassEx( &wc );

		m_createPos.x = 10;
		m_createPos.y = 10;

	}

	/*
	*	デストラクタ
	*/
	Window::~Window()
	{
	}

	/*
	*	ウインドウ作成
	*	@param width   スクリーンの横幅
	*	@param height  スクリーンの縦幅
	*	@param title   バーの名前
	*/
	void Window::Create(int width, int height, char *title)
	{
		RECT rect = {0,0,width,height};
		AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,false);
		width = rect.right + (-rect.left);
		height = rect.bottom + (-rect.top);
		// Abe Debug
		//if(MessageBox(NULL, "フルスクリーンにしますか？", "CheckFullscrean", MB_YESNO) == IDYES){
		//	m_FullScrean = true;

		//	hWnd = CreateWindow(
		//							  "D3D My_framework", /**上でつけた名前*/
		//							  title, /**自由につけられる名前（ゲーム名とか）。画面上の青いとこに表示される*/
		//							  WS_POPUPWINDOW,/**ウインドウのタイプ*/
		//							  /**画面の位置*/
		//							  100,                /**Xの位置*/
		//							  100,                /**Yの位置*/
		//							  width,              /**Widthの長さ*/
		//							  height,             /**Heightの長さ*/
		//							  GetDesktopWindow(), /**デスクトップのハンドル*/
		//							  NULL,               /**メニューハンドルまたはオーナーウインドウID*/
		//							  wc.hInstance,       /**アプリケーションインスタンスのハンドル*/
		//							  NULL                /**ウインドウ作成データ*/
		//							  );
		//}else
		{
			m_FullScrean = false;

			hWnd = CreateWindow(
									  "D3D My_framework", /**上でつけた名前*/
									  title, /**自由につけられる名前（ゲーム名とか）。画面上の青いとこに表示される*/
									  WS_OVERLAPPEDWINDOW,/**ウインドウのタイプ*/
									  /**画面の位置*/
									  100,                /**Xの位置*/
									  100,                /**Yの位置*/
									  width,              /**Widthの長さ*/
									  height,             /**Heightの長さ*/
									  GetDesktopWindow(), /**デスクトップのハンドル*/
									  NULL,               /**メニューハンドルまたはオーナーウインドウID*/
									  wc.hInstance,       /**アプリケーションインスタンスのハンドル*/
									  NULL                /**ウインドウ作成データ*/
									  );
		}
	}

	/*
	*	ウインドウを表示させる
	*/
	void Window::Show()
	{
			ShowWindow( 
					hWnd, /**ウインドウのハンドル*/
					SW_SHOWDEFAULT /**表示状態*/
					);

				UpdateWindow( hWnd );
	}
}
