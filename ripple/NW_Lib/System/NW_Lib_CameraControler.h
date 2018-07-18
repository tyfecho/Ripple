/**
*	@file	CameraControler.h
*
*	@brief	カメラを操作するクラス
*
*	@date	2013/09/19
*
*	@author	吉田　将也
*	@author	皆光　健太
*/

#include "NW_Lib_CameraManager.h"

#pragma once
namespace NW_Lib
{
	namespace System
	{
		class CameraControler
		{
			public:
				// 視点の設定
				static void SetPosition(DrawingScreen drawingscreen,float x,float y,float z);
				static void SetPosition(DrawingScreen drawingscreen,VECTOR3 pos);
				// 注視点の設定
				static void SetTarget(DrawingScreen drawingscreen,float x,float y,float z);
				static void SetTarget(DrawingScreen drawingscreen,VECTOR3 pos);
				// 上方向ベクトルの設定
				static void SetUpVec(DrawingScreen drawingscreen,float x,float y,float z);
				static void SetUpVec(DrawingScreen drawingscreen,VECTOR3 pos);
				// 正射影・透視射影の切り替え
				static void SetMode(DrawingScreen drawingscreen,MODE cameramode);
		};
	}
}

