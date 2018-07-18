/**
*	@file	sprite3D.cpp
*
*	@brief	3Dスプライト用クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/


#include "NW_Lib_sprite3D.h"
#include "NW_Lib_Texture.h"


namespace NW_Lib
{
	/**
	*	コンストラクタ
	*	@param pD3DDevice  デバイス
	*/
	Sprite3D::Sprite3D(LPDIRECT3DDEVICE9 pD3DDevice)
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
	,m_Rot(0.0f,0.0f,0.0f)
	{
	}

	/**
	*	デストラクタ
	*/
	Sprite3D::~Sprite3D()
	{
	}

	/**
	*	テクスチャの設定
	*	@param texture  テクスチャ情報
	*/
	void Sprite3D::SetTexture(const Texture *texture)
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
	void Sprite3D::SetTexture(const Texture *texture, float textureX, float textureY, float textureWidth, float textureHeight)
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
	void Sprite3D::ChangeSize(float width, float height)
	{
		m_width = width;
		m_height = height;
	}

	/**
	*	描画
	*	@param x      X位置
	*	@param y      Y位置
	*	@param z      Z位置
	*	@param angle  角度
	*	@param alfa   透明度
	*/
	void Sprite3D::Draw( float x, float y,float z,float angle,float alfa)
	{

		// アルファの計算
		DWORD alp = static_cast<DWORD>(0xff * alfa) << ( 8 * 3 );

		VERTEX_TEXTURE3D vertices[4] = 
		{
			{D3DXVECTOR3(- m_width/2,+ m_height/2,0.0f), m_Color + alp, m_uLeftUp   , m_vLeftUp   },
			{D3DXVECTOR3(+ m_width/2,+ m_height/2,0.0f), m_Color + alp, m_uRightDown, m_vLeftUp   },
			{D3DXVECTOR3(- m_width/2,- m_height/2,0.0f), m_Color + alp, m_uLeftUp   , m_vRightDown},
			{D3DXVECTOR3(+ m_width/2,- m_height/2,0.0f), m_Color + alp, m_uRightDown, m_vRightDown}
		};



		// 中心位置（Ｘ）　＝　 幅の半分  + Ｘ位置
		float coreX = m_width + x;
		// 中心位置（Ｙ）　＝　高さの半分 + Ｙ位置
		float coreY = m_height + y;
		float rad = (angle * D3DX_PI / 180.0f);

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
		if(m_texture != NULL){
			m_pD3DDevice->SetTexture(0,m_texture->GetTexture());
		}else{
			m_pD3DDevice->SetTexture(0,NULL);
		}

		// 頂点フォーマットの設定
		m_pD3DDevice->SetFVF(D3DFVF_TEXTURE3D);


		D3DXMATRIX m;
		D3DXMATRIX moveRot,moveMat;
		D3DXMatrixIdentity(&m);
		D3DXMatrixRotationYawPitchRoll(&moveRot,m_Rot.y * D3DX_PI/180.0f,m_Rot.x * D3DX_PI/180.0f,m_Rot.z * D3DX_PI/180.0f);
		D3DXMatrixTranslation(&moveMat,x,y,z);
		m *= moveRot;
		m *= moveMat;
		m_pD3DDevice->SetTransform(D3DTS_WORLD,&m);

		// 描画
		m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VERTEX_TEXTURE3D));

		// テクスチャの初期化
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
	*	@param z             Z位置
	*	@param texturex      テクスチャのX
	*	@param texturey      テクスチャのY
	*	@param textureWidth  幅
	*	@param textureHeight 高さ
	*	@param angle         角度
	*	@param alfa          透明度
	*/
	void Sprite3D::DrawAll(const NW_Lib::Texture *texture,float x, float y,float z, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,float angle,float alpha)
	{
		ChangeSize(width,height);
		SetTexture(texture,textureX,textureY,textureWidth,textureHeight);
		Draw(x,y,z,angle,alpha); // 背景
	}
}














