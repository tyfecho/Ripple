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
#pragma once

#include <map>
#include "../System/NW_Lib_common.h"
#include "NW_Lib_dsutil.h"
#include "../System/NW_Lib_ResourceManager.h"

// SE�𓯎��Đ��ł��鐔
#define SE_PLAY_MAX 5

namespace NW_Lib
{
	class Sound : public Common
	{
	private:

		//BGM SE
		CSoundManager* m_seMgr;
		CSoundManager* m_bgmMgr;
		
		ResourceManager<CSound*> m_readSeMgr;
		ResourceManager<CSound*> m_readBgmMgr;

		int m_seMax,m_bgmMax;
		int m_currentBgm;
		bool m_seList[512];

		struct ReadSoundInfo
		{
			int id;
			std::string str;
			int handle;
		};

		/**
		 * @brief   �R���X�g���N�^.
		 */
		Sound();
		Sound(const Sound& val){(void)val;}
		Sound& operator=(const Sound& val){(void)val;return *this;}


		/**
		 * @brief   �f�X�g���N�^.
		 */
		~Sound();

	public:

		/// �V���O���g����������
		static Sound& GetInstance()
		{
			static Sound instance;
			return instance;
		}

		/**
		 * @brief   ����������.
		 */
		void Initialize(char* filename);

		/**
		 * @brief   �������.
		 */
		void Finalize();

		/**
		 * @brief   �T�E���h�̍X�V.
		 */
		void Update();

		/**
		 * @brief   SE��o�^����.
		 * 
		 * @param   soundname SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
		 * 
		 * @returns ����ɓo�^������true.
		 * 
		 * @remarks �G���[��f�����ۂ͏o�͂����Ă�������.
		 */
		bool SE_Entry(unsigned int soundname);

		/**
		 * @brief   SE�̍Đ�.
		 * 
		 * @param   soundname SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
		 * 
		 * SE���Đ����܂��B�Đ�����SE�Ƃ��Ԃ�܂��B�����Đ��\����7�ł�.
		 *  
		 * @remarks LOG��(1)���\�������ΐ���.
		 */
		void SE_Play(unsigned int soundname);

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
		void STRM_Play(unsigned int soundname, int fadeframe = 0, int fadeframe2 = 0);

		/**
		 * @brief   SE�̈ꎞ��~.
		 * 
		 * @param   soundname SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
		 * 
		 * @param   stop true�ňꎞ��~�Ffalse�ōĐ�.
		 * 
		 * @param   fadename �t�F�[�h�A�E�g�̒���(�f�t�H���g0).
		 * 
		 * @remarks �S�Ă�SE�ɓK�p.
		 */
		void SE_SetPause(unsigned int soundname, bool stop, int fadename = 0);

		/**
		 * @brief   �S�Ă�SE�̈ꎞ��~.
		 * 
		 * @param   stop true�ňꎞ��~�Ffalse�ōĐ�.
		 * 
		 * @param   fadename �t�F�[�h�A�E�g�̒���(�f�t�H���g0).
		 * 
		 * @remarks �S�Ă�SE�ɓK�p.
		 */
		void SE_SetPauseAll(bool stop, int feadframe = 0);

		/**
		 * @brief   SE���~�߂�.
		 * 
		 * @param   soundname SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
		 * 
		 * @param   feadframe �t�F�[�h�A�E�g�̒���(�f�t�H���g0).
		 * 
		 * @remarks �S�Ă�SE�ɓK�p.
		 */
		void SE_SetStop(unsigned int soundname, int feadframe = 0);

		/**
		 * @brief   �S�Ă�SE���~�߂�.
		 * 
		 * @param   feadframe �t�F�[�h�A�E�g�̒���(�f�t�H���g0).
		 * 
		 * @remarks �S�Ă�SE�ɓK�p.
		 */
		void SE_SetStopAll(int feadframe = 0);

		/**
		 * @brief   SE�̉��ʂ�ς���.
		 * 
		 * @param   soundname SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
		 * 
		 * @param   volume ����(0.0 �` 1.0).
		 * 
		 * @param   fadename �t�F�[�h�̒���(�f�t�H���g0).
		 * 
		 * @remarks ���ʂ͎w��O�̐��l������ƕύX����܂���.
		 */
		void SE_SetVolume(unsigned int soundname, float volume = 1.0f, int fadename = 0);

