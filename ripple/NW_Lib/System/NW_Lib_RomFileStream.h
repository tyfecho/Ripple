/**
*	@file	NW_Lib_RomFileStream.h
*
*	@brief	ファイルストリームクラス
*
*	@date	2013/10/7
*
*	@author 皆光　健太
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
	// コンストラクタ
	RomFileStream(const char* fileName);
	RomFileStream(){};
	// デストラクタ
	~RomFileStream();

	// ファイルリーダー
	void Open(const char* fileName);
	void Close();
	
	// ストリームオペレータ
	RomFileStream& operator>>(int &value);
	RomFileStream& operator>>(float &value);
	RomFileStream& operator>>(double &value);
	RomFileStream& operator>>(char &value);
	RomFileStream& operator>>(std::string &value);

};

} // System

} // NW_Lib
