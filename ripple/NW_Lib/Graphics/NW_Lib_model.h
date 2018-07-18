/**
*	@file	model.h
*
*	@brief	3Dモデル用クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#pragma once
#include "d3dx9.h"
#include "NW_Lib_texture.h"
#include "../System//NW_Lib_Common.h"

#include "NW_Lib_IModel.h"


namespace NW_Lib
{
	class AnimController;

	class Model:public Common , public IModel
	{
		private:
			LPD3DXMESH    m_mesh;
			D3DMATERIAL9* m_meshMaterials;
			Texture**     m_meshtexture;
			DWORD         m_numMaterials;

			Texture** m_ppSetTex;
			D3DCOLORVALUE* m_pMasterAmbient;

			D3DXVECTOR3 m_Pos;    // 位置
			D3DXVECTOR3 m_Angle;  // 角度（ディグリー）
			D3DXVECTOR3 m_Radian; // 角度（ラジアン）
			D3DXVECTOR3 m_Scale;  // スケール（倍率）

			// ローカルワールド
			D3DXMATRIX m_LocalWorld;
			// 親行列
			D3DXMATRIX* m_pParentWorld;
			// ローカル計算前にかけておく行列
			D3DXMATRIX* m_pInitWorld;

			bool m_NoMaterial;
			bool m_MaterialGlay;
		public:
			Model();
			Model(char* filename);
			~Model();
			void Calc();
			void Draw(/*D3DVIEWPORT9* pViewport,int viewport,DWORD* pBackColor*/); // 描画処理
			void LoadMesh(const char* filename);                  // ロード処理
			void Release();                                 // 解放処理

			// テクスチャの設定
			void SetTexture(const char *filename,unsigned int tex_num);
			void SetTexture(Texture* pTex,unsigned int tex_num);
			void NoTexture();

			// サイズ変更
			void Scale(float scale)
			{
				m_Scale.x = scale;
				m_Scale.y = scale;
				m_Scale.z = scale;
			}
			void Scale(D3DXVECTOR3 scale)
			{
				m_Scale = scale;
			}
			void Scale(float scl_x,float scl_y,float scl_z)
			{
				m_Scale.x = scl_x;
				m_Scale.y = scl_y;
				m_Scale.z = scl_z;
			}
			// 移動
			void Move(D3DXVECTOR3 pos)
			{
				m_Pos = pos;
			}
			void Move(float x,float y,float z)
			{
				m_Pos.x = x;
				m_Pos.y = y;
				m_Pos.z = z;
			}
			void MoveX(float pos_x)
			{
				m_Pos.x = pos_x;
			}
			void MoveY(float pos_y)
			{
				m_Pos.y = pos_y;
			}
			void MoveZ(float pos_z)
			{
				m_Pos.z = pos_z;
			}
			// 回転
			void Rotation(D3DXVECTOR3 rot)
			{
				m_Angle = rot;
				m_Radian.x = m_Angle.x * D3DX_PI / 180.0f;
				m_Radian.y = m_Angle.y * D3DX_PI / 180.0f;
				m_Radian.z = m_Angle.z * D3DX_PI / 180.0f;
			}
			void Rotation(float deg_x,float deg_y,float deg_z)
			{
				m_Angle.x = deg_x;
				m_Radian.x = m_Angle.x * D3DX_PI / 180.0f;

				m_Angle.y = deg_y;
				m_Radian.y = m_Angle.y * D3DX_PI / 180.0f;

				m_Angle.z = deg_z;
				m_Radian.z = m_Angle.z * D3DX_PI / 180.0f;

			}
			void RotationX(float degX)
			{
				m_Angle.x = degX;
				m_Radian.x = m_Angle.x * D3DX_PI / 180.0f;
			}
			void RotationY(float degY)
			{
				m_Angle.y = degY;
				m_Radian.y = m_Angle.y * D3DX_PI / 180.0f;
			}
			void RotationZ(float degZ)
			{
				m_Angle.z = degZ;
				m_Radian.z = m_Angle.z * D3DX_PI / 180.0f;
			}
			// 透明度の変更
			void SetAlfa(float alfa)
			{
				for(DWORD i = 0;i < m_numMaterials;++i){
					m_meshMaterials[i].Diffuse.a = alfa;
					m_meshMaterials[i].Ambient.a = m_meshMaterials[i].Diffuse.a;
				}
			}

			// マテリアル色の設定
			void SetColor(float r,float g,float b)
			{
				for(DWORD i = 0;i < m_numMaterials;++i){
					m_meshMaterials[i].Diffuse.r = r;
					m_meshMaterials[i].Diffuse.g = g;
					m_meshMaterials[i].Diffuse.b = b;
					m_meshMaterials[i].Ambient.r = r;
					m_meshMaterials[i].Ambient.g = g;
					m_meshMaterials[i].Ambient.b = b;
				}
			}
			// 元のマテリアルに戻す
			void TakeBackMaterial()
			{
				for(DWORD i = 0;i < m_numMaterials;++i){
					m_meshMaterials[i].Diffuse = m_pMasterAmbient[i];
					m_meshMaterials[i].Ambient = m_pMasterAmbient[i];
				}
			}

			// 親行列の指定
			void SetParentMat(D3DXMATRIX* pParentWorld)
			{
				m_pParentWorld = pParentWorld;
			}
			// 初期化行列の指定
			void SetInittMat(D3DXMATRIX* pInitWorld)
			{
				m_pInitWorld = pInitWorld;
			}

			// メッシュ情報を返す
			LPD3DXMESH GetMesh()
			{
				return m_mesh;
			}
			// ローカル行列を返す
			D3DXMATRIX* GetLocal()
			{
				return &m_LocalWorld;
			}

			// マテリアルを設定しない
			void NoSetMaterial()
			{
				m_NoMaterial = true;
				m_MaterialGlay = false;
			}

			// マテリアルを固有のものに設定する
			void SetMaterial()
			{
				m_NoMaterial = false;
				m_MaterialGlay = false;
			}
			// マテリアルをグレーにする
			void SetMaterialGlay()
			{
				m_NoMaterial = false;
				m_MaterialGlay = true;
			}

			/*=======================================================
				関数名	：	Update
				説明	：	更新
				引数	：	アニメーションコントローラ
							ワールド行列
				戻り値	：　なし
			========================================================*/
			void Update(AnimController* pAnimController , LPD3DXMATRIX pWorld)
			{
				m_LocalWorld = *pWorld;
			};

			/*=======================================================
				関数名	：	Draw
				説明	：	メッシュの描画
				引数	：	ワールドマトリックス
							アニメーションコントローラ
							アルファ
				戻り値	：　なし
			========================================================*/
			void Draw( D3DXMATRIX world , AnimController* pAnimController , float alpha = 1.0f)
			{
				m_LocalWorld = world;

				Draw();
			};

	};
}


