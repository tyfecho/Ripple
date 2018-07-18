/**
*	@file	Billboard.cpp
*
*	@brief	ビルボードクラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#include "NW_Lib_Billboard.h"
#include "NW_Lib_Texture.h"
#include "NW_Lib_sprite3D.h"
#include "../Utility/NW_Lib_HardwareDefine.h"


namespace NW_Lib
{
	 CAMERA* Billboard::ms_camera;

	/**
	*           コンストラクタ
	*/
	Billboard::Billboard(float width, float height)
	:m_width(width)      // 幅
	,m_height(height)    // 高さ
	,m_texture(NULL)     // テクスチャ
	,m_uLeftUp(0.0f)     // 左上Ｕ座標
	,m_vLeftUp(0.0f)     // 左上Ｖ座標
	,m_uRightDown(0.0f)  // 右下Ｕ座標
	,m_vRightDown(0.0f)  // 右下Ｖ座標
	{
	}

	/**
	*            デストラクタ
	*/
	Billboard::~Billboard()
	{
	}

	/**
	*  テクスチャの設定    （全体バージョン）
	*	@param texture  テクスチャ情報
	*/
	void Billboard::SetTexture(const Texture *texture)
	{
		m_texture = texture;

		m_uLeftUp  = 0.0f; // 左上設定
		m_vLeftUp  = 0.0f; // 左上設定
		m_uRightDown = 1.0f; // 右下設定
		m_vRightDown = 1.0f; // 右下設定
	}

	/**
	*  テクスチャの設定    （指定バージョン）
	*/
	void Billboard::SetTexture(const Texture *texture, float textureX, float textureY, float textureWidth, float textureHeight)
	{
		m_texture = texture;

		m_uLeftUp  = static_cast<float>(textureX) / m_texture->GetWidth(); // 左上設定
		m_vLeftUp  = static_cast<float>(textureY) / m_texture->GetHeight(); // 左上設定
		m_uRightDown = static_cast<float>(textureX + textureWidth) / m_texture->GetWidth(); // 右下設定
		m_vRightDown = static_cast<float>(textureY + textureHeight) / m_texture->GetHeight(); // 右下設定
	}
	/**
	*           サイズの変更
	*/
	void Billboard::ChangeSize(float width, float height)
	{
		m_width = width;
		m_height = height;
	}

	/**
	*              描    画
	*/
	void Billboard::Draw( float x, float y,float z,float angle)
	{
		/// 頂点データ
		VERTEX_TEXTURE3D vertices[4];
		D3DXMATRIX matWorld, matRot, matTrans;

		// ワールド行列の設定
		matRot = GetBillboardMatrix();
		D3DXMatrixTranslation(&matTrans, x, y, z);
		matWorld = matRot * matTrans;
		// 回転用行列
		D3DXMATRIX matWorld2, matRot2;

		D3DXMatrixIdentity(&matWorld2);
		D3DXMatrixRotationZ( &matRot2 , angle * (D3DX_PI/180.0f));
		matWorld2 *= matRot2;
		matWorld2 *= matWorld;

		m_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld2);

		vertices[0].position = D3DXVECTOR3(-m_width, m_height, 0.0f);
		vertices[0].diffuse  = 0xffffffff;
		vertices[0].u        = m_uLeftUp;
		vertices[0].v        = m_vLeftUp;

		vertices[1].position = D3DXVECTOR3(m_width, m_height, 0.0f);
		vertices[1].diffuse  = 0xffffffff;
		vertices[1].u        = m_uRightDown;
		vertices[1].v        = m_vLeftUp;

		vertices[2].position = D3DXVECTOR3(-m_width, -m_height, 0.0f);
		vertices[2].diffuse  = 0xffffffff;
		vertices[2].u        = m_uLeftUp;
		vertices[2].v        = m_vRightDown;

		vertices[3].position = D3DXVECTOR3(m_width, -m_height, 0.0f);
		vertices[3].diffuse  = 0xffffffff;
		vertices[3].u        = m_uRightDown;
		vertices[3].v        = m_vRightDown;


		// Ｚバッファを使用
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

		// アルファブレンド(ON)
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// アルファ値が０の場合Ｚバッファに書き込まない
		m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 1);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

		// テクスチャステージの設定
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		// テクスチャの設定
		if (m_texture != NULL)
		{
			m_pD3DDevice->SetTexture(0, m_texture->GetTexture());
		}
		else
		{
			m_pD3DDevice->SetTexture(0, NULL);
		}

		// 頂点フォーマットの設定
		m_pD3DDevice->SetFVF(D3DFVF_TEXTURE3D);

		// プリミティブの描画
		m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VERTEX_TEXTURE3D));
	}
	/**
	*        ビルボード行列を返す
	*/
	D3DXMATRIX Billboard::GetBillboardMatrix()
	{
		D3DXMATRIX matView;
		D3DXMATRIX matBillboard;

		m_pD3DDevice->GetTransform(D3DTS_VIEW,&matView);

		D3DXMatrixIdentity(&matBillboard);

		matBillboard._11 = matView._11;
		matBillboard._12 = matView._21;
		matBillboard._13 = matView._31;
		matBillboard._14 = 0.0f;

		matBillboard._21 = matView._12;
		matBillboard._22 = matView._22;
		matBillboard._23 = matView._32;
		matBillboard._24 = 0.0f;

		matBillboard._31 = matView._13;
		matBillboard._32 = matView._23;
		matBillboard._33 = matView._33;
		matBillboard._34 = 0.0f;

		matBillboard._41 = 0.0f;
		matBillboard._42 = 0.0f;
		matBillboard._43 = 0.0f;
		matBillboard._44 = 1.0f;

		return matBillboard;
	}
	/**
	*         描画関係のまとめ
	*/
	void Billboard::DrawAll(const NW_Lib::Texture *texture,float x, float y,float z/**, float width, float height*/, float textureX, float textureY, float textureWidth, float textureHeight,float angle)
	{
		//ChangeSize(width,height);
		SetTexture(texture,textureX,textureY,textureWidth,textureHeight);
		Draw(x,y,z,angle); // 背景
	}

	void Billboard::DrawBillboard(TEXTURE* pTex, float dx, float dy,float dz , float dw, float dh,
						float tx, float ty, float tw, float th, float rot, float alpha)
	{
		float x_Left,x_Right,y_Up,y_Down;

		if( DrawingScreenID == NW_Lib::UPPER_SCREEN_DRAWING )
		{
			// 左上基点に合わせる
			x_Left  = dx;
			x_Right = dx + dw;
			y_Up    = dy;
			y_Down  = dy + dh;
		}
		else if( DrawingScreenID == NW_Lib::LOWER_SCREEN_DRAWING )
		{
			x_Left  = dx + LOWER_SCREEN_OFFSET_WIDTH;
			x_Right = dx + LOWER_SCREEN_OFFSET_WIDTH + dw;
			y_Up    = dy + LOWER_SCREEN_OFFSET_HEIGHT;
			y_Down  = dy + LOWER_SCREEN_OFFSET_HEIGHT + dh;
		}

		/// 頂点データ
		VERTEX_TEXTURE3D vertices[4];
		D3DXMATRIX matWorld, matRot, matTrans;

		// ワールド行列の設定
		{
			D3DXMATRIX matView;
			D3DXMATRIX matBillboard;

			m_pD3DDevice->GetTransform(D3DTS_VIEW,&matView);

			D3DXMatrixIdentity(&matRot);

			matBillboard._11 = matView._11;
			matBillboard._12 = matView._21;
			matBillboard._13 = matView._31;
			matBillboard._14 = 0.0f;

			matBillboard._21 = matView._12;
			matBillboard._22 = matView._22;
			matBillboard._23 = matView._32;
			matBillboard._24 = 0.0f;

			matBillboard._31 = matView._13;
			matBillboard._32 = matView._23;
			matBillboard._33 = matView._33;
			matBillboard._34 = 0.0f;

			matBillboard._41 = 0.0f;
			matBillboard._42 = 0.0f;
			matBillboard._43 = 0.0f;
			matBillboard._44 = 1.0f;

			matRot = matBillboard;
		}


		D3DXMatrixTranslation(&matTrans, dx, dy, dz);
		matWorld = matRot * matTrans;
		// 回転用行列
		D3DXMATRIX matWorld2, matRot2;

		D3DXMatrixIdentity(&matWorld2);
		D3DXMatrixRotationZ( &matRot2 , rot * (D3DX_PI/180.0f));
		matWorld2 *= matRot2;
		matWorld2 *= matWorld;

		float uLeftUp	   = static_cast<float>(tx) / pTex->GetWidth(); // 左上設定
		float vLeftUp	   = static_cast<float>(ty) / pTex->GetHeight(); // 左上設定
		float uRightDown = static_cast<float>(tx + tw) / pTex->GetWidth(); // 右下設定
		float vRightDown = static_cast<float>(ty + th) / pTex->GetHeight(); // 右下設定

		m_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld2);

		vertices[0].position = D3DXVECTOR3(-dw / 2.0f, dh / 2.0f, 0.0f);
		vertices[0].diffuse  = 0xffffffff;
		vertices[0].u        = uLeftUp;
		vertices[0].v        = vLeftUp;

		vertices[1].position = D3DXVECTOR3(dw / 2.0f, dh / 2.0f, 0.0f);
		vertices[1].diffuse  = 0xffffffff;
		vertices[1].u        = uRightDown;
		vertices[1].v        = vLeftUp;

		vertices[2].position = D3DXVECTOR3(-dw / 2.0f, -dh / 2.0f, 0.0f);
		vertices[2].diffuse  = 0xffffffff;
		vertices[2].u        = uLeftUp;
		vertices[2].v        = vRightDown;

		vertices[3].position = D3DXVECTOR3(dw / 2.0f, -dh / 2.0f, 0.0f);
		vertices[3].diffuse  = 0xffffffff;
		vertices[3].u        = uRightDown;
		vertices[3].v        = vRightDown;


		// Ｚバッファを使用
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

		// アルファブレンド(ON)
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// アルファ値が０の場合Ｚバッファに書き込まない
		m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 1);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

		// テクスチャステージの設定
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		// テクスチャの設定
		if (pTex != NULL)
		{
			m_pD3DDevice->SetTexture(0, pTex->GetTexture());
		}
		else
		{
			m_pD3DDevice->SetTexture(0, NULL);
		}

		// 頂点フォーマットの設定
		m_pD3DDevice->SetFVF(D3DFVF_TEXTURE3D);

		// プリミティブの描画
		m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VERTEX_TEXTURE3D));
		m_pD3DDevice->SetTexture(0,NULL);

		// Zバッファをオンにする
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
		// Ｚバッファと比較する
		m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		// Ｚバッファに書き込む
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

