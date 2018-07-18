/**
*	@file	GamePad.h
*
*	@brief	ゲームパッド用関数
*
*	@date	2012/08/25
*
*	@author	吉田　将也
*/
#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <d3dx9.h>
#include "../System/NW_Lib_Common.h"

namespace NW_Lib
{
	// コントローラーのボタン一覧
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
			// DirectInputのデバイス
			LPDIRECTINPUTDEVICE8 m_pDinputDevice;
			// ボタンの状態
			DIJOYSTATE2 m_GamePadState;
			DIJOYSTATE2 m_GamePadStateBefore;
		public:
			// コンストラクタ
			GamePad();
			// デストラクタ
			~GamePad();

			// 初期化
			HRESULT Initialize();

			// 更新
			void Update();

			// 押しっぱなしかどうか
			bool PushButton(int buttonnum);
			// 押された瞬間かどうか
			bool TrigerButton(int buttonnum);
			// 離された瞬間
			bool PushOffButton(int buttonnum);
			// 十字キーの状態を返す
			D3DXVECTOR3 GetAllow();

			// 利用可能なジョイスティックを列挙するコールバック関数
			static BOOL CALLBACK EnumJoysticksCBfunc(const DIDEVICEINSTANCE* pdidInstance, LPVOID pContext);
			// アプリケーションで使用するコントローラーのプロパティを列挙して設定する
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


