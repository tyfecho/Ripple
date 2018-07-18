/**
*	@file	Colision.h
*
*	@brief	当たり判定用クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#pragma once

#include <d3dx9.h>


// 衝突判定球
struct Sphere
{
	D3DXVECTOR3 center; // 球の中心
	float Radius;       // 半径
};

// 衝突判定用線分
struct Segment
{
	D3DXVECTOR3 Start;
	D3DXVECTOR3 End;
};

// 衝突判定用メッシュ情報
struct MeshInfo
{
	// メッシュ
	LPD3DXMESH pMesh;
	// メッシュのワールド行列
	D3DXMATRIX* pWorldMatrix;
};

// 衝突判定用ポリゴン
struct PolygonInfo
{
	D3DXVECTOR3 vPoint[3];
};

// OBB判定用情報
struct OBBInfo
{
	// 箱そのものの回転行列
	D3DXMATRIX rotation;

	// 基本位置
	D3DXVECTOR3 position;

	// X,Y,Zそれぞれのの軸情報
	D3DXVECTOR3 AxisX;
	D3DXVECTOR3 AxisY;
	D3DXVECTOR3 AxisZ;

	float LengthX;
	float LengthY;
	float LengthZ;

	// 最大の位置
	D3DXVECTOR3 Max;
	// 最小の位置
	D3DXVECTOR3 Min;
};


namespace NW_Lib
{
	class G3DObj;

	class Colision
	{
		public:
			Colision(){};
			~Colision(){};
			// スフィア
			bool ColisionCheckSphere2(Sphere* pBS1,Sphere* pBS2,D3DXVECTOR3* inter = NULL,float* distance = NULL);
			// 普通のポリゴン
			bool ColisionCheckSegmentToMesh(Segment* pSegment,MeshInfo* pMeshInfo,float* pDistance,D3DXVECTOR3* pInter);
			bool FindVerticesOnPoly(LPD3DXMESH pMesh,DWORD dwIndex,D3DXVECTOR3* pvVertices);
			// アニメーションメッシュとのレイ判定
			bool ColisionCheckSegmentToAnimationMesh(LPD3DXFRAME pFrameBase,Segment* pSegment,G3DObj* pAnimationModel,BOOL* pboHit,
						float* pfDistance,LPD3DXMESHCONTAINER* ppContainer,D3DXVECTOR3* pvIntersect);
			// OBB
			bool OBBCollisionDetection(OBBInfo* pThingA,OBBInfo* pThingB);
			bool CompareLength(OBBInfo* pboxA,OBBInfo* pboxB,D3DXVECTOR3* pvSeparate,D3DXVECTOR3* pvDistance);
			// AABB
			bool AABBColision2(OBBInfo* pThingA,OBBInfo* pThingB);

			// 平面とレイ
			bool PolyRay(Segment* pSegment,PolygonInfo* pPoly,D3DXVECTOR3* pInfo = NULL);
			bool IsInside(D3DXVECTOR3* pvI,D3DXVECTOR3* pvA,D3DXVECTOR3* pvB,D3DXVECTOR3* pvC);
	};
}