/**
*	@file	utility
*
*	@brief	ユーティリティ
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/
#pragma once



#include <cstdlib>

namespace NW_Lib
{

	/*
		安全なDelete関数
	*/
	template<class T>
	inline void SafeDelete(T*& var)
	{
		if (var != NULL)
		{
			delete var;
			var = NULL;
		}
	}

	/*
		安全なリリース関数
	*/
	template<class T>
	inline void SafeRelease(T*& var)
	{
		if (var != NULL)
		{
			var->Release();
			var = NULL;
		}
	}

	/*
		安全な配列Delete関数
	*/
	template<class T>
	inline void SafeDeleteArray(T*& var)
	{
		if (var != NULL)
		{
			delete[] var;
			var = NULL;
		}
	}
}
