/*-------------------------------------------------------------------------

	�N���X���@�F�@���[�h�A�j���[�V�����Ǘ��N���X
	�쐬�ҁ@�@�F�@�F���@����
	�ŏI�X�V���F�@2012 / 03 / 15

-------------------------------------------------------------------------*/
#include "LoadAnimation.h"

#include "../../NW_Lib/Utility/NW_Lib_utility.h"
#include "../../NW_Lib/Graphics/NW_Lib_SpriteDrawer.h"

namespace Game
{

static const int CarpAnimeIndex[16] = { 0,1,2,3,4,3,2,1,0,5,6,7,8,7,6,5 };

/*=======================================================
	�֐���	�F	LoadAnimation
	����	�F	�R���X�g���N�^
	����	�F	�Ȃ�
	�߂�l	�F�@�Ȃ�
========================================================*/
LoadAnimation::LoadAnimation()
: m_loadMode(LOADIN)
, m_beforeMode(LOADUNKNOWN)
, m_animeTime(m_loadMaxTimer)
, m_loadMaxTimer(30)
, m_oneFlameTime(1.0f / m_loadMaxTimer)
{ 
	m_loadTex = new NW_Lib::Texture( "TitleScreenSprite.tga" );
	m_loadTex2 = new NW_Lib::Texture( "wave.tga" );
	m_loadTexCarp[0] = new NW_Lib::Texture( "LevelSelectScene/Carp anime.tga" );
	m_loadTexCarp[1] = new NW_Lib::Texture( "LevelSelectScene/carp anime gley.tga" );
	m_loadTexCarp[2] = new NW_Lib::Texture( "LevelSelectScene/carp anime gold.tga" );

	m_texPosX = 0.0f;

	m_enable = false;
}

/*=======================================================
	�֐���	�F	~LoadAnimation
	����	�F	�f�X�g���N�^
	����	�F	�Ȃ�
	�߂�l	�F�@�Ȃ�
========================================================*/
LoadAnimation::~LoadAnimation()
{
	SafeDelete(m_loadTex);
	SafeDelete(m_loadTex2);
	for(int i = 0 ; i < 3 ; ++i)
	{
		SafeDelete(m_loadTexCarp[i]);
	}
}

/*=======================================================
	�֐���	�F	Initialize
	����	�F	����������
	����	�F	�Ȃ�
	�߂�l	�F�@�Ȃ�
========================================================*/
void LoadAnimation::Initialize()
{
	int typeRnd = 0;
	for(int i = 0 ; i < CarpNum ; ++i)
	{
		m_carpUpper[i].anime = Random() % 36;
		m_carpLower[i].anime = Random() % 36;
		m_carpUpper[i].pos.y = 100 + Random() % 120 - 100.0f;
		m_carpLower[i].pos.y = 100 + Random() % 120 - 60.0f;
		m_carpUpper[i].pos.x = 40.0f + Random() % 40 * 8.0f;
		m_carpLower[i].pos.x = 40.0f + Random() % 40 * 8.0f;
		m_carpUpper[i].size = (Random() % 100 / 100.0f) * 0.7f - 0.1f + 1.3f;
		m_carpLower[i].size = (Random() % 100 / 100.0f) * 0.7f - 0.1f + 1.3f;

		typeRnd = Random() % 1000;
		if	   ( typeRnd < 600 ) 
			m_carpUpper[i].type  = 0;
		else if( typeRnd < 995 && typeRnd >= 600 )
			m_carpUpper[i].type  = 1;
		else
		{
			m_carpUpper[i].type  = 2;
			m_carpUpper[i].size = 2.5f;
		}

		typeRnd = Random() % 1000;
		if	   ( typeRnd < 600 ) 
			m_carpLower[i].type  = 0;
		else if( typeRnd < 995 && typeRnd >= 600 )
			m_carpLower[i].type  = 1;
		else
		{
			m_carpLower[i].type  = 2;
			m_carpLower[i].size = 2.5f;
		}
	}
}

/*=======================================================
	�֐���	�F	isLoadAnimationEnd
	����	�F	�t�F�[�h�̏I��肩
	����	�F	�Ȃ�
	�߂�l	�F�@����
========================================================*/
bool LoadAnimation::isLoadAnimationEnd()
{
	if( m_loadMode == LOADEND)
	{
		return true;
	}
	else
	{
		return false;	
	}

}

/*=======================================================
	�֐���	�F	isLoadAnimationNow
	����	�F	�t�F�[�h����
	����	�F	�Ȃ�
	�߂�l	�F�@����
========================================================*/
bool LoadAnimation::isLoadAnimationNow()
{
	if( m_animeTime > 0)
	{
		return true;
	}
	else
	{
		return false;	
	}

}

/*=======================================================
	�֐���	�F	LoadAnimationModeChanger
	����	�F	�t�F�[�h��Ԃ�ς���
	����	�F	�ύX������
	�߂�l	�F�@�Ȃ�
========================================================*/
void LoadAnimation::LoadAnimationModeChanger(LOAD next_load )
{
	if( m_loadMode != next_load && m_beforeMode != LOADUNKNOWN)
	{
		m_loadMode = next_load;
		m_animeTime = m_loadMaxTimer;

		if( m_loadMode == LOADOUT )
		{
			Initialize();
		}
		
		if( !m_enable )
		{
			m_animeTime = 10;
		}
	}
}

/*=======================================================
	�֐���	�F	LoadAnimationDraw
	����	�F	�`��֐�
	����	�F	�X�N���[��
	�߂�l	�F�@�Ȃ�
========================================================*/
void LoadAnimation::LoadAnimationDraw( NW_Lib::DrawingScreen screenNum )
{

	float alpha = 0;			//�A���t�@�l
	m_animeTime--;				//�A�j����i�߂�

	if( !m_enable )
	{
		//m_animeTime = 0;
	}

	if( m_loadMode == LOADOUT)
	{
		if( m_animeTime <= 0)
		{
			//������
			m_animeTime	= 0;
			m_loadMode	= LOADEND;
			m_beforeMode = LOADOUT;
		}

		// Load out animation
		m_texPosX = 280.0f - 360.0f * (1.0f - static_cast<float>(m_animeTime) / m_loadMaxTimer);


	}
	else if( m_loadMode == LOADIN)
	{
		if( m_animeTime <= 0)
		{
			//������
			m_animeTime	= 0;
			m_loadMode	= LOADEND;
			m_beforeMode = LOADIN;
		}
		
		// Load in animation
		m_texPosX = 280.0f - (360.0f + 420.0f * (1.0f - static_cast<float>(m_animeTime) / m_loadMaxTimer));

	}
	
	if( m_enable && ( m_loadMode != LOADEND && m_loadMode != LOADUNKNOWN || m_loadMode == LOADEND && m_beforeMode == LOADOUT ) )
	{
		if( screenNum == NW_Lib::UPPER_SCREEN_DRAWING )
		{
			//Drawing
			NW_Lib::SpriteDrawer::Draw(m_loadTex2,40.0f + m_texPosX,0.0f,440.0f,242.0f,0.0f,0.0f,440.0f,240.0f);

			for(int i = 0 ; i < CarpNum ; ++i)
			{
				m_carpUpper[i].anime++;
				m_carpUpper[i].anime %= 32;

				NW_Lib::SpriteDrawer::Draw(m_loadTexCarp[m_carpUpper[i].type],m_texPosX - 35.0f + m_carpUpper[i].pos.x,m_carpUpper[i].pos.y,84.0f * m_carpUpper[i].size,42.0f * m_carpUpper[i].size ,(CarpAnimeIndex[m_carpUpper[i].anime / 2]) % 3 * 84.0f,(CarpAnimeIndex[m_carpUpper[i].anime / 2]) / 3 * 42.0f ,84.0f,42.0f);
			}
		}
		else if( screenNum == NW_Lib::LOWER_SCREEN_DRAWING )
		{
			//Drawing
			NW_Lib::SpriteDrawer::Draw(m_loadTex2,m_texPosX,0.0f,440.0f,242.0f,0.0f,0.0f,440.0f,240.0f);

			for(int i = 0 ; i < CarpNum ; ++i)
			{
				m_carpLower[i].anime++;
				m_carpLower[i].anime %= 32;

				NW_Lib::SpriteDrawer::Draw(m_loadTexCarp[m_carpLower[i].type],m_texPosX - 35.0f + m_carpLower[i].pos.x,m_carpLower[i].pos.y,84.0f * m_carpLower[i].size,42.0f * m_carpLower[i].size ,(CarpAnimeIndex[m_carpLower[i].anime / 2]) % 3 * 84.0f,(CarpAnimeIndex[m_carpLower[i].anime / 2]) / 3 * 42.0f ,84.0f,42.0f);
			}
		}
	
		if( screenNum == NW_Lib::UPPER_SCREEN_DRAWING )
		{	
			//LogFrame
			NW_Lib::SpriteDrawer::Draw(m_loadTex,          0.0f,0.0f,40.0f,240.0f,          0.0f,0.0f,40.0f,240.0f);
			NW_Lib::SpriteDrawer::Draw(m_loadTex,40.0f + 320.0f,0.0f,40.0f,240.0f,40.0f + 320.0f,0.0f,40.0f,240.0f);
		}
	}
}

/*=======================================================
	�֐���	�F	SetLoadAnimationTime
	����	�F	�t�F�[�h�ɂ����鎞��
	����	�F	�t�F�[�h�ɂ����鎞��
	�߂�l	�F�@�Ȃ�
========================================================*/
void LoadAnimation::SetLoadAnimationTime(int num )
{
	m_loadMaxTimer = num;
	m_animeTime = num;
	m_oneFlameTime = 1.0f / m_loadMaxTimer;
}

}