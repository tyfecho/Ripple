/**
*	@file	TextureManager.cpp
*
*	@brief	�e�N�X�`�������Ǘ����邽�߂̃N���X
*
*	@date	2013/08/27
*
*	@author	�g�c  ����
*/

#include "NW_Lib_TextureManager.h"
#include "NW_Lib_texture.h"

#include "../System/NW_Lib_SceneManager.h"

#include <string>

namespace NW_Lib
{
	// �e�N�X�`���̎擾
	TEXTURE* TextureManager::Get(int texID)
	{
		return System::SceneManager::GetInstance().m_textureMnager.Get(texID);
	}

	// �e�N�X�`���̐���
	void TextureManager::Entry(int texID , char* filename)
	{
		std::string name;
		name += filename;
		name += ".tga";
		Texture* tex = new Texture(name.c_str());
		System::SceneManager::GetInstance().m_textureMnager.Entry(texID,tex);

	}

	// �e�N�X�`���̍폜
	void TextureManager::Remove(int texID)
	{
		System::SceneManager::GetInstance().m_textureMnager.Remove(texID);
	}

	// �S�Ẵe�N�X�`���̍폜
	void TextureManager::AllRemove()
	{
		System::SceneManager::GetInstance().m_textureMnager.AllRemove();
	}

}

