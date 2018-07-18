/**
*	@file	DirectXInputManager.h
*
*	@brief	入力処理用クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
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
			// キーボードの状態
			BYTE keyState[256];
			BYTE keyState_mae[256];
			// マウスのポジション
			POINT m_mousePos[2];
			// ゲームパッド
			GamePad* m_pGamePad;
			bool m_NoGamePad;

			// ウインドウ情報
			Window* m_pWindow;
		public:
			DirectXInputManager(Window* pWindow);
			~DirectXInputManager();
			// 全体の更新
			void Update();
			// キーボード
			bool PushKey(BYTE keyCode);
			bool TrigerKey(BYTE keyCode);
			bool PushOffKey(BYTE keyCode);
			// ゲームパッド
			bool PushButton(int button);
			bool TrigerButton(int button);
			bool PushOffButton(int button);
			D3DXVECTOR3 GetAllow();

			// マウスのポジションの取得
			POINT GetMousePos()
			{
				return m_mousePos[0]; 
			}
			// マウスのポジションの制限
			void MouseCliping(long left = 0, long top = 0, long right = 0, long bottom = 0);
			void MouseClipingOff();
			// マウスの移動ベクトルの取得
			D3DXVECTOR2 GetMouseVec();
			// プログラム上でのキーボード操作用関数
			void OnKey(BYTE keyCode);
			void OffKey(BYTE keyCode);
	};
}



