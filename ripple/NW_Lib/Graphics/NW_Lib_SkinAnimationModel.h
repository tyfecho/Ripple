/*-------------------------------------------------------------------------

	�N���X���@�F�@�X�L���A�j���[�V�����N���X
	�쐬�ҁ@�@�F�@�F���@����
	�ŏI�X�V���F�@2012 / 05 / 25

-------------------------------------------------------------------------*/
#pragma once

#include "NW_Lib_AnimController.h"
#include "../System/NW_Lib_Common.h"
#include <d3dx9.h>
#include <d3d9.h>

#include "../Utility/NW_Lib_utility.h"

#include "NW_Lib_IModel.h"

namespace NW_Lib
{

class Texture;


//�h���t���[���\���� (���ꂼ��̃��b�V���p�̍ŏI���[���h�s���ǉ�����j
struct MYFRAME: public D3DXFRAME
{
    D3DXMATRIXA16 CombinedTransformationMatrix;
};

//�h�����b�V���R���e�i�[�\����(
//�R���e�i�[���e�N�X�`���𕡐����Ă�悤�Ƀ|�C���^�[�̃|�C���^�[��ǉ�����j
struct MYMESHCONTAINER: public D3DXMESHCONTAINER
{
    LPDIRECT3DTEXTURE9*  ppTextures;
    DWORD dwWeight;//�d�݂̌��i�d�݂Ƃ͒��_�ւ̉e���B�j
	DWORD dwBoneNum;//�{�[���̐�
    LPD3DXBUFFER pBoneBuffer;//�{�[���E�e�[�u��
    D3DXMATRIX** ppBoneMatrix;//�S�Ẵ{�[���̃��[���h�s��̐擪�|�C���^�[
    D3DXMATRIX* pBoneOffsetMatrices;//�t���[���Ƃ��Ẵ{�[���̃��[���h�s��̃|�C���^�[
};

//X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����Ă����N���X��h��������B
//ID3DXAllocateHierarchy�͔h�����邱�Ƒz�肵�Đ݌v����Ă���B
class MY_ANIME_HIERARCHY: public ID3DXAllocateHierarchy
{
public:
	MY_ANIME_HIERARCHY(){}
    STDMETHOD(CreateFrame)(THIS_ LPCSTR , LPD3DXFRAME *);
    STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR ,CONST D3DXMESHDATA* ,CONST D3DXMATERIAL* , 
		CONST D3DXEFFECTINSTANCE* , DWORD , CONST DWORD *, LPD3DXSKININFO , LPD3DXMESHCONTAINER *);
    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME );
    STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER );	
};

typedef struct ANIMATIONMESHINFO
{	
	LPD3DXMESH pMesh;
	D3DMATERIAL9*	pMaterials;
	LPD3DXFRAME pFrameRoot;
    LPD3DXANIMATIONCONTROLLER pAnimController;
	AnimController* animController;
	Texture* pTexture;
	DWORD dwNumMaterial;
	LPD3DXANIMATIONSET m_pAnimSet[MOTION_MAX];

	~ANIMATIONMESHINFO()
	{
		SafeDelete(this->animController);
	}

	//D3DXMATRIX 

}ANIMATIONMESHINFO;

typedef ANIMATIONMESHINFO* LPANIMATIONMESHINFO ;

class SkinAnimationModel : public Common , public IModel
{
	private:
		LPANIMATIONMESHINFO		m_pInfo;			// ���b�V�����
		
		MY_ANIME_HIERARCHY		m_cHierarchy;		// �K�w���
		float					m_AnimeTime;		//�A�j���[�V�����^�C��
	
		//D3DXTRACK_DESC			m_TrackDesc;		//�g���b�N���
		int						m_truck;			//�g���b�N

		D3DXMATRIX				m_findBoneMatrix;	//�������{�[���̃}�g���N�X�ۑ��p
		//LPEffect				m_pEffect;			//�G�t�F�N�g
		float					m_alpha;			//�����x

	public:

