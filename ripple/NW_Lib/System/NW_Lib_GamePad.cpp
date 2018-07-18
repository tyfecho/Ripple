/**
*	@file	GamePad.cpp
*
*	@brief	ゲームパッド用関数
*
*	@date	2012/08/25
*
*	@author	吉田　将也
*/
#include "NW_Lib_GamePad.h"
#include "../Utility/NW_Lib_utility.h"

namespace NW_Lib
{
	/**
	*	利用可能なジョイスティックを列挙するコールバック関数
	*/
	BOOL CALLBACK GamePad::EnumJoysticksCBfunc(const DIDEVICEINSTANCE* pdidInstance, LPVOID pContext)
	{
		// 静的ではない変数を使うため、thisポインタを貰ってくる
		GamePad* obj = (GamePad*)pContext;

		// 「DirectInputデバイス」オブジェクトの作成
		if(FAILED( obj->GetDInput()->CreateDevice(pdidInstance->guidInstance,  
							 obj->GetDInputDevice(), NULL)))
		{
			return DIENUM_CONTINUE;
		}
		return DIENUM_STOP;
	}

	/**
	*	アプリケーションで使用するコントローラーのプロパティを列挙して設定する
	*/
	BOOL CALLBACK GamePad::EnumObjectsCBfunc( const DIDEVICEOBJECTINSTANCE* pdidoi,LPVOID pContext )
	{
		if( pdidoi->dwType & DIDFT_AXIS )
		{
			DIPROPRANGE diprg;
			ZeroMemory(&diprg,sizeof(diprg));
			diprg.diph.dwSize = sizeof(diprg); 
			diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
			diprg.diph.dwObj = pdidoi->dwType;
			diprg.diph.dwHow = DIPH_BYID;
			diprg.lMin = -1000;
			diprg.lMax = +1000;
	 
			LPDIRECTINPUTDEVICE8 lpJoy = (LPDIRECTINPUTDEVICE8)pContext;
			lpJoy->SetProperty( DIPROP_RANGE, &diprg.diph);
		}
		return DIENUM_CONTINUE;
	}


	/**
	*	コンストラクタ
	*/
	GamePad::GamePad()
	:m_pDinput(NULL)
	,m_pDinputDevice(NULL)
	{
	}

	/**
	*	デストラクタ
	*/
	GamePad::~GamePad()
	{
		if(m_pDinputDevice) 
		{
			m_pDinputDevice->Unacquire();
		}
		SafeRelease( m_pDinputDevice );
		SafeRelease( m_pDinput );
	}

