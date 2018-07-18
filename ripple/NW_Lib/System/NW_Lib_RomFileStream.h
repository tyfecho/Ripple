/**
*	@file	NW_Lib_RomFileStream.h
*
*	@brief	�t�@�C���X�g���[���N���X
*
*	@date	2013/10/7
*
*	@author �F���@����
*/
#pragma once

#include <fstream>
#include <string>

namespace NW_Lib
{

namespace System
{

class RomFileStream
{
private:
		std::ifstream ifs;
public:
	// �R���X�g���N�^
	RomFileStream(const char* fileName);
	RomFileStream(){};
	// �f�X�g���N�^
	~RomFileStream();

	// �t�@�C�����[�_�[
	void Open(const char* fileName);
	void Close();
	
	// �X�g���[���I�y���[�^
	RomFileStream& operator>>(int &value);
	RomFileStream& operator>>(float &value);
	RomFileStream& operator>>(double &value);
	RomFileStream& operator>>(char &value);
	RomFileStream& operator>>(std::string &value);

};

} // System

} // NW_Lib