		/*=======================================================
			�֐���	�F	SkinAnimationModel
			����	�F	�R���X�g���N�^
			����	�F	�t�@�C����
			�߂�l	�F�@�Ȃ�
		========================================================*/
		SkinAnimationModel( char* pFilename );

		/*=======================================================
			�֐���	�F	~SkinAnimationModel
			����	�F	�f�X�g���N�^
			����	�F	�Ȃ�
			�߂�l	�F�@�Ȃ�
		========================================================*/
		~SkinAnimationModel();

		/*=======================================================
			�֐���	�F	ORG_LoadMesh
			����	�F	���b�V���̃��[�h
			����	�F	�t�@�C���̖��O
			�߂�l	�F�@����
		========================================================*/
		HRESULT ORG_LoadMesh(char *pFilename);


		/*=======================================================
			�֐���	�F	ORG_DrawMesh
			����	�F	���b�V���̕`��
			����	�F	���b�V���C���t�H�̃|�C���^
						�A�j���[�V�����R���g���[��
						�A���t�@
			�߂�l	�F�@����
		========================================================*/
		HRESULT ORG_DrawMesh( D3DXMATRIX world , AnimController* pAnimController, float alpha );
		
		/*=======================================================
			�֐���	�F	ORG_ReleaseMesh
			����	�F	���b�V���̉��
			����	�F	���b�V���C���t�H�̃|�C���^
			�߂�l	�F�@����
		========================================================*/
		HRESULT ORG_ReleaseMesh();

		/*=======================================================
			�֐���	�F	Update
			����	�F	�X�V
			����	�F	�A�j���[�V�����R���g���[��
						���[���h�s��
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void Update(AnimController* pAnimController , LPD3DXMATRIX pWorld);

		/*=======================================================
			�֐���	�F	ReleaseAnimationMesh
			����	�F	�A�j���[�V�������b�V���̉��
			����	�F	�A�j���[�V�������b�V���C���t�H�̃|�C���^
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void ReleaseAnimationMesh(LPD3DXFRAME pFrame);

		/*=======================================================
			�֐���	�F	Draw
			����	�F	���b�V���̕`��
			����	�F	���[���h�}�g���b�N�X
						�A�j���[�V�����R���g���[��
						�A���t�@
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void Draw( D3DXMATRIX world , AnimController* pAnimController , float alpha = 1.0f) { ORG_DrawMesh( world , pAnimController , alpha); }

		/*=======================================================
			�֐���	�F	SetTruck
			����	�F	�g���b�N�̐ݒ�
			����	�F	�g���b�N�ԍ�
						(���[�v���)
						(�ڍs����)
						(�P�񃋁[�v����)
						(���[�v��)
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void SetTruck( int num , AnimController::LOOP_TYPE loopType = AnimController::NORMAL , float shiftTime = 0.5f , float loopTime = 1.0f , unsigned int loopNum = 0 ) 
		{ 
			m_pInfo->animController->ChangeAnimation(num ,loopType, loopNum);

			m_truck = num;
		}

		/*=======================================================
			�֐���	�F	SetTruck
			����	�F	�g���b�N�̐ݒ�
			����	�F	�A�j���[�V�������
						�g���b�N�ԍ�
						(���[�v���)
						(�ڍs����)
						(�P�񃋁[�v����)
						(���[�v��)
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void SetTruck(AnimController* pAnimController , int num , AnimController::LOOP_TYPE loopType = AnimController::NORMAL , float shiftTime = 0.5f , float loopTime = 1.0f , unsigned int loopNum = 0 ) 
		{ 
			pAnimController->SetLoopTime(num , loopTime);
			pAnimController->SetShiftTime(num , shiftTime);
			pAnimController->ChangeAnimation(num , loopType , loopNum);

			m_truck = num;
		}

		/*=======================================================
			�֐���	�F	GetTruck
			����	�F	�g���b�N�̎擾
			����	�F	�Ȃ�
			�߂�l	�F�@�g���b�N�ԍ�
		========================================================*/
		int GetTruck() { return m_truck; }

