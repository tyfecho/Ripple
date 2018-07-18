/**
*	@file	NW_Lib_ParticleControler.h
*
*	@brief	パーティクル描画システムクラス
*
*	@date	2013/09/24
*
*	@author	吉田　将也
*/

#pragma once

#include "NW_Lib_Drawer.h"

namespace NW_Lib
{

	/*
		スプライト描画システムクラス
	*/
	class ParticleControler : public Drawer
	{
		public:
			/// モデルの描画
			static void Draw(int handle){}
			/// モデルの位置の設定
			static void SetPosision(int handle,VECTOR3 position){}
			/// モデルの回転の設定
			static void SetRotate(int handle,VECTOR3 rotation){}
			/// モデルのサイズの設定
			static void SetScale(int handle,VECTOR3 scale){}
			/// 放出量の設定
			static void SetEmission(int handle,float emission){}
	};
}
