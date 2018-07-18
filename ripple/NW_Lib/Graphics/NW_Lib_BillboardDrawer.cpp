/**
*	@file	BillboardDrawer.cpp
*
*	@brief	ビルボード描画システムクラス
*
*	@date	2013/08/30
*
*	@author	皆光　健太
*/

#pragma once

#include "NW_Lib_BillboardDrawer.h"
#include "NW_Lib_Billboard.h"

namespace NW_Lib
{
	void BillboardDrawer::Draw( TEXTURE* texture,float x, float y, float z, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight, float cx, float cy, float angle, float alpha)
	{
		Billboard::DrawBillboard(texture,x,y,z,width,height,textureX,textureY,textureWidth,textureHeight,angle,alpha);
	}
}
