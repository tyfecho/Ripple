/**
 * @file    ModelManager.h
 * @brief   ���f�����Ǘ����邽�߂̃N���X
 * 
 * @author  Masaya Yoshida
 * @date    2013/08/28
 * @version 1.0
 */
#ifndef MODELMANAGER_H_
#define MODELMANAGER_H_

#include "../Utility/NW_Lib_Constant.h"
#include "../Graphics/NW_Lib_texture.h"
#include "../System//NW_Lib_ResourceManager.h"
#include "NW_Lib_SkinAnimationModel.h"
#include "NW_Lib_AnimController.h"
#include "../System/NW_Lib_Common.h"

namespace NW_Lib
{
	struct UseModel
	{
		IModel* m_pModel;
		AnimController* m_pAnimController;

		VECTOR3 m_Pos;
		VECTOR3 m_Rot;
		VECTOR3 m_Scale;
		MATRIX  m_Mat;

		float m_alpha;

		UseModel()
		:m_pModel(NULL)
		,m_pAnimController(NULL)
		,m_Pos(0.0f,0.0f,0.0f)
		,m_Rot(0.0f,0.0f,0.0f)
		,m_Scale(1.0f,1.0f,1.0f)
		,m_alpha(1.0f)
		{
		}
			
		~UseModel()
		{
			SafeDelete(m_pAnimController);
		}

	};

	struct LoadModelInfo
	{
		IModel* model;
		bool animFlag;

		~LoadModelInfo()
		{
			SafeDelete(model);
		}
	};

	class ModelManager : public Common
	{
		private:
			// �t�@�C�����̒���
			static const int FILENAME_LENGTH = 64;

			ResourceManager<LoadModelInfo*> m_pLoadModelManager;
			ResourceManager<UseModel*> m_pModelManager;


		private:
			/// �R���X�g���N�^
			ModelManager(){}
			ModelManager(const ModelManager& val){(void)val;}
			ModelManager& operator=(const ModelManager& val){(void)val;return *this;}

		public:
			/// �V���O���g����������
			static ModelManager& GetInstance()
			{
				static ModelManager instance;
				return instance;
			}

			/// �t�@�C���ǂݍ���
			void LoadModel(int loadHandle,char* filename);

			/// ���f���̐���
			void Entry(int handle,int loadHandle);

			/// ���f���̈ʒu�̐ݒ�
			void SetPosision(int handle,VECTOR3 position);
			/// ���f���̉�]�̐ݒ�
			void SetRotate(int handle,VECTOR3 rotation);
			/// ���f���̃T�C�Y�̐ݒ�
			void SetScale(int handle,VECTOR3 scale);
			/// ���f���̍s��ݒ�
			void SetMatrix(int handle,MATRIX* pMat);

			/// ���f���̍X�V
			void Update(int hundle);

			/// ���f���̕`��
			void Draw(DrawingScreen drawingScreen,int handle);

			/// ���f���̓����x�̐ݒ�
			void SetAlpha(int handle,float alpha);

			/// ���f���̃A�j���[�V�����g���b�N�̐ݒ�
			void SetAnimationTruck(int handle, int truck);

			/// ���f���̃��[���h���W��ݒ�
			void SettingMatrix(int handle);

			/// �m�ۂ��Ă��郂�f��������
			void Remove(int handle);
 
			/// �m�ۂ��Ă��郂�f����S�ď���
			void AllRemove();
	};
}

#endif
