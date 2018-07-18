/**
*	@file	NW_Lib_RomFileStream.cpp
*
*	@brief	ファイルストリームクラス(Romファイルから読み込むクラスです
*	@brief	※仕様上Romファイルは読み込み専用なので、
*	@brief	書き込みも行う場合はSDFileStreamを使用してください。
*
*	@date	2013/10/7
*
*	@author 皆光　健太
*/

#include "NW_Lib_RomFileStream.h"

#include <string>

namespace NW_Lib
{
	
namespace System
{
/**
*	コンストラクタ
*	@param fileName  ファイル名
*/
RomFileStream::RomFileStream(const char* fileName)
{
	Open(fileName);
}

/**
*	デストラクタ
*/
RomFileStream::~RomFileStream()
{
	Close();
}

/**
*	ファイルオープン
*	@param fileName  ファイル名
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
*	ファイルクローズ
*/
void RomFileStream::Close()
{
	if(ifs.is_open())
	{
		ifs.close();
	}
}

/**
*	ストリームオペレータ
*	@param value  値
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