/**
*	@file	BillboardDrawer.h
*
*	@brief	�r���{�[�h�`��V�X�e���N���X
*
*	@date	2013/08/30
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
	class BillboardDrawer : public Drawer
	{
		public:
			static void Draw( TEXTURE* texture,float x, float y, float z, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight, float cx = 0.0f, float cy = 0.0f, float angle = 0.0f, float alpha = 1.0f);
	};
}
