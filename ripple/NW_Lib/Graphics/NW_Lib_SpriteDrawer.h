/**
*	@file	SpriteDrawer.h
*
*	@brief	�X�v���C�g�`��V�X�e���N���X
*
*	@date	2013/08/27
*
*	@author	�F���@����
*/

#pragma once

#include "NW_Lib_Drawer.h"
#include "../Utility/NW_Lib_Constant.h"
#include "../Graphics/NW_Lib_texture.h"

namespace NW_Lib
{

	/*
		�X�v���C�g�`��V�X�e���N���X
	*/
	class SpriteDrawer : public Drawer
	{
		public:
			static void Draw(const TEXTURE* texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight, float cx = 0.0f, float cy = 0.0f, float angle = 0.0f, float alpha = 1.0f);
	};
}
