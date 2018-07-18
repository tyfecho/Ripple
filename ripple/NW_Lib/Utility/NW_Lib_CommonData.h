/**
*	@file	NW_Lib_CommonData.h
*
*	@brief	共有データクラス
*
*	@date	2013/10/17
*
*	@author	皆光　健太
*/

#pragma once

namespace NW_Lib
{
	// ステージの数と１ステージにあるレベルの数
	const int NUM_OF_STAGES = 7;
	const int NUM_OF_LEVELS_INLEVEL = 5;

	/*
		共有データクラス
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

			// コンストラクタ
			CommonData();
			CommonData(const CommonData& val){(void)val;}
			CommonData& operator=(const CommonData& val){(void)val;return *this;}

			// デストラクタ
			~CommonData();


		public:
			/// シングルトン化させる
			static CommonData& GetInstance()
			{
				static CommonData instance;
				return instance;
			}

			// オプションデータの取得
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