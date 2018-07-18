/**
*	@file	Billboard.h
*
*	@brief	ビルボードクラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#pragma once

#include <d3dx9.h>
#include "../System/NW_Lib_Common.h"
#include "../Utility/NW_Lib_Constant.h"
#include "../Graphics/NW_Lib_texture.h"

namespace NW_Lib
{
	class Texture;

	//// 頂点フォーマット構造体
	//struct VERTEX_TEXTURE3D
	//{
	//	D3DXVECTOR3 position; // 頂点座標
	//	DWORD diffuse; // 頂点カラー
	//	float u;       // テクスチャＵ座標
	//	float v;       // テクスチャＶ座標
	//};

	//// フォーマット形式
	//static const DWORD D3DFVF_TEXTURE3D = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	class Billboard : public Common , public Drawer
	{
		private:
			float m_width;  // 幅
			float m_height; // 高さ
			const Texture* m_texture; // テクスチャ
			float m_uLeftUp;    // 左上Ｕ座標
			float m_vLeftUp;    // 左上Ｖ座標
			float m_uRightDown; // 右下Ｕ座標
			float m_vRightDown; // 右下Ｖ座標

			static CAMERA* ms_camera; 
		public:
			Billboard(float width,float height); // コンストラクタ
			~Billboard(); // デストラクタ
			// テクスチャの設定（２パターン）
			void SetTexture(const Texture* texture);
			void SetTexture(const Texture* texture,float textureX,float textureY,float textureWidth,float textureHeight);
			void ChangeSize(float width,float height);
			void Draw( float x, float y,float z,float angle); // 描画
			void DrawAll(const NW_Lib::Texture *texture,float x, float y,float z/*, float width, float height*/, float textureX, float textureY, float textureWidth, float textureHeight,float angle); // 左上を設定する
			D3DXMATRIX GetBillboardMatrix();
			
			/**
			 * @brief   描画
			 * 
			 * @param   pTex テクスチャ情報
			 * 
			 * @param   dx 表示時のX座標を指定します。
			 * @param   dy 表示時のY座標を指定します。
			 * @param   dz 表示時のZ座標を指定します。
			 * 
			 * @param   dw 表示時の幅を指定します。
			 * @param   dh 表示時の高さを指定します。
			 * 
			 * @param   tx テクスチャのX座標を指定します。
			 * @param   ty テクスチャのY座標を指定します。
			 *
			 * @param   tw テクスチャの切り抜き幅を指定します。
			 * @param   th テクスチャの切り抜き高さを指定します。
			 * 
			 * @param   rot 回転角を指定します。（0 ～ 360度）
			 * 
			 * @param   alpha を指定します。（0.0f ～ 1.0f）
			 */
			static void DrawBillboard(TEXTURE* pTex, float dx, float dy,float dz , float dw, float dh,
						float tx, float ty, float tw, float th, float rot = 0.0f, float alpha = 1.0f);
	};
}


