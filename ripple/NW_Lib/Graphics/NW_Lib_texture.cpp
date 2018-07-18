/**
*	@file	texture.cpp
*
*	@brief	�e�N�X�`���N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#include "NW_Lib_texture.h"
#include <windows.h>
#include <sstream>
#include <string>

namespace NW_Lib
{
	/*
	*	�R���X�g���N�^
	*	@param filename  �ǂݍ��ރt�@�C���̖��O
	*/
	Texture::Texture(const char *filename)
	: m_texture(NULL)
	, m_width(0)
	, m_height(0)
	{
		if (FAILED(D3DXCreateTextureFromFile(m_pD3DDevice, filename, &m_texture)))
		{
			// ���̂܂܂ł͌�����Ȃ�����
			std::string map = filename;
			map.insert(0,"works_romfiles/texture/");
			HRESULT hr = D3DXCreateTextureFromFile(m_pD3DDevice, map.c_str(), &m_texture);
			if (FAILED(hr))
			{
				// sprite�t�@�C���̒��ɂ��Ȃ�����
				// �G���[����
				std::ostringstream sout;
				sout << "\"" << filename << "\"��������܂���B";
				MessageBox(NULL, sout.str().c_str(), "class Texture", MB_OK);
			}
			else
			{
				// ��������
				D3DSURFACE_DESC desc;
				m_texture->GetLevelDesc(0, &desc);

				m_width  = desc.Width;	 // ��
				m_height = desc.Height;  // ����
			}
		}
		else
		{
			// ��������
			D3DSURFACE_DESC desc;
			m_texture->GetLevelDesc(0, &desc);

			m_width  = desc.Width;	 // ��
			m_height = desc.Height;  // ����
		}
	}
	/*
	*	�f�X�g���N�^
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
	*	�e�N�X�`���I�u�W�F�N�g��Ԃ�
	*/
	LPDIRECT3DTEXTURE9 Texture::GetTexture() const
	{
		return m_texture;
	}
	/*
	*	������Ԃ�
	*/
	DWORD Texture::GetWidth() const
	{
		return m_width;
	}
	/*
	*	�c����Ԃ�
	*/
	DWORD Texture::GetHeight() const
	{
		return m_height;
	}
}



