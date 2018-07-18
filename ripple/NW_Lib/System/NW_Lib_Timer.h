/**
*	@file	timer.h
*
*	@brief	タイマークラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#pragma once



#include <Windows.h>



namespace NW_Lib
{

	/*
		タイマークラス
	*/
	class Timer
	{
		private:
			/// CPUのクロック周波数
			LARGE_INTEGER m_frequency;

			/// カウンターを保持するワーク
			LARGE_INTEGER m_oldcounter;

		public:
			/// コンストラクタ
			Timer();
			
			/// 指定された時間が経つまで待機
			void Wait(double waitTime);
	};

}
