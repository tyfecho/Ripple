/**
*	@file	DirectX.h
*
*	@brief	DirectX�Ɋւ��鏈��
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/


#pragma once

#include <d3dx9.h>

namespace NW_Lib
{
	class DirectX
	{
		private:
			LPDIRECT3D9       pD3D;
			LPDIRECT3DDEVICE9 pd3dDevice;
			D3DPRESENT_PARAMETERS d3dpp; 
		public:
			DirectX();
			~DirectX();
			void Initialize(HWND hwnd,bool isFullScrean); // ������
			LPDIRECT3DDEVICE9 GetD3DDevice(); // �f�o�C�X�̎擾
			D3DPRESENT_PARAMETERS* GetParameters();
	};
}