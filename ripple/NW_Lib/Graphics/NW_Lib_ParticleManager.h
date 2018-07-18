/**
 * @file    NW_Lib_ParticleManager.h
 * @brief   �p�[�e�B�N���𑀍삷��N���X
 * 
 * @author  Masaya Yoshida
 * @date    2013/09/19
 * @version 1.0
 */
#ifndef PARTICLE_MANAGER
#define PARTICLE_MANAGER

#include "../System/NW_Lib_ResourceManager.h"
#include "NW_Lib_Particle.h"

namespace NW_Lib
{
	namespace Graphics
	{
		class ParticleManager
		{
			private:

			private:
				/// �R���X�g���N�^
				ParticleManager(){}
				ParticleManager(const ParticleManager& val){(void)val;}
				ParticleManager& operator=(const ParticleManager& val){(void)val;return *this;}

			public:
				/// �V���O���g����������
				static ParticleManager& GetInstance()
				{
					static ParticleManager instance;
					return instance;
				}

				void Initialize(){}


				/// �t�@�C���ǂݍ���
				void LoadFile(int loadHandle,char* filename){}
				/// �p�[�e�B�N���̐���
				void Entry(int handle,int loadHandle){}

				/// �ʒu�̐ݒ�
				void SetPosision(int handle,VECTOR3 position){}
				/// ��]�̐ݒ�
				void SetRotate(int handle,VECTOR3 rotation){}
				/// �T�C�Y�̐ݒ�
				void SetScale(int handle,VECTOR3 scale){}

				/// ���o�ʂ̐ݒ�
				void SetEmission(int handle,float emission){}

			private:
				/// �X�V
				void Update(DrawingScreen drawingScreen,int handle){}

			public:
				/// �`��
				void Draw(DrawingScreen drawingScreen,int handle){}

				void Remove(int handle){}

				void AllRemove(){}
		};
	}
}


#endif
