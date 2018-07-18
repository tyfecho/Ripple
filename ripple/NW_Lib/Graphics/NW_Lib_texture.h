/**
*	@file	texture.h
*
*	@brief	テクスチャクラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#pragma once

#include <d3dx9.h>
#include "../System/NW_Lib_Common.h"

namespace NW_Lib
{
	class Texture : public Common
	{
		private:
			LPDIRECT3DTEXTURE9 m_texture; // テクスチャオブジェクト

			DWORD m_width;  // テクスチャの幅
			DWORD m_height; // テクスチャの幅
		public:
			Texture( const char* filename);
			~Texture();
			LPDIRECT3DTEXTURE9 GetTexture() const; // テクスチャオブジェクトを返す
			DWORD GetWidth() const;                // テクスチャの幅を返す
			DWORD GetHeight() const;               // テクスチャの高さを返す
	};
}
