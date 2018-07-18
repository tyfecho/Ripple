/**
*	@file	Constant.h
*
*	@brief	３ＤＳマルチプラットフォーム用共有定義
*
*	@date	2013/08/27
*
*	@author	皆光　健太
*/

#pragma once
#include <d3dx9.h>

//#define _3DS
#define _DirectX

namespace NW_Lib
{
	class Texture;
	class Camera;

	typedef enum DrawingScreen
	{
		UPPER_SCREEN_DRAWING = 0,
		LOWER_SCREEN_DRAWING,
		NUM_SCREEN_DRAWING,
	};
}

//ボタンの配置
enum Button_Key{
	//BUTTON_A         = 3,               /* @brief Aボタン */
	//BUTTON_B         = 2,               /* @brief Bボタン */
	//BUTTON_X         = 4,               /* @brief Xボタン */
	//BUTTON_Y         = 1,               /* @brief Yボタン */
	BUTTON_A         = 1,               /* @brief Aボタン */
	BUTTON_B         = 2,               /* @brief Bボタン */
	BUTTON_X         = 3,               /* @brief Xボタン */
	BUTTON_Y         = 4,               /* @brief Yボタン */

	BUTTON_R         = 6,               /* @brief Rボタン */
	BUTTON_L         = 5,               /* @brief Lボタン */

	BUTTON_UP        = VK_UP,              /* @brief 十字↑ボタン */
	BUTTON_DOWN      = VK_DOWN,            /* @brief 十字↓ボタン */
	BUTTON_RIGHT     = VK_RIGHT,           /* @brief 十字→ボタン */
	BUTTON_LEFT      = VK_LEFT,            /* @brief 十字←ボタン */

	//BUTTON_PAD_UP    =dnwdwadadwadwa /* @brief スライドパッド↑ボタン */
	//BUTTON_PAD_DOWN  =dnwdwadadwadwa /* @brief スライドパッド↓ボタン */
	//BUTTON_PAD_RIGHT =dnwdwadadwadwa /* @brief スライドパッド→ボタン */
	//BUTTON_PAD_LEFT  =dnwdwadadwadwa /* @brief スライドパッド←ボタン */
	
	//BUTTON_START     = 10,           /* @brief STARTボタン */
	//BUTTON_SELECT    = 9			/* @brief SELECTボタン */
	BUTTON_START     = 8,           /* @brief STARTボタン */
	BUTTON_SELECT    = 7			/* @brief SELECTボタン */
};

//テクスチャ共通定義
typedef NW_Lib::Texture TEXTURE;

//カメラ共通定義
typedef NW_Lib::Camera CAMERA;

//ベクトルクラス共通定義
typedef D3DXVECTOR2 VECTOR2;
typedef D3DXVECTOR3 VECTOR3;

//行列クラス共通定義
typedef D3DXMATRIX MATRIX;
typedef D3DXMATRIX* MATRIX33;

static unsigned long Random()
{ 
    static unsigned long x=123456789,y=362436069,z=521288629,w=88675123; 
    unsigned long t; 
    t=(x^(x<<11));x=y;y=z;z=w; return( w=(w^(w>>19))^(t^(t>>8)) ); 
} 

