/**
*	@file	Sound.h
*
*	@brief	Sound�Ɋւ��鏈��
*
*	@date	2013/09/24
*
*	@author	�g�c�@����
*	@author	�F���@����
*/

#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <string>

#include "../Utility/NW_Lib_utility.h"
#include "NW_Lib_Sound.h"

namespace
{
	const int SOUND_THREAD_PRIORITY = 4;
	const int LOAD_THREAD_PRIORITY = 3;
	const int SOUND_HEAP_SIZE = 1 * 1024 * 1024;
	const char FILE[] = "rom:/sound/";
}

namespace NW_Lib
{

	/**
	 * @brief   �R���X�g���N�^.
	 */
	Sound::Sound()       
	{
	}

	/**
	 * @brief   �f�X�g���N�^.
	 */
	Sound::~Sound()
	{
		Finalize();
	}

	/**
	 * @brief   ����������.
	 * 
	 * @param   filename �t�@�C����.
	 */
	void Sound::Initialize(char* filename)
	{
		std::string fstr = "works_romfiles/sound/";
		fstr += filename;
		fstr += ".txt";
		std::ifstream fs(fstr);

		//�T�E���h�}�l�[�W���[�̐���
		m_seMgr  = new CSoundManager();
		m_bgmMgr = new CSoundManager();
		m_bgmMgr->Initialize( m_hwnd ,DSSCL_NORMAL);
		m_seMgr->Initialize( m_hwnd ,DSSCL_NORMAL);

		DWORD flag = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS |
					DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;


		CSound** ppSE;
		CSound** ppBGM;

		std::string str;

		//�I���܂œǂݍ���
		while (getline (fs, str)) 
		{
			std::istringstream iss(str);

			//STREAM��������
			if( str.find("strm/") != std::string::npos)
			{
				ppBGM = new CSound*;
				ReadSoundInfo temp;

				iss >> temp.id >> temp.str >> hex >> temp.handle >> dec;

				char filename[1024];
				strcpy(filename , "works_romfiles/sound/");
				strcat(filename , temp.str.c_str());

				HRESULT hr = m_bgmMgr->Create( ppBGM	, filename , flag, GUID_NULL, 1 );
				m_readBgmMgr.Entry(temp.id,*ppBGM);

				++m_bgmMax;
			}

			//SE��������
			else if(str.find("se/") != std::string::npos )
			{
				ppSE = new CSound*;
				ReadSoundInfo temp;

				iss >> temp.id >> temp.str >> hex >> temp.handle >> dec;

				char filename[1024];
				strcpy(filename , "works_romfiles/sound/");
				strcat(filename , temp.str.c_str());

				HRESULT hr = m_seMgr->Create( ppSE , filename , flag, GUID_NULL, SE_PLAY_MAX );
				m_readSeMgr.Entry(temp.id,*ppSE);

				++m_seMax;
			}
		}

		if( m_seMax > 0 )
		{
			//m_seList = new bool[m_seMax];

			for( int i = 0 ; i < m_seMax ; ++i )
			{
				m_seList[i] = false;
			}
		}

		m_currentBgm = -1;
		
	}

	/**
	 * @brief   �������.
	 */
	void Sound::Finalize()
	{
		m_bgmMax = 0;
		m_seMax = 0;
		m_readBgmMgr.AllRemove();
		m_readSeMgr.AllRemove();
		SafeDelete(m_seMgr);
		SafeDelete(m_bgmMgr);
	}

	/**
	 * @brief   �T�E���h�̍X�V.
	 */
	void Sound::Update()
	{

	}

	/**
	 * @brief   SE��o�^����.
	 * 
	 * @param   soundname SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
	 * 
	 * @returns ����ɓo�^������true.
	 * 
	 * @remarks �G���[��f�����ۂ͏o�͂����Ă�������.
	 */
	bool Sound::SE_Entry(unsigned int soundname)
	{
		if( m_seMax > 0 )
		{
			m_seList[soundname - m_bgmMax] = true;
			// �o�^�����I
			return true;
		}

		return false;
	}

	/**
	 * @brief   SE�̍Đ�.
	 * 
	 * @param   soundname SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
	 * 
	 * SE���Đ����܂��B�Đ�����SE�Ƃ��Ԃ�܂��B�����Đ��\����7�ł�.
	 *  
	 * @remarks LOG��(1)���\�������ΐ���.
	 */
	void Sound::SE_Play(unsigned int soundname)
	{
		if( m_seMax > 0 && m_seList[soundname - m_bgmMax] )
		{
			m_readSeMgr.Get(soundname)->Play();
		}
	}

