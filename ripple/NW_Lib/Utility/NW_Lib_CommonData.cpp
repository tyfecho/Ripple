/**
*	@file	NW_Lib_CommonData.h
*
*	@brief	���L�f�[�^�N���X
*
*	@date	2013/10/17
*
*	@author	�F���@����
*/

#include "NW_Lib_CommonData.h"

namespace NW_Lib
{
	// �R���X�g���N�^
	CommonData::CommonData()
	{
		optionInfo.bgmVolume = 4;
		optionInfo.sfxVolume= 4;
		optionInfo.language = 1;	//	Set to ENG by default

		this->numOfLevels = NUM_OF_STAGES*NUM_OF_LEVELS_INLEVEL;
	
		for ( int level = 0; level < this->numOfLevels; level++ )
		{
			this->levelInfo[level].ID = level+1;
			this->levelInfo[level].highScore = 0;
			this->levelInfo[level].shortestTime = 300000;	//	set all score to 0
			if ( level != 0 )	//	Check to see if it is not first level
				this->levelInfo[level].unlocked = false;
			else
				this->levelInfo[level].unlocked = true;
		}
	}

	// �f�X�g���N�^
	CommonData::~CommonData()
	{

	}
}