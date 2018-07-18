/**
*	@file	SpriteDrawer.h
*
*	@brief	スプライト描画システムクラス
*
*	@date	2013/08/27
*
*	@author	皆光　健太
*/

#pragma once

#include "NW_Lib_Drawer.h"
#include "../Utility/NW_Lib_Constant.h"
#include "../Graphics/NW_Lib_texture.h"

namespace NW_Lib
{

	/*
		スプライト描画システムクラス
	*/
	class SpriteDrawer : public Drawer
	{
		public:
			static void Draw(const TEXTURE* texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight, float cx = 0.0f, float cy = 0.0f, float angle = 0.0f, float alpha = 1.0f);
	};
}
