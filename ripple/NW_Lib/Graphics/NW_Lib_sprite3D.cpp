/**
*	@file	sprite3D.cpp
*
*	@brief	3D�X�v���C�g�p�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/


#include "NW_Lib_sprite3D.h"
#include "NW_Lib_Texture.h"


namespace NW_Lib
{
	/**
	*	�R���X�g���N�^
	*	@param pD3DDevice  �f�o�C�X
	*/
	Sprite3D::Sprite3D(LPDIRECT3DDEVICE9 pD3DDevice)
	:m_width(1.0f)      // ��
	,m_height(1.0f)    // ����
	,m_texture(NULL)     // �e�N�X�`��
	,m_uLeftUp(0.0f)     // ����t���W
	,m_vLeftUp(0.0f)     // ����u���W
	,m_uRightDown(0.0f)  // �E���t���W
	,m_vRightDown(0.0f)  // �E���u���W
	,m_pD3DDevice(pD3DDevice)
	,m_Alpha(1.0f)
	,m_Color(0x00ffffff)
	,m_Rot(0.0f,0.0f,0.0f)
	{
	}

	/**
	*	�f�X�g���N�^
	*/
	Sprite3D::~Sprite3D()
	{
	}

	/**
	*	�e�N�X�`���̐ݒ�
	*	@param texture  �e�N�X�`�����
	*/
	void Sprite3D::SetTexture(const Texture *texture)
	{
		m_texture = texture;

		m_uLeftUp  = 0.0f; // ����ݒ�
		m_vLeftUp  = 0.0f; // ����ݒ�
		m_uRightDown = 1.0f; // �E���ݒ�
		m_vRightDown = 1.0f; // �E���ݒ�
	}

	/**
	*  �e�N�X�`���̐ݒ� �i�w��o�[�W�����j
	*	@param texture        �e�N�X�`�����
	*	@param textureX       �e�N�X�`����X�ʒu
	*	@param textureY       �e�N�X�`����Y�ʒu
	*	@param textureWidth   �e�N�X�`���̕�
	*	@param textureHeight  �e�N�X�`���̍���
	*/
	void Sprite3D::SetTexture(const Texture *texture, float textureX, float textureY, float textureWidth, float textureHeight)
	{
		m_texture = texture;

		m_uLeftUp  = static_cast<float>(textureX) / m_texture->GetWidth(); // ����ݒ�
		m_vLeftUp  = static_cast<float>(textureY) / m_texture->GetHeight(); // ����ݒ�
		m_uRightDown = static_cast<float>(textureX + textureWidth) / m_texture->GetWidth(); // �E���ݒ�
		m_vRightDown = static_cast<float>(textureY + textureHeight) / m_texture->GetHeight(); // �E���ݒ�
	}
	/**
	*	�e�N�X�`���̃T�C�Y�ύX
	*	@param width  ��
	*	@param height ����
	*/
	void Sprite3D::ChangeSize(float width, float height)
	{
		m_width = width;
		m_height = height;
	}

	/**
	*	�`��
	*	@param x      X�ʒu
	*	@param y      Y�ʒu
	*	@param z      Z�ʒu
	*	@param angle  �p�x
	*	@param alfa   �����x
	*/
	void Sprite3D::Draw( float x, float y,float z,float angle,float alfa)
	{

		// �A���t�@�̌v�Z
		DWORD alp = static_cast<DWORD>(0xff * alfa) << ( 8 * 3 );

		VERTEX_TEXTURE3D vertices[4] = 
		{
			{D3DXVECTOR3(- m_width/2,+ m_height/2,0.0f), m_Color + alp, m_uLeftUp   , m_vLeftUp   },
			{D3DXVECTOR3(+ m_width/2,+ m_height/2,0.0f), m_Color + alp, m_uRightDown, m_vLeftUp   },
			{D3DXVECTOR3(- m_width/2,- m_height/2,0.0f), m_Color + alp, m_uLeftUp   , m_vRightDown},
			{D3DXVECTOR3(+ m_width/2,- m_height/2,0.0f), m_Color + alp, m_uRightDown, m_vRightDown}
		};



		// ���S�ʒu�i�w�j�@���@ ���̔���  + �w�ʒu
		float coreX = m_width + x;
		// ���S�ʒu�i�x�j�@���@�����̔��� + �x�ʒu
		float coreY = m_height + y;
		float rad = (angle * D3DX_PI / 180.0f);

		// �y�o�b�t�@���g�p
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

		// �A���t�@�u�����h(ON)
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// �A���t�@�l���O�̏ꍇ�y�o�b�t�@�ɏ������܂Ȃ�
		m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 1);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

		// �e�N�X�`���X�e�[�W�̐ݒ�
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		// �e�N�X�`���̐ݒ�
		if(m_texture != NULL){
			m_pD3DDevice->SetTexture(0,m_texture->GetTexture());
		}else{
			m_pD3DDevice->SetTexture(0,NULL);
		}

		// ���_�t�H�[�}�b�g�̐ݒ�
		m_pD3DDevice->SetFVF(D3DFVF_TEXTURE3D);


		D3DXMATRIX m;
		D3DXMATRIX moveRot,moveMat;
		D3DXMatrixIdentity(&m);
		D3DXMatrixRotationYawPitchRoll(&moveRot,m_Rot.y * D3DX_PI/180.0f,m_Rot.x * D3DX_PI/180.0f,m_Rot.z * D3DX_PI/180.0f);
		D3DXMatrixTranslation(&moveMat,x,y,z);
		m *= moveRot;
		m *= moveMat;
		m_pD3DDevice->SetTransform(D3DTS_WORLD,&m);

		// �`��
		m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VERTEX_TEXTURE3D));

		// �e�N�X�`���̏�����
		m_pD3DDevice->SetTexture(0,NULL);

		// Z�o�b�t�@���I���ɂ���
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
		// �y�o�b�t�@�Ɣ�r����
		m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		// �y�o�b�t�@�ɏ�������
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	}
	/**
	*	�`��̂܂Ƃ�
	*	@param texture       �e�N�X�`�����
	*	@param x             X�ʒu
	*	@param y             Y�ʒu
	*	@param z             Z�ʒu
	*	@param texturex      �e�N�X�`����X
	*	@param texturey      �e�N�X�`����Y
	*	@param textureWidth  ��
	*	@param textureHeight ����
	*	@param angle         �p�x
	*	@param alfa          �����x
	*/
	void Sprite3D::DrawAll(const NW_Lib::Texture *texture,float x, float y,float z, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,float angle,float alpha)
	{
		ChangeSize(width,height);
		SetTexture(texture,textureX,textureY,textureWidth,textureHeight);
		Draw(x,y,z,angle,alpha); // �w�i
	}
}