		/**
		 * @brief   �S�Ă�SE�̉��ʂ�ς���.
		 * 
		 * @param   volume ����(0.0 �` 1.0).
		 * 
		 * @param   fadename �t�F�[�h�̒���(�f�t�H���g0).
		 * 
		 * @remarks ���ʂ͎w��O�̐��l������ƕύX����܂���.
		 */
		void SE_SetVolumeAll(float volume = 1.0f, int fadename = 0);

		/**
		 * @brief   SE�̉�����ύX����.
		 * 
		 * @param   soundname SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
		 * 
		 * @param   Pitch ����(�f�t�H���g1.0f).
		 * 
		 * @remarks �S�Ă�SE�ɓK�p�B�}�C�i�X�l�͎g�p�s��.
		 */
		void SE_SetPitch(unsigned int soundname, float pitch = 1.0f);

		/**
		 * @brief   �S�Ă�SE�̉�����ύX����.
		 * 
		 * @param   Pitch ����(�f�t�H���g1.0f).
		 * 
		 * @remarks �S�Ă�SE�ɓK�p�B�}�C�i�X�l�͎g�p�s��.
		 */
		void SE_SetPitchAll(float pitch = 1.0f);
		
		/**
		 * @brief   SE�̍Đ��`�F�b�N
		 * 
		 * @returns SE���Đ����Ă���Ȃ�true�A���Ă��Ȃ��Ȃ�false.
		 *
		 * @param   soundname SoundID�̒��ɂ���g�p����Ȃ̕ϐ���.
		 * 
		 * SE���Đ�����Ă��邩�`�F�b�N����
		 */
		bool SE_CheckPlay(unsigned int soundname);

		/**
		 * @brief   �X�g���[���̈ꎞ��~.
		 * 
		 * @param   stop true�ňꎞ��~�Ffalse�ōĐ�.
		 * 
		 * @param   fadename �ꎞ��~�̃t�F�[�h�A�E�g�̃t���[����(�f�t�H���g0).
		 * 
		 * @remarks �S�Ă�SE�ɓK�p. 
		 */
		void STRM_SetPause(bool stop, int fadename = 0);

		/**
		 * @brief   �X�g���[���̒�~.
		 * 
		 * @param   fadename �t�F�[�h�A�E�g�̒���(�f�t�H���g0).
		 * 
		 * @remarks �S�ẴX�g���[���ɓK��.
		 */
		void STRM_SetStop(int fadename = 0);

		/**
		 * @brief   �X�g���[���̉��ʂ�ς���.
		 * 
		 * @param   volume ����(0.0 �` 1.0).
		 * 
		 * @param   fadename �t�F�[�h.
		 * 
		 * @remarks ���ʂ͎w��O�̐��l������ƕύX����܂���.
		 */
		void STRM_SetVolume(float volume = 1.0f, int fadename = 0);

		/**
		 * @brief   �X�g���[���̉�����ύX����.
		 * 
		 * @param   Pitch ����(�f�t�H���g1.0f).
		 * 
		 * @remarks �S�ẴX�g���[���ɓK�p�B�}�C�i�X�l�͎g�p�s��.   
		 */
		void STRM_SetPitch(float pitch = 1.0f);

		/**
		 * @brief   �X�g���[���̃t�F�[�h�C��.
		 * 
		 * @param   fadename �t�F�[�h�C���̒���(�f�t�H���g0).
		 * 
		 * �X�g���[���̍Đ��J�n���Ƀt�F�[�h�C��������.
		 */
		void STRM_SetFadein(int fadename = 0);
		
		/**
		 * @brief   �X�g���[���̍Đ��`�F�b�N
		 * 
		 * @returns �X�g���[�����Đ����Ă���Ȃ�true�A���Ă��Ȃ��Ȃ�false.
		 * 
		 * �X�g���[�����Đ�����Ă��邩�`�F�b�N����
		 */
		bool STRM_CheckPlay();

		/**
		 * @brief   ���X�g���[���̍Đ��`�F�b�N
		 * 
		 * @returns ���X�g���[�����Đ����Ă���Ȃ�true�A���Ă��Ȃ��Ȃ�false.
		 * 
		 * �X�g���[�����Đ�����Ă��邩�`�F�b�N����
		 */
		bool STRM_SubCheckPlay();

		/**
		 * @brief   �f�o�C�X�A���P�[�^���Z�b�g����
		 * 
		 * @param   deviceAlloc �f�o�C�X�A���P�[�^.
		 */
		//void SetDevice(System::DeviceAllocator* deviceAlloc);
	};

} // NW_Lib

