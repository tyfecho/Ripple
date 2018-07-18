/***
*	@file	light.cpp
*
*	@brief	��{�I�ȃ��C�g�̐ݒ�p�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#include "NW_Lib_light.h"


namespace NW_Lib
{
	/**
	*	�R���X�g���N�^
	*/
	Light::Light()
	{
		// ��������������
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

		// ���
		m_DirectionalLight = DirectionalLight;
		m_Material = Material;

	}
	/**
	*	�f�X�g���N�^
	*/
	Light::~Light()
	{
	}

	/**
	*	���C�g�̕\��
	*/
	void Light::ShowLight()
	{
		m_DirectionalLight.Range       = 1000.0f;
		// ���C�g�̐ݒ�
		m_pD3DDevice->SetLight( 0, &m_DirectionalLight );
		m_pD3DDevice->LightEnable( 0, TRUE );
		m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

		// Finally, turn on some ambient light.
		m_pD3DDevice->SetRenderState( D3DRS_AMBIENT, 0x00ffffff );

		// �}�e���A���̃Z�b�g
		m_pD3DDevice->SetMaterial(&m_Material);
	}

	
	/**
	*	�J���[�̕ύX
	*/
	void Light::SetColor(float r,float g,float b)
	{
		m_DirectionalLight.Diffuse.r = r;
		m_DirectionalLight.Diffuse.g = g;
		m_DirectionalLight.Diffuse.b = b;
	}
	
	/**
	*	���C�g�̕����̕ύX
	*/
	void Light::SetDirection(float x,float y,float z)
	{
		m_DirectionalLight.Direction.x = x;
		m_DirectionalLight.Direction.y = y;
		m_DirectionalLight.Direction.z = z;
	}
	
	/**
	*	���C�g�̃|�W�V�����̕ύX
	*/
	void Light::SetPos(float x,float y,float z)
	{
		m_DirectionalLight.Position.x = x;
		m_DirectionalLight.Position.y = y;
		m_DirectionalLight.Position.z = z;
	}

}





