/**
*	@file	DirectX.cpp
*
*	@brief	DirectX�Ɋւ��鏈��
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#include "NW_Lib_DirectX.h"



namespace NW_Lib
{
	/**
	*	�R���X�g���N�^
	*/
	DirectX::DirectX()
	{
		pD3D = NULL;
		pd3dDevice = NULL;
	}
	/**
	*	����������
	*	@param hwnd  �E�C���h�E�n���h��
	*	@param isFullScreen  true�Ńt���X�N���[���̏��
	*/
	void DirectX::Initialize(HWND hwnd,bool isFullScrean)
	{
		pD3D = Direct3DCreate9( D3D_SDK_VERSION );

		if(isFullScrean){
			// �t���X�N���[��

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
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//Z�o�b�t�@�̃t�H�[�}�b�g�ƃf�o�C�X�i�T�[�t�F�C�X�j�̃t�H�[�}�b�g���������Ȃ�����
			d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		}else{
			// �E�C���h�E���[�h

			ZeroMemory( &d3dpp,sizeof(d3dpp) ); // �\���̂̒������ׂĂO�ŏ���������vz
			d3dpp.Windowed = TRUE; // �E�C���h�E���[�h���t���X�N���[�����[�h���̂ǂ�����
			d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

			// Z�o�b�t�@
			d3dpp.EnableAutoDepthStencil = TRUE;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		}

		pD3D->CreateDevice( D3DADAPTER_DEFAULT, // �}�X�^�[�ƂȂ��Ă������
											D3DDEVTYPE_HAL,     // �n�[�h�E�F�A�𒊏ۉ��������C���[
											hwnd,               // �E�C���h�E
											D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, // ���_�̌v�Z��CPU�ɂ�点��
											&d3dpp,              // ���ʂ̏������ݗp
											&pd3dDevice       // ���ʂ̏������ݗp
											);

	}
	/**
	*	�f�o�C�X��n��
	*	return �f�o�C�X
	*/
	LPDIRECT3DDEVICE9 DirectX::GetD3DDevice()
	{
		return pd3dDevice;
	}

	/**
	*	�p�����[�^�[��n��
	*	return �p�����[�^�[
	*/
	D3DPRESENT_PARAMETERS* DirectX::GetParameters()
	{
		return &d3dpp;
	}

	/**
	*	�f�X�g���N�^
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
