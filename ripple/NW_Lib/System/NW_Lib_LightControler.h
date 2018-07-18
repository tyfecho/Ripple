/**
*	@file	LightControler.h
*
*	@brief	���C�g�𑀍삷��N���X
*
*	@date	2013/09/19
*
*	@author	�g�c�@����
*	@author	�F���@����
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
				// ���C�g�̈ʒu
				static void SetPosition(DrawingScreen drawingscreen,float x,float y,float z);
				static void SetPosition(DrawingScreen drawingscreen,VECTOR3 pos);
				// ���C�g�̌���
				static void SetDirection(DrawingScreen drawingscreen,float x,float y,float z);
				static void SetDirection(DrawingScreen drawingscreen,VECTOR3 vec);
				// ���C�g�̐F
				static void SetColor(DrawingScreen drawingscreen,float _r, float _g, float _b);
		};
	}
}