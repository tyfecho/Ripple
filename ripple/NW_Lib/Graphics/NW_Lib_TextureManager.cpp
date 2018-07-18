/**
*	@file	TextureManager.cpp
*
*	@brief	テクスチャ情報を管理するためのクラス
*
*	@date	2013/08/27
*
*	@author	吉田  将也
*/

#include "NW_Lib_TextureManager.h"
#include "NW_Lib_texture.h"

#include "../System/NW_Lib_SceneManager.h"

#include <string>

namespace NW_Lib
{
	// テクスチャの取得
	TEXTURE* TextureManager::Get(int texID)
	{
		return System::SceneManager::GetInstance().m_textureMnager.Get(texID);
	}

	// テクスチャの生成
	void TextureManager::Entry(int texID , char* filename)
	{
		std::string name;
		name += filename;
		name += ".tga";
		Texture* tex = new Texture(name.c_str());
		System::SceneManager::GetInstance().m_textureMnager.Entry(texID,tex);

	}

	// テクスチャの削除
	void TextureManager::Remove(int texID)
	{
		System::SceneManager::GetInstance().m_textureMnager.Remove(texID);
	}

	// 全てのテクスチャの削除
	void TextureManager::AllRemove()
	{
		System::SceneManager::GetInstance().m_textureMnager.AllRemove();
	}

}

