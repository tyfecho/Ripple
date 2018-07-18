#pragma once

#include <atlbase.h>
#include <d3dx9.h>

#include "../System/NW_Lib_Common.h"


//  �e�N�X�`�������F1
//  �G���g���[�\���F1000
namespace NW_Lib
{
		// �`��p�̒��_�t�H�[�}�b�g
		struct VERTEX
		{
			// 3D���W
			float x,y,z;
			// �F���
			unsigned int color;
			// �e�N�X�`����UV���W
			float u,v;
		};

		// �G���g���[���
		struct Info
		{
			// ���݂̍��W
			float x,y,z;
			// ���x
			float vx,vy,vz;
			// �����x
			float ax,ay,az;

			// ���݂̊p�x
			float angle;
			// �J�n�p�x
			float s_angle;
			// �I���p�x
			float e_angle;

			// ���݂̃X�P�[��
			float scaleX,scaleY;
			// �J�n�X�P�[��
			float s_scaleX,s_scaleY;
			// �I���X�P�[��
			float e_scaleX,e_scaleY;

			// ���݂̐F
			float r,g,b,a;
			// �J�n�̐F
			float s_r,s_g,s_b,s_a;
			// �I���̐F
			float e_r,e_g,e_b,e_a;

			// ���݂̃t���[���J�E���g
			int   frame;
			// �I���̃t���[���J�E���g
			int   num_frame;
		
			// ���̃f�[�^�|�C���^
			Info* next;
			// �O�̃f�[�^�|�C���^
			Info* prev;

			// �e�s��
			D3DXMATRIX* pParentMat;
		};


	class ParticleManager: public Common
	{
		private:
			// �e�N�X�`�����
			CComPtr<IDirect3DTexture9> texture;
			// �d�͉����x
			float gx,gy,gz;

			// �r���{�[�h�ϊ��s��
			D3DXMATRIX matBillbord;

			// �G���g���[�̌��E��
			static const int tableNum=1000;
			// �G���g���[���̔z��
			Info infoTable[tableNum];

			// �t���[�ȃG���g���[���
			Info freeList;
			// �A�N�e�B�u�ȃG���g���[���
			Info activeList;
		public:

			~ParticleManager();
			// �e�N�X�`���̓ǂݍ���
			bool LoadTexture(LPCTSTR filename);
	
			// �r���{�[�h�̏����X�V����i�f�o�C�X�ɐݒ肳�ꂽ�J�������(�r���[�s��)���g���j
			void SetBillbord();

			// �G���g���[���̍X�V
			bool Update(Info *info);

			// �P�̃G���g������`�悷��
			void Draw(Info* info);

			// ������
			void Initialize();

			// �A�N�e�B�u���X�g�ɃG���g���[����ǉ�����
			Info* AddInfo();

			// �p�[�e�B�N���̃G���g���[
			void Entry(int frame,	// �t���[����
				float x,   float y,   float z,	// ���W
				float vx,  float vy,  float vz,	// ���x
				float ax, float ay, float az,	// �����x
				float start_angle, // �J�n�p�x
				float end_angle,   // �I���p�x
				float start_scaleX,float start_scaleY, // �g�k�E�J�n�{��
				float end_scaleX,float end_scaleY,     // �g�k�E�I���{��
				float start_r,float start_g,float start_b,float start_a, // �J�n�J���[
				float end_r,  float end_g,  float end_b,  float end_a,    // �I���J���[
				D3DXMATRIX* ParentMat = NULL
				);

			// �w��̃G���g���[�����A�N�e�B�u���X�g����A�t���[���X�g�ֈڂ�
			Info* Free(Info *info);

			// ���ׂẴA�N�e�B�u�ȃG���g���[�����X�V����
			void Update();

			// �A�N�e�B�u�ȃG���g���[�������ׂĕ`�悷��
			void Draw();

			void Erase();

			// �d�͉����x�̐ݒ�
			void SetGravity(float _gx,float _gy,float _gz)
			{
				gx = _gx;
				gy = _gy;
				gz = _gz;
			}
	};
}
