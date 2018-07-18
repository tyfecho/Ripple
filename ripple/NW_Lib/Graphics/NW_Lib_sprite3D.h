/**
*	@file	sprite3D.h
*
*	@brief	3D�X�v���C�g�p�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/


#pragma once

#include <d3dx9.h>

namespace NW_Lib
{

	class Texture;

	// ���_�t�H�[�}�b�g�\����
	struct VERTEX_TEXTURE3D
	{
		D3DXVECTOR3 position; // ���_���W
		DWORD diffuse; // ���_�J���[
		float u;       // �e�N�X�`���t���W
		float v;       // �e�N�X�`���u���W
	};

	// �t�H�[�}�b�g�`��
	static const DWORD D3DFVF_TEXTURE3D = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	class Sprite3D
	{
		private:
			float m_width;  // ��
			float m_height; // ����
			const Texture* m_texture; // �e�N�X�`��
			float m_uLeftUp;    // ����t���W
			float m_vLeftUp;    // ����u���W
			float m_uRightDown; // �E���t���W
			float m_vRightDown; // �E���u���W
			float m_Alpha;      // �A���t�@�l
			DWORD m_Color;

			D3DXVECTOR3 m_Rot;  // ��]�p�i�f�B�O���[�j

			LPDIRECT3DDEVICE9 m_pD3DDevice;
		public:
			Sprite3D(LPDIRECT3DDEVICE9 pD3DDevice); // �R���X�g���N�^
			~Sprite3D(); // �f�X�g���N�^
			// �e�N�X�`���̐ݒ�i�Q�p�^�[���j
			void SetTexture(const Texture* texture);
			void SetTexture(const Texture* texture,float textureX,float textureY,float textureWidth,float textureHeight);
			void ChangeSize(float width,float height);
			void Draw( float x, float y,float z,float angle,float alpha); // �`��
			void DrawAll(const NW_Lib::Texture *texture,float x, float y,float z, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,float angle = 0.0f,float alpha = 1.0f); // �����ݒ肷��

			void SetColor(DWORD color)
			{
				m_Color = color;
			}

			void SetRot(float x,float y,float z)
			{
				m_Rot = D3DXVECTOR3(x,y,z);
			}
			void SetRot(D3DXVECTOR3 rot)
			{
				m_Rot = rot;
			}
	};
}