	/**
	 * @brief   �X�g���[���̍Đ�.
	 * 
	 * @param   soundname	SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
	 * 
	 * @param   fadeframe	�t�F�[�h�C���̎��Ԃ��w�肷��(�f�t�H���g0).
	 *
	 * @param   fadeframe2	���ݗ����Ă���Ȃ̃t�F�[�h�A�E�g�̎��Ԃ��w�肷��(�f�t�H���g0).
	 * 
	 * ���݂̋Ȃ��~�߁A�w�肵���Ȃ𗬂�.
	 */
	void Sound::STRM_Play(unsigned int soundname, int fadeframe, int fadeframe2)
	{
		//// �Đ����Ȃ�X�g�b�v
		if( m_readBgmMgr.Get(soundname)->IsSoundPlaying() )
		{
			m_readBgmMgr.Get(soundname)->Stop();
		}
		//// �Đ�����
		m_readBgmMgr.Get(soundname)->Play(0,DSBPLAY_LOOPING);
		m_currentBgm = soundname;

	}

	/**
	 * @brief   SE�̈ꎞ��~.
	 * 
	 * @param   soundname SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
	 *
	 * @param   stop true�ňꎞ��~�Ffalse�ōĐ�.
	 * 
	 * @param   fadeframe �t�F�[�h�A�E�g�̒���(�f�t�H���g0).
	 * 
	 * @remarks �S�Ă�SE�ɓK�p.
	 */
	void Sound::SE_SetPause(unsigned int soundname, bool stop, int fadeframe)
	{
		m_readSeMgr.Get(soundname)->Stop();
	}

	/**
	 * @brief   �S�Ă�SE�̈ꎞ��~.
	 *
	 * @param   stop true�ňꎞ��~�Ffalse�ōĐ�.
	 * 
	 * @param   fadeframe �t�F�[�h�A�E�g�̒���(�f�t�H���g0).
	 * 
	 * @remarks �S�Ă�SE�ɓK�p.
	 */
	void Sound::SE_SetPauseAll(bool stop, int fadeframe)
	{
		for(int i = 0; i < m_seMax; i ++)
		{
			m_readSeMgr.Get(m_bgmMax+i)->Stop();
		}
	}

	/**
	 * @brief   SE���~�߂�.
	 * 
	 * @param   soundname SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
	 * 
	 * @param   fadeframe �t�F�[�h�A�E�g�̒���(�f�t�H���g0).
	 * 
	 * @remarks �S�Ă�SE�ɓK�p.
	 */
	void Sound::SE_SetStop(unsigned int soundname, int fadeframe)
	{
		m_readSeMgr.Get(soundname)->Reset();
		m_readSeMgr.Get(soundname)->Stop();
	}

	/**
	 * @brief   �S�Ă�SE���~�߂�.
	 * 
	 * @param   fadeframe �t�F�[�h�A�E�g�̒���(�f�t�H���g0).
	 * 
	 * @remarks �S�Ă�SE�ɓK�p.
	 */
	void Sound::SE_SetStopAll(int fadeframe)
	{
		for(int i = 0; i < m_seMax; i ++)
		{
			m_readSeMgr.Get(m_bgmMax+i)->Reset();
			m_readSeMgr.Get(m_bgmMax+i)->Stop();
		}
	}

	/**
	 * @brief   SE�̉��ʂ�ς���.
	 * 
	 * @param   soundname SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
	 * 
	 * @param   volume ����(0.0 �` 1.0).
	 * 
	 * @param   fadeframe �t�F�[�h(�f�t�H���g0).
	 * 
	 * @remarks ���ʂ͎w��O�̐��l������ƕύX����܂���.
	 */
	void Sound::SE_SetVolume(unsigned int soundname, float volume, int fadeframe)
	{
		m_seMgr->SetVolume( volume );
	}

	/**
	 * @brief   �S�Ă�SE�̉��ʂ�ς���.
	 * 
	 * @param   volume ����(0.0 �` 1.0).
	 * 
	 * @param   fadeframe �t�F�[�h(�f�t�H���g0).
	 * 
	 * @remarks ���ʂ͎w��O�̐��l������ƕύX����܂���.
	 */
	void Sound::SE_SetVolumeAll(float volume, int fadeframe)
	{
		m_seMgr->SetVolume( volume );
	}

