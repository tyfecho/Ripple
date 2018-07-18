/**
*	@file	utility
*
*	@brief	���[�e�B���e�B
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/
#pragma once



#include <cstdlib>

namespace NW_Lib
{

	/*
		���S��Delete�֐�
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
		���S�ȃ����[�X�֐�
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
		���S�Ȕz��Delete�֐�
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
