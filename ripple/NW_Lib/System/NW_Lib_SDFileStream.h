/**
*	@file	NW_Lib_SDFileStream.h
*
*	@brief	ファイルストリームクラス
*
*	@date	2013/10/7
*
*	@author 皆光　健太
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
	// コンストラクタ
	SDFileStream(const char* fileName , unsigned int openMode);
	SDFileStream(){};
	// デストラクタ
	~SDFileStream();

	// ファイルの読み込み
	void Open(const char* fileName , unsigned int openMode);
	void Close();

	// ファイルを書き込む
	void Flush();
	
	// ストリームオペレータ
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
