/**
*	@file	Window.h
*
*	@brief	ウィンドウクラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#pragma once

#include <Windows.h>


namespace NW_Lib
{	
	//マウス構造体
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

			// フルスクリーンかどうかを格納
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

			// ウィンドウハンドルを渡す
			HWND Window::GetWindowHandle()
			{
				return hWnd;
			}

			// 作られる座標の取得
			POINT GetCreatePos()
			{
				return m_createPos; 
			}

			// フルスクリーン状態かどうかを返す
			bool IsFullScrean()
			{
				return m_FullScrean;
			}

			// ウィンドウアクティブ
			static bool IsActive()
			{
				return m_windowActiveFlag;
			}

	};
}
