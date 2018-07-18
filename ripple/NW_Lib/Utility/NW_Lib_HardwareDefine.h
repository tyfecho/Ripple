/**
*	@file	HardwareDefine.h
*
*	@brief	ハードウェアのシステム定義
*
*	@date	2013/08/20
*
*	@author	皆光 健太
*/

#pragma once
	
///  スクリーンサイズ 上画面
#define UPPER_SCREEN_WIDTH	400
#define UPPER_SCREEN_HEIGHT	240
///  スクリーンサイズ 下画面
#define LOWER_SCREEN_WIDTH	320
#define LOWER_SCREEN_HEIGHT 240
#define LOWER_SCREEN_OFFSET_WIDTH ((UPPER_SCREEN_WIDTH - LOWER_SCREEN_WIDTH) / 2.0f)
#define LOWER_SCREEN_OFFSET_HEIGHT (UPPER_SCREEN_HEIGHT)

// 画面番号
enum SCREEN_NUMBER
{
	SCREEN_UNKNOWN = -1, // 画面無し
	SCREEN_UPPER = 0   , // 上画面
	SCREEN_LOWER       , // 下画面
	
	SCREEN_NUM
};



