/**
*	@file	Colision.h
*
*	@brief	�����蔻��p�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#pragma once

#include <d3dx9.h>


// �Փ˔��苅
struct Sphere
{
	D3DXVECTOR3 center; // ���̒��S
	float Radius;       // ���a
};

// �Փ˔���p����
struct Segment
{
	D3DXVECTOR3 Start;
	D3DXVECTOR3 End;
};

// �Փ˔���p���b�V�����
struct MeshInfo
{
	// ���b�V��
	LPD3DXMESH pMesh;
	// ���b�V���̃��[���h�s��
	D3DXMATRIX* pWorldMatrix;
};

// �Փ˔���p�|���S��
struct PolygonInfo
{
	D3DXVECTOR3 vPoint[3];
};

// OBB����p���
struct OBBInfo
{
	// �����̂��̂̉�]�s��
	D3DXMATRIX rotation;

	// ��{�ʒu
	D3DXVECTOR3 position;

	// X,Y,Z���ꂼ��̂̎����
	D3DXVECTOR3 AxisX;
	D3DXVECTOR3 AxisY;
	D3DXVECTOR3 AxisZ;

	float LengthX;
	float LengthY;
	float LengthZ;

	// �ő�̈ʒu
	D3DXVECTOR3 Max;
	// �ŏ��̈ʒu
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
			// �X�t�B�A
			bool ColisionCheckSphere2(Sphere* pBS1,Sphere* pBS2,D3DXVECTOR3* inter = NULL,float* distance = NULL);
			// ���ʂ̃|���S��
			bool ColisionCheckSegmentToMesh(Segment* pSegment,MeshInfo* pMeshInfo,float* pDistance,D3DXVECTOR3* pInter);
			bool FindVerticesOnPoly(LPD3DXMESH pMesh,DWORD dwIndex,D3DXVECTOR3* pvVertices);
			// �A�j���[�V�������b�V���Ƃ̃��C����
			bool ColisionCheckSegmentToAnimationMesh(LPD3DXFRAME pFrameBase,Segment* pSegment,G3DObj* pAnimationModel,BOOL* pboHit,
						float* pfDistance,LPD3DXMESHCONTAINER* ppContainer,D3DXVECTOR3* pvIntersect);
			// OBB
			bool OBBCollisionDetection(OBBInfo* pThingA,OBBInfo* pThingB);
			bool CompareLength(OBBInfo* pboxA,OBBInfo* pboxB,D3DXVECTOR3* pvSeparate,D3DXVECTOR3* pvDistance);
			// AABB
			bool AABBColision2(OBBInfo* pThingA,OBBInfo* pThingB);

			// ���ʂƃ��C
			bool PolyRay(Segment* pSegment,PolygonInfo* pPoly,D3DXVECTOR3* pInfo = NULL);
			bool IsInside(D3DXVECTOR3* pvI,D3DXVECTOR3* pvA,D3DXVECTOR3* pvB,D3DXVECTOR3* pvC);
	};
}