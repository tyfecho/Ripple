/**
 * @file    InputManager.h
 * @brief   インプットを担当するへッダです
 * 
 * @author  Masaya Yoshida
 * @author  Kenta Minamitsu
 * @date    2013/09/18
 * @version 1.0
 */

#include "NW_Lib_InputManager.h"

#include "../Utility/NW_Lib_utility.h"

#include "NW_Lib_Touch.h"

namespace NW_Lib
{
	namespace System
	{
		Touch*			InputManager::m_pTouch			= NULL;

		/**
		 * @brief   初期化処理
		 */
		void InputManager::Initialize()
		{
			//// タッチの生成
			m_pTouch = new Touch();
			m_pTouch->Initialize();
		}
		/**
		 * @brief   更新処理
		 */
		void InputManager::Update()
		{
			//// タッチの更新
			m_pTouch->Update();
		}
		/**
		 * @brief   解放処理
		 */
		void InputManager::Finalize()
		{
			//// タッチの解放
			m_pTouch->Finalize();
			SafeDelete(m_pTouch);
		}


	//////////////////////////  Touch  //////////////////////////
		/**
		 * @brief   タッチした場所を返す処理
		 *
		 * @return タッチした場所を返すVECTOR2
		 */
		VECTOR2 InputManager::GetTouchPosition()
		{
			return VECTOR2(m_pTouch->GetTouchX(),m_pTouch->GetTouchY());
		}

		/**
		 * @brief   タッチし続けているかの判定処理
		 *
		 * @return true:タッチし続けている false:タッチされていない
		 */
		bool InputManager::GetTouchPush()
		{
			return m_pTouch->IsTouch();
		}

		/**
		 * @brief   タッチした瞬間かの判定処理
		 *
		 * @return true:タッチした瞬間 false:タッチされた瞬間ではない
		 */
		bool InputManager::GetTouchTrigger()
		{
			return m_pTouch->IsTrigger();
		}

		/**
		 * @brief   タッチを終えた瞬間かの判定処理
		 *
		 * @return true:タッチを離した false:タッチを離した瞬間ではない
		 */
		bool InputManager::GetTouchRelease()
		{
			return m_pTouch->IsRelease();
		}

	//////////////////////////  Button  //////////////////////////
		/**
		 * @brief   ボタンを押し続けているかの判定処理
		 *
		 * @param  key キーコード
		 *
		 * @return true:ボタンを押し続けている false:ボタンが押されていない
		 */
		bool InputManager::GetButtonPush(Button_Key key)
		{
			return m_pInput->PushButton(key);
		}

		/**
		 * @brief   ボタンを押した瞬間かの判定処理
		 *
		 * @param  key キーコード
		 *
		 * @return true:ボタンが押された瞬間 false:ボタンが押された瞬間でない
		 */
		bool InputManager::GetButtonTrigger(Button_Key key)
		{
			return m_pInput->TrigerButton(key);
		}

		/**
		 * @brief   ボタンを離した瞬間かの判定処理
		 *
		 * @param  key キーコード
		 *
		 * @return true:ボタンを離した瞬間 false:ボタンを離した瞬間でない
		 */
		bool InputManager::GetButtonRelease(Button_Key key)
		{
			return m_pInput->PushOffButton(key);
		}

	//////////////////////////  Mic  //////////////////////////
		/**
		 * @brief   マイクに反応してる音量を取得
		 *
		 * @return マイクの音量
		 */		
		int InputManager::GetMicLoudness()
		{
			return 0;//m_pMic->GetLoudness();
		}
	//////////////////////////  Accel  //////////////////////////
		/**
		 * @brief   加速度センサーの取得
		 *
		 * @return 各XYZ方向の加速度を格納したVECTOR3型
		 */
		VECTOR3* InputManager::GetAccel()
		{
			return &VECTOR3(0.0f,0.0f,0.0f);//m_pGyro->GetAccele();
		}

	//////////////////////////  SlidePad  //////////////////////////
		/**
		 * @brief   スライドパッドの取得
		 *
		 * @return 各XY方向のスティック軸を格納したVECTOR2型
		 */		
		VECTOR2 InputManager::GetSlidePad()
		{
			return VECTOR2(m_pInput->GetAllow().x,m_pInput->GetAllow().y);
		}

	//////////////////////////  Gyro  //////////////////////////
		/**
		 * @brief   ジャイロの姿勢を取得
		 *
		 * @return ジャイロの姿勢を格納した回転行列
		 */		
		MATRIX33 InputManager::GetGyroDirection()
		{
			static D3DXMATRIX mat;
			D3DXMatrixIdentity(&mat);

			return &mat;
		}
		/**
		 * @brief   ジャイロの角速度の積算を取得
		 *
		 * @return ジャイロの角速度の積算
		 */	
		VECTOR3 InputManager::GetGyroAngle()
		{
			return VECTOR3(0.0f,0.0f,0.0f);
		}
		/**
		 * @brief   ジャイロの角速度を取得
		 *
		 * @return ジャイロの角速度
		 */	
		VECTOR3 InputManager::GetGyroSpeed()
		{
			return VECTOR3(0.0f,0.0f,0.0f);
		}
	}
}





