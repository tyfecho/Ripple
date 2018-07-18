/**
*	@file	NW_Lib_SDFileStream.h
*
*	@brief	�t�@�C���X�g���[���N���X
*
*	@date	2013/10/7
*
*	@author �F���@����
*/
#pragma once

#include <fstream>
#include <sstream>
#include <string>

namespace NW_Lib
{

namespace System
{
enum OPENMODE
{
	OPEN_MODE_READ,
	OPEN_MODE_WRITE,
};

class SDFileStream
{
private:
		std::fstream fs;
		std::fstream ss;
		std::string tempStr;
		std::string outputStr;
public:
	// �R���X�g���N�^
	SDFileStream(const char* fileName , unsigned int openMode);
	SDFileStream(){};
	// �f�X�g���N�^
	~SDFileStream();

	// �t�@�C���̓ǂݍ���
	void Open(const char* fileName , unsigned int openMode);
	void Close();

	// �t�@�C������������
	void Flush();
	
	// �X�g���[���I�y���[�^
	SDFileStream& operator>>(int &value);
	SDFileStream& operator>>(float &value);
	SDFileStream& operator>>(double &value);
	SDFileStream& operator>>(char &value);
	SDFileStream& operator>>(std::string &value);
	
	SDFileStream& operator<<(int &value);
	SDFileStream& operator<<(float &value);
	SDFileStream& operator<<(double &value);
	SDFileStream& operator<<(char &value);
	SDFileStream& operator<<(std::string &value);
	SDFileStream& operator<<(const char* value);

};

} // System

} // NW_Lib
