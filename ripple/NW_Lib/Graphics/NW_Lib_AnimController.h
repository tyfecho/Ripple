/*-------------------------------------------------------------------------

	�N���X���@�F�@�A�j���[�V�����R���g���[���[�N���X
	�쐬�ҁ@�@�F�@�F���@����
	�ŏI�X�V���F�@2012 / 12 / 17

-------------------------------------------------------------------------*/
#pragma once

#include <d3dx9.h>
#include <d3d9.h>

namespace NW_Lib
{
	
	class SkinAnimationModel;

	static const int MOTION_MAX = 20;	//���[�V�����ݒ�̍ő吔

typedef struct 
{
	LPD3DXANIMATIONSET	pAnimSet;			// �A�j���[�V�����Z�b�g
	unsigned int		id;					// �A�j���[�V����ID
	unsigned int		loopNum;			// �A�j���[�V�����̃��[�v��
	float				loopTime;			// 1���[�v�̎���
	float				trackSpeed;			// �g���b�N�X�s�[�h���ߒl
	float				shiftTime;			// �V�t�g����̂ɂ����鎞��
	float				currentWeightTime;	// ���݂̃E�F�C�g����
} ANIMATION_DESC;

class AnimController
{

	public:

		//�A�j���[�V�����̃��[�v�^�C�v
		typedef enum 
		{
			NORMAL = 0,
			INVERSE,
		}LOOP_TYPE;

	private:

		LPD3DXANIMATIONCONTROLLER m_pAnimController;	//�A�j���[�V�����R���g���[��
		//LPD3DXANIMATIONSET	m_pAnimSet;					// �A�j���[�V�����Z�b�g

		ANIMATION_DESC m_animeDesc[MOTION_MAX];			//��ʃg���b�N���
		unsigned int m_currentId;						//���݂�ID
		unsigned int m_prevId;							//�O���ID

		bool m_animeFrag;								//�A�j���[�V�������邩

		LOOP_TYPE m_loopType;							//�A�j���[�V�����̃��[�v���
		float m_currentAnimTime;						//���݂̃A�j���[�V��������
		
	public:

		/*=======================================================
			�֐���	�F	AnimController
			����	�F	�R���X�g���N�^
			����	�F	�Ȃ�
			�߂�l	�F�@�Ȃ�
		========================================================*/
		AnimController();

		/*=======================================================
			�֐���	�F	~AnimController
			����	�F	�f�X�g���N�^
			����	�F	�Ȃ�
			�߂�l	�F�@�Ȃ�
		========================================================*/
		~AnimController();

		/*=======================================================
			�֐���	�F	GetAnimationController
			����	�F	�A�j���[�V�������̎擾
			����	�F	�Ȃ�
			�߂�l	�F�@�A�j���[�V�������
		========================================================*/
		LPD3DXANIMATIONCONTROLLER GetAnimationController() { return m_pAnimController; }

		/*=======================================================
			�֐���	�F	CreateAnimController
			����	�F	�V���ɃA�j���[�V�����R���g���[�����쐬����
			����	�F	�X�L�����f�����
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void CreateAnimController(NW_Lib::SkinAnimationModel* obj);

		/*=======================================================
			�֐���	�F	SetLoopType
			����	�F	���[�v����ݒ�
			����	�F	�A�j���[�V�������
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void SetLoopType( LOOP_TYPE loopType){ m_loopType = loopType;}

		/*=======================================================
			�֐���	�F	GetLoopType
			����	�F	���[�v����ݒ�
			����	�F	�Ȃ�
			�߂�l	�F�@�A�j���[�V������� 
		========================================================*/
		LOOP_TYPE GetLoopType(){ return m_loopType;}

		/*=======================================================
			�֐���	�F	SetLoopTime
			����	�F	���[�v���Ԃ�ݒ�
			����	�F	�A�j���[�V����ID
						���ԁi�b�j
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void SetLoopTime( unsigned int animId, float time );

		/*=======================================================
			�֐���	�F	SetShiftTime
			����	�F	����J�n�ɂ����鎞�Ԃ�ݒ�
			����	�F	�A�j���[�V����ID
						���ԁi�b�j
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void SetShiftTime( unsigned int animId, float interval );

		/*=======================================================
			�֐���	�F	ChangeAnimation
			����	�F	�A�j���[�V������؂�ւ�
			����	�F	�A�j���[�V����ID
						�i���[�v���j
						�i���[�v�񐔁j
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void ChangeAnimation( unsigned int animId , LOOP_TYPE loopType = NORMAL, unsigned int loopNum = 0 );

		/*=======================================================
			�֐���	�F	SetAnimationPos
			����	�F	�A�j���[�V�����ʒu��ݒ�
			����	�F	�A�j���[�V����ID
						�A�j���[�V�����ʒu
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void SetAnimationPos( unsigned int animId , float time );

		/*=======================================================
			�֐���	�F	StopAnimation
			����	�F	�A�j���[�V�����n�߂�
			����	�F	�Ȃ�
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void StartAnimation() { m_animeFrag = true;}

		/*=======================================================
			�֐���	�F	StopAnimation
			����	�F	�A�j���[�V�������Ƃ߂�
			����	�F	�Ȃ�
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void StopAnimation() { m_animeFrag = false;}
	
		/*=======================================================
			�֐���	�F	AdvanceTime
			����	�F	�X�V
			����	�F	�Ȃ�
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void AdvanceTime( float time );

		/*=======================================================
			�֐���	�F	GetRemainingTime
			����	�F	���[�v�c�莞�Ԃ̎擾
						(���[�v�񐔂�ݒ肵�Ă��Ȃ��ꍇ�͐i�s���Ԃ̎擾)
			����	�F	�Ȃ�
			�߂�l	�F�@�c�莞�ԁi�b�j
		========================================================*/
		double GetRemainingTime() { return m_animeDesc[m_currentId].loopTime * m_animeDesc[m_currentId].loopNum - m_currentAnimTime; }
		
		/*=======================================================
			�֐���	�F	DrawSetUp
			����	�F	�`�揀��
			����	�F	�Ȃ�
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void DrawSetUp();

};

}