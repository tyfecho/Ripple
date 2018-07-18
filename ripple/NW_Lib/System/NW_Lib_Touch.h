/**
 * @file    Touch.h
 * @brief   タッチを担当するへッダです
 *
 * @author  Kenta Minamitsu
 * @date    2013/09/18
 * @version 1.0
 */

#pragma once

#include "../System/NW_Lib_Common.h"

/**
 * @namespace NW_Lib
 * @brief ライブラリ名前空間
 */
namespace NW_Lib
{
/**
 * @namespace NW_Lib::System
 * @brief システム名前空間
 */
namespace System
{
	class Touch : public Common
	{
	private:
		
		VECTOR2 m_touchPos;
		bool m_prevTouchFlag;
		bool m_touchFlag;
		
	public:
		/**
		 * @brief   コンストラクタ
		 * 
		 * @see     
		 */
		Touch();

		/**
		 * @brief   デストラクタ
		 * 
		 * @see     
		 */
		~Touch();

		/**
			@brief 初期化を行います
		*/
		void Initialize();

		/**
			@brief タッチ入力をアップデートします
		*/
		void Update();

		/**
			@brief メモリを開放します
		*/
		void Finalize();
		
		/**
		*	@brief タッチされているかどうか
		*/
		bool IsTouch() const;
		
		/**
		*	@brief タッチされているかどうか
		*/
		bool IsTrigger() const;

		/**
		*	@brief タッチされているかどうか
		*/
		bool IsRelease() const;

		/**
		*	@brief X座標取得
		*/
		float GetTouchX() const;

		/**
		*	@brief Y座標取得
		*/
		float GetTouchY() const;
	};
}// end namespace System
}// end namespace NW_Lib
