/**
*	@file	sprite.cpp
*
*	@brief	２Dスプライト用クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#include "NW_Lib_sprite.h"
#include "NW_Lib_Texture.h"

#include "../Utility/NW_Lib_HardwareDefine.h"

#include "../System/NW_Lib_SceneManager.h"

namespace NW_Lib
{
	/**
	*	コンストラクタ
	*	@param pD3DDevice  デバイス
	*/
	Sprite::Sprite(LPDIRECT3DDEVICE9 pD3DDevice)
	:m_width(1.0f)      // 幅
	,m_height(1.0f)    // 高さ
	,m_texture(NULL)     // テクスチャ
	,m_uLeftUp(0.0f)     // 左上Ｕ座標
	,m_vLeftUp(0.0f)     // 左上Ｖ座標
	,m_uRightDown(0.0f)  // 右下Ｕ座標
	,m_vRightDown(0.0f)  // 右下Ｖ座標
	,m_pD3DDevice(pD3DDevice)
	,m_Alpha(1.0f)
	,m_Color(0x00ffffff)
	{
	}

	/**
	*	デストラクタ
	*/
	Sprite::~Sprite()
	{
	}

	/**
	*	テクスチャの設定
	*	@param texture  テクスチャ情報
	*/
	void Sprite::SetTexture(const Texture *texture)
	{
		m_texture = texture;

		m_uLeftUp  = 0.0f; // 左上設定
		m_vLeftUp  = 0.0f; // 左上設定
		m_uRightDown = 1.0f; // 右下設定
		m_vRightDown = 1.0f; // 右下設定
	}

	/**
	*  テクスチャの設定 （指定バージョン）
	*	@param texture        テクスチャ情報
	*	@param textureX       テクスチャのX位置
	*	@param textureY       テクスチャのY位置
	*	@param textureWidth   テクスチャの幅
	*	@param textureHeight  テクスチャの高さ
	*/
	void Sprite::SetTexture(const Texture *texture, float textureX, float textureY, float textureWidth, float textureHeight)
	{
		m_texture = texture;

		m_uLeftUp  = static_cast<float>(textureX) / m_texture->GetWidth(); // 左上設定
		m_vLeftUp  = static_cast<float>(textureY) / m_texture->GetHeight(); // 左上設定
		m_uRightDown = static_cast<float>(textureX + textureWidth) / m_texture->GetWidth(); // 右下設定
		m_vRightDown = static_cast<float>(textureY + textureHeight) / m_texture->GetHeight(); // 右下設定
	}

	/**
	*	テクスチャのサイズ変更
	*	@param width  幅
	*	@param height 高さ
	*/
	void Sprite::ChangeSize(float width, float height)
	{
		m_width = width;
		m_height = height;
	}

	/**
	*	描画
	*	@param x      X位置
	*	@param y      Y位置
	*	@param angle  角度
	*	@param alfa   透明度
	*	@param center trueで中心を指定、falseで左上を指定して表示
	*/
	void Sprite::DrawSprite( float x, float y,float angle,float alfa,bool center)
	{
		float x_Left,x_Right,y_Up,y_Down;

		if(center){
			// 真ん中基点に合わせる
			x_Left  = x - m_width/2;
			x_Right = x + m_width/2;
			y_Up    = y - m_height/2;
			y_Down  = y + m_height/2;
		}else{
			// 左上基点に合わせる
			x_Left  = x;
			x_Right = x + m_width;
			y_Up    = y;
			y_Down  = y + m_height;
		}

		// アルファの計算
		DWORD alp = static_cast<DWORD>(0xff * alfa) << ( 8 * 3 );

		/// 元の頂点データ
		VERTEX_TEXTURE2D vertices[4] = 
		{
			{D3DXVECTOR3(x_Left , y_Up ,0.0f), 1.0f, m_Color + alp, m_uLeftUp   , m_vLeftUp   },
			{D3DXVECTOR3(x_Right, y_Up ,0.0f), 1.0f, m_Color + alp, m_uRightDown, m_vLeftUp   },
			{D3DXVECTOR3(x_Left ,y_Down,0.0f), 1.0f, m_Color + alp, m_uLeftUp   , m_vRightDown},
			{D3DXVECTOR3(x_Right,y_Down,0.0f), 1.0f, m_Color + alp, m_uRightDown, m_vRightDown}
		};
		// 中心位置（Ｘ）　＝　 幅の半分  + Ｘ位置
		float coreX = m_width/2 + x_Left;
		// 中心位置（Ｙ）　＝　高さの半分 + Ｙ位置
		float coreY = m_height/2 + y_Up;
		float rad = (angle * D3DX_PI / 180.0f);

		// 回転の調整
		for(int i = 0;i < 4;++i){

			int flgx[4] = {-1,1,-1,1};
			int flgy[4] = {1,1,-1,-1};

			vertices[i].position.x = coreX + (m_width/2*flgx[i] * cos(rad) - (m_height/2 * sin(rad) * flgy[i]));
			vertices[i].position.y = coreY - (m_width/2*flgx[i] * sin(rad) + (m_height/2 * cos(rad) * flgy[i]));
		}


		// Ｚバッファに書き込まない
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// Ｚバッファと比較しない
		m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

		//m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);

		// 透明化させる
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


		// テクスチャステージの設定
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		// テクスチャの設定
		if(m_texture != NULL){
			m_pD3DDevice->SetTexture(0,m_texture->GetTexture());
		}else{
			m_pD3DDevice->SetTexture(0,NULL);
		}

		// 頂点フォーマットの設定
		m_pD3DDevice->SetFVF(D3DFVF_TEXTURE2D);

		// 描画
		m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VERTEX_TEXTURE2D));

		m_pD3DDevice->SetTexture(0,NULL);

		// Zバッファをオンにする
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
		// Ｚバッファと比較する
		m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		// Ｚバッファに書き込む
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	}

	/**
	*	描画のまとめ
	*	@param texture       テクスチャ情報
	*	@param x             X位置
	*	@param y             Y位置
	*	@param texturex      テクスチャのX
	*	@param texturey      テクスチャのY
	*	@param textureWidth  幅
	*	@param textureHeight 高さ
	*	@param center trueで中心を指定、falseで左上を指定して表示
	*	@param angle         角度
	*	@param alfa          透明度
	*/
	void Sprite::DrawAll(const NW_Lib::Texture *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,bool center,float angle,float alpha)
	{
		ChangeSize(width,height);
		SetTexture(texture,textureX,textureY,textureWidth,textureHeight);
		DrawSprite(x,y,angle,alpha,center);
	}

	void Sprite::DrawUpper(const NW_Lib::Texture *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,bool center,float angle,float alpha)
	{
		ChangeSize(width,height);
		SetTexture(texture,textureX,textureY,textureWidth,textureHeight);
		DrawSprite(x,y,angle,alpha,center);
	}
	void Sprite::DrawLower(const NW_Lib::Texture *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,bool center,float angle,float alpha)
	{
		ChangeSize(width,height);
		SetTexture(texture,textureX,textureY,textureWidth,textureHeight);
		DrawSprite(x + LOWER_SCREEN_OFFSET_WIDTH ,y + LOWER_SCREEN_OFFSET_HEIGHT,angle,alpha,center);
	}

	/**
	*	描画の設定
	*	@param texture       テクスチャ情報
	*	@param x             X位置
	*	@param y             Y位置
	*	@param texturex      テクスチャのX
	*	@param texturey      テクスチャのY
	*	@param textureWidth  幅
	*	@param textureHeight 高さ
	*	@param cx			 中心点X
	*	@param cy			 中心点Y
	*	@param angle         角度
	*	@param alfa          透明度
	*/	
	void Sprite::Draw(const NW_Lib::Texture *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight, float cx, float cy, float angle, float alpha)
	{

		float x_Left,x_Right,y_Up,y_Down;

		if( DrawingScreenID == NW_Lib::UPPER_SCREEN_DRAWING )
		{
			// 左上基点に合わせる
			x_Left  = x;
			x_Right = x + width;
			y_Up    = y;
			y_Down  = y + height;
		}
		else if( DrawingScreenID == NW_Lib::LOWER_SCREEN_DRAWING )
		{
			x_Left  = x + LOWER_SCREEN_OFFSET_WIDTH;
			x_Right = x + LOWER_SCREEN_OFFSET_WIDTH + width;
			y_Up    = y + LOWER_SCREEN_OFFSET_HEIGHT;
			y_Down  = y + LOWER_SCREEN_OFFSET_HEIGHT + height;
		}

		// アルファの計算
		DWORD alp = static_cast<DWORD>(0xff * alpha) << ( 8 * 3 );

		float uLeftUp	   = static_cast<float>(textureX) / texture->GetWidth(); // 左上設定
		float vLeftUp	   = static_cast<float>(textureY) / texture->GetHeight(); // 左上設定
		float uRightDown = static_cast<float>(textureX + textureWidth) / texture->GetWidth(); // 右下設定
		float vRightDown = static_cast<float>(textureY + textureHeight) / texture->GetHeight(); // 右下設定

		/// 元の頂点データ
		VERTEX_TEXTURE2D vertices[4] = 
		{
			{D3DXVECTOR3(x_Left , y_Up ,0.0f), 1.0f, 0x00ffffff + alp, uLeftUp   , vLeftUp   },
			{D3DXVECTOR3(x_Right, y_Up ,0.0f), 1.0f, 0x00ffffff + alp, uRightDown, vLeftUp   },
			{D3DXVECTOR3(x_Left ,y_Down,0.0f), 1.0f, 0x00ffffff + alp, uLeftUp   , vRightDown},
			{D3DXVECTOR3(x_Right,y_Down,0.0f), 1.0f, 0x00ffffff + alp, uRightDown, vRightDown}
		};
		// 中心位置（Ｘ）　＝　 幅の半分  + Ｘ位置
		float coreX = width/2 + x_Left - cx;
		// 中心位置（Ｙ）　＝　高さの半分 + Ｙ位置
		float coreY = height/2 + y_Up - cy;
		float rad = (angle * D3DX_PI / 180.0f);

		// 回転の調整
		for(int i = 0;i < 4;++i){

			int flgx[4] = {-1,1,-1,1};
			int flgy[4] = {1,1,-1,-1};

			vertices[i].position.x = coreX + (width/2*flgx[i] * cos(rad) - (height/2 * sin(rad) * flgy[i]));
			vertices[i].position.y = coreY - (width/2*flgx[i] * sin(rad) + (height/2 * cos(rad) * flgy[i]));
		}
		
		LPDIRECT3DDEVICE9 pD3DDevice = NW_Lib::System::SceneManager::GetInstance().GetDevice();

		// Ｚバッファに書き込まない
		pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// Ｚバッファと比較しない
		pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

		//pD3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);

		// 透明化させる
		pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


		// テクスチャステージの設定
		pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		// テクスチャの設定
		if(texture != NULL){
			pD3DDevice->SetTexture(0,texture->GetTexture());
		}else{
			pD3DDevice->SetTexture(0,NULL);
		}

		// 頂点フォーマットの設定
		pD3DDevice->SetFVF(D3DFVF_TEXTURE2D);

		// 描画
		pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VERTEX_TEXTURE2D));

		pD3DDevice->SetTexture(0,NULL);

		// Zバッファをオンにする
		pD3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
		// Ｚバッファと比較する
		pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		// Ｚバッファに書き込む
		pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		
	}

}



