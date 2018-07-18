/**
*	@file	sprite.h
*
*	@brief	２Dスプライト用クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/


#pragma once

#include <d3dx9.h>
#include "NW_Lib_Drawer.h"

namespace NW_Lib
{

	class Texture;

	// 頂点フォーマット構造体
	struct VERTEX_TEXTURE2D
	{
		D3DXVECTOR3 position; // 頂点座標
		float w;       // RHW
		DWORD diffuse; // 頂点カラー
		float u;       // テクスチャＵ座標
		float v;       // テクスチャＶ座標
	};

	// フォーマット形式
	static const DWORD D3DFVF_TEXTURE2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	class Sprite : public Drawer
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

			LPDIRECT3DDEVICE9 m_pD3DDevice;
		public:
			Sprite(LPDIRECT3DDEVICE9 pD3DDevice); // コンストラクタ
			~Sprite(); // デストラクタ
			// テクスチャの設定（２パターン）
			void SetTexture(const Texture* texture);
			void SetTexture(const Texture* texture,float textureX,float textureY,float textureWidth,float textureHeight);
			void ChangeSize(float width,float height);
			void DrawSprite( float x, float y,float angle,float alpha,bool center); // 描画
			void DrawAll(const NW_Lib::Texture *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,bool center = true,float angle = 0.0f,float alpha = 1.0f); // 左上を設定する
			
			void DrawUpper(const NW_Lib::Texture *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,bool center = true,float angle = 0.0f,float alpha = 1.0f); // 左上を設定する
			void DrawLower(const NW_Lib::Texture *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,bool center = true,float angle = 0.0f,float alpha = 1.0f); // 左上を設定する

			void SetColor(DWORD color)
			{
				m_Color = color;
			}

			// 単体の描画
			static void Draw(const TEXTURE *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight, float cx = 0.0f, float cy = 0.0f, float angle = 0.0f, float alpha = 1.0f); // 描画を設定する

	};
}


