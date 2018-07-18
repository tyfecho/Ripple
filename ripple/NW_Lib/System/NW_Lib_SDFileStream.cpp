/**
*	@file	NW_Lib_SDFileStream.cpp
*
*	@brief	ファイルストリームクラス(SDカードにファイルの読み書きを行うクラスです。)
*	@brief	書き込み側にファイルがない場合、新しいファイルが作成されます。
*	@brief	また、ファイルへの書き込みは、すべて上書きになります。
*
*	@date	2013/10/29
*
*	@author 皆光　健太
*/

#include "NW_Lib_SDFileStream.h"

#include <string>

namespace NW_Lib
{
	
namespace System
{
/**
*	コンストラクタ
*	@param fileName  ファイル名
*/
SDFileStream::SDFileStream(const char* fileName , unsigned int openMode)
{
	Open(fileName,openMode);
}

/**
*	デストラクタ
*/
SDFileStream::~SDFileStream()
{
	Close();
}

/**
*	ファイルオープン
*	@param fileName  ファイル名
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
*	ファイルクローズ
*/
void SDFileStream::Close()
{
	if(fs.is_open())
	{
		fs.close();
	}
}

/**
*	ファイルに書き込む
*/
void SDFileStream::Flush()
{
	fs << outputStr;
	outputStr = "";
}

/**
*	ストリームオペレータ
*	@param value  値
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