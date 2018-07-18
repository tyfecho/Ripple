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
#pragma once

#include <map>
#include "../System/NW_Lib_common.h"
#include "NW_Lib_dsutil.h"
#include "../System/NW_Lib_ResourceManager.h"

// SEを同時再生できる数
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
		 * @brief   コンストラクタ.
		 */
		Sound();
		Sound(const Sound& val){(void)val;}
		Sound& operator=(const Sound& val){(void)val;return *this;}


		/**
		 * @brief   デストラクタ.
		 */
		~Sound();

	public:

		/// シングルトン化させる
		static Sound& GetInstance()
		{
			static Sound instance;
			return instance;
		}

		/**
		 * @brief   初期化処理.
		 */
		void Initialize(char* filename);

		/**
		 * @brief   解放処理.
		 */
		void Finalize();

		/**
		 * @brief   サウンドの更新.
		 */
		void Update();

		/**
		 * @brief   SEを登録する.
		 * 
		 * @param   soundname SoundIDの中にある使用する曲の変数名.
		 * 
		 * @returns 正常に登録されるとtrue.
		 * 
		 * @remarks エラーを吐いた際は出力を見てください.
		 */
		bool SE_Entry(unsigned int soundname);

		/**
		 * @brief   SEの再生.
		 * 
		 * @param   soundname SoundIDの中にある使用する曲の変数名.
		 * 
		 * SEを再生します。再生中のSEとかぶります。同時再生可能数は7つです.
		 *  
		 * @remarks LOGに(1)が表示されれば成功.
		 */
		void SE_Play(unsigned int soundname);

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
		void STRM_Play(unsigned int soundname, int fadeframe = 0, int fadeframe2 = 0);

		/**
		 * @brief   SEの一時停止.
		 * 
		 * @param   soundname SoundIDの中にある使用する曲の変数名.
		 * 
		 * @param   stop trueで一時停止：falseで再生.
		 * 
		 * @param   fadename フェードアウトの長さ(デフォルト0).
		 * 
		 * @remarks 全てのSEに適用.
		 */
		void SE_SetPause(unsigned int soundname, bool stop, int fadename = 0);

		/**
		 * @brief   全てのSEの一時停止.
		 * 
		 * @param   stop trueで一時停止：falseで再生.
		 * 
		 * @param   fadename フェードアウトの長さ(デフォルト0).
		 * 
		 * @remarks 全てのSEに適用.
		 */
		void SE_SetPauseAll(bool stop, int feadframe = 0);

		/**
		 * @brief   SEを止める.
		 * 
		 * @param   soundname SoundIDの中にある使用する曲の変数名.
		 * 
		 * @param   feadframe フェードアウトの長さ(デフォルト0).
		 * 
		 * @remarks 全てのSEに適用.
		 */
		void SE_SetStop(unsigned int soundname, int feadframe = 0);

		/**
		 * @brief   全てのSEを止める.
		 * 
		 * @param   feadframe フェードアウトの長さ(デフォルト0).
		 * 
		 * @remarks 全てのSEに適用.
		 */
		void SE_SetStopAll(int feadframe = 0);

		/**
		 * @brief   SEの音量を変える.
		 * 
		 * @param   soundname SoundIDの中にある使用する曲の変数名.
		 * 
		 * @param   volume 音量(0.0 ～ 1.0).
		 * 
		 * @param   fadename フェードの長さ(デフォルト0).
		 * 
		 * @remarks 音量は指定外の数値を入れると変更されません.
		 */
		void SE_SetVolume(unsigned int soundname, float volume = 1.0f, int fadename = 0);

		/**
		 * @brief   全てのSEの音量を変える.
		 * 
		 * @param   volume 音量(0.0 ～ 1.0).
		 * 
		 * @param   fadename フェードの長さ(デフォルト0).
		 * 
		 * @remarks 音量は指定外の数値を入れると変更されません.
		 */
		void SE_SetVolumeAll(float volume = 1.0f, int fadename = 0);

		/**
		 * @brief   SEの音程を変更する.
		 * 
		 * @param   soundname SoundIDの中にある使用する曲の変数名.
		 * 
		 * @param   Pitch 音程(デフォルト1.0f).
		 * 
		 * @remarks 全てのSEに適用。マイナス値は使用不可.
		 */
		void SE_SetPitch(unsigned int soundname, float pitch = 1.0f);

		/**
		 * @brief   全てのSEの音程を変更する.
		 * 
		 * @param   Pitch 音程(デフォルト1.0f).
		 * 
		 * @remarks 全てのSEに適用。マイナス値は使用不可.
		 */
		void SE_SetPitchAll(float pitch = 1.0f);
		
		/**
		 * @brief   SEの再生チェック
		 * 
		 * @returns SEが再生しているならtrue、していないならfalse.
		 *
		 * @param   soundname SoundIDの中にある使用する曲の変数名.
		 * 
		 * SEが再生されているかチェックする
		 */
		bool SE_CheckPlay(unsigned int soundname);

		/**
		 * @brief   ストリームの一時停止.
		 * 
		 * @param   stop trueで一時停止：falseで再生.
		 * 
		 * @param   fadename 一時停止のフェードアウトのフレーム数(デフォルト0).
		 * 
		 * @remarks 全てのSEに適用. 
		 */
		void STRM_SetPause(bool stop, int fadename = 0);

		/**
		 * @brief   ストリームの停止.
		 * 
		 * @param   fadename フェードアウトの長さ(デフォルト0).
		 * 
		 * @remarks 全てのストリームに適応.
		 */
		void STRM_SetStop(int fadename = 0);

		/**
		 * @brief   ストリームの音量を変える.
		 * 
		 * @param   volume 音量(0.0 ～ 1.0).
		 * 
		 * @param   fadename フェード.
		 * 
		 * @remarks 音量は指定外の数値を入れると変更されません.
		 */
		void STRM_SetVolume(float volume = 1.0f, int fadename = 0);

		/**
		 * @brief   ストリームの音程を変更する.
		 * 
		 * @param   Pitch 音程(デフォルト1.0f).
		 * 
		 * @remarks 全てのストリームに適用。マイナス値は使用不可.   
		 */
		void STRM_SetPitch(float pitch = 1.0f);

		/**
		 * @brief   ストリームのフェードイン.
		 * 
		 * @param   fadename フェードインの長さ(デフォルト0).
		 * 
		 * ストリームの再生開始時にフェードインさせる.
		 */
		void STRM_SetFadein(int fadename = 0);
		
		/**
		 * @brief   ストリームの再生チェック
		 * 
		 * @returns ストリームが再生しているならtrue、していないならfalse.
		 * 
		 * ストリームが再生されているかチェックする
		 */
		bool STRM_CheckPlay();

		/**
		 * @brief   副ストリームの再生チェック
		 * 
		 * @returns 副ストリームが再生しているならtrue、していないならfalse.
		 * 
		 * ストリームが再生されているかチェックする
		 */
		bool STRM_SubCheckPlay();

		/**
		 * @brief   デバイスアロケータをセットする
		 * 
		 * @param   deviceAlloc デバイスアロケータ.
		 */
		//void SetDevice(System::DeviceAllocator* deviceAlloc);
	};

} // NW_Lib

