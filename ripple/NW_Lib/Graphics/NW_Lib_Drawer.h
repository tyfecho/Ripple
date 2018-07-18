#pragma once
#include "../Utility/NW_Lib_Constant.h"
namespace NW_Lib
{
	class Drawer
	{
		protected:
			static NW_Lib::DrawingScreen DrawingScreenID;
		public:
			static void SetDrawUpper()
			{
				DrawingScreenID = UPPER_SCREEN_DRAWING;
			}
			static void SetDrawLower()
			{
				DrawingScreenID = LOWER_SCREEN_DRAWING;
			}
	};
}