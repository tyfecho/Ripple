/**
*	@file	SpriteDrawer.cpp
*
*	@brief	スプライト描画システムクラス
*
*	@date	2013/08/27
*
*	@author	皆光　健太
*/

#pragma once

#include "NW_Lib_SpriteDrawer.h"
#include "NW_Lib_sprite.h"

namespace NW_Lib
{
	void SpriteDrawer::Draw(const TEXTURE* texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight, float cx, float cy, float angle, float alpha)
	{
		Sprite::Draw(texture,x,y,width,height,textureX,textureY,textureWidth,textureHeight,cx,cy,angle,alpha);
	}
}
