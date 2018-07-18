/**
*	@file	Camera.h
*
*	@brief	カメラクラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#pragma once


#include <d3dx9.h>
#include "../Utility/NW_Lib_Constant.h"
#include "../System/NW_Lib_Common.h"
#include "../Graphics/NW_Lib_texture.h"


enum MODE
{
	MODE_PERSPECTIVE,
	MODE_ORTHO,
};


namespace NW_Lib
{
	class Camera : public Common
	{
		private:
			D3DXVECTOR3 m_EyePos;    // カメラ自身の位置
			D3DXVECTOR3 m_LookAtPos; // カメラの見ている位置
			D3DXVECTOR3 m_UpVec;     // 上方向ベクトル

			D3DXVECTOR3 m_AddEyePos;
			D3DXVECTOR3 m_AddLookAtPos;

			D3DXMATRIX m_matView;

			float m_Aspect;   // アスペクト比
			float m_NearClip; // 近い限界値
			float m_FarClip;  // 遠い限界値
			float m_Rot;

			bool m_fog;

			int m_viewMode; //Parspectiive / Ortho
			VECTOR2 m_orthoSize;

			//LPDIRECT3DDEVICE9 m_pD3DDevice;
		public:
			Camera();
			~Camera();
			void SetAspect(float width,float height);
			void ShowCamera(D3DXVECTOR3* pView,int viewport); // カメラの設定
			void ShowCamera();                                // カメラの設定

			void Fog();

			bool Culling(D3DXVECTOR3 pos);

			// インライン関数

			// カメラの位置の取得
			D3DXVECTOR3 GetCameraPos()
			{
				return m_EyePos; 
			}
			// 現在設定されているデータで最終の位置を予測し、それを返す
			D3DXVECTOR3 GetResultEyePos()
			{
				return m_EyePos + m_AddEyePos;
			}

			// カメラの視点を取得
			D3DXVECTOR3 GetCameraTarget()
			{
				return m_LookAtPos; 
			}
			// 現在設定されているデータで最終の位置を予測し、それを返す
			D3DXVECTOR3 GetResultLookAtPos()
			{
				return m_LookAtPos + m_AddLookAtPos;
			}

			//カメラ自身の位置のセット
			void SetCameraPosX(float pos_x)
			{
				m_EyePos.x = pos_x; 
			}
			void SetCameraPosY(float pos_y)
			{
				m_EyePos.y = pos_y; 
			}
			void SetCameraPosZ(float pos_z)
			{
				m_EyePos.z = pos_z;
			}
			void SetCameraPos(float pos_x, float pos_y, float pos_z)
			{
				m_EyePos.x = pos_x;
				m_EyePos.y = pos_y;
				m_EyePos.z = pos_z;
			}
			void SetCameraPos(D3DXVECTOR3 pos) 
			{ 
				m_EyePos = pos; 
			}
			void AddCameraPos(D3DXVECTOR3 vec)
			{
				m_AddEyePos += vec;
			}


			// カメラの見る先の設定
			void SetCameraTargetX(float pos_x) 
			{ 
				m_LookAtPos.x = pos_x; 
			}
			void SetCameraTargetY(float pos_y) 
			{ 
				m_LookAtPos.y = pos_y; 
			}
			void SetCameraTargetZ(float pos_z) 
			{
				m_LookAtPos.z = pos_z; 
			}
			void SetCameraTarget(D3DXVECTOR3 pos) 
			{ 
				m_LookAtPos = pos; 
			}
			void SetCameraTarget(float pos_x, float pos_y, float pos_z)
			{
				m_LookAtPos.x = pos_x;
				m_LookAtPos.y = pos_y;
				m_LookAtPos.z = pos_z;
			}
			void AddCameraTarget(D3DXVECTOR3 vec)
			{
				m_LookAtPos += vec;
			}


			// カメラの上方向ベクトルの設定
			void SetVecUp(float pos_x, float pos_y, float pos_z)
			{
				m_UpVec.x = pos_x;
				m_UpVec.y = pos_y;
				m_UpVec.z = pos_z;
			}

			// フォグをセットするかどうか
			void SetFog(bool set)
			{
				m_fog = set;
			}

			// アスペクト比を返す
			float GetAspect()
			{
				return m_Aspect;
			}

			// 手前のクリップ値を設定
			void SetNearClip(float nearClip)
			{
				m_NearClip = nearClip;
			}

			// 奥のクリップ値を設定
			void SetFarClip(float farClip)
			{
				m_FarClip = farClip;
			}

			// 足しこむベクトルを実際に足しこむ
			void Add()
			{
				m_EyePos    += m_AddEyePos;
				m_LookAtPos += m_AddLookAtPos;
			}
			
			// パースペクティブに設定する
			void SetPerspective()
			{
				m_viewMode = MODE_PERSPECTIVE;
			}

			// 正射影に設定する
			void SetOrtho(float width , float height)
			{
				m_viewMode = MODE_ORTHO;

				m_orthoSize.x = width;
				m_orthoSize.y = height;
			}

	};
}



