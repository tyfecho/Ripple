/***
*	@file	light.cpp
*
*	@brief	基本的なライトの設定用クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#include "NW_Lib_light.h"


namespace NW_Lib
{
	/**
	*	コンストラクタ
	*/
	Light::Light()
	{
		// 初期化したもの
		D3DLIGHT9 DirectionalLight = {
			D3DLIGHT_DIRECTIONAL,
			1,1,1,0,
			1,1,1,0,
			0,0,0,0,
			0,0,0,
			0,-1,0,
		};
		D3DMATERIAL9 Material = {
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
		};

		// 代入
		m_DirectionalLight = DirectionalLight;
		m_Material = Material;

	}
	/**
	*	デストラクタ
	*/
	Light::~Light()
	{
	}

	/**
	*	ライトの表示
	*/
	void Light::ShowLight()
	{
		m_DirectionalLight.Range       = 1000.0f;
		// ライトの設定
		m_pD3DDevice->SetLight( 0, &m_DirectionalLight );
		m_pD3DDevice->LightEnable( 0, TRUE );
		m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

		// Finally, turn on some ambient light.
		m_pD3DDevice->SetRenderState( D3DRS_AMBIENT, 0x00ffffff );

		// マテリアルのセット
		m_pD3DDevice->SetMaterial(&m_Material);
	}

	
	/**
	*	カラーの変更
	*/
	void Light::SetColor(float r,float g,float b)
	{
		m_DirectionalLight.Diffuse.r = r;
		m_DirectionalLight.Diffuse.g = g;
		m_DirectionalLight.Diffuse.b = b;
	}
	
	/**
	*	ライトの方向の変更
	*/
	void Light::SetDirection(float x,float y,float z)
	{
		m_DirectionalLight.Direction.x = x;
		m_DirectionalLight.Direction.y = y;
		m_DirectionalLight.Direction.z = z;
	}
	
	/**
	*	ライトのポジションの変更
	*/
	void Light::SetPos(float x,float y,float z)
	{
		m_DirectionalLight.Position.x = x;
		m_DirectionalLight.Position.y = y;
		m_DirectionalLight.Position.z = z;
	}

}





