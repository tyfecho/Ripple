/**
*	@file	DirectX.cpp
*
*	@brief	DirectXに関する処理
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#include "NW_Lib_DirectX.h"



namespace NW_Lib
{
	/**
	*	コンストラクタ
	*/
	DirectX::DirectX()
	{
		pD3D = NULL;
		pd3dDevice = NULL;
	}
	/**
	*	初期化処理
	*	@param hwnd  ウインドウハンドル
	*	@param isFullScreen  trueでフルスクリーンの状態
	*/
	void DirectX::Initialize(HWND hwnd,bool isFullScrean)
	{
		pD3D = Direct3DCreate9( D3D_SDK_VERSION );

		if(isFullScrean){
			// フルスクリーン

			ZeroMemory( &d3dpp, sizeof(d3dpp) );   
			D3DDISPLAYMODE DMode;
			pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT ,&DMode);
			d3dpp.BackBufferFormat = DMode.Format;
			d3dpp.BackBufferWidth = 640;
			d3dpp.BackBufferHeight = 480;
			d3dpp.BackBufferCount = 1;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; 
			d3dpp.Windowed = false;
			d3dpp.EnableAutoDepthStencil = true;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//Zバッファのフォーマットとデバイス（サーフェイス）のフォーマットを混同しないこと
			d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		}else{
			// ウインドウモード

			ZeroMemory( &d3dpp,sizeof(d3dpp) ); // 構造体の中をすべて０で初期化する」z
			d3dpp.Windowed = TRUE; // ウインドウモード化フルスクリーンモードかのどっちか
			d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

			// Zバッファ
			d3dpp.EnableAutoDepthStencil = TRUE;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		}

		pD3D->CreateDevice( D3DADAPTER_DEFAULT, // マスターとなっているもの
											D3DDEVTYPE_HAL,     // ハードウェアを抽象化したレイヤー
											hwnd,               // ウインドウ
											D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, // 頂点の計算をCPUにやらせる
											&d3dpp,              // 結果の書き込み用
											&pd3dDevice       // 結果の書き込み用
											);

	}
	/**
	*	デバイスを渡す
	*	return デバイス
	*/
	LPDIRECT3DDEVICE9 DirectX::GetD3DDevice()
	{
		return pd3dDevice;
	}

	/**
	*	パラメーターを渡す
	*	return パラメーター
	*/
	D3DPRESENT_PARAMETERS* DirectX::GetParameters()
	{
		return &d3dpp;
	}

	/**
	*	デストラクタ
	*/
	DirectX::~DirectX()
	{
		if( pd3dDevice != NULL )
		{
			pd3dDevice->Release();
		}


		if( pD3D != NULL )
		{
			pD3D->Release();
		}
	}
}
