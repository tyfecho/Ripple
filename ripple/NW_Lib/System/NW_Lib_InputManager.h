/**
 * @file    InputManager.h
 * @brief   インプットを担当するへッダです
 * 
 * @author  Masaya Yoshida
 * @author  Kenta Minamitsu
 * @date    2013/09/18
 * @version 1.0
 */

#include "../Utility/NW_Lib_Constant.h"
#include "NW_Lib_common.h"

#pragma once

namespace NW_Lib
{
	namespace System
	{
		class Touch;
		class ButtonInput;
		class Mic;
		class GyroInput;

		class InputManager : public Common
		{
			private:
				/// タッチ関連
				static Touch* m_pTouch;
				/// ボタン関連
				//static ButtonInput* m_pButton;
				/// マイク関連
				//static Mic* m_pMic;
				/// ジャイロ・加速度センサー関連
				//static GyroInput* m_pGyro;
			public:
				/// 初期化
				static void Initialize();
				/// 更新
				static void Update();
				/// 解放
				static void Finalize();


				/// Touch
				static VECTOR2 GetTouchPosition();
				static bool GetTouchPush();
				static bool GetTouchTrigger();
				static bool GetTouchRelease();

				/// Button
				static bool GetButtonPush(Button_Key key);
				static bool GetButtonTrigger(Button_Key key);
				static bool GetButtonRelease(Button_Key key);

				/// Mic
				static int GetMicLoudness();

				/// Accel
				static VECTOR3* GetAccel();

				/// SlidePad
				static VECTOR2 GetSlidePad();

				/// Gyro
				static MATRIX33 GetGyroDirection();
				static VECTOR3 GetGyroAngle();
				static VECTOR3 GetGyroSpeed();
				/**
				 * @brief   ジャイロセンサーの遊びの設定.
				 * 
				 * @param   play 遊び(0 ～ SENSITIVITY).
				 * 
				 * @param   sensitivity 検出感度(0 ～ SENSITIVITY)
				 *                      0 ... 反応なし
				 *                      SENSITIVITY ... デバイスから取得した値を返す.
				 */
				static void SetGyroSensitivity(signed short int play,signed short int sensivity);
		};
	}
}
