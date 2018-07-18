/**
*	@file	Camera.cpp
*
*	@brief	カメラクラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#include "NW_Lib_camera.h"


namespace NW_Lib
{
	/**
	*	コンストラクタ
	*	@param pD3DDevice  デバイス
	*/
	Camera::Camera()
	:m_EyePos(0.0f,0.0f,-10.0f)
	,m_LookAtPos(0.0f,0.0f,0.0f)
	,m_UpVec(0.0f,1.0f,0.0f)
	,m_AddEyePos(0.0f,0.0f,0.0f)
	,m_AddLookAtPos(0.0f,0.0f,0.0f)
	,m_Aspect(1.0f)
	,m_NearClip(1.0f)
	,m_FarClip(100.0f)
	,m_Rot(D3DX_PI/4)
	,m_fog(false)
	,m_viewMode(0)
	,m_orthoSize(400.0f,240.0f)
	{
	}
	/**
	*	デストラクタ
	*/
	Camera::~Camera()
	{
	}
	/**
	*	アスペクト比の設定
	*	@param width  幅
	*	@param height 高さ
	*/
	void Camera::SetAspect(float width,float height){
		m_Aspect = width / height;
	}
	/**
	*	カメラの設定
	*/
	void Camera::ShowCamera(D3DXVECTOR3* pView,int viewport)
	{
		// カメラ位置の設定
		D3DXMatrixLookAtLH(&m_matView,&pView[viewport],&m_LookAtPos,&m_UpVec);
		m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );

		D3DXMATRIXA16 matProj;

		if(m_viewMode == MODE_PERSPECTIVE)
		{
			// フィールドビュー、視野の設定
			D3DXMatrixPerspectiveFovLH( &matProj,
										m_Rot,
										m_Aspect,
										m_NearClip,
										m_FarClip );
		}
		else if(m_viewMode == MODE_ORTHO)
		{
			//Ortho
			D3DXMatrixOrthoLH(&matProj,m_orthoSize.x,m_orthoSize.y,0.0f,0.0f);
		}
		m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );

		// フォグ
		if(m_fog){
			Fog();
		}

	}
	/**
	*	カメラの設定
	*/
	void Camera::ShowCamera()
	{
		// カメラ位置の設定
		D3DXMatrixLookAtLH(&m_matView,&m_EyePos,&m_LookAtPos,&m_UpVec);
		m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );
		D3DXMATRIXA16 matProj;

		// フィールドビュー、視野の設定
		D3DXMatrixPerspectiveFovLH( &matProj,
									m_Rot,
									m_Aspect,
									m_NearClip,
									m_FarClip );
		//D3DXMatrixOrthoLH(
		//	&matProj,
		//	320.0f,
		//	240.0f,
		//	1.0f,
		//	10000.0f
		//	);

		//D3DXMATRIX world;
		//D3DXMatrixIdentity(&world);
		//D3DXMatrixTranslation(&world,0.0f,0.0f,10.0f);
		//D3DXMatrixOrthoLH(&world,640.0f,480.0f,0.0f,0.0f);

		m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );

		// フォグ
		if(m_fog){
			Fog();
		}

		m_AddEyePos    = D3DXVECTOR3(0.0f,0.0f,0.0f);
		m_AddLookAtPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}

	/**
	*	フォグの処理
	*/
	void Camera::Fog()
	{
		float Start   = 15.5f;    // For linear mode
		float End     = 110.8f;
		float Density = 80.0f;   // For exponential modes
		 
		// Enable fog blending.
		m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		 
		// Set the fog color.
		m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR,  D3DCOLOR_ARGB(0x80,0x00,0x00,0x00));
		    
		// Set fog parameters.
		if( D3DFOG_LINEAR == D3DFOG_LINEAR )
		{
			m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
			m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
			m_pD3DDevice->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&End));
		}
		else
		{
			m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
			m_pD3DDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&Density));
		}

		m_AddEyePos    = D3DXVECTOR3(0.0f,0.0f,0.0f);
		m_AddLookAtPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}

	/**
	*	視錐台の中かどうか
	*	@param pos  判定する点
	*	return true:画面内 false:画面外
	*/
	bool Camera::Culling(D3DXVECTOR3 pos)
	{
		D3DXPLANE VFLeftPlane,VFRightPlane,VFTopPlane,VFBottomPlane;
		float Radius = 0.0f;
		float farclip = m_FarClip + 50.0f;

		D3DXVECTOR3 v;

		D3DXVec3Normalize(&v,&(m_EyePos-m_LookAtPos));

		D3DXVECTOR3 eyepos = m_EyePos + v*16;

		D3DXMatrixLookAtLH(&m_matView,&eyepos,&m_LookAtPos,&m_UpVec);

		//まず、ジオメトリの位置ベクトルをワールドからビュー空間に変換
		D3DXVec3TransformCoord(&pos,&pos,&m_matView);

		//左右、上下の平面を計算
		D3DXVECTOR3 p1,p2,p3;
		//左面
		p1 = D3DXVECTOR3(0,0,0);
		p2.x = -farclip * (static_cast<float>(tan(m_Rot/2) * m_Aspect));
		p2.y = -farclip * static_cast<float>(tan(m_Rot/2));
		p2.z = farclip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFLeftPlane,&p1,&p2,&p3);
		//右面
		p1 = D3DXVECTOR3(0,0,0);
		p2.x = farclip * (static_cast<float>(tan(m_Rot/2) * m_Aspect));
		p2.y = farclip * static_cast<float>(tan(m_Rot/2));
		p2.z = farclip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFRightPlane,&p1,&p2,&p3);
		//上面
		p1 = D3DXVECTOR3(0,0,0);
		p2.x = -farclip * (static_cast<float>(tan(m_Rot/2) * m_Aspect));
		p2.y = farclip * static_cast<float>(tan(m_Rot/2));
		p2.z = farclip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFTopPlane,&p1,&p2,&p3);
		//下面
		p1 = D3DXVECTOR3(0,0,0);
		p2.x = farclip * (static_cast<float>(tan(m_Rot/2) * m_Aspect));
		p2.y = -farclip * static_cast<float>(tan(m_Rot/2));
		p2.z = farclip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFBottomPlane,&p1,&p2,&p3);

		//6つの平面とジオメトリ境界球をチェック
		//ニアクリップ面について
		if( (pos.z + Radius) < m_NearClip){
			return false;
		}
		//ファークリップ面について
		if( (pos.z - Radius) > farclip){
			return false;
		}		
		//左クリップ面について
		float Distance=(pos.x * VFLeftPlane.a) + (pos.z * VFLeftPlane.c);
		if(Distance > Radius){
			return false;
		}
		//右クリップ面について
		Distance=(pos.x * VFRightPlane.a) + (pos.z * VFRightPlane.c);
		if(Distance > Radius){
			return false;
		}
		//上クリップ面について
		Distance=(pos.y * VFTopPlane.b) + (pos.z * VFTopPlane.c);
		if(Distance > Radius){
			return false;
		}
		//下クリップ面について
		Distance=(pos.y * VFBottomPlane.b) + (pos.z * VFBottomPlane.c);
		if(Distance > Radius){
			return false;
		}				

		// 画面内だった
		return true;
	}
}




