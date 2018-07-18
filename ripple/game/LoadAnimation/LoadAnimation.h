/*-------------------------------------------------------------------------

	�N���X���@�F�@���[�h�A�j���[�V�����Ǘ��N���X
	�쐬�ҁ@�@�F�@�F���@����
	�ŏI�X�V���F�@2012 / 03 / 15

-------------------------------------------------------------------------*/
#pragma once

#include "../../NW_Lib/Graphics/NW_Lib_texture.h"
#include "../../NW_Lib/Utility/NW_Lib_Constant.h"

// ���[�h���[�h
typedef enum
{
	LOADUNKNOWN = -1,
	LOADOUT		,
	LOADIN		,
	LOADEND		,
}LOAD;

namespace Game
{

	class TEXTURE;

class LoadAnimation
{

	private:

		int m_loadMaxTimer;		//���[�h�A�j���[�V��������
		float m_oneFlameTime;	//��t���[���̎���

		LOAD m_loadMode;		// ���[�h�A�j���[�V�����C���@���[�h�A�j���[�V�����A�E�g
		LOAD m_beforeMode;		//�O��̃��[�h�A�j���[�V�������[�h
		int m_animeTime;		//�A�j���[�V�����^�C�}�[

		struct Carp
		{
			int type;	//��̃^�C�v
			int anime;	//��̃A�j���[�V�����J�E���g(loop 0�`32)
			float size; //��̃T�C�Y
			VECTOR2 pos;//��̃|�W�V�����i�㉺�����j
		};
		static const int CarpNum = 15;
		
		Carp m_carpUpper[CarpNum];
		Carp m_carpLower[CarpNum];
		
		NW_Lib::Texture* m_loadTex;				//���[�h�A�j���[�V�����p�e�N�X�`��
		NW_Lib::Texture* m_loadTex2;			//���[�h�A�j���[�V�����p�e�N�X�`��
		NW_Lib::Texture* m_loadTexCarp[3];		//���[�h�A�j���[�V�����p�e�N�X�`��

		bool m_enable;

		float m_texPosX;

	public:

		/*=======================================================
			�֐���	�F	LoadAnimation
			����	�F	�R���X�g���N�^
			����	�F	�Ȃ�
			�߂�l	�F�@�Ȃ�
		========================================================*/
		LoadAnimation();

		/*=======================================================
			�֐���	�F	~LoadAnimation
			����	�F	�f�X�g���N�^
			����	�F	�Ȃ�
			�߂�l	�F�@�Ȃ�
		========================================================*/
		~LoadAnimation();

		/*=======================================================
			�֐���	�F	Initialize
			����	�F	����������
			����	�F	�Ȃ�
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void Initialize();

		/*=======================================================
			�֐���	�F	isLoadAnimationEnd
			����	�F	���[�h�A�j���[�V�����̏I��肩
			����	�F	�Ȃ�
			�߂�l	�F�@����
		========================================================*/
		bool isLoadAnimationEnd();

		/*=======================================================
			�֐���	�F	isLoadAnimationNow
			����	�F	���[�h�A�j���[�V��������
			����	�F	�Ȃ�
			�߂�l	�F�@����
		========================================================*/
		bool isLoadAnimationNow();

		/*=======================================================
			�֐���	�F	LoadAnimationModeChanger
			����	�F	���[�h�A�j���[�V������Ԃ�ς���
			����	�F	�ύX������
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void LoadAnimationModeChanger(LOAD next_load );

		/*=======================================================
			�֐���	�F	LoadAnimationDraw
			����	�F	�`��֐�
			����	�F	�X�N���[��
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void LoadAnimationDraw( NW_Lib::DrawingScreen screenNum );
		
		/*=======================================================
			�֐���	�F	isLoadAnimationNow
			����	�F	���[�h�A�j���[�V��������
			����	�F	�Ȃ�
			�߂�l	�F�@���[�h�A�j���[�V�������
		========================================================*/
		LOAD GetLoadAnimationMode() { return m_loadMode; }

		/*=======================================================
			�֐���	�F	SetEnable
			����	�F	���݂̐ݒ�
			����	�F	�t���O
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void SetEnable(bool flag) { m_enable = flag; }

		/*=======================================================
			�֐���	�F	SetBeforeLoadAnimationMode
			����	�F	�O��̃��[�h�A�j���[�V������Ԃ�ς���
			����	�F	�ύX������
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void SetBeforeLoadAnimationMode(LOAD loadMode ){ m_beforeMode = loadMode; };

		/*=======================================================
			�֐���	�F	SetLoadAnimationTime
			����	�F	���[�h�A�j���[�V�����ɂ����鎞��
			����	�F	���[�h�A�j���[�V�����ɂ����鎞��
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void SetLoadAnimationTime(int num = 30 );
};

}