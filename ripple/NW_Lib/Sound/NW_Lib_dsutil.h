/**
*	@file	dsutil.h
*
*	@brief	サウンドに関するクラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/
#ifndef DSUTIL_H
#define DSUTIL_H

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>
#include <math.h>


namespace NW_Lib
{

	//-----------------------------------------------------------------------------
	// Classes used by this header
	//-----------------------------------------------------------------------------
	class CSoundManager;
	class CSound;
	class CStreamingSound;
	class CWaveFile;




	//-----------------------------------------------------------------------------
	// Typing macros 
	//-----------------------------------------------------------------------------
	#define WAVEFILE_READ   1
	#define WAVEFILE_WRITE  2

	#define DSUtil_StopSound(s)         { if(s) s->Stop(); }
	#define DSUtil_PlaySound(s)         { if(s) s->Play( 0, 0 ); }
	#define DSUtil_PlaySoundLooping(s)  { if(s) s->Play( 0, DSBPLAY_LOOPING ); }


	
	inline LONG CulVolume(FLOAT num)
	{
		if(num <= 0.0f)
		{
			return -10000;
		}
		else if( num >= 1.0f )
		{
			return 0;
		}
	
		return static_cast<LONG>( 33.2f * log10(num) *  50 );
	}

	//-----------------------------------------------------------------------------
	// Name: class CSoundManager
	// Desc: 
	//-----------------------------------------------------------------------------
	class CSoundManager
	{
	protected:
		LPDIRECTSOUND8 m_pDS;
		FLOAT		   m_fVolume;

	public:
		CSoundManager();
		~CSoundManager();

		HRESULT Initialize( HWND hWnd, DWORD dwCoopLevel );
		inline  LPDIRECTSOUND8 GetDirectSound() { return m_pDS; }
		HRESULT SetPrimaryBufferFormat( DWORD dwPrimaryChannels, DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate );
		HRESULT Get3DListenerInterface( LPDIRECTSOUND3DLISTENER* ppDSListener );
		inline  VOID SetVolume(FLOAT Num = 0.0f) { m_fVolume = Num; }
		inline  FLOAT* GetVolume() { return &m_fVolume; }

		HRESULT Create( CSound** ppSound, LPTSTR strWaveFileName, DWORD dwCreationFlags = 0, GUID guid3DAlgorithm = GUID_NULL, DWORD dwNumBuffers = 1 );
		HRESULT CreateFromMemory( CSound** ppSound, BYTE* pbData, ULONG ulDataSize, LPWAVEFORMATEX pwfx, DWORD dwCreationFlags = 0, GUID guid3DAlgorithm = GUID_NULL, DWORD dwNumBuffers = 1 );
		HRESULT CreateStreaming( CStreamingSound** ppStreamingSound, LPTSTR strWaveFileName, DWORD dwCreationFlags, GUID guid3DAlgorithm, DWORD dwNotifyCount, DWORD dwNotifySize, HANDLE hNotifyEvent );
	};




	//-----------------------------------------------------------------------------
	// Name: class CSound
	// Desc: Encapsulates functionality of a DirectSound buffer.
	//-----------------------------------------------------------------------------
	class CSound
	{
	protected:
		LPDIRECTSOUNDBUFFER* m_apDSBuffer;
		DWORD                m_dwDSBufferSize;
		CWaveFile*           m_pWaveFile;
		DWORD                m_dwNumBuffers;
		DWORD                m_dwCreationFlags;
		FLOAT*				 m_pfVolume;

		HRESULT RestoreBuffer( LPDIRECTSOUNDBUFFER pDSB, BOOL* pbWasRestored );

	public:
		CSound( LPDIRECTSOUNDBUFFER* apDSBuffer, DWORD dwDSBufferSize, DWORD dwNumBuffers, CWaveFile* pWaveFile, DWORD dwCreationFlags , FLOAT* pfVolume );
		virtual ~CSound();

		HRESULT Get3DBufferInterface( DWORD dwIndex, LPDIRECTSOUND3DBUFFER* ppDS3DBuffer );
		HRESULT FillBufferWithSound( LPDIRECTSOUNDBUFFER pDSB, BOOL bRepeatWavIfBufferLarger );
		LPDIRECTSOUNDBUFFER GetFreeBuffer();
		LPDIRECTSOUNDBUFFER GetBuffer( DWORD dwIndex );

		HRESULT Play( DWORD dwPriority = 0, DWORD dwFlags = 0, FLOAT FVolume = 1.0f, LONG lFrequency = -1, LONG lPan = 0 );
		HRESULT Play3D( LPDS3DBUFFER p3DBuffer, DWORD dwPriority = 0, DWORD dwFlags = 0, LONG lFrequency = 0 );
		HRESULT Stop();
		HRESULT Reset();
		BOOL    IsSoundPlaying();
	};




	//-----------------------------------------------------------------------------
	// Name: class CStreamingSound
	// Desc: Encapsulates functionality to play a wave file with DirectSound.  
	//       The Create() method loads a chunk of wave file into the buffer, 
	//       and as sound plays more is written to the buffer by calling 
	//       HandleWaveStreamNotification() whenever hNotifyEvent is signaled.
	//-----------------------------------------------------------------------------
	class CStreamingSound : public CSound
	{
	protected:
		DWORD m_dwLastPlayPos;
		DWORD m_dwPlayProgress;
		DWORD m_dwNotifySize;
		DWORD m_dwNextWriteOffset;
		BOOL  m_bFillNextNotificationWithSilence;
		FLOAT m_fVolume;

	public:
		CStreamingSound( LPDIRECTSOUNDBUFFER pDSBuffer, DWORD dwDSBufferSize, CWaveFile* pWaveFile, DWORD dwNotifySize );
		~CStreamingSound();

		HRESULT HandleWaveStreamNotification( BOOL bLoopedPlay );
		HRESULT Reset();
	};




	//-----------------------------------------------------------------------------
	// Name: class CWaveFile
	// Desc: Encapsulates reading or writing sound data to or from a wave file
	//-----------------------------------------------------------------------------
	class CWaveFile
	{
	public:
		WAVEFORMATEX* m_pwfx;        // Pointer to WAVEFORMATEX structure
		HMMIO         m_hmmio;       // MM I/O handle for the WAVE
		MMCKINFO      m_ck;          // Multimedia RIFF chunk
		MMCKINFO      m_ckRiff;      // Use in opening a WAVE file
		DWORD         m_dwSize;      // The size of the wave file
		MMIOINFO      m_mmioinfoOut;
		DWORD         m_dwFlags;
		BOOL          m_bIsReadingFromMemory;
		BYTE*         m_pbData;
		BYTE*         m_pbDataCur;
		ULONG         m_ulDataSize;
		CHAR*         m_pResourceBuffer;

	protected:
		HRESULT ReadMMIO();
		HRESULT WriteMMIO( WAVEFORMATEX *pwfxDest );

	public:
		CWaveFile();
		~CWaveFile();

		HRESULT Open( LPTSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags );
		HRESULT OpenFromMemory( BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags );
		HRESULT Close();

		HRESULT Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead );
		HRESULT Write( UINT nSizeToWrite, BYTE* pbData, UINT* pnSizeWrote );

		DWORD   GetSize();
		HRESULT ResetFile();
		WAVEFORMATEX* GetFormat() { return m_pwfx; };
	};

}


#endif // DSUTIL_H
