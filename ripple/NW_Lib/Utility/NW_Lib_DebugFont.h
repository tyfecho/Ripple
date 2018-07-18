/**
*	@file	DebugFont.h
*
*	@brief	�f�o�b�O�t�H���g�\���p�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#pragma once

#include "d3dx9core.h"
#include "../System/NW_Lib_Common.h"
#include "NW_Lib_HardwareDefine.h"

namespace NW_Lib
{
	namespace Utility
	{
	/// �f�o�b�O�t�H���g�N���X
	class DebugFont : public NW_Lib::Common
	{
	public:
		// ������
		static const int STR_NUM = 128;
		// �o�b�t�@�T�C�Y
		static const int STRBUFF_SIZE = 256;
		// �����̒���(������������)
		static const int STRLEN = STRBUFF_SIZE-1;


	private:
		

		//�e�L�X�g�ۑ��p�\����
		typedef struct
		{
			// �����~�ϗp�o�b�t�@
			char StrBuff[STRBUFF_SIZE];
			// �����\�����WX
			int	StrPosX;
			// �����\�����WY
			int	StrPosY;
			// �����x
			float alpha;
		}TextBuf;

		//�e�L�X�g�ۑ��p�o�b�t�@
		TextBuf m_textBuf[SCREEN_NUM][STR_NUM];

		// D3DXFont�C���^�[�t�F�[�X�̃C���X�^���X
		LPD3DXFONT	m_pD3DXFont;
		// �e�L�X�g�C���f�b�N�X
		int StrIndex[SCREEN_NUM];
		
		//�J���[
		int m_a,m_r,m_g,m_b;
		// �R���X�g���N�^
		DebugFont();
		DebugFont(const DebugFont& val){(void)val;}
		DebugFont& operator=(const DebugFont& val){(void)val;return *this;}

	public:

		// �f�X�g���N�^
		DebugFont::~DebugFont();

	public:
		/// �V���O���g����������
		static DebugFont& GetInstance()
		{
			static DebugFont instance;
			return instance;
		}
		// ��������o�b�t�@�ɏo��
		void PrintfUpper( int x, int y, char* str, ... );
		void PrintfLower( int x, int y, char* str, ... );
		// �������\��
		void DrawUpper();
		void DrawLower();
		//�F�̐ݒ�
		void SetColor( int r = 255 , int g = 255 , int b = 255 ,int a = 255);
		//�T�C�Y�̐ݒ�
		void SetSize( int width , int height);


	};
	}
}