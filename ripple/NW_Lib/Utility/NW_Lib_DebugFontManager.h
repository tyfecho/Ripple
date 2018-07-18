/**
 * @file    NW_Lib_DebugFontManager.h
 * @brief   �f�o�b�O�t�H���g���Ǘ����邽�߂̃N���X
 * 
 * @author  Masaya Yoshida
 * @date    2013/09/05
 * @version 1.0
 */
#ifndef DEBUGFONT_MANAGER_H_
#define DEBUGFONT_MANAGER_H_

#include "../Utility/NW_Lib_Constant.h"

namespace NW_Lib
{
	class DebugFontManager
	{
		public:
			static void SetSize(float scaleX,float scaleY);
			static void SetSize(float scale);
			static void SetColor( unsigned long red, unsigned long green, unsigned long blue, unsigned long alpha = 255 );
			static void Printf( DrawingScreen screenNum, int x, int y, const char* format, ... );
	};
}

#endif