	/**
	*	初期化処理
	*/
	HRESULT GamePad::Initialize()
	{
		HRESULT hr;

		// DirectInputの作成
		hr = DirectInput8Create(GetModuleHandle(NULL),DIRECTINPUT_VERSION,IID_IDirectInput8,(VOID**)&m_pDinput, NULL);
		if(FAILED(hr))
			return hr;
		// 利用可能なゲームコントローラーの列挙関数を実行
		hr = m_pDinput->EnumDevices(DI8DEVCLASS_GAMECTRL, GamePad::EnumJoysticksCBfunc,(void *)this, DIEDFL_ATTACHEDONLY);
		if(FAILED(hr)) return hr;

		// デバイスが作られなかった
		if(!m_pDinputDevice) return E_FAIL;

		// 協調レベルの設定
		hr = m_pDinputDevice->SetCooperativeLevel(m_hwnd,DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
		if(FAILED(hr)) return hr;

		// デバイスをジョイスティックに設定
		hr = m_pDinputDevice->SetDataFormat(&c_dfDIJoystick2);
		if(FAILED(hr)) return hr;

		//アプリケーションで使用するコントローラーのプロパティを列挙して設定する
		hr = m_pDinputDevice->EnumObjects( GamePad::EnumObjectsCBfunc,(void *)m_pDinputDevice, DIDFT_ALL );
		if(FAILED(hr)) return hr;

		// デバイスを「取得」する
		m_pDinputDevice->Acquire();

		return S_OK;
	}

	/**
	*	更新処理
	*/
	void GamePad::Update()
	{
		// ボタンの状態をコピーする（トリガー処理の為）
		memcpy(&m_GamePadStateBefore,&m_GamePadState,sizeof(m_GamePadStateBefore));
		HRESULT hr = m_pDinputDevice->Acquire();
		if((hr == DI_OK) || (hr == S_FALSE))
		{
			// ボタンの状態を更新
			m_pDinputDevice->GetDeviceState(sizeof(DIJOYSTATE2), &m_GamePadState);
		}
	}

	/**
	*	押しっぱなしかどうか
	*	@param buttonnum  ボタン番号
	*/
	bool GamePad::PushButton(int buttonnum)
	{
		if( buttonnum >= 37 && buttonnum <= 40 && m_GamePadState.rgdwPOV[0] != 0xffffffff )
		{
			switch(m_GamePadState.rgdwPOV[0])
			{
				case 0:
					return buttonnum == VK_UP;
				case 4500:
					return (buttonnum == VK_UP || buttonnum == VK_RIGHT);
				case 9000:
					return buttonnum == VK_RIGHT;
				case 13500:
					return (buttonnum == VK_DOWN || buttonnum == VK_RIGHT);
				case 18000:
					return buttonnum == VK_DOWN;
				case 22500:
					return (buttonnum == VK_DOWN || buttonnum == VK_LEFT);
				case 27000:
					return buttonnum == VK_LEFT;
				case 31500:
					return (buttonnum == VK_UP || buttonnum == VK_LEFT);
			}		

		}

		if(m_GamePadState.rgbButtons[buttonnum] & 0x80)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	*	押された瞬間かどうか
	*	@param buttonnum  ボタン番号
	*/
	bool GamePad::TrigerButton(int buttonnum)
	{

		if( buttonnum >= 37 && buttonnum <= 40 && m_GamePadState.rgdwPOV[0] != 0xffffffff )
		{
			int state  = -1,stateB  = -1,prevState = -1,prevStateB = -1;
			switch(m_GamePadState.rgdwPOV[0])
			{
				case 0:
					state = VK_UP;
					break;
				case 4500:
					state = VK_UP;
					stateB = VK_RIGHT;
					break;
				case 9000:
					state = VK_RIGHT;
					break;
				case 13500:
					state = VK_RIGHT;
					stateB = VK_DOWN;
					break;
				case 18000:
					state = VK_DOWN;
					break;
				case 22500:
					state = VK_DOWN;
					stateB = VK_LEFT;
					break;
				case 27000:
					state = VK_LEFT;
					break;
				case 31500:
					state = VK_LEFT;
					stateB = VK_UP;
					break;
			}
			switch(m_GamePadStateBefore.rgdwPOV[0])
			{
				case 0:
					prevState = VK_UP;
					break;
				case 4500:
					prevState = VK_UP;
					prevStateB = VK_RIGHT;
					break;
				case 9000:
					prevState = VK_RIGHT;
					break;
				case 13500:
					prevState = VK_RIGHT;
					prevStateB = VK_DOWN;
					break;
				case 18000:
					prevState = VK_DOWN;
					break;
				case 22500:
					prevState = VK_DOWN;
					prevStateB = VK_LEFT;
					break;
				case 27000:
					prevState = VK_LEFT;
					break;
				case 31500:
					prevState = VK_LEFT;
					prevStateB = VK_UP;
					break;
			}

			if ( (state == buttonnum || stateB == buttonnum) && (prevState != buttonnum && prevStateB != buttonnum) )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if(!(m_GamePadStateBefore.rgbButtons[buttonnum] & 0x80) && m_GamePadState.rgbButtons[buttonnum] & 0x80)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	/**
	*	離された瞬間かどうか
	*	@param buttonnum  ボタン番号
	*/
	bool GamePad::PushOffButton(int buttonnum)
	{

		if( buttonnum >= 37 && buttonnum <= 40 && m_GamePadStateBefore.rgdwPOV[0] != 0xffffffff )
		{
			int state  = -1,stateB  = -1,prevState = -1,prevStateB = -1;
			switch(m_GamePadState.rgdwPOV[0])
			{
				case 0:
					state = VK_UP;
					break;
				case 4500:
					state = VK_UP;
					stateB = VK_RIGHT;
					break;
				case 9000:
					state = VK_RIGHT;
					break;
				case 13500:
					state = VK_RIGHT;
					stateB = VK_DOWN;
					break;
				case 18000:
					state = VK_DOWN;
					break;
				case 22500:
					state = VK_DOWN;
					stateB = VK_LEFT;
					break;
				case 27000:
					state = VK_LEFT;
					break;
				case 31500:
					state = VK_LEFT;
					stateB = VK_UP;
					break;
			}
			switch(m_GamePadStateBefore.rgdwPOV[0])
			{
				case 0:
					prevState = VK_UP;
					break;
				case 4500:
					prevState = VK_UP;
					prevStateB = VK_RIGHT;
					break;
				case 9000:
					prevState = VK_RIGHT;
					break;
				case 13500:
					prevState = VK_RIGHT;
					prevStateB = VK_DOWN;
					break;		
				case 18000:
					prevState = VK_DOWN;
					break;
				case 22500:
					prevState = VK_DOWN;
					prevStateB = VK_LEFT;
					break;
				case 27000:
					prevState = VK_LEFT;
					break;
				case 31500:
					prevState = VK_LEFT;
					prevStateB = VK_UP;
					break;
			}

			if ( (state == -1 && stateB == -1) && (prevState == buttonnum || prevStateB == buttonnum) )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{

			if(!(m_GamePadState.rgbButtons[buttonnum] & 0x80) && m_GamePadStateBefore.rgbButtons[buttonnum] & 0x80)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	/**
	*	十字キーの状態を返す
	*/
	D3DXVECTOR3 GamePad::GetAllow()
	{
		return D3DXVECTOR3( static_cast<float>(m_GamePadState.lX),
							static_cast<float>(m_GamePadState.lY),
							static_cast<float>(m_GamePadState.lZ));
	}
}



