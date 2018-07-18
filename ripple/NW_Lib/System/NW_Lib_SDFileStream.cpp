/**
*	@file	NW_Lib_SDFileStream.cpp
*
*	@brief	�t�@�C���X�g���[���N���X(SD�J�[�h�Ƀt�@�C���̓ǂݏ������s���N���X�ł��B)
*	@brief	�������ݑ��Ƀt�@�C�����Ȃ��ꍇ�A�V�����t�@�C�����쐬����܂��B
*	@brief	�܂��A�t�@�C���ւ̏������݂́A���ׂď㏑���ɂȂ�܂��B
*
*	@date	2013/10/29
*
*	@author �F���@����
*/

#include "NW_Lib_SDFileStream.h"

#include <string>

namespace NW_Lib
{
	
namespace System
{
/**
*	�R���X�g���N�^
*	@param fileName  �t�@�C����
*/
SDFileStream::SDFileStream(const char* fileName , unsigned int openMode)
{
	Open(fileName,openMode);
}

/**
*	�f�X�g���N�^
*/
SDFileStream::~SDFileStream()
{
	Close();
}

/**
*	�t�@�C���I�[�v��
*	@param fileName  �t�@�C����
*/
void SDFileStream::Open(const char* fileName , unsigned int openMode)
{
	if(fs.is_open())
	{
		fs.close();
	}
	std::string filePath = "SD/";
	filePath += fileName;

	if(openMode == OPEN_MODE_WRITE)
	{
		fs.open(filePath.c_str(),std::ios_base::out | std::ios_base::trunc);
	}
	else if(openMode == OPEN_MODE_READ)
	{
		fs.open(filePath.c_str(),std::ios_base::in);
	}

}
/**
*	�t�@�C���N���[�Y
*/
void SDFileStream::Close()
{
	if(fs.is_open())
	{
		fs.close();
	}
}

/**
*	�t�@�C���ɏ�������
*/
void SDFileStream::Flush()
{
	fs << outputStr;
	outputStr = "";
}

/**
*	�X�g���[���I�y���[�^
*	@param value  �l
*/
SDFileStream& SDFileStream::operator>>(int &value)
{
	fs >> value;
	return *this;
}
SDFileStream& SDFileStream::operator>>(float &value)
{
	fs >> value;
	return *this;
}
SDFileStream& SDFileStream::operator>>(double &value)
{
	fs >> value;
	return *this;
}
SDFileStream& SDFileStream::operator>>(char &value)
{
	fs >> value;
	return *this;
}
SDFileStream& SDFileStream::operator>>(std::string &value)
{
	fs >> value;
	return *this;
}


SDFileStream& SDFileStream::operator<<(int &value)
{
	ss << value;
	ss >> tempStr;
	outputStr += outputStr;
	outputStr += " ";
	return *this;
}
SDFileStream& SDFileStream::operator<<(float &value)
{
	ss << value;
	ss >> tempStr;
	outputStr += outputStr;
	outputStr += " ";
	return *this;
}
SDFileStream& SDFileStream::operator<<(double &value)
{
	ss << value;
	ss >> tempStr;
	outputStr += outputStr;
	outputStr += " ";
	return *this;
}
SDFileStream& SDFileStream::operator<<(char &value)
{
	ss << value;
	ss >> tempStr;
	outputStr += outputStr;
	outputStr += " ";
	return *this;
}
SDFileStream& SDFileStream::operator<<(std::string &value)
{
	outputStr += value;
	outputStr += " ";
	return *this;
}

SDFileStream& SDFileStream::operator<<(const char* value)
{
	outputStr += value;
	outputStr += " ";
	return *this;
}

} // System

} // NW_Lib