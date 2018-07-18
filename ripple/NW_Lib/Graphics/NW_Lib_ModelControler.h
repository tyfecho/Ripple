/**
*	@file	ModelControler.h
*
*	@brief	モデル描画システムクラス
*
*	@date	2013/09/03
*
*	@author	吉田　将也
*	@author	皆光　健太
*/

#pragma once

#include "../Utility/NW_Lib_Constant.h"
#include "../Graphics/NW_Lib_texture.h"
#include "NW_Lib_Drawer.h"

namespace NW_Lib
{

	/*
		モデル描画システムクラス
	*/
	class ModelControler : public Drawer
	{
		public:
			/// モデルの描画
			static void Draw(int handle);
			/// モデルの位置の設定
			static void SetPosision(int handle,VECTOR3 position);
			/// モデルの回転の設定
			static void SetRotate(int handle,VECTOR3 rotation);
			/// モデルのサイズの設定
			static void SetScale(int handle,VECTOR3 scale);
			/// モデルに行列を設定
			static void SetWorldMatrix(int handle,MATRIX* matrix);
	};
}
