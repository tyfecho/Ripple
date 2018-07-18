/**
*	@file	Sound.h
*
*	@brief	Soundに関する処理
*
*	@date	2013/09/24
*
*	@author	吉田　将也
*	@author	皆光　健太
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
	 * @brief   コンストラクタ.
	 */
	Sound::Sound()       
	{
	}

	/**
	 * @brief   デストラクタ.
	 */
	Sound::~Sound()
	{
		Finalize();
	}

	/**
	 * @brief   初期化処理.
	 * 
	 * @param   filename ファイル名.
	 */
	void Sound::Initialize(char* filename)
	{
		std::string fstr = "works_romfiles/sound/";
		fstr += filename;
		fstr += ".txt";
		std::ifstream fs(fstr);

		//サウンドマネージャーの生成
		m_seMgr  = new CSoundManager();
		m_bgmMgr = new CSoundManager();
		m_bgmMgr->Initialize( m_hwnd ,DSSCL_NORMAL);
		m_seMgr->Initialize( m_hwnd ,DSSCL_NORMAL);

		DWORD flag = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS |
					DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;


		CSound** ppSE;
		CSound** ppBGM;

		std::string str;

		//終了まで読み込む
		while (getline (fs, str)) 
		{
			std::istringstream iss(str);

			//STREAMだったら
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

			//SEだったら
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
	 * @brief   解放処理.
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
	 * @brief   サウンドの更新.
	 */
	void Sound::Update()
	{

	}

	/**
	 * @brief   SEを登録する.
	 * 
	 * @param   soundname SoundIDの中にある使用する曲の変数名.
	 * 
	 * @returns 正常に登録されるとtrue.
	 * 
	 * @remarks エラーを吐いた際は出力を見てください.
	 */
	bool Sound::SE_Entry(unsigned int soundname)
	{
		if( m_seMax > 0 )
		{
			m_seList[soundname - m_bgmMax] = true;
			// 登録成功！
			return true;
		}

		return false;
	}

	/**
	 * @brief   SEの再生.
	 * 
	 * @param   soundname SoundIDの中にある使用する曲の変数名.
	 * 
	 * SEを再生します。再生中のSEとかぶります。同時再生可能数は7つです.
	 *  
	 * @remarks LOGに(1)が表示されれば成功.
	 */
	void Sound::SE_Play(unsigned int soundname)
	{
		if( m_seMax > 0 && m_seList[soundname - m_bgmMax] )
		{
			m_readSeMgr.Get(soundname)->Play();
		}
	}

	/**
	 * @brief   ストリームの再生.
	 * 
	 * @param   soundname	SoundIDの中にある使用する曲の変数名.
	 * 
	 * @param   fadeframe	フェードインの時間を指定する(デフォルト0).
	 *
	 * @param   fadeframe2	現在流している曲のフェードアウトの時間を指定する(デフォルト0).
	 * 
	 * 現在の曲を止め、指定した曲を流す.
	 */
	void Sound::STRM_Play(unsigned int soundname, int fadeframe, int fadeframe2)
	{
		//// 再生中ならストップ
		if( m_readBgmMgr.Get(soundname)->IsSoundPlaying() )
		{
			m_readBgmMgr.Get(soundname)->Stop();
		}
		//// 再生する
		m_readBgmMgr.Get(soundname)->Play(0,DSBPLAY_LOOPING);
		m_currentBgm = soundname;

	}

	/**
	 * @brief   SEの一時停止.
	 * 
	 * @param   soundname SoundIDの中にある使用する曲の変数名.
	 *
	 * @param   stop trueで一時停止：falseで再生.
	 * 
	 * @param   fadeframe フェードアウトの長さ(デフォルト0).
	 * 
	 * @remarks 全てのSEに適用.
	 */
	void Sound::SE_SetPause(unsigned int soundname, bool stop, int fadeframe)
	{
		m_readSeMgr.Get(soundname)->Stop();
	}

	/**
	 * @brief   全てのSEの一時停止.
	 *
	 * @param   stop trueで一時停止：falseで再生.
	 * 
	 * @param   fadeframe フェードアウトの長さ(デフォルト0).
	 * 
	 * @remarks 全てのSEに適用.
	 */
	void Sound::SE_SetPauseAll(bool stop, int fadeframe)
	{
		for(int i = 0; i < m_seMax; i ++)
		{
			m_readSeMgr.Get(m_bgmMax+i)->Stop();
		}
	}

	/**
	 * @brief   SEを止める.
	 * 
	 * @param   soundname SoundIDの中にある使用する曲の変数名.
	 * 
	 * @param   fadeframe フェードアウトの長さ(デフォルト0).
	 * 
	 * @remarks 全てのSEに適用.
	 */
	void Sound::SE_SetStop(unsigned int soundname, int fadeframe)
	{
		m_readSeMgr.Get(soundname)->Reset();
		m_readSeMgr.Get(soundname)->Stop();
	}

	/**
	 * @brief   全てのSEを止める.
	 * 
	 * @param   fadeframe フェードアウトの長さ(デフォルト0).
	 * 
	 * @remarks 全てのSEに適用.
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
	 * @brief   SEの音量を変える.
	 * 
	 * @param   soundname SoundIDの中にある使用する曲の変数名.
	 * 
	 * @param   volume 音量(0.0 ～ 1.0).
	 * 
	 * @param   fadeframe フェード(デフォルト0).
	 * 
	 * @remarks 音量は指定外の数値を入れると変更されません.
	 */
	void Sound::SE_SetVolume(unsigned int soundname, float volume, int fadeframe)
	{
		m_seMgr->SetVolume( volume );
	}

	/**
	 * @brief   全てのSEの音量を変える.
	 * 
	 * @param   volume 音量(0.0 ～ 1.0).
	 * 
	 * @param   fadeframe フェード(デフォルト0).
	 * 
	 * @remarks 音量は指定外の数値を入れると変更されません.
	 */
	void Sound::SE_SetVolumeAll(float volume, int fadeframe)
	{
		m_seMgr->SetVolume( volume );
	}

	/**
	 * @brief   SEの音程を変更する.
	 * 
	 * @param   soundname SoundIDの中にある使用する曲の変数名.
	 * 
	 * @param   Pitch 音程(デフォルト1.0f).
	 *
	 * @remarks 全てのSEに適用。マイナス値は使用不可.
	 */
	void Sound::SE_SetPitch(unsigned int soundname, float pitch)
	{
		// DirectX no implement
	}

	/**
	 * @brief   全てのSEの音程を変更する.
	 * 
	 * @param   Pitch 音程(デフォルト1.0f).
	 *
	 * @remarks 全てのSEに適用。マイナス値は使用不可.
	 */
	void Sound::SE_SetPitchAll(float pitch)
	{
		// DirectX no implement
	}
		
	/**
	 * @brief   SEの再生チェック
	 * 
	 * @returns SEが再生しているならtrue、していないならfalse.
	 *
	 * @param   soundname SoundIDの中にある使用する曲の変数名.
	 * 
	 * SEが再生されているかチェックする
	 */
	bool Sound::SE_CheckPlay(unsigned int soundname)
	{
		return m_readSeMgr.Get(m_bgmMax+soundname)->IsSoundPlaying() != 0;
	}

	/**
	 * @brief   ストリームの一時停止.
	 * 
	 * @param   stop trueで一時停止：falseで再生.
	 * 
	 * @param   fadeframe 一時停止のフェードアウトのフレーム数(デフォルト0).
	 * 
	 * @remarks 全てのSEに適用. 
	 */
	void Sound::STRM_SetPause(bool stop, int fadeframe)
	{
		for(int i = 0; i < m_bgmMax; i++)
		{
			m_readBgmMgr.Get(m_readBgmMgr.GetId(i))->Stop();
		}
	}

	/**
	 * @brief   ストリームの停止.
	 * 
	 * @param   fadeframe フェードアウトの長さ(デフォルト0).
	 * 
	 * @remarks 全てのストリームに適応.
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
	 * @brief   ストリームの音量を変える.
	 * 
	 * @param   volume 音量(0.0 ～ 1.0).
	 * 
	 * @param   fadeframe フェード.
	 * 
	 * @remarks 音量は指定外の数値を入れると変更されません.
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
	 * @brief   ストリームの音程を変更する.
	 * 
	 * @param   Pitch 音程(デフォルト1.0f).
	 * 
	 * @remarks 全てのストリームに適用。マイナス値は使用不可.   
	 */
	void Sound::STRM_SetPitch(float pitch)
	{
		// DirectX no implement
	}

	/**
	 * @brief   ストリームの再生チェック
	 * 
	 * @returns ストリームが再生しているならtrue、していないならfalse.
	 * 
	 * ストリームが再生されているかチェックする
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
