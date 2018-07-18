/*-------------------------------------------------------------------------

	�N���X���@�F�@�A�j���[�V�����Ǘ��N���X
	�쐬�ҁ@�@�F�@�F���@����
	�ŏI�X�V���F�@2012 / 03 / 15

-------------------------------------------------------------------------*/
#include "NW_Lib_AnimController.h"

#include "NW_Lib_SkinAnimationModel.h"

namespace NW_Lib
{

/*=======================================================
	�֐���	�F	AnimController
	����	�F	�R���X�g���N�^
	����	�F	�Ȃ�
	�߂�l	�F�@�Ȃ�
========================================================*/
AnimController::AnimController()
{
	m_currentId = 0;
	m_prevId = 0;
	m_animeFrag = true;
}

/*=======================================================
	�֐���	�F	~AnimController
	����	�F	�f�X�g���N�^
	����	�F	�Ȃ�
	�߂�l	�F�@�Ȃ�
========================================================*/
AnimController::~AnimController()
{
	m_pAnimController->Release();
}

/*=======================================================
	�֐���	�F	CreateAnimController
	����	�F	�V���ɃA�j���[�V�����R���g���[�����쐬����
	����	�F	�X�L�����f�����
	�߂�l	�F�@�Ȃ�
========================================================*/
void AnimController::CreateAnimController(NW_Lib::SkinAnimationModel* obj)
{
	m_pAnimController = NULL;
	if( obj->GetAnimationController() )
	{
		obj->CloneAnimationController( &m_pAnimController );

		if( m_pAnimController )
		{
			//�A�j���[�V�����g���b�N�𓾂�
			int num = 0;
			for(DWORD i = 0 ; i < m_pAnimController->GetNumAnimationSets() ; i++ )
			{
				num = ( m_pAnimController->GetNumAnimationSets() - 1 ) - i;

				m_pAnimController->GetAnimationSet(i,&m_animeDesc[num].pAnimSet);
				m_animeDesc[num].id = num;
				m_animeDesc[num].loopTime = 1.0f;
				m_animeDesc[num].shiftTime = 0.1f;
				m_animeDesc[num].trackSpeed = 1.0f;
				m_animeDesc[num].currentWeightTime = 1.0f;
			}
		}
	}
}


/*=======================================================
	�֐���	�F	SetLoopTime
	����	�F	���[�v���Ԃ�ݒ�
	����	�F	�A�j���[�V����ID
				���ԁi�b�j
	�߂�l	�F�@�Ȃ�
========================================================*/
void AnimController::SetLoopTime( unsigned int animId, float time )
{
	//�w��̃A�j���[�V����ID�̑��݂��邩
	if( m_pAnimController->GetNumAnimationSets() < animId )
	{
		return;
	}

	//�g���b�N�X�s�[�h���ߒl���v�Z
	float defTime = static_cast<float>(m_animeDesc[animId].pAnimSet->GetPeriod());
	m_animeDesc[animId].loopTime = time;
	m_animeDesc[animId].trackSpeed = defTime / time;

}

/*=======================================================
	�֐���	�F	SetShiftTime
	����	�F	����J�n�ɂ����鎞�Ԃ�ݒ�
	����	�F	�A�j���[�V����ID
				���ԁi�b�j
	�߂�l	�F�@�Ȃ�
========================================================*/
void AnimController::SetShiftTime( unsigned int animId, float interval )
{
	//�w��̃A�j���[�V����ID�̑��݂��邩
	if( m_pAnimController->GetNumAnimationSets() < animId )
	{
		return;
	}
	// �V�t�g���Ԃ�o�^
	m_animeDesc[animId].shiftTime = interval;
}

/*=======================================================
	�֐���	�F	ChangeAnimation
	����	�F	�A�j���[�V������؂�ւ�
	����	�F	�A�j���[�V����ID
				�i���[�v���j
				�i���[�v�񐔁j
	�߂�l	�F�@�Ȃ�
========================================================*/
void AnimController::ChangeAnimation( unsigned int animId , LOOP_TYPE loopType , unsigned int loopNum )
{
	//�w��̃A�j���[�V����ID�̑��݂��邩
	if( m_pAnimController->GetNumAnimationSets() < animId )
	{
		return;
	}
	// �قȂ�A�j���[�V�����ł��邩���`�F�b�N
	if( m_currentId == animId )
	{
		return;
	}
	//���[�v�񐔂�ݒ�
	m_animeDesc[animId].loopNum = loopNum;

	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l
	D3DXTRACK_DESC TD;		// �g���b�N
	m_pAnimController->GetTrackDesc( 0, &TD );

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ڍs
	m_pAnimController->SetTrackAnimationSet( 1, m_animeDesc[m_currentId].pAnimSet );
	m_pAnimController->SetTrackDesc( 1, &TD );

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	m_pAnimController->SetTrackAnimationSet( 0, m_animeDesc[animId].pAnimSet );

	// �g���b�N�̃X�s�[�h�̐ݒ�
	m_pAnimController->SetTrackSpeed( 0, m_animeDesc[animId].trackSpeed );

	// �g���b�N�̍���������
	m_pAnimController->SetTrackEnable( 0, true );
	m_pAnimController->SetTrackEnable( 1, true );

	// �E�F�C�g���Ԃ�������
	m_animeDesc[animId].currentWeightTime = 0.0f;

	// ���݂̃A�j���[�V�����ԍ���؂�ւ�
	m_prevId = m_currentId;
	m_currentId = animId;

	// ���Ԃ����Z�b�g
	m_pAnimController->ResetTime();
	m_pAnimController->AdvanceTime( 0 , NULL );
	m_currentAnimTime = 0;

	//���[�v����ݒ�
	m_loopType = loopType;
}

/*=======================================================
	�֐���	�F	SetAnimationPos
	����	�F	�A�j���[�V�����ʒu��ݒ�
	����	�F	�A�j���[�V����ID
				�A�j���[�V�����ʒu
	�߂�l	�F�@�Ȃ�
========================================================*/
void AnimController::SetAnimationPos( unsigned int animId , float time )
{

}

/*=======================================================
	�֐���	�F	AdvanceTime
	����	�F	�X�V
	����	�F	�Ȃ�
	�߂�l	�F�@�Ȃ�
========================================================*/
void AnimController::AdvanceTime( float time )
{
	if(m_animeFrag)
	{
		// ���������ۂ��𔻒�
		m_animeDesc[m_currentId].currentWeightTime += fabs(time);
		if( m_animeDesc[m_currentId].currentWeightTime <= m_animeDesc[m_currentId].shiftTime )
		{
			// �������B�E�F�C�g���Z�o
			FLOAT weight = m_animeDesc[m_currentId].currentWeightTime / m_animeDesc[m_currentId].shiftTime;

			// �E�F�C�g��o�^
			m_pAnimController->SetTrackWeight( 0, weight );       // ���݂̃A�j���[�V����
			m_pAnimController->SetTrackWeight( 1, 1 - weight );   // �O�̃A�j���[�V����
		}
		else
		{
			// �����I�����B�ʏ�A�j���[�V����������Track0�̃E�F�C�g���ő�l��
			m_pAnimController->SetTrackWeight( 0, 1.0f );       // ���݂̃A�j���[�V����
			m_pAnimController->SetTrackEnable( 1, false );      // �O�̃A�j���[�V�����𖳌��ɂ���
		}

		// ���[�v�񐔂��w�肳��Ă���A
		// �w�胋�[�v�񐔉�����ꍇ�́A���̃A�j���[�V�����ɖ߂�
		double currentTime = m_pAnimController->GetTime();

		if( m_animeDesc[m_currentId].loopNum &&
			( currentTime >= m_animeDesc[m_currentId].loopTime * m_animeDesc[m_currentId].loopNum ))
		{
			ChangeAnimation(m_prevId);
		}

		//���[�v���ɂ���Đ؂�ւ���
		if( m_loopType == NORMAL )
		{
			m_currentAnimTime += time;
			// ���Ԃ��X�V
			m_pAnimController->AdvanceTime( time, NULL );
		}
		else if( m_loopType == INVERSE )
		{
			
			m_currentAnimTime += time;
			// ���Ԃ��X�V
			m_pAnimController->SetTrackPosition( m_currentId , m_animeDesc[m_currentId].loopTime - m_currentAnimTime);
			m_pAnimController->AdvanceTime( 0, NULL );
		}
	}
}
		
/*=======================================================
	�֐���	�F	DrawSetUp
	����	�F	�`�揀��
	����	�F	�Ȃ�
	�߂�l	�F�@�Ȃ�
========================================================*/
void AnimController::DrawSetUp()
{
	m_pAnimController->AdvanceTime( 0, NULL );
}

}