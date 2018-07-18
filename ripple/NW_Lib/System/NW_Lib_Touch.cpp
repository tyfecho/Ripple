/**
 * @file    Touch.h
 * @brief   タッチを担当するへッダです
 *
 * @author  Kenta Minamitsu
 * @date    2013/09/18
 * @version 1.0
 */

#include "NW_Lib_Touch.h"

#include "../Utility/NW_Lib_HardwareDefine.h"

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
	/**
	* @brief   コンストラクタ
	* 
	* @see     
	*/
	Touch::Touch()
		:m_touchPos(0.0f,0.0f)
		,m_touchFlag(false)
		,m_prevTouchFlag(false)
	{
	}
	
	/**
	* @brief   デストラクタ
	* 
	* @see     
	*/
	Touch::~Touch()
	{
	}

	/**
		@brief 初期化を行います
	*/
	void Touch::Initialize()
	{
	}

	/**
		@brief パッド入力をアップデートします
	*/
	void Touch::Update()
	{
		if(m_pInput->PushKey(VK_LBUTTON))
		{
			m_touchPos.x = static_cast<float>(m_pInput->GetMousePos().x);
			m_touchPos.y = static_cast<float>(m_pInput->GetMousePos().y);
			
			if( m_touchPos.x < LOWER_SCREEN_OFFSET_WIDTH )
			{
				m_touchPos.x = LOWER_SCREEN_OFFSET_WIDTH;
			}
			if( m_touchPos.x > LOWER_SCREEN_WIDTH + LOWER_SCREEN_OFFSET_WIDTH )
			{
				m_touchPos.x = LOWER_SCREEN_WIDTH + LOWER_SCREEN_OFFSET_WIDTH;
			}
			if( m_touchPos.y < UPPER_SCREEN_HEIGHT )
			{
				m_touchPos.y = UPPER_SCREEN_HEIGHT;
			}
			if( m_touchPos.y > UPPER_SCREEN_HEIGHT + LOWER_SCREEN_HEIGHT )
			{
				m_touchPos.y = UPPER_SCREEN_HEIGHT + LOWER_SCREEN_HEIGHT;
			}
		}
		
	 	 m_prevTouchFlag = m_touchFlag;
	 	 m_touchFlag = m_pInput->PushKey(VK_LBUTTON);
	}

	/**
		@brief メモリを開放します
	*/
	void Touch::Finalize()
	{

	}
	
	///画面にタッチされているか確認
	bool Touch::IsTouch() const
	{
		return m_touchFlag;
	}

	///画面に一回のみタッチされているかどうか判定
	bool Touch::IsTrigger() const
	{
		return m_touchFlag && !m_prevTouchFlag ? true : false;
	}

	///画面にタッチが離されたかどうか
	bool Touch::IsRelease() const
	{
		return !m_touchFlag && m_prevTouchFlag ? true : false;
	}

	///X座標取得 (5 <= x <= 314)
	float Touch::GetTouchX() const
	{
		return m_touchPos.x - LOWER_SCREEN_OFFSET_WIDTH;
	}

	///Y座標取得 (5 <= y <= 234)
	float Touch::GetTouchY() const
	{
		return m_touchPos.y - UPPER_SCREEN_HEIGHT;
	}
}// end namespace System
}// end namespace NW_Lib
