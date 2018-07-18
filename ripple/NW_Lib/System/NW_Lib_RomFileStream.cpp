/**
*	@file	NW_Lib_RomFileStream.cpp
*
*	@brief	�t�@�C���X�g���[���N���X(Rom�t�@�C������ǂݍ��ރN���X�ł�
*	@brief	���d�l��Rom�t�@�C���͓ǂݍ��ݐ�p�Ȃ̂ŁA
*	@brief	�������݂��s���ꍇ��SDFileStream���g�p���Ă��������B
*
*	@date	2013/10/7
*
*	@author �F���@����
*/

#include "NW_Lib_RomFileStream.h"

#include <string>

namespace NW_Lib
{
	
namespace System
{
/**
*	�R���X�g���N�^
*	@param fileName  �t�@�C����
*/
RomFileStream::RomFileStream(const char* fileName)
{
	Open(fileName);
}

/**
*	�f�X�g���N�^
*/
RomFileStream::~RomFileStream()
{
	Close();
}

/**
*	�t�@�C���I�[�v��
*	@param fileName  �t�@�C����
*/
void RomFileStream::Open(const char* fileName)
{
	if(ifs.is_open())
	{
		ifs.close();
	}
	std::string filePath = "works_romfiles/";
	filePath += fileName;
	ifs.open(filePath.c_str());

}
/**
*	�t�@�C���N���[�Y
*/
void RomFileStream::Close()
{
	if(ifs.is_open())
	{
		ifs.close();
	}
}

/**
*	�X�g���[���I�y���[�^
*	@param value  �l
*/
RomFileStream& RomFileStream::operator>>(int &value)
{
	ifs >> value;
	return *this;
}
RomFileStream& RomFileStream::operator>>(float &value)
{
	ifs >> value;
	return *this;
}
RomFileStream& RomFileStream::operator>>(double &value)
{
	ifs >> value;
	return *this;
}
RomFileStream& RomFileStream::operator>>(char &value)
{
	ifs >> value;
	return *this;
}
RomFileStream& RomFileStream::operator>>(std::string &value)
{
	ifs >> value;
	return *this;
}

} // System

} // NW_Lib