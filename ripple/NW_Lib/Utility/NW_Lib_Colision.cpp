/**
*	@file	Colision.cpp
*
*	@brief	�����蔻��p�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#include "NW_Lib_Colision.h"


namespace NW_Lib
{

	/**
	*	���Ƌ��̓����蔻��
	*/
	bool Colision::ColisionCheckSphere2(Sphere* pBS1,Sphere* pBS2,D3DXVECTOR3* inter,float* distance)
	{
		D3DXVECTOR3 temp = pBS1->center - pBS2->center;

		float d = D3DXVec3Length(&temp);

		if(distance != NULL){
			// ���܂��Ă��鋗�����v�Z����
			*distance = d - (pBS1->Radius + pBS2->Radius);
		}

		if(d > pBS1->Radius + pBS2->Radius){
			// �������ĂȂ�
			return false;
		}
		// �������Ă���
		if(inter != NULL){
			// ��̂̌�_���v�Z
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
	*	�����ɂ�郁�b�V���Ƃ̌�������
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

		// �����̍��W�����[���h���烁�b�V�����W�n�ɂɕϊ�
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
					// �d�S���W����x�_���Z�o
					// ��_��v0+u * ()v1-v0) * uc
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
	*	���b�V�����̃|���S���ԍ����璸�_���W������o��
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
	*	�A�j���[�V�������f���ɂ�郌�C�ƃ|���S���̔���
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

		//		// �����̍��W�����[���h���烁�b�V�����W�n�ɂɕϊ�
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
		//			//�d�S���W�����_���Z�o�@���[�J����_p�́@v0+U*(v1-v0)+V*(v2-v0)
		//			*pvIntersect=vVertex[0]+U*(vVertex[1]-vVertex[0])+V*(vVertex[2]-vVertex[0]);
		//			//���[�J����_�����[���h�n�Ɋ��Z ���ɃA�j���[�V�����̏ꍇ�A��������Ȃ���
		//			//���[�V�������Ă���ɂ��ւ炸�i�܂胏�[���h�n�ňʒu���ς���Ă��j�������W�ɂȂ��Ă��܂�
		//			D3DXVec3TransformCoord(pvIntersect,pvIntersect,&pFrame->CombinedTransformationMatrix);

		//			return true;
		//		}
		//	}	
		//}
		//// �ċA���f�����@�i�K�w�̑����j
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
	*	OBB�ɂ�铖���蔻��
	*/
	bool Colision::OBBCollisionDetection(OBBInfo* pThingA,OBBInfo* pThingB)
	{	
		//�{�b�N�X�̋����x�N�g���i���S�ƒ��S�����񂾃x�N�g���j
		D3DXVECTOR3 vDistance = pThingB->position - pThingA->position;
		//������
		D3DXVECTOR3 vSeparate;
		//���ꂼ��̃��[�J����ꎲ�x�N�g���ɁA���ꂼ��̉�]�𔽉f������
		// vAxis���Ƃ肠����������
		pThingA->AxisX=D3DXVECTOR3(1,0,0);
		pThingA->AxisY=D3DXVECTOR3(0,1,0);
		pThingA->AxisZ=D3DXVECTOR3(0,0,1);

		pThingB->AxisX=D3DXVECTOR3(1,0,0);
		pThingB->AxisY=D3DXVECTOR3(0,1,0);
		pThingB->AxisZ=D3DXVECTOR3(0,0,1);

		// vAxis�̂��ꂼ���rotation�s�񕪂̉�]�𔽉f������
		D3DXVec3TransformCoord(&pThingA->AxisX,&pThingA->AxisX,&pThingA->rotation);
		D3DXVec3TransformCoord(&pThingA->AxisY,&pThingA->AxisY,&pThingA->rotation);
		D3DXVec3TransformCoord(&pThingA->AxisZ,&pThingA->AxisZ,&pThingA->rotation);

		D3DXVec3TransformCoord(&pThingB->AxisX,&pThingB->AxisX,&pThingB->rotation);
		D3DXVec3TransformCoord(&pThingB->AxisY,&pThingB->AxisY,&pThingB->rotation);
		D3DXVec3TransformCoord(&pThingB->AxisZ,&pThingB->AxisZ,&pThingB->rotation);

		//�{�b�N�XA
		//X���𕪗����Ƃ����ꍇ
		if(!CompareLength(pThingA,pThingB,&pThingA->AxisX,&vDistance)) return false;
		//Y���𕪗����Ƃ����ꍇ
		if(!CompareLength(pThingA,pThingB,&pThingA->AxisY,&vDistance)) return false;
		//Z���𕪗����Ƃ����ꍇ
		if(!CompareLength(pThingA,pThingB,&pThingA->AxisZ,&vDistance)) return false;
		//�{�b�N�XB
		//X���𕪗����Ƃ����ꍇ
		if(!CompareLength(pThingA,pThingB,&pThingB->AxisX,&vDistance)) return false;
		//Y���𕪗����Ƃ����ꍇ
		if(!CompareLength(pThingA,pThingB,&pThingB->AxisY,&vDistance)) return false;
		//Z���𕪗����Ƃ����ꍇ
		if(!CompareLength(pThingA,pThingB,&pThingB->AxisZ,&vDistance)) return false;

		//���݂��̊�ꎲ���m�̊O�σx�N�g������ɂ����A�h�e�h�̎Z�o�i9�p�^�[���j
		
		/**�{�b�N�XA��X��*/

		//�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
		D3DXVec3Cross( &vSeparate, &pThingA->AxisX,&pThingB->AxisX);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		//�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
		D3DXVec3Cross( &vSeparate, &pThingA->AxisX,&pThingB->AxisY);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		//�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
		D3DXVec3Cross( &vSeparate, &pThingA->AxisX,&pThingB->AxisZ);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		
		/**�{�b�N�XA��Y��*/
		
		//�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
		D3DXVec3Cross( &vSeparate, &pThingA->AxisY,&pThingB->AxisX);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		//�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
		D3DXVec3Cross( &vSeparate, &pThingA->AxisY,&pThingB->AxisY);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		//�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
		D3DXVec3Cross( &vSeparate, &pThingA->AxisY,&pThingB->AxisZ);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		
		/**�{�b�N�XA��Z��*/

		//�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
		D3DXVec3Cross( &vSeparate, &pThingA->AxisZ,&pThingB->AxisX);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		//�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
		D3DXVec3Cross( &vSeparate, &pThingA->AxisZ,&pThingB->AxisY);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		//�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
		D3DXVec3Cross( &vSeparate, &pThingA->AxisZ,&pThingB->AxisZ);
		if(!CompareLength(pThingA,pThingB,&vSeparate,&vDistance)) return false;
		return true;
	}	
	
	/**
	*	�e�̎Z�o
	*/
	bool Colision::CompareLength(OBBInfo* pboxA,OBBInfo* pboxB,D3DXVECTOR3* pvSeparate,D3DXVECTOR3* pvDistance)
	{
		//�{�b�N�X�`�̒��S����{�b�N�X�a�̒��S�܂ł̋����i��������j
		float fDistance=fabsf( D3DXVec3Dot( pvDistance, pvSeparate) );
		//�{�b�N�XA�̒��S����ł������{�b�N�XA�̒��_�܂ł̋����i��������j
		float fShadowA=0;
		//�{�b�N�XB�̒��S����ł������{�b�N�XB�̒��_�܂ł̋����i��������j
		float fShadowB=0;		
		//���ꂼ��̃{�b�N�X�́h�e�h���Z�o
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
	*	AABB�ɂ�锻��
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
	*	���������ʂƐ����̌����𔻒� �i�������Ă���ꍇ��true���A���Ă��Ȃ��ꍇ��false��Ԃ��j
	*/
	bool Colision::PolyRay(Segment* pSegment,PolygonInfo* pPoly,D3DXVECTOR3* pInfo)
	{
		D3DXVECTOR3 vP[2];

		//�����i���C�j�̒[�_�𓾂�
		vP[0]=pSegment->Start;
		vP[1]=pSegment->End;

		//���蕔��
		D3DXPLANE p;
		D3DXPlaneFromPoints(&p,&pPoly->vPoint[0],&pPoly->vPoint[1],&pPoly->vPoint[2]);
		//�p�����g���b�N�������̔}��ϐ��h t "�������B
		FLOAT t=- ( ( p.a * vP[1].x) + (p.b*vP[1].y)+(p.c*vP[1].z)+p.d ) / 
					( ( (p.a*vP[0].x) + (p.b*vP[0].y)+(p.c*vP[0].z) ) - ( (p.a*vP[1].x) + (p.b*vP[1].y)+(p.c*vP[1].z) ) ); 
		// t ���O����P�̊Ԃł���Ȃ�������Ă��邱�ƂɂȂ�i���̎��_�ł́A�܂����������ʂƂ̌����j
		if(t >= 0 && t <= 1.0)
		{
			//��_���W�𓾂�@�����������Ă���Η��[�_����ȒP�ɋ��܂�
			D3DXVECTOR3 v;
			v.x = t*vP[0].x+(1-t)*vP[1].x;
			v.y = t*vP[0].y+(1-t)*vP[1].y;
			v.z = t*vP[0].z+(1-t)*vP[1].z;
			*pInfo = v;
			//��_���O�p�`�̓����O���𔻒�@�i�����œ����ƂȂ�΁A���S�Ȍ����ƂȂ�j
			if(IsInside(&v,&pPoly->vPoint[0],&pPoly->vPoint[1],&pPoly->vPoint[2]))
			{
				return true;
			}
			return false;
		}
		return false;
	}

	/**
	*	��_���ʂ̓��ɂ���Ƃ���true���A�O�ɂ���Ƃ���false��Ԃ�
	*/
	bool Colision::IsInside(D3DXVECTOR3* pvI,D3DXVECTOR3* pvA,D3DXVECTOR3* pvB,D3DXVECTOR3* pvC)
	{
		//�Ӄx�N�g��
		D3DXVECTOR3 vAB,vBC,vCA;
		vAB=*pvB-*pvA;
		vBC=*pvC-*pvB;
		vCA=*pvA-*pvC;
		//�Ӄx�N�g���Ɓu���_�����_�֌������x�N�g���v�Ƃ́A���ꂼ��̊O�ϗp
		D3DXVECTOR3 vCrossAB,vCrossBC,vCrossCA;	
		//�u�O�ό��ʂ̃x�N�g���v�ƕ��ʖ@���x�N�g���Ƃ́A���ꂼ��̓��ϗp
		FLOAT fAB,fBC,fCA;
		// �@���p
		D3DXVECTOR3 vNormal;
		// �܂��A3���_���畽�ʕ����������߂�B����́A�����ɕ��ʂ̖@�������߂邱�Ƃł�����
		D3DXPLANE pln;
		D3DXPlaneFromPoints(&pln,pvA,pvB,pvC);
		vNormal.x=pln.a;//�@����x�����͕��ʕ�������x�W��
		vNormal.y=pln.b;//�@����y�����͕��ʕ�������y�W��
		vNormal.z=pln.c;//�@����z�����͕��ʕ�������z�W��
		D3DXVec3Normalize(&vNormal,&vNormal);

		// �e���_�����_I�Ɍ������x�N�g����vV�Ƃ���
		D3DXVECTOR3 vV;	
		// ��AB�x�N�g���i���_B�x�N�g��-���_A�x�N�g��)�ƁA���_A�����_I�֌������x�N�g���A�̊O�ς����߂�
		vV=*pvI-*pvA;
		D3DXVec3Cross(&vCrossAB,&vAB,&vV);
		// ��BC�x�N�g���i���_C�x�N�g��-���_B�x�N�g��)�ƁA���_B�����_I�֌������x�N�g���A�̊O�ς����߂�
		vV=*pvI-*pvB;
		D3DXVec3Cross(&vCrossBC,&vBC,&vV);
		// ��CA�x�N�g���i���_A�x�N�g��-���_C�x�N�g��)�ƁA���_C�����_I�֌������x�N�g���A�̊O�ς����߂�
		vV=*pvI-*pvC;
		D3DXVec3Cross(&vCrossCA,&vCA,&vV);
		// ���ꂼ��́A�O�σx�N�g���Ƃ̓��ς��v�Z����
		fAB=D3DXVec3Dot(&vNormal,&vCrossAB);
		fBC=D3DXVec3Dot(&vNormal,&vCrossBC);
		fCA=D3DXVec3Dot(&vNormal,&vCrossCA);
	
		// �R�̓��ό��ʂ̂����A��ł��}�C�i�X�����̂��̂�����΁A��_�͊O�ɂ���B
		if(fAB>=0 && fBC>=0 && fCA>=0)
		{
			// ��_�́A�ʂ̓��ɂ���
			return true;
		}
		// ��_�͖ʂ̊O�ɂ���
		return false;
	}


}
