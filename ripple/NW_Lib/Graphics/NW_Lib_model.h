/**
*	@file	model.h
*
*	@brief	3D���f���p�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#pragma once
#include "d3dx9.h"
#include "NW_Lib_texture.h"
#include "../System//NW_Lib_Common.h"

#include "NW_Lib_IModel.h"


namespace NW_Lib
{
	class AnimController;

	class Model:public Common , public IModel
	{
		private:
			LPD3DXMESH    m_mesh;
			D3DMATERIAL9* m_meshMaterials;
			Texture**     m_meshtexture;
			DWORD         m_numMaterials;

			Texture** m_ppSetTex;
			D3DCOLORVALUE* m_pMasterAmbient;

			D3DXVECTOR3 m_Pos;    // �ʒu
			D3DXVECTOR3 m_Angle;  // �p�x�i�f�B�O���[�j
			D3DXVECTOR3 m_Radian; // �p�x�i���W�A���j
			D3DXVECTOR3 m_Scale;  // �X�P�[���i�{���j

			// ���[�J�����[���h
			D3DXMATRIX m_LocalWorld;
			// �e�s��
			D3DXMATRIX* m_pParentWorld;
			// ���[�J���v�Z�O�ɂ����Ă����s��
			D3DXMATRIX* m_pInitWorld;

			bool m_NoMaterial;
			bool m_MaterialGlay;
		public:
			Model();
			Model(char* filename);
			~Model();
			void Calc();
			void Draw(/*D3DVIEWPORT9* pViewport,int viewport,DWORD* pBackColor*/); // �`�揈��
			void LoadMesh(const char* filename);                  // ���[�h����
			void Release();                                 // �������

			// �e�N�X�`���̐ݒ�
			void SetTexture(const char *filename,unsigned int tex_num);
			void SetTexture(Texture* pTex,unsigned int tex_num);
			void NoTexture();

			// �T�C�Y�ύX
			void Scale(float scale)
			{
				m_Scale.x = scale;
				m_Scale.y = scale;
				m_Scale.z = scale;
			}
			void Scale(D3DXVECTOR3 scale)
			{
				m_Scale = scale;
			}
			void Scale(float scl_x,float scl_y,float scl_z)
			{
				m_Scale.x = scl_x;
				m_Scale.y = scl_y;
				m_Scale.z = scl_z;
			}
			// �ړ�
			void Move(D3DXVECTOR3 pos)
			{
				m_Pos = pos;
			}
			void Move(float x,float y,float z)
			{
				m_Pos.x = x;
				m_Pos.y = y;
				m_Pos.z = z;
			}
			void MoveX(float pos_x)
			{
				m_Pos.x = pos_x;
			}
			void MoveY(float pos_y)
			{
				m_Pos.y = pos_y;
			}
			void MoveZ(float pos_z)
			{
				m_Pos.z = pos_z;
			}
			// ��]
			void Rotation(D3DXVECTOR3 rot)
			{
				m_Angle = rot;
				m_Radian.x = m_Angle.x * D3DX_PI / 180.0f;
				m_Radian.y = m_Angle.y * D3DX_PI / 180.0f;
				m_Radian.z = m_Angle.z * D3DX_PI / 180.0f;
			}
			void Rotation(float deg_x,float deg_y,float deg_z)
			{
				m_Angle.x = deg_x;
				m_Radian.x = m_Angle.x * D3DX_PI / 180.0f;

				m_Angle.y = deg_y;
				m_Radian.y = m_Angle.y * D3DX_PI / 180.0f;

				m_Angle.z = deg_z;
				m_Radian.z = m_Angle.z * D3DX_PI / 180.0f;

			}
			void RotationX(float degX)
			{
				m_Angle.x = degX;
				m_Radian.x = m_Angle.x * D3DX_PI / 180.0f;
			}
			void RotationY(float degY)
			{
				m_Angle.y = degY;
				m_Radian.y = m_Angle.y * D3DX_PI / 180.0f;
			}
			void RotationZ(float degZ)
			{
				m_Angle.z = degZ;
				m_Radian.z = m_Angle.z * D3DX_PI / 180.0f;
			}
			// �����x�̕ύX
			void SetAlfa(float alfa)
			{
				for(DWORD i = 0;i < m_numMaterials;++i){
					m_meshMaterials[i].Diffuse.a = alfa;
					m_meshMaterials[i].Ambient.a = m_meshMaterials[i].Diffuse.a;
				}
			}

			// �}�e���A���F�̐ݒ�
			void SetColor(float r,float g,float b)
			{
				for(DWORD i = 0;i < m_numMaterials;++i){
					m_meshMaterials[i].Diffuse.r = r;
					m_meshMaterials[i].Diffuse.g = g;
					m_meshMaterials[i].Diffuse.b = b;
					m_meshMaterials[i].Ambient.r = r;
					m_meshMaterials[i].Ambient.g = g;
					m_meshMaterials[i].Ambient.b = b;
				}
			}
			// ���̃}�e���A���ɖ߂�
			void TakeBackMaterial()
			{
				for(DWORD i = 0;i < m_numMaterials;++i){
					m_meshMaterials[i].Diffuse = m_pMasterAmbient[i];
					m_meshMaterials[i].Ambient = m_pMasterAmbient[i];
				}
			}

			// �e�s��̎w��
			void SetParentMat(D3DXMATRIX* pParentWorld)
			{
				m_pParentWorld = pParentWorld;
			}
			// �������s��̎w��
			void SetInittMat(D3DXMATRIX* pInitWorld)
			{
				m_pInitWorld = pInitWorld;
			}

			// ���b�V������Ԃ�
			LPD3DXMESH GetMesh()
			{
				return m_mesh;
			}
			// ���[�J���s���Ԃ�
			D3DXMATRIX* GetLocal()
			{
				return &m_LocalWorld;
			}

			// �}�e���A����ݒ肵�Ȃ�
			void NoSetMaterial()
			{
				m_NoMaterial = true;
				m_MaterialGlay = false;
			}

			// �}�e���A�����ŗL�̂��̂ɐݒ肷��
			void SetMaterial()
			{
				m_NoMaterial = false;
				m_MaterialGlay = false;
			}
			// �}�e���A�����O���[�ɂ���
			void SetMaterialGlay()
			{
				m_NoMaterial = false;
				m_MaterialGlay = true;
			}

			/*=======================================================
				�֐���	�F	Update
				����	�F	�X�V
				����	�F	�A�j���[�V�����R���g���[��
							���[���h�s��
				�߂�l	�F�@�Ȃ�
			========================================================*/
			void Update(AnimController* pAnimController , LPD3DXMATRIX pWorld)
			{
				m_LocalWorld = *pWorld;
			};

			/*=======================================================
				�֐���	�F	Draw
				����	�F	���b�V���̕`��
				����	�F	���[���h�}�g���b�N�X
							�A�j���[�V�����R���g���[��
							�A���t�@
				�߂�l	�F�@�Ȃ�
			========================================================*/
			void Draw( D3DXMATRIX world , AnimController* pAnimController , float alpha = 1.0f)
			{
				m_LocalWorld = world;

				Draw();
			};

	};
}


