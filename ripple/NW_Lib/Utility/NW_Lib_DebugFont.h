/**
*	@file	DebugFont.h
*
*	@brief	デバッグフォント表示用クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#pragma once

#include "d3dx9core.h"
#include "../System/NW_Lib_Common.h"
#include "NW_Lib_HardwareDefine.h"

namespace NW_Lib
{
	namespace Utility
	{
	/// デバッグフォントクラス
	class DebugFont : public NW_Lib::Common
	{
	public:
		// 文字列数
		static const int STR_NUM = 128;
		// バッファサイズ
		static const int STRBUFF_SIZE = 256;
		// 文字の長さ(書き換え厳禁)
		static const int STRLEN = STRBUFF_SIZE-1;


	private:
		

		//テキスト保存用構造体
		typedef struct
		{
			// 文字蓄積用バッファ
			char StrBuff[STRBUFF_SIZE];
			// 文字表示座標X
			int	StrPosX;
			// 文字表示座標Y
			int	StrPosY;
			// 透明度
			float alpha;
		}TextBuf;

		//テキスト保存用バッファ
		TextBuf m_textBuf[SCREEN_NUM][STR_NUM];

		// D3DXFontインターフェースのインスタンス
		LPD3DXFONT	m_pD3DXFont;
		// テキストインデックス
		int StrIndex[SCREEN_NUM];
		
		//カラー
		int m_a,m_r,m_g,m_b;
		// コンストラクタ
		DebugFont();
		DebugFont(const DebugFont& val){(void)val;}
		DebugFont& operator=(const DebugFont& val){(void)val;return *this;}

	public:

		// デストラクタ
		DebugFont::~DebugFont();

	public:
		/// シングルトン化させる
		static DebugFont& GetInstance()
		{
			static DebugFont instance;
			return instance;
		}
		// 文字列をバッファに出力
		void PrintfUpper( int x, int y, char* str, ... );
		void PrintfLower( int x, int y, char* str, ... );
		// 文字列を表示
		void DrawUpper();
		void DrawLower();
		//色の設定
		void SetColor( int r = 255 , int g = 255 , int b = 255 ,int a = 255);
		//サイズの設定
		void SetSize( int width , int height);


	};
	}
}