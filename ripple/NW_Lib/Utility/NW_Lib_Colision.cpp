/**
*	@file	Colision.cpp
*
*	@brief	当たり判定用クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#include "NW_Lib_Colision.h"


namespace NW_Lib
{

	/**
	*	球と球の当たり判定
	*/
	bool Colision::ColisionCheckSphere2(Sphere* pBS1,Sphere* pBS2,D3DXVECTOR3* inter,float* distance)
	{
		D3DXVECTOR3 temp = pBS1->center - pBS2->center;

		float d = D3DXVec3Length(&temp);

		if(distance != NULL){
			// 埋まっている距離を計算する
			*distance = d - (pBS1->Radius + pBS2->Radius);
		}

		if(d > pBS1->Radius + pBS2->Radius){
			// 当たってない
			return false;
		}
		// 当たっている
		if(inter != NULL){
			// 大体の交点を計算
			*inter = D3DXVECTOR3(0.0f,0.0f,0.0f);
			D3DXVECTOR3 adda;
			D3DXVECTOR3 addb;

			D3DXVec3Scale(&adda,&pBS1->center,pBS1->Radius);
			D3DXVec3Scale(&addb,&pBS2->center,pBS2->Radius);

			D3DXVec3Add(&temp,&adda,&addb);

			D3DXVec3Scale(inter,&temp,1.0f/(pBS1->Radius + pBS2->Radius));
		}

		return true;
	}

	/**
	*	線分によるメッシュとの交差判定
	*/
	bool Colision::ColisionCheckSegmentToMesh(Segment* pSegment,MeshInfo* pMeshInfo,float* pDistance,D3DXVECTOR3* pInter)
	{
		D3DXVECTOR3 vStart,vEnd,vDirection;
		vStart = pSegment->Start;
		vDirection = pSegment->End - pSegment->Start;
		D3DXMATRIX InvWorld;

		BOOL HitFlg = FALSE;

		DWORD dwIndex = 0;
		float u = 0,v = 0.0f,distance = 0.0f;

		D3DXVECTOR3 vVertex[3];


		
		//D3DXVec3Normalize(&vDirection,&vDirection);

		// 直線の座標をワールドからメッシュ座標系にに変換
		D3DXMatrixInverse(&InvWorld,NULL,pMeshInfo->pWorldMatrix);
		D3DXVec3TransformCoord(&vStart,&pSegment->Start,&InvWorld);
		D3DXVec3TransformCoord(&vEnd,&pSegment->End,&InvWorld);

		vDirection = vEnd - vStart;
		float start2end = D3DXVec3Length(&vDirection);
		D3DXVec3Normalize(&vDirection,&vDirection);

		HRESULT hr = D3DXIntersect(pMeshInfo->pMesh,&vStart,&vDirection,&HitFlg,&dwIndex,&u,&v,&distance,NULL,NULL);

		if(hr == S_OK && HitFlg){

			if(distance <= start2end){

				if(FindVerticesOnPoly(pMeshInfo->pMesh,dwIndex,vVertex)){
					// 重心座標から支点を算出
					// 交点はv0+u * ()v1-v0) * uc
					*pInter = vVertex[0] + u*(vVertex[1] - vVertex[0]) + v*(vVertex[2] - vVertex[0]);

					D3DXVec3TransformCoord(pInter,pInter,pMeshInfo->pWorldMatrix);

					*pDistance = distance;
					return true;
				}
			}
		}

		return false;
	}

	/**
	*	メッシュ中のポリゴン番号から頂点座標を割り出す
	*/
	bool Colision::FindVerticesOnPoly(LPD3DXMESH pMesh,DWORD dwIndex,D3DXVECTOR3* pvVertices)
	{
		DWORD dwStride = pMesh->GetNumBytesPerVertex();
		DWORD dwvertexNum = pMesh->GetNumVertices();

		WORD* pwPoly = NULL;

		if( FAILED(pMesh->LockIndexBuffer(D3DLOCK_READONLY,(void**)(&pwPoly)))){
			return false;
		}

		LPDIRECT3DVERTEXBUFFER9 VB = NULL;
		pMesh->GetVertexBuffer(&VB);

		unsigned char* pbVertices;

		if(SUCCEEDED(VB->Lock(0,0,(void**)&pbVertices,0))){
			float* pfVertices;

			pfVertices = (float*)(&pbVertices[dwStride*pwPoly[dwIndex * 3]]);
			pvVertices[0].x = pfVertices[0];
			pvVertices[0].y = pfVertices[1];
			pvVertices[0].z = pfVertices[2];

			pfVertices = (float*)(&pbVertices[dwStride*pwPoly[dwIndex * 3 + 1]]);
			pvVertices[1].x = pfVertices[0];
			pvVertices[1].y = pfVertices[1];
			pvVertices[1].z = pfVertices[2];

			pfVertices = (float*)(&pbVertices[dwStride*pwPoly[dwIndex * 3 + 2]]);
			pvVertices[2].x = pfVertices[0];
			pvVertices[2].y = pfVertices[1];
			pvVertices[2].z = pfVertices[2];

			pMesh->UnlockIndexBuffer();
			VB->Unlock();
			VB->Release();
			return true;
		}

		pMesh->UnlockIndexBuffer();
		VB->Release();
		return false;
	}

	/**
	*	アニメーションモデルによるレイとポリゴンの判定
	*/
	bool Colision::ColisionCheckSegmentToAnimationMesh(LPD3DXFRAME pFrameBase,Segment* pSegment,G3DObj* pAnimationModel,BOOL* pboHit,
				float* pfDistance,LPD3DXMESHCONTAINER* ppContainer,D3DXVECTOR3* pvIntersect)
	{
		//G3DOBJFRAME* pFrame = (G3DOBJFRAME*)pFrameBase;
		//if(NULL !=pFrame->pMeshContainer)
		//{
		//	LPD3DXMESH pMesh=pFrame->pMeshContainer->MeshData.pMesh;
		//	if(NULL != pMesh)
		//	{
		//		D3DXVECTOR3 vStart,vEnd,vDirection;
		//		vStart = pSegment->Start;
		//		vDirection = pSegment->End - pSegment->Start;
		//		D3DXMATRIX InvWorld;

		//		BOOL HitFlg = FALSE;

		//		DWORD dwIndex = 0;
		//		float U = 0,V = 0.0f,distance = 0.0f;

		//		D3DXVECTOR3 vVertex[3];


		//
		//		//D3DXVec3Normalize(&vDirection,&vDirection);

		//		// 直線の座標をワールドからメッシュ座標系にに変換
		//		D3DXMatrixInverse(&InvWorld,NULL,pAnimationModel->GetLocalWorld());
		//		D3DXVec3TransformCoord(&vStart,&pSegment->Start,&InvWorld);
		//		D3DXVec3TransformCoord(&vEnd,&pSegment->End,&InvWorld);

		//		vDirection = vEnd - vStart;
		//		float start2end = D3DXVec3Length(&vDirection);
		//		D3DXVec3Normalize(&vDirection,&vDirection);

		//		D3DXIntersect(pMesh,&vStart,&vDirection,pboHit,&dwIndex,&U,&V, pfDistance,NULL,NULL);
		//		if(*pboHit)
		//		{
		//			*ppContainer=pFrame->pMeshContainer;
		//			FindVerticesOnPoly(pMesh,dwIndex,vVertex);
		//			//重心座標から交点を算出　ローカル交点pは　v0+U*(v1-v0)+V*(v2-v0)
		//			*pvIntersect=vVertex[0]+U*(vVertex[1]-vVertex[0])+V*(vVertex[2]-vVertex[0]);
		//			//ローカル交点をワールド系に換算 特にアニメーションの場合、これをしないと
		//			//モーションしているにも関らず（つまりワールド系で位置が変わっても）同じ座標になってしまう
		//			D3DXVec3TransformCoord(pvIntersect,pvIntersect,&pFrame->CombinedTransformationMatrix);

		//			return true;
		//		}
		//	}	
		//}
		//// 再帰判断部分　（階層の走査）
		//if(pFrame->pFrameSibling != NULL)
		//{
		//	if(ColisionCheckSegmentToAnimationMesh(pFrame->pFrameSibling,pSegment,pAnimationModel,pboHit,pfDistance,ppContainer,pvIntersect))
		//	{
		//		return true;
		//	}
		//}
		//if(pFrame->pFrameFirstChild != NULL)
		//{
		//	if(ColisionCheckSegmentToAnimationMesh(pFrame->pFrameFirstChild,pSegment,pAnimationModel,pboHit,pfDistance,ppContainer,pvIntersect))	
		//	{
		//		return true;
		//	}
		//}
		return false;
	}

	/**
	*	OBBによる当たり判定
	*/
	bool Colision::OBBCollisionDetection(OBBInfo* pThingA,OBBInfo* pThingB)
	{	
		//ボックスの距離ベクトル（中心と中心を結んだベクトル）
		D3DXVECTOR3 vDistance = pThingB->position - pThingA->position;
		//分離軸
		D3DXVECTOR3 vSeparate;
		//それぞれのローカル基底軸ベクトルに、それぞれの回転を反映させる
		// vAxisをとりあえず初期化
		pThingA->AxisX=D3DXVECTOR3(1,0,0);
		pThingA->AxisY=D3DXVECTOR3(0,1,0);
		pThingA->AxisZ=D3DXVECTOR3(0,0,1);

		pThingB->AxisX=D3DXVECTOR3(1,0,0);
		pThingB->AxisY=D3DXVECTOR3(0,1,0);
		pThingB->AxisZ=D3DXVECTOR3(0,0,1);

		// vAxisのそれぞれにrotation行列分の回転を反映させる
		D3DXVec3TransformCoord(&pThingA->AxisX,&pThingA->AxisX,&pThingA->rotation);
		D3DXVec3TransformCoord(&pThingA->AxisY,&pThingA->AxisY,&pThingA->rotation);
		D3DXVec3TransformCoord(&pThingA->AxisZ,&pThingA->AxisZ,&pThingA->rotation);

		D3DXVec3TransformCoord(&pThingB->AxisX,&pThingB->AxisX,&pThingB->rotation);
		D3DXVec3TransformCoord(&pThingB->AxisY,&pThingB->AxisY,&pThingB->rotation);
		D3DXVec3TransformCoord(&pThingB->AxisZ,&pThingB->AxisZ,&pThingB->rotation);

		//ボックスA
		//X軸を分離軸とした場合
		if(!CompareLength(pThingA,pThingB,&pThingA->AxisX,&vDistance)) return false;
		//Y軸を分離軸とした場合
		if(!CompareLength(pThingA,pThingB,&pThingA->AxisY,&vDistance)) return false;
		//Z軸を分離軸とした場合
		if(!CompareLength(pThingA,pThingB,&pThingA->AxisZ,&vDistance)) return false;
		//ボックスB
		//X軸を分離軸とした場合
		if(!CompareLength(pThingA,pThingB,&pThingB->AxisX,&vDistance)) return false;
		//Y軸を分離軸とした場合
		if(!CompareLength(pThingA,pThingB,&pThingB->AxisY,&vDistance)) return false;
		//Z軸を分離軸とした場合
		if(!CompareLength(pThingA,pThingB,&pThingB->AxisZ,&vDistance)) return false;

		//お互いの基底軸同士の外積ベクトルを基準にした、”影”の算出（9パターン）
		
		/**ボックスAのX軸*/

		//と　ボックスBのX軸との外積ベクトルを分離軸とした場合
		D3DXVec3Cross( &vSeparate, &pThingA->AxisX,&pThingB->AxisX);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		//と　ボックスBのY軸との外積ベクトルを分離軸とした場合
		D3DXVec3Cross( &vSeparate, &pThingA->AxisX,&pThingB->AxisY);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		//と　ボックスBのZ軸との外積ベクトルを分離軸とした場合
		D3DXVec3Cross( &vSeparate, &pThingA->AxisX,&pThingB->AxisZ);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		
		/**ボックスAのY軸*/
		
		//と　ボックスBのX軸との外積ベクトルを分離軸とした場合
		D3DXVec3Cross( &vSeparate, &pThingA->AxisY,&pThingB->AxisX);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		//と　ボックスBのY軸との外積ベクトルを分離軸とした場合
		D3DXVec3Cross( &vSeparate, &pThingA->AxisY,&pThingB->AxisY);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		//と　ボックスBのZ軸との外積ベクトルを分離軸とした場合
		D3DXVec3Cross( &vSeparate, &pThingA->AxisY,&pThingB->AxisZ);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		
		/**ボックスAのZ軸*/

		//と　ボックスBのX軸との外積ベクトルを分離軸とした場合
		D3DXVec3Cross( &vSeparate, &pThingA->AxisZ,&pThingB->AxisX);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		//と　ボックスBのY軸との外積ベクトルを分離軸とした場合
		D3DXVec3Cross( &vSeparate, &pThingA->AxisZ,&pThingB->AxisY);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		//と　ボックスBのZ軸との外積ベクトルを分離軸とした場合
		D3DXVec3Cross( &vSeparate, &pThingA->AxisZ,&pThingB->AxisZ);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		return true;
	}	
	
	/**
	*	影の算出
	*/
	bool Colision::CompareLength(OBBInfo* pboxA,OBBInfo* pboxB,D3DXVECTOR3* pvSeparate,D3DXVECTOR3* pvDistance)
	{
		//ボックスＡの中心からボックスＢの中心までの距離（分離軸上）
		float fDistance=fabsf( D3DXVec3Dot( pvDistance, pvSeparate) );
		//ボックスAの中心から最も遠いボックスAの頂点までの距離（分離軸上）
		float fShadowA=0;
		//ボックスBの中心から最も遠いボックスBの頂点までの距離（分離軸上）
		float fShadowB=0;		
		//それぞれのボックスの”影”を算出
		fShadowA = fabsf( D3DXVec3Dot( &pboxA->AxisX, pvSeparate ) * pboxA->LengthX)+
			fabsf( D3DXVec3Dot( &pboxA->AxisY, pvSeparate ) * pboxA->LengthY)+
			fabsf( D3DXVec3Dot( &pboxA->AxisZ, pvSeparate ) * pboxA->LengthZ);

		fShadowB = fabsf( D3DXVec3Dot( &pboxB->AxisX, pvSeparate ) * pboxB->LengthX)+
			fabsf( D3DXVec3Dot( &pboxB->AxisY, pvSeparate ) * pboxB->LengthY)+
			fabsf( D3DXVec3Dot( &pboxB->AxisZ, pvSeparate ) * pboxB->LengthZ);
		if(fDistance > fShadowA + fShadowB){
			return false;
		}
		return true;
	}

	/**
	*	AABBによる判定
	*/
	bool Colision::AABBColision2(OBBInfo* pThingA,OBBInfo* pThingB)
	{
		if( pThingA->position.x + pThingA->Max.x > pThingB->position.x + pThingB->Min.x &&
			pThingA->position.x + pThingA->Min.x < pThingB->position.x + pThingB->Max.x &&
			pThingA->position.y + pThingA->Max.y > pThingB->position.y + pThingB->Min.y &&
			pThingA->position.y + pThingA->Min.y < pThingB->position.y + pThingB->Max.y
			){
				return true;

		}

		return false;
	}


	/**
	*	無限遠平面と線分の交差を判定 （交差している場合はtrueを、していない場合はfalseを返す）
	*/
	bool Colision::PolyRay(Segment* pSegment,PolygonInfo* pPoly,D3DXVECTOR3* pInfo)
	{
		D3DXVECTOR3 vP[2];

		//線分（レイ）の端点を得る
		vP[0]=pSegment->Start;
		vP[1]=pSegment->End;

		//判定部分
		D3DXPLANE p;
		D3DXPlaneFromPoints(&p,&pPoly->vPoint[0],&pPoly->vPoint[1],&pPoly->vPoint[2]);
		//パラメトリック方程式の媒介変数” t "を解く。
		FLOAT t=- ( ( p.a * vP[1].x) + (p.b*vP[1].y)+(p.c*vP[1].z)+p.d ) / 
					( ( (p.a*vP[0].x) + (p.b*vP[0].y)+(p.c*vP[0].z) ) - ( (p.a*vP[1].x) + (p.b*vP[1].y)+(p.c*vP[1].z) ) ); 
		// t が０から１の間であるなら交差していることになる（この時点では、まだ無限遠平面との交差）
		if(t >= 0 && t <= 1.0)
		{
			//交点座標を得る　ｔが分かっていれば両端点から簡単に求まる
			D3DXVECTOR3 v;
			v.x = t*vP[0].x+(1-t)*vP[1].x;
			v.y = t*vP[0].y+(1-t)*vP[1].y;
			v.z = t*vP[0].z+(1-t)*vP[1].z;
			*pInfo = v;
			//交点が三角形の内か外かを判定　（ここで内部となれば、完全な交差となる）
			if(IsInside(&v,&pPoly->vPoint[0],&pPoly->vPoint[1],&pPoly->vPoint[2]))
			{
				return true;
			}
			return false;
		}
		return false;
	}

	/**
	*	交点が面の内にあるときはtrueを、外にあるときはfalseを返す
	*/
	bool Colision::IsInside(D3DXVECTOR3* pvI,D3DXVECTOR3* pvA,D3DXVECTOR3* pvB,D3DXVECTOR3* pvC)
	{
		//辺ベクトル
		D3DXVECTOR3 vAB,vBC,vCA;
		vAB=*pvB-*pvA;
		vBC=*pvC-*pvB;
		vCA=*pvA-*pvC;
		//辺ベクトルと「頂点から交点へ向かうベクトル」との、それぞれの外積用
		D3DXVECTOR3 vCrossAB,vCrossBC,vCrossCA;	
		//「外積結果のベクトル」と平面法線ベクトルとの、それぞれの内積用
		FLOAT fAB,fBC,fCA;
		// 法線用
		D3DXVECTOR3 vNormal;
		// まず、3頂点から平面方程式を求める。これは、同時に平面の法線を求めることでもある
		D3DXPLANE pln;
		D3DXPlaneFromPoints(&pln,pvA,pvB,pvC);
		vNormal.x=pln.a;//法線のx成分は平面方程式のx係数
		vNormal.y=pln.b;//法線のy成分は平面方程式のy係数
		vNormal.z=pln.c;//法線のz成分は平面方程式のz係数
		D3DXVec3Normalize(&vNormal,&vNormal);

		// 各頂点から交点Iに向かうベクトルをvVとする
		D3DXVECTOR3 vV;	
		// 辺ABベクトル（頂点Bベクトル-頂点Aベクトル)と、頂点Aから交点Iへ向かうベクトル、の外積を求める
		vV=*pvI-*pvA;
		D3DXVec3Cross(&vCrossAB,&vAB,&vV);
		// 辺BCベクトル（頂点Cベクトル-頂点Bベクトル)と、頂点Bから交点Iへ向かうベクトル、の外積を求める
		vV=*pvI-*pvB;
		D3DXVec3Cross(&vCrossBC,&vBC,&vV);
		// 辺CAベクトル（頂点Aベクトル-頂点Cベクトル)と、頂点Cから交点Iへ向かうベクトル、の外積を求める
		vV=*pvI-*pvC;
		D3DXVec3Cross(&vCrossCA,&vCA,&vV);
		// それぞれの、外積ベクトルとの内積を計算する
		fAB=D3DXVec3Dot(&vNormal,&vCrossAB);
		fBC=D3DXVec3Dot(&vNormal,&vCrossBC);
		fCA=D3DXVec3Dot(&vNormal,&vCrossCA);
	
		// ３つの内積結果のうち、一つでもマイナス符号のものがあれば、交点は外にある。
		if(fAB>=0 && fBC>=0 && fCA>=0)
		{
			// 交点は、面の内にある
			return true;
		}
		// 交点は面の外にある
		return false;
	}


}
