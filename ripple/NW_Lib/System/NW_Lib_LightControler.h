/**
*	@file	LightControler.h
*
*	@brief	ライトを操作するクラス
*
*	@date	2013/09/19
*
*	@author	吉田　将也
*	@author	皆光　健太
*/

#include "NW_Lib_LightManager.h"

#pragma once

namespace NW_Lib
{
	namespace System
	{
		class LightControler
		{
			public:
				// ライトの位置
				static void SetPosition(DrawingScreen drawingscreen,float x,float y,float z);
				static void SetPosition(DrawingScreen drawingscreen,VECTOR3 pos);
				// ライトの向き
				static void SetDirection(DrawingScreen drawingscreen,float x,float y,float z);
				static void SetDirection(DrawingScreen drawingscreen,VECTOR3 vec);
				// ライトの色
				static void SetColor(DrawingScreen drawingscreen,float _r, float _g, float _b);
		};
	}
}