/***
*	@file	DirectXInputManager.cpp
*
*	@brief	入力処理用クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
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
	*	コンストラクタ
	*	@param ウインドウ情報
	*/
	DirectXInputManager::DirectXInputManager(Window* pWindow)
	:m_pWindow(pWindow)
	,m_pGamePad(NULL)
	,m_NoGamePad(false)
	{
		ZeroMemory(keyState,sizeof(keyState));

		m_pGamePad = new GamePad();
		if(FAILED(m_pGamePad->Initialize())){
			// ゲームパッドで何か異常があったので、ゲームパッドを使わない
			m_NoGamePad = true;
		}
	}
	/**
	*	デストラクタ
	*/
	DirectXInputManager::~DirectXInputManager()
	{
		SafeDelete(m_pGamePad);
	}
	/**
	*	更新処理（毎フレーム）
	*/
	void DirectXInputManager::Update()
	{
		// キーボード
		memcpy(keyState_mae,keyState,sizeof(keyState_mae));
		GetKeyboardState(keyState);

		// マウス
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

		// ゲームパッド
		if(!m_NoGamePad){
			m_pGamePad->Update();
		}
	}
	/**
	*	何かキーが押されたときの処理
	*	@param keyCode  キーコード
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
	*	何かキーが押された瞬間の処理
	*	@param keyCode  キーコード
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
	*	何かキーが離された瞬間の処理
	*	@param keyCode  キーコード
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
	*	引数で指定されたゲームパッドのボタンが押されっぱなしかどうか
	*	@param button  ボタン番号
	*/
	bool DirectXInputManager::PushButton(int button)
	{
		if(m_NoGamePad){
			// ゲームパッドがないときや使えないとき
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
	*	引数で指定されたゲームパッドのボタンが押された瞬間かどうか
	*	@param button  ボタン番号	
	*/
	bool DirectXInputManager::TrigerButton(int button)
	{
		if(m_NoGamePad){
			// ゲームパッドがないときや使えないとき
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
	*	引数で指定されたゲームパッドのボタンが離された瞬間かどうか
	*	@param button  ボタン番号	
	*/
	bool DirectXInputManager::PushOffButton(int button)
	{
		if(m_NoGamePad){
			// ゲームパッドがないときや使えないとき
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
	*	十字キーの状態を返す
	*/
	D3DXVECTOR3 DirectXInputManager::GetAllow()
	{
		if(m_NoGamePad){
			// ゲームパッドがないときや使えないとき
			return D3DXVECTOR3(-8.0f,-8.0f,0.0f);
		}

		return m_pGamePad->GetAllow();
	}


	/**
	*	強制的にキーを押させる処理
	*	@param keyCode  キーコード
	*/
	void DirectXInputManager::OnKey(BYTE keyCode)
	{
		if(!PushKey(keyCode)){
			keyState[keyCode] += 0x80;
		}
	}

	/**
	*	強制的にキーを押させない処理
	*	@param keyCode  キーコード
	*/
	void DirectXInputManager::OffKey(BYTE keyCode)
	{
		keyState[keyCode] = 0x00;
	}

	/**
	*	マウスのポジションの制限
	*	@param left    左制限
	*	@param top     上制限
	*	@param right   右制限
	*	@param bottom  下制限
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
	*	マウスのポジションの制限を解除する
	*/
	void DirectXInputManager::MouseClipingOff()
	{
		ClipCursor( NULL );
	}

	/**
	*	マウスの移動ベクトルの取得
	*/
	D3DXVECTOR2 DirectXInputManager::GetMouseVec()
	{
		return D3DXVECTOR2(static_cast<float>(m_mousePos[0].x - m_mousePos[1].x), static_cast<float>(m_mousePos[0].y - m_mousePos[1].y));
	}
}

