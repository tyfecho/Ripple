/**
*	@file	light.h
*
*	@brief	��{�I�ȃ��C�g�̐ݒ�p�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#pragma once

#include <d3dx9.h>
#include "../System/NW_Lib_Common.h"


namespace NW_Lib
{
	class Light:public Common
	{
		private:
			D3DLIGHT9 m_DirectionalLight;
			D3DMATERIAL9 m_Material;
		public:
			Light();
			~Light();
			void ShowLight();

			void SetColor(float r,float g,float b);
			void SetDirection(float x,float y,float z);
			void SetPos(float x,float y,float z);
	};
}



