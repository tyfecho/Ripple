/**
*	@file	Billboard.cpp
*
*	@brief	�r���{�[�h�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#include "NW_Lib_Billboard.h"
#include "NW_Lib_Texture.h"
#include "NW_Lib_sprite3D.h"
#include "../Utility/NW_Lib_HardwareDefine.h"


namespace NW_Lib
{
	 CAMERA* Billboard::ms_camera;

	/**
	*           �R���X�g���N�^
	*/
	Billboard::Billboard(float width, float height)
	:m_width(width)      // ��
	,m_height(height)    // ����
	,m_texture(NULL)     // �e�N�X�`��
	,m_uLeftUp(0.0f)     // ����t���W
	,m_vLeftUp(0.0f)     // ����u���W
	,m_uRightDown(0.0f)  // �E���t���W
	,m_vRightDown(0.0f)  // �E���u���W
	{
	}

	/**
	*            �f�X�g���N�^
	*/
	Billboard::~Billboard()
	{
	}

	/**
	*  �e�N�X�`���̐ݒ�    �i�S�̃o�[�W�����j
	*	@param texture  �e�N�X�`�����
	*/
	void Billboard::SetTexture(const Texture *texture)
	{
		m_texture = texture;

		m_uLeftUp  = 0.0f; // ����ݒ�
		m_vLeftUp  = 0.0f; // ����ݒ�
		m_uRightDown = 1.0f; // �E���ݒ�
		m_vRightDown = 1.0f; // �E���ݒ�
	}

	/**
	*  �e�N�X�`���̐ݒ�    �i�w��o�[�W�����j
	*/
	void Billboard::SetTexture(const Texture *texture, float textureX, float textureY, float textureWidth, float textureHeight)
	{
		m_texture = texture;

		m_uLeftUp  = static_cast<float>(textureX) / m_texture->GetWidth(); // ����ݒ�
		m_vLeftUp  = static_cast<float>(textureY) / m_texture->GetHeight(); // ����ݒ�
		m_uRightDown = static_cast<float>(textureX + textureWidth) / m_texture->GetWidth(); // �E���ݒ�
		m_vRightDown = static_cast<float>(textureY + textureHeight) / m_texture->GetHeight(); // �E���ݒ�
	}
	/**
	*           �T�C�Y�̕ύX
	*/
	void Billboard::ChangeSize(float width, float height)
	{
		m_width = width;
		m_height = height;
	}

	/**
	*              �`    ��
	*/
	void Billboard::Draw( float x, float y,float z,float angle)
	{
		/// ���_�f�[�^
		VERTEX_TEXTURE3D vertices[4];
		D3DXMATRIX matWorld, matRot, matTrans;

		// ���[���h�s��̐ݒ�
		matRot = GetBillboardMatrix();
		D3DXMatrixTranslation(&matTrans, x, y, z);
		matWorld = matRot * matTrans;
		// ��]�p�s��
		D3DXMATRIX matWorld2, matRot2;

		D3DXMatrixIdentity(&matWorld2);
		D3DXMatrixRotationZ( &matRot2 , angle * (D3DX_PI/180.0f));
		matWorld2 *= matRot2;
		matWorld2 *= matWorld;

		m_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld2);

		vertices[0].position = D3DXVECTOR3(-m_width, m_height, 0.0f);
		vertices[0].diffuse  = 0xffffffff;
		vertices[0].u        = m_uLeftUp;
		vertices[0].v        = m_vLeftUp;

		vertices[1].position = D3DXVECTOR3(m_width, m_height, 0.0f);
		vertices[1].diffuse  = 0xffffffff;
		vertices[1].u        = m_uRightDown;
		vertices[1].v        = m_vLeftUp;

		vertices[2].position = D3DXVECTOR3(-m_width, -m_height, 0.0f);
		vertices[2].diffuse  = 0xffffffff;
		vertices[2].u        = m_uLeftUp;
		vertices[2].v        = m_vRightDown;

		vertices[3].position = D3DXVECTOR3(m_width, -m_height, 0.0f);
		vertices[3].diffuse  = 0xffffffff;
		vertices[3].u        = m_uRightDown;
		vertices[3].v        = m_vRightDown;


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
		if (m_texture != NULL)
		{
			m_pD3DDevice->SetTexture(0, m_texture->GetTexture());
		}
		else
		{
			m_pD3DDevice->SetTexture(0, NULL);
		}

		// ���_�t�H�[�}�b�g�̐ݒ�
		m_pD3DDevice->SetFVF(D3DFVF_TEXTURE3D);

		// �v���~�e�B�u�̕`��
		m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VERTEX_TEXTURE3D));
	}
	/**
	*        �r���{�[�h�s���Ԃ�
	*/
	D3DXMATRIX Billboard::GetBillboardMatrix()
	{
		D3DXMATRIX matView;
		D3DXMATRIX matBillboard;

		m_pD3DDevice->GetTransform(D3DTS_VIEW,&matView);

		D3DXMatrixIdentity(&matBillboard);

		matBillboard._11 = matView._11;
		matBillboard._12 = matView._21;
		matBillboard._13 = matView._31;
		matBillboard._14 = 0.0f;

		matBillboard._21 = matView._12;
		matBillboard._22 = matView._22;
		matBillboard._23 = matView._32;
		matBillboard._24 = 0.0f;

		matBillboard._31 = matView._13;
		matBillboard._32 = matView._23;
		matBillboard._33 = matView._33;
		matBillboard._34 = 0.0f;

		matBillboard._41 = 0.0f;
		matBillboard._42 = 0.0f;
		matBillboard._43 = 0.0f;
		matBillboard._44 = 1.0f;

		return matBillboard;
	}
	/**
	*         �`��֌W�̂܂Ƃ�
	*/
	void Billboard::DrawAll(const NW_Lib::Texture *texture,float x, float y,float z/**, float width, float height*/, float textureX, float textureY, float textureWidth, float textureHeight,float angle)
	{
		//ChangeSize(width,height);
		SetTexture(texture,textureX,textureY,textureWidth,textureHeight);
		Draw(x,y,z,angle); // �w�i
	}

	void Billboard::DrawBillboard(TEXTURE* pTex, float dx, float dy,float dz , float dw, float dh,
						float tx, float ty, float tw, float th, float rot, float alpha)
	{
		float x_Left,x_Right,y_Up,y_Down;

		if( DrawingScreenID == NW_Lib::UPPER_SCREEN_DRAWING )
		{
			// �����_�ɍ��킹��
			x_Left  = dx;
			x_Right = dx + dw;
			y_Up    = dy;
			y_Down  = dy + dh;
		}
		else if( DrawingScreenID == NW_Lib::LOWER_SCREEN_DRAWING )
		{
			x_Left  = dx + LOWER_SCREEN_OFFSET_WIDTH;
			x_Right = dx + LOWER_SCREEN_OFFSET_WIDTH + dw;
			y_Up    = dy + LOWER_SCREEN_OFFSET_HEIGHT;
			y_Down  = dy + LOWER_SCREEN_OFFSET_HEIGHT + dh;
		}

		/// ���_�f�[�^
		VERTEX_TEXTURE3D vertices[4];
		D3DXMATRIX matWorld, matRot, matTrans;

		// ���[���h�s��̐ݒ�
		{
			D3DXMATRIX matView;
			D3DXMATRIX matBillboard;

			m_pD3DDevice->GetTransform(D3DTS_VIEW,&matView);

			D3DXMatrixIdentity(&matRot);

			matBillboard._11 = matView._11;
			matBillboard._12 = matView._21;
			matBillboard._13 = matView._31;
			matBillboard._14 = 0.0f;

			matBillboard._21 = matView._12;
			matBillboard._22 = matView._22;
			matBillboard._23 = matView._32;
			matBillboard._24 = 0.0f;

			matBillboard._31 = matView._13;
			matBillboard._32 = matView._23;
			matBillboard._33 = matView._33;
			matBillboard._34 = 0.0f;

			matBillboard._41 = 0.0f;
			matBillboard._42 = 0.0f;
			matBillboard._43 = 0.0f;
			matBillboard._44 = 1.0f;

			matRot = matBillboard;
		}


		D3DXMatrixTranslation(&matTrans, dx, dy, dz);
		matWorld = matRot * matTrans;
		// ��]�p�s��
		D3DXMATRIX matWorld2, matRot2;

		D3DXMatrixIdentity(&matWorld2);
		D3DXMatrixRotationZ( &matRot2 , rot * (D3DX_PI/180.0f));
		matWorld2 *= matRot2;
		matWorld2 *= matWorld;

		float uLeftUp	   = static_cast<float>(tx) / pTex->GetWidth(); // ����ݒ�
		float vLeftUp	   = static_cast<float>(ty) / pTex->GetHeight(); // ����ݒ�
		float uRightDown = static_cast<float>(tx + tw) / pTex->GetWidth(); // �E���ݒ�
		float vRightDown = static_cast<float>(ty + th) / pTex->GetHeight(); // �E���ݒ�

		m_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld2);

		vertices[0].position = D3DXVECTOR3(-dw / 2.0f, dh / 2.0f, 0.0f);
		vertices[0].diffuse  = 0xffffffff;
		vertices[0].u        = uLeftUp;
		vertices[0].v        = vLeftUp;

		vertices[1].position = D3DXVECTOR3(dw / 2.0f, dh / 2.0f, 0.0f);
		vertices[1].diffuse  = 0xffffffff;
		vertices[1].u        = uRightDown;
		vertices[1].v        = vLeftUp;

		vertices[2].position = D3DXVECTOR3(-dw / 2.0f, -dh / 2.0f, 0.0f);
		vertices[2].diffuse  = 0xffffffff;
		vertices[2].u        = uLeftUp;
		vertices[2].v        = vRightDown;

		vertices[3].position = D3DXVECTOR3(dw / 2.0f, -dh / 2.0f, 0.0f);
		vertices[3].diffuse  = 0xffffffff;
		vertices[3].u        = uRightDown;
		vertices[3].v        = vRightDown;


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
		if (pTex != NULL)
		{
			m_pD3DDevice->SetTexture(0, pTex->GetTexture());
		}
		else
		{
			m_pD3DDevice->SetTexture(0, NULL);
		}

		// ���_�t�H�[�}�b�g�̐ݒ�
		m_pD3DDevice->SetFVF(D3DFVF_TEXTURE3D);

		// �v���~�e�B�u�̕`��
		m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VERTEX_TEXTURE3D));
		m_pD3DDevice->SetTexture(0,NULL);

		// Z�o�b�t�@���I���ɂ���
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
		// �y�o�b�t�@�Ɣ�r����
		m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		// �y�o�b�t�@�ɏ�������
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

