/**
*	@file	sprite.h
*
*	@brief	�QD�X�v���C�g�p�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/


#pragma once

#include <d3dx9.h>
#include "NW_Lib_Drawer.h"

namespace NW_Lib
{

	class Texture;

	// ���_�t�H�[�}�b�g�\����
	struct VERTEX_TEXTURE2D
	{
		D3DXVECTOR3 position; // ���_���W
		float w;       // RHW
		DWORD diffuse; // ���_�J���[
		float u;       // �e�N�X�`���t���W
		float v;       // �e�N�X�`���u���W
	};

	// �t�H�[�}�b�g�`��
	static const DWORD D3DFVF_TEXTURE2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	class Sprite : public Drawer
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

			LPDIRECT3DDEVICE9 m_pD3DDevice;
		public:
			Sprite(LPDIRECT3DDEVICE9 pD3DDevice); // �R���X�g���N�^
			~Sprite(); // �f�X�g���N�^
			// �e�N�X�`���̐ݒ�i�Q�p�^�[���j
			void SetTexture(const Texture* texture);
			void SetTexture(const Texture* texture,float textureX,float textureY,float textureWidth,float textureHeight);
			void ChangeSize(float width,float height);
			void DrawSprite( float x, float y,float angle,float alpha,bool center); // �`��
			void DrawAll(const NW_Lib::Texture *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,bool center = true,float angle = 0.0f,float alpha = 1.0f); // �����ݒ肷��
			
			void DrawUpper(const NW_Lib::Texture *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,bool center = true,float angle = 0.0f,float alpha = 1.0f); // �����ݒ肷��
			void DrawLower(const NW_Lib::Texture *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,bool center = true,float angle = 0.0f,float alpha = 1.0f); // �����ݒ肷��

			void SetColor(DWORD color)
			{
				m_Color = color;
			}

			// �P�̂̕`��
			static void Draw(const TEXTURE *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight, float cx = 0.0f, float cy = 0.0f, float angle = 0.0f, float alpha = 1.0f); // �`���ݒ肷��

	};
}


