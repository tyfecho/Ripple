/**
 * @file    NW_Lib_DebugFontManager.cpp
 * @brief   デバッグフォントを管理するためのクラス
 * 
 * @author  Masaya Yoshida
 * @date    2013/09/05
 * @version 1.0
 */
#include "NW_Lib_DebugFontManager.h"
#include "NW_Lib_DebugFont.h"
#include <stdio.h>
#include <stdarg.h>

namespace NW_Lib
{
	void DebugFontManager::SetSize(float scaleX,float scaleY)
	{
		Utility::DebugFont::GetInstance().SetSize(static_cast<int>(scaleX),static_cast<int>(scaleY));
	}
	void DebugFontManager::SetSize(float scale)
	{
		Utility::DebugFont::GetInstance().SetSize(0,static_cast<int>(scale));
	}

	void DebugFontManager::SetColor(unsigned long red, unsigned long green, unsigned long blue, unsigned long alpha)
	{
		Utility::DebugFont::GetInstance().SetColor(red,green,blue,alpha);
	}

	void DebugFontManager::Printf( DrawingScreen screenNum, int x, int y, const char* format, ... )
	{
		va_list args;

		va_start(args, format);
		char buffer[256];
		vsprintf_s(buffer, 256 , format, args);
		
		if(screenNum == UPPER_SCREEN_DRAWING)
		{
			Utility::DebugFont::GetInstance().PrintfUpper(x,y,buffer);
		}
		else if(screenNum == LOWER_SCREEN_DRAWING)
		{
			Utility::DebugFont::GetInstance().PrintfLower(x,y,buffer);

		}

		va_end(args);
	}
}




