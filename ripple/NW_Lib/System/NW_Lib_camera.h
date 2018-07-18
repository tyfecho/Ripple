/**
*	@file	Camera.h
*
*	@brief	�J�����N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#pragma once


#include <d3dx9.h>
#include "../Utility/NW_Lib_Constant.h"
#include "../System/NW_Lib_Common.h"
#include "../Graphics/NW_Lib_texture.h"


enum MODE
{
	MODE_PERSPECTIVE,
	MODE_ORTHO,
};


namespace NW_Lib
{
	class Camera : public Common
	{
		private:
			D3DXVECTOR3 m_EyePos;    // �J�������g�̈ʒu
			D3DXVECTOR3 m_LookAtPos; // �J�����̌��Ă���ʒu
			D3DXVECTOR3 m_UpVec;     // ������x�N�g��

			D3DXVECTOR3 m_AddEyePos;
			D3DXVECTOR3 m_AddLookAtPos;

			D3DXMATRIX m_matView;

			float m_Aspect;   // �A�X�y�N�g��
			float m_NearClip; // �߂����E�l
			float m_FarClip;  // �������E�l
			float m_Rot;

			bool m_fog;

			int m_viewMode; //Parspectiive / Ortho
			VECTOR2 m_orthoSize;

			//LPDIRECT3DDEVICE9 m_pD3DDevice;
		public:
			Camera();
			~Camera();
			void SetAspect(float width,float height);
			void ShowCamera(D3DXVECTOR3* pView,int viewport); // �J�����̐ݒ�
			void ShowCamera();                                // �J�����̐ݒ�

			void Fog();

			bool Culling(D3DXVECTOR3 pos);

			// �C�����C���֐�

			// �J�����̈ʒu�̎擾
			D3DXVECTOR3 GetCameraPos()
			{
				return m_EyePos; 
			}
			// ���ݐݒ肳��Ă���f�[�^�ōŏI�̈ʒu��\�����A�����Ԃ�
			D3DXVECTOR3 GetResultEyePos()
			{
				return m_EyePos + m_AddEyePos;
			}

			// �J�����̎��_���擾
			D3DXVECTOR3 GetCameraTarget()
			{
				return m_LookAtPos; 
			}
			// ���ݐݒ肳��Ă���f�[�^�ōŏI�̈ʒu��\�����A�����Ԃ�
			D3DXVECTOR3 GetResultLookAtPos()
			{
				return m_LookAtPos + m_AddLookAtPos;
			}

			//�J�������g�̈ʒu�̃Z�b�g
			void SetCameraPosX(float pos_x)
			{
				m_EyePos.x = pos_x; 
			}
			void SetCameraPosY(float pos_y)
			{
				m_EyePos.y = pos_y; 
			}
			void SetCameraPosZ(float pos_z)
			{
				m_EyePos.z = pos_z;
			}
			void SetCameraPos(float pos_x, float pos_y, float pos_z)
			{
				m_EyePos.x = pos_x;
				m_EyePos.y = pos_y;
				m_EyePos.z = pos_z;
			}
			void SetCameraPos(D3DXVECTOR3 pos) 
			{ 
				m_EyePos = pos; 
			}
			void AddCameraPos(D3DXVECTOR3 vec)
			{
				m_AddEyePos += vec;
			}


			// �J�����̌����̐ݒ�
			void SetCameraTargetX(float pos_x) 
			{ 
				m_LookAtPos.x = pos_x; 
			}
			void SetCameraTargetY(float pos_y) 
			{ 
				m_LookAtPos.y = pos_y; 
			}
			void SetCameraTargetZ(float pos_z) 
			{
				m_LookAtPos.z = pos_z; 
			}
			void SetCameraTarget(D3DXVECTOR3 pos) 
			{ 
				m_LookAtPos = pos; 
			}
			void SetCameraTarget(float pos_x, float pos_y, float pos_z)
			{
				m_LookAtPos.x = pos_x;
				m_LookAtPos.y = pos_y;
				m_LookAtPos.z = pos_z;
			}
			void AddCameraTarget(D3DXVECTOR3 vec)
			{
				m_LookAtPos += vec;
			}


			// �J�����̏�����x�N�g���̐ݒ�
			void SetVecUp(float pos_x, float pos_y, float pos_z)
			{
				m_UpVec.x = pos_x;
				m_UpVec.y = pos_y;
				m_UpVec.z = pos_z;
			}

			// �t�H�O���Z�b�g���邩�ǂ���
			void SetFog(bool set)
			{
				m_fog = set;
			}

			// �A�X�y�N�g���Ԃ�
			float GetAspect()
			{
				return m_Aspect;
			}

			// ��O�̃N���b�v�l��ݒ�
			void SetNearClip(float nearClip)
			{
				m_NearClip = nearClip;
			}

			// ���̃N���b�v�l��ݒ�
			void SetFarClip(float farClip)
			{
				m_FarClip = farClip;
			}

			// �������ރx�N�g�������ۂɑ�������
			void Add()
			{
				m_EyePos    += m_AddEyePos;
				m_LookAtPos += m_AddLookAtPos;
			}
			
			// �p�[�X�y�N�e�B�u�ɐݒ肷��
			void SetPerspective()
			{
				m_viewMode = MODE_PERSPECTIVE;
			}

			// ���ˉe�ɐݒ肷��
			void SetOrtho(float width , float height)
			{
				m_viewMode = MODE_ORTHO;

				m_orthoSize.x = width;
				m_orthoSize.y = height;
			}

	};
}