		/*=======================================================
			�֐���	�F	GetAnimationController
			����	�F	�A�j���[�V�������̎擾
			����	�F	�Ȃ�
			�߂�l	�F�@�A�j���[�V�������
		========================================================*/
		LPD3DXANIMATIONCONTROLLER GetAnimationController() { return m_pInfo->pAnimController; }

		/*=======================================================
			�֐���	�F	UpdateFrameMatrices
			����	�F	�t���[�����̃��b�V�����Ƀ��[���h�ϊ��s����X�V
			����	�F	LPD3DXFRAME		pFrameBase
						LPD3DXMATRIX	pParentMatrix
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);

		/*=======================================================
			�֐���	�F	DrawFrame
			����	�F	�����̃��b�V�����琬��t���[���������_�����O����B
			����	�F	LPD3DXFRAME pFrame
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void DrawFrame( LPD3DXFRAME pFrameBase);

		/*=======================================================
			�֐���	�F	RenderMeshContainer
			����	�F	�t���[�����̂��ꂼ��̃��b�V���������_�����O����
			����	�F	LPD3DXMESHCONTAINER pMeshContainerBase
						LPD3DXFRAME pFrameBase
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void RenderMeshContainer(MYMESHCONTAINER* pMeshContainer, MYFRAME* pFrame);

		/*=======================================================
			�֐���	�F	AllocateBoneMatrix
			����	�F	�{�[���̃}�g���N�X�����蓖�Ă�
			����	�F	SkinAnimationModel	pMeshContainerBase
					LPD3DXMESHCONTAINER pMeshContainerBase
			�߂�l	�F�@����
		========================================================*/
		HRESULT AllocateBoneMatrix( LPANIMATIONMESHINFO	*pInfo, LPD3DXMESHCONTAINER pMeshContainerBase );

		/*=======================================================
			�֐���	�F	AllocateAllBoneMatrices
			����	�F	�{�[���̃}�g���N�X�����蓖�Ă�
			����	�F	SkinAnimationModel	pMeshContainerBase
					 LPD3DXFRAME		pFrameBase
			�߂�l	�F�@����
		========================================================*/
		HRESULT AllocateAllBoneMatrices( LPANIMATIONMESHINFO *pInfo,  LPD3DXFRAME pFrameBase );

		/*=======================================================
			�֐���	�F	CloneAnimationController
			����	�F	�A�j���[�V�����R���g���[���𕡐�
			����	�F	�A�j���[�V�����R���g���[��
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void CloneAnimationController( LPD3DXANIMATIONCONTROLLER* pAnimController );

		/*=======================================================
			�֐���	�F	SetAnimationSet
			����	�F	�A�j���[�V�����Z�b�g��ݒ�
			����	�F	�A�j���[�V�����Z�b�g
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void SetAnimationSet( LPD3DXANIMATIONSET* pAnimSet );

		/*=======================================================
			�֐���	�F	GetMesh
			����	�F	���b�V�����̎擾
			����	�F	�Ȃ�
			�߂�l	�F�@���b�V�����
		========================================================*/
		LPD3DXMESH GetMesh() { return m_pInfo->pMesh; }

		/*=======================================================
			�֐���	�F	FindFrameMatrices
			����	�F	�t���[�����̃��b�V�����̃��[���h�ϊ��s���������
			����	�F	const char* boneName
					LPD3DXFRAME		pFrameBase
					LPD3DXMATRIX	pParentMatrix
			�߂�l	�F�@�Ȃ�
		========================================================*/
		void FindFrameMatrices( const char* boneName , LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);

		/*=======================================================
			�֐���	�F	FindMatrices
			����	�F	�t���[�����̃��b�V�����̃��[���h�ϊ��s���������
			����	�F	const char* boneName
					LPD3DXMATRIX worldMatrix
			�߂�l	�F�@�}�g���N�X
		========================================================*/
		LPD3DXMATRIX FindMatrices(const char* boneName , LPD3DXMATRIX world);

};


}