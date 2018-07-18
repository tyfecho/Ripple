/***
*	@file	DebugFont.cpp
*
*	@brief	デバッグフォント表示用クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#include "NW_Lib_DebugFont.h"
#include <stdio.h>
#include <stdarg.h>

namespace NW_Lib
{
	namespace Utility
	{
	/**
	*	コンストラクタ
	*	@param pD3DDevice  デバイス
	*/
	DebugFont::DebugFont()
	{
		D3DXFONT_DESC desc;
		ZeroMemory( &desc, sizeof( desc ) );
		desc.Height = 20;
		desc.Width = 0;
		desc.Weight = FW_BOLD;
		desc.MipLevels = 0;
		desc.Italic = FALSE;
		desc.CharSet = SHIFTJIS_CHARSET;
		desc.OutputPrecision = 0;
		desc.Quality = 0;
		desc.PitchAndFamily = 0;
		lstrcpy( desc.FaceName, "MS ゴシック" );

		HRESULT hr;
		hr = D3DXCreateFontIndirect( m_pD3DDevice, &desc, &m_pD3DXFont );

		ZeroMemory( m_textBuf, sizeof(TextBuf) );

		StrIndex[SCREEN_UPPER] = StrIndex[SCREEN_LOWER] = 0;
		
		m_a = 255;
		m_r = 255;
		m_g = 255;
		m_b = 255;
	}

	/**
	*	デストラクタ
	*/
	DebugFont::~DebugFont()
	{
		if(m_pD3DXFont){
			m_pD3DXFont->Release();
			m_pD3DXFont = NULL;
		}
	}
	
	/**
	*	出力
	*/
	void DebugFont::PrintfUpper( int x, int y, char* format, ... )
	{
		// １フレームの表示数オーバー
		if ( StrIndex[SCREEN_UPPER] >= STR_NUM )	return;

		va_list arg;

		va_start(arg, format);

		vsnprintf_s(m_textBuf[SCREEN_UPPER][StrIndex[SCREEN_UPPER]].StrBuff, STRLEN, format, arg);
		m_textBuf[SCREEN_UPPER][StrIndex[SCREEN_UPPER]].StrBuff[STRLEN] = '\0';

		va_end(arg);

		m_textBuf[SCREEN_UPPER][StrIndex[SCREEN_UPPER]].StrPosX = x;
		m_textBuf[SCREEN_UPPER][StrIndex[SCREEN_UPPER]].StrPosY = y;

		StrIndex[SCREEN_UPPER]++;
	}
	/**
	*	出力
	*/
	void DebugFont::PrintfLower( int x, int y, char* format, ... )
	{
		// １フレームの表示数オーバー
		if ( StrIndex[SCREEN_LOWER] >= STR_NUM )	return;

		va_list arg;

		va_start(arg, format);

		vsnprintf_s(m_textBuf[SCREEN_LOWER][StrIndex[SCREEN_LOWER]].StrBuff, STRLEN, format, arg);
		m_textBuf[SCREEN_LOWER][StrIndex[SCREEN_LOWER]].StrBuff[STRLEN] = '\0';

		va_end(arg);

		m_textBuf[SCREEN_LOWER][StrIndex[SCREEN_LOWER]].StrPosX = static_cast<int>(x + LOWER_SCREEN_OFFSET_WIDTH);
		m_textBuf[SCREEN_LOWER][StrIndex[SCREEN_LOWER]].StrPosY = static_cast<int>(y + LOWER_SCREEN_OFFSET_HEIGHT);

		StrIndex[SCREEN_LOWER]++;
	}

	/**
	*	文字列を表示
	*/
	void DebugFont::DrawUpper()
	{
		RECT rect;
		
		rect.left = 0;
		rect.right = 0;
		rect.top = 0;
		rect.bottom = 0;

		for ( int i = 0; i < StrIndex[SCREEN_UPPER]; i++ )
		{
			rect.left = m_textBuf[SCREEN_UPPER][i].StrPosX;
			rect.top = m_textBuf[SCREEN_UPPER][i].StrPosY;
			m_pD3DXFont->DrawText( NULL, m_textBuf[SCREEN_UPPER][i].StrBuff, -1, &rect, DT_LEFT|DT_NOCLIP, D3DCOLOR_ARGB(255,m_r,m_g,m_b) );
		}

		StrIndex[SCREEN_UPPER] = 0;
	}
	void DebugFont::DrawLower()
	{
		RECT rect;
		
		rect.left = 0;
		rect.right = 0;
		rect.top = 0;
		rect.bottom = 0;

		for ( int i = 0; i < StrIndex[SCREEN_LOWER]; i++ )
		{
			rect.left = m_textBuf[SCREEN_LOWER][i].StrPosX;
			rect.top = m_textBuf[SCREEN_LOWER][i].StrPosY;
			m_pD3DXFont->DrawText( NULL, m_textBuf[SCREEN_LOWER][i].StrBuff, -1, &rect, DT_LEFT|DT_NOCLIP, D3DCOLOR_XRGB(m_r,m_g,m_b) );
		}

		StrIndex[SCREEN_LOWER] = 0;
	}
	
	/**
	*	色の設定
	*/
	void DebugFont::SetColor( int r , int g , int  b,int a)
	{
		m_a = a;
		m_r = r;
		m_g = g;
		m_b = b;
	}
	
	/**
	*	サイズの設定
	*/
	void DebugFont::SetSize( int width , int height)
	{
		
		//今あるフォントの消す
		if(m_pD3DXFont){
			m_pD3DXFont->Release();
			m_pD3DXFont = NULL;
		}

		//フォントの再定義
		D3DXFONT_DESC desc;
		ZeroMemory( &desc, sizeof( desc ) );
		desc.Height = height;
		desc.Width = width;
		desc.Weight = FW_BOLD;
		desc.MipLevels = 0;
		desc.Italic = FALSE;
		desc.CharSet = SHIFTJIS_CHARSET;
		desc.OutputPrecision = 0;
		desc.Quality = 0;
		desc.PitchAndFamily = 0;
		lstrcpy( desc.FaceName, "MS ゴシック" );

		HRESULT hr;
		hr = D3DXCreateFontIndirect( m_pD3DDevice, &desc, &m_pD3DXFont );
	}

	}
}