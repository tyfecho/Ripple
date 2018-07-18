/**
*	@file	sprite3D.h
*
*	@brief	3Dスプライト用クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/


#pragma once

#include <d3dx9.h>

namespace NW_Lib
{

	class Texture;

	// 頂点フォーマット構造体
	struct VERTEX_TEXTURE3D
	{
		D3DXVECTOR3 position; // 頂点座標
		DWORD diffuse; // 頂点カラー
		float u;       // テクスチャＵ座標
		float v;       // テクスチャＶ座標
	};

	// フォーマット形式
	static const DWORD D3DFVF_TEXTURE3D = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	class Sprite3D
	{
		private:
			float m_width;  // 幅
			float m_height; // 高さ
			const Texture* m_texture; // テクスチャ
			float m_uLeftUp;    // 左上Ｕ座標
			float m_vLeftUp;    // 左上Ｖ座標
			float m_uRightDown; // 右下Ｕ座標
			float m_vRightDown; // 右下Ｖ座標
			float m_Alpha;      // アルファ値
			DWORD m_Color;

			D3DXVECTOR3 m_Rot;  // 回転用（ディグリー）

			LPDIRECT3DDEVICE9 m_pD3DDevice;
		public:
			Sprite3D(LPDIRECT3DDEVICE9 pD3DDevice); // コンストラクタ
			~Sprite3D(); // デストラクタ
			// テクスチャの設定（２パターン）
			void SetTexture(const Texture* texture);
			void SetTexture(const Texture* texture,float textureX,float textureY,float textureWidth,float textureHeight);
			void ChangeSize(float width,float height);
			void Draw( float x, float y,float z,float angle,float alpha); // 描画
			void DrawAll(const NW_Lib::Texture *texture,float x, float y,float z, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,float angle = 0.0f,float alpha = 1.0f); // 左上を設定する

			void SetColor(DWORD color)
			{
				m_Color = color;
			}

			void SetRot(float x,float y,float z)
			{
				m_Rot = D3DXVECTOR3(x,y,z);
			}
			void SetRot(D3DXVECTOR3 rot)
			{
				m_Rot = rot;
			}
	};
}


