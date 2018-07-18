/**
*	@file	NW_Lib_CommonData.h
*
*	@brief	���L�f�[�^�N���X
*
*	@date	2013/10/17
*
*	@author	�F���@����
*/

#pragma once

namespace NW_Lib
{
	// �X�e�[�W�̐��ƂP�X�e�[�W�ɂ��郌�x���̐�
	const int NUM_OF_STAGES = 7;
	const int NUM_OF_LEVELS_INLEVEL = 5;

	/*
		���L�f�[�^�N���X
	*/

	//	Database Data
	
	class CommonData
	{
		public:

		private:
			struct OptionInfo
			{
				int bgmVolume;	//	BGM Volume  0 to 1.0f
				int sfxVolume;	//	SFX Volume  0 to 1.0f
				int language;		//	1 For ENG, 2 for JAP
			};

			struct LevelInfo
			{
				int ID;	//	ID for Level
				bool unlocked;	//	Is it Unlocked?
				int highScore;	//	High Score per Level
				int shortestTime;	//	Shortest Time per Level
			};

			int numOfLevels;	//	Number Of Levels in Game

			OptionInfo optionInfo;
			LevelInfo levelInfo[NUM_OF_STAGES*NUM_OF_LEVELS_INLEVEL];

			// �R���X�g���N�^
			CommonData();
			CommonData(const CommonData& val){(void)val;}
			CommonData& operator=(const CommonData& val){(void)val;return *this;}

			// �f�X�g���N�^
			~CommonData();


		public:
			/// �V���O���g����������
			static CommonData& GetInstance()
			{
				static CommonData instance;
				return instance;
			}

			// �I�v�V�����f�[�^�̎擾
			OptionInfo* GetOptionData()
			{
				return &optionInfo; 
			}

			LevelInfo* GetLevelInfo(int digit)
			{
				return &levelInfo[digit];
			}
	};
}