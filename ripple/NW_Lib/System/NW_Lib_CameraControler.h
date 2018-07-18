/**
*	@file	CameraControler.h
*
*	@brief	�J�����𑀍삷��N���X
*
*	@date	2013/09/19
*
*	@author	�g�c�@����
*	@author	�F���@����
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
				// ���_�̐ݒ�
				static void SetPosition(DrawingScreen drawingscreen,float x,float y,float z);
				static void SetPosition(DrawingScreen drawingscreen,VECTOR3 pos);
				// �����_�̐ݒ�
				static void SetTarget(DrawingScreen drawingscreen,float x,float y,float z);
				static void SetTarget(DrawingScreen drawingscreen,VECTOR3 pos);
				// ������x�N�g���̐ݒ�
				static void SetUpVec(DrawingScreen drawingscreen,float x,float y,float z);
				static void SetUpVec(DrawingScreen drawingscreen,VECTOR3 pos);
				// ���ˉe�E�����ˉe�̐؂�ւ�
				static void SetMode(DrawingScreen drawingscreen,MODE cameramode);
		};
	}
}

