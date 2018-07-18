/**
*	@file	timer.cpp
*
*	@brief	タイマークラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#include "NW_Lib_Timer.h"


namespace NW_Lib
{
	/**
	*	コンストラクタ
	*/
	Timer::Timer()
	{
		QueryPerformanceFrequency(&m_frequency); // CPUの速さをとってくる
		QueryPerformanceCounter(&m_oldcounter);  // どれだけ時間がたったか
	}



	/**
	*	指定された時間が経つまで待機
	*	@param waitTime 待ち時間
	*/
	void Timer::Wait(double waitTime)
	{
		if (waitTime > 1.0) waitTime = 1.0;
		if (waitTime < 0.0) waitTime = 0.0;


		LARGE_INTEGER s;
		LARGE_INTEGER w; // １回で待つべき時間を格納

		w.QuadPart = (LONGLONG)(m_frequency.QuadPart * waitTime); // どれだけ待つべきなのか
		QueryPerformanceCounter(&s);
		s.QuadPart -= m_oldcounter.QuadPart; // 前回からどれだけ経ったか

		// 前フレームからの経過時間　＜　１回で待つべき時間
		while (s.QuadPart < w.QuadPart)
		{
			Sleep(0);                            // 指定した時間処理をとめる
			QueryPerformanceCounter(&s);         // もう１度今の時間を取得
			s.QuadPart -= m_oldcounter.QuadPart; // 前回からどれだけ経ったか
		}

		QueryPerformanceCounter(&m_oldcounter);
	}
}
