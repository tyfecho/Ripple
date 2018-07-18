/**
*	@file	TextureGetter.h
*
*	@brief	テクスチャ情報を管理するためのクラス
*
*	@date	2013/08/27
*
*	@author	吉田  将也
*/
#pragma once

#include "../Utility/NW_Lib_Constant.h"
#include "../Graphics/NW_Lib_texture.h"

namespace NW_Lib
{
	class TextureManager
	{
		public:

			// テクスチャの取得
			static TEXTURE* Get(int texID);

			// テクスチャの生成
			static void Entry(int texID , char* filename);

			// テクスチャの削除
			static void Remove(int texID);

			// 全てのテクスチャの削除
			static void AllRemove();
	};
}