	/**
	 * @brief   SE�̉�����ύX����.
	 * 
	 * @param   soundname SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
	 * 
	 * @param   Pitch ����(�f�t�H���g1.0f).
	 *
	 * @remarks �S�Ă�SE�ɓK�p�B�}�C�i�X�l�͎g�p�s��.
	 */
	void Sound::SE_SetPitch(unsigned int soundname, float pitch)
	{
		// DirectX no implement
	}

	/**
	 * @brief   �S�Ă�SE�̉�����ύX����.
	 * 
	 * @param   Pitch ����(�f�t�H���g1.0f).
	 *
	 * @remarks �S�Ă�SE�ɓK�p�B�}�C�i�X�l�͎g�p�s��.
	 */
	void Sound::SE_SetPitchAll(float pitch)
	{
		// DirectX no implement
	}
		
	/**
	 * @brief   SE�̍Đ��`�F�b�N
	 * 
	 * @returns SE���Đ����Ă���Ȃ�true�A���Ă��Ȃ��Ȃ�false.
	 *
	 * @param   soundname SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
	 * 
	 * SE���Đ�����Ă��邩�`�F�b�N����
	 */
	bool Sound::SE_CheckPlay(unsigned int soundname)
	{
		return m_readSeMgr.Get(m_bgmMax+soundname)->IsSoundPlaying() != 0;
	}

	/**
	 * @brief   �X�g���[���̈ꎞ��~.
	 * 
	 * @param   stop true�ňꎞ��~�Ffalse�ōĐ�.
	 * 
	 * @param   fadeframe �ꎞ��~�̃t�F�[�h�A�E�g�̃t���[����(�f�t�H���g0).
	 * 
	 * @remarks �S�Ă�SE�ɓK�p. 
	 */
	void Sound::STRM_SetPause(bool stop, int fadeframe)
	{
		for(int i = 0; i < m_bgmMax; i++)
		{
			m_readBgmMgr.Get(m_readBgmMgr.GetId(i))->Stop();
		}
	}

	/**
	 * @brief   �X�g���[���̒�~.
	 * 
	 * @param   fadeframe �t�F�[�h�A�E�g�̒���(�f�t�H���g0).
	 * 
	 * @remarks �S�ẴX�g���[���ɓK��.
	 */
	void Sound::STRM_SetStop(int fadeframe)
	{
		for(int i = 0; i < m_bgmMax; i++)
		{
			m_readBgmMgr.Get(m_readBgmMgr.GetId(i))->Stop();
			m_readBgmMgr.Get(m_readBgmMgr.GetId(i))->Reset();
		}
	}

	/**
	 * @brief   �X�g���[���̉��ʂ�ς���.
	 * 
	 * @param   volume ����(0.0 �` 1.0).
	 * 
	 * @param   fadeframe �t�F�[�h.
	 * 
	 * @remarks ���ʂ͎w��O�̐��l������ƕύX����܂���.
	 */
	void Sound::STRM_SetVolume(float volume, int fadeframe)
	{
		if( m_currentBgm != -1 )
		{
			m_readBgmMgr.Get(m_currentBgm)->Stop();
			m_readBgmMgr.Get(m_currentBgm)->Play();
			m_bgmMgr->SetVolume( volume );
		}
	}

	/**
	 * @brief   �X�g���[���̉�����ύX����.
	 * 
	 * @param   Pitch ����(�f�t�H���g1.0f).
	 * 
	 * @remarks �S�ẴX�g���[���ɓK�p�B�}�C�i�X�l�͎g�p�s��.   
	 */
	void Sound::STRM_SetPitch(float pitch)
	{
		// DirectX no implement
	}

	/**
	 * @brief   �X�g���[���̍Đ��`�F�b�N
	 * 
	 * @returns �X�g���[�����Đ����Ă���Ȃ�true�A���Ă��Ȃ��Ȃ�false.
	 * 
	 * �X�g���[�����Đ�����Ă��邩�`�F�b�N����
	 */
	bool Sound::STRM_CheckPlay()
	{
		for(int i = 0; i < m_bgmMax; i++)
		{
			if( m_readBgmMgr.Get(m_readBgmMgr.GetId(i))->IsSoundPlaying())
			{
				return true;
			}
		}

		return false;
	}

} // NW_Lib
