/**
*	@file	DirectX.h
*
*	@brief	DirectXに関する処理
*
*	@date	2012/08/01
*
*	@author	吉田　将也
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
			void Initialize(HWND hwnd,bool isFullScrean); // 初期化
			LPDIRECT3DDEVICE9 GetD3DDevice(); // デバイスの取得
			D3DPRESENT_PARAMETERS* GetParameters();
	};
}