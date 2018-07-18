/*-------------------------------------------------------------------------

	クラス名　：　モデルのインターフェースクラス
	作成者　　：　皆光　健太
	最終更新日：　2013 / 09 / 17

-------------------------------------------------------------------------*/
#pragma once

#include "NW_Lib_AnimController.h"
#include "../System/NW_Lib_Common.h"
#include <d3dx9.h>
#include <d3d9.h>

#include "../Utility/NW_Lib_utility.h"

namespace NW_Lib
{

class Texture;

class IModel
{

	public:

		/*=======================================================
			関数名	：	Update
			説明	：	更新
			引数	：	アニメーションコントローラ
						ワールド行列
			戻り値	：　なし
		========================================================*/
		virtual void Update(AnimController* pAnimController , LPD3DXMATRIX pWorld) = 0;

		/*=======================================================
			関数名	：	Draw
			説明	：	メッシュの描画
			引数	：	ワールドマトリックス
						アニメーションコントローラ
						アルファ
			戻り値	：　なし
		========================================================*/
		virtual void Draw( D3DXMATRIX world , AnimController* pAnimController , float alpha = 1.0f) = 0;

};


}