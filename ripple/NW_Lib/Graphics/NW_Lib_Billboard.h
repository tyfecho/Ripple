/**
*	@file	Billboard.h
*
*	@brief	�r���{�[�h�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#pragma once

#include <d3dx9.h>
#include "../System/NW_Lib_Common.h"
#include "../Utility/NW_Lib_Constant.h"
#include "../Graphics/NW_Lib_texture.h"

namespace NW_Lib
{
	class Texture;

	//// ���_�t�H�[�}�b�g�\����
	//struct VERTEX_TEXTURE3D
	//{
	//	D3DXVECTOR3 position; // ���_���W
	//	DWORD diffuse; // ���_�J���[
	//	float u;       // �e�N�X�`���t���W
	//	float v;       // �e�N�X�`���u���W
	//};

	//// �t�H�[�}�b�g�`��
	//static const DWORD D3DFVF_TEXTURE3D = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	class Billboard : public Common , public Drawer
	{
		private:
			float m_width;  // ��
			float m_height; // ����
			const Texture* m_texture; // �e�N�X�`��
			float m_uLeftUp;    // ����t���W
			float m_vLeftUp;    // ����u���W
			float m_uRightDown; // �E���t���W
			float m_vRightDown; // �E���u���W

			static CAMERA* ms_camera; 
		public:
			Billboard(float width,float height); // �R���X�g���N�^
			~Billboard(); // �f�X�g���N�^
			// �e�N�X�`���̐ݒ�i�Q�p�^�[���j
			void SetTexture(const Texture* texture);
			void SetTexture(const Texture* texture,float textureX,float textureY,float textureWidth,float textureHeight);
			void ChangeSize(float width,float height);
			void Draw( float x, float y,float z,float angle); // �`��
			void DrawAll(const NW_Lib::Texture *texture,float x, float y,float z/*, float width, float height*/, float textureX, float textureY, float textureWidth, float textureHeight,float angle); // �����ݒ肷��
			D3DXMATRIX GetBillboardMatrix();
			
			/**
			 * @brief   �`��
			 * 
			 * @param   pTex �e�N�X�`�����
			 * 
			 * @param   dx �\������X���W���w�肵�܂��B
			 * @param   dy �\������Y���W���w�肵�܂��B
			 * @param   dz �\������Z���W���w�肵�܂��B
			 * 
			 * @param   dw �\�����̕����w�肵�܂��B
			 * @param   dh �\�����̍������w�肵�܂��B
			 * 
			 * @param   tx �e�N�X�`����X���W���w�肵�܂��B
			 * @param   ty �e�N�X�`����Y���W���w�肵�܂��B
			 *
			 * @param   tw �e�N�X�`���̐؂蔲�������w�肵�܂��B
			 * @param   th �e�N�X�`���̐؂蔲���������w�肵�܂��B
			 * 
			 * @param   rot ��]�p���w�肵�܂��B�i0 �` 360�x�j
			 * 
			 * @param   alpha ���w�肵�܂��B�i0.0f �` 1.0f�j
			 */
			static void DrawBillboard(TEXTURE* pTex, float dx, float dy,float dz , float dw, float dh,
						float tx, float ty, float tw, float th, float rot = 0.0f, float alpha = 1.0f);
	};
}


