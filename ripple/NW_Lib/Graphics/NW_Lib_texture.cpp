/**
*	@file	texture.cpp
*
*	@brief	テクスチャクラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#include "NW_Lib_texture.h"
#include <windows.h>
#include <sstream>
#include <string>

namespace NW_Lib
{
	/*
	*	コンストラクタ
	*	@param filename  読み込むファイルの名前
	*/
	Texture::Texture(const char *filename)
	: m_texture(NULL)
	, m_width(0)
	, m_height(0)
	{
		if (FAILED(D3DXCreateTextureFromFile(m_pD3DDevice, filename, &m_texture)))
		{
			// そのままでは見つからなかった
			std::string map = filename;
			map.insert(0,"works_romfiles/texture/");
			HRESULT hr = D3DXCreateTextureFromFile(m_pD3DDevice, map.c_str(), &m_texture);
			if (FAILED(hr))
			{
				// spriteファイルの中にもなかった
				// エラー処理
				std::ostringstream sout;
				sout << "\"" << filename << "\"が見つかりません。";
				MessageBox(NULL, sout.str().c_str(), "class Texture", MB_OK);
			}
			else
			{
				// 成功した
				D3DSURFACE_DESC desc;
				m_texture->GetLevelDesc(0, &desc);

				m_width  = desc.Width;	 // 幅
				m_height = desc.Height;  // 高さ
			}
		}
		else
		{
			// 成功した
			D3DSURFACE_DESC desc;
			m_texture->GetLevelDesc(0, &desc);

			m_width  = desc.Width;	 // 幅
			m_height = desc.Height;  // 高さ
		}
	}
	/*
	*	デストラクタ
	*/
	Texture::~Texture()
	{
		if(m_texture != NULL)
		{
			m_texture->Release();
			m_texture = NULL;
		}
	}

	/*
	*	テクスチャオブジェクトを返す
	*/
	LPDIRECT3DTEXTURE9 Texture::GetTexture() const
	{
		return m_texture;
	}
	/*
	*	横幅を返す
	*/
	DWORD Texture::GetWidth() const
	{
		return m_width;
	}
	/*
	*	縦幅を返す
	*/
	DWORD Texture::GetHeight() const
	{
		return m_height;
	}
}



