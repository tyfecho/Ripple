/*-------------------------------------------------------------------------

	クラス名　：　スキンアニメーションクラス
	作成者　　：　皆光　健太
	最終更新日：　2012 / 05 / 25

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


//派生フレーム構造体 (それぞれのメッシュ用の最終ワールド行列を追加する）
struct MYFRAME: public D3DXFRAME
{
    D3DXMATRIXA16 CombinedTransformationMatrix;
};

//派生メッシュコンテナー構造体(
//コンテナーがテクスチャを複数持てるようにポインターのポインターを追加する）
struct MYMESHCONTAINER: public D3DXMESHCONTAINER
{
    LPDIRECT3DTEXTURE9*  ppTextures;
    DWORD dwWeight;//重みの個数（重みとは頂点への影響。）
	DWORD dwBoneNum;//ボーンの数
    LPD3DXBUFFER pBoneBuffer;//ボーン・テーブル
    D3DXMATRIX** ppBoneMatrix;//全てのボーンのワールド行列の先頭ポインター
    D3DXMATRIX* pBoneOffsetMatrices;//フレームとしてのボーンのワールド行列のポインター
};

//Xファイル内のアニメーション階層を読み下してくれるクラスを派生させる。
//ID3DXAllocateHierarchyは派生すること想定して設計されている。
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
		LPANIMATIONMESHINFO		m_pInfo;			// メッシュ情報
		
		MY_ANIME_HIERARCHY		m_cHierarchy;		// 階層情報
		float					m_AnimeTime;		//アニメーションタイム
	
		//D3DXTRACK_DESC			m_TrackDesc;		//トラック情報
		int						m_truck;			//トラック

		D3DXMATRIX				m_findBoneMatrix;	//見つけたボーンのマトリクス保存用
		//LPEffect				m_pEffect;			//エフェクト
		float					m_alpha;			//透明度

	public:

		/*=======================================================
			関数名	：	SkinAnimationModel
			説明	：	コンストラクタ
			引数	：	ファイル名
			戻り値	：　なし
		========================================================*/
		SkinAnimationModel( char* pFilename );

		/*=======================================================
			関数名	：	~SkinAnimationModel
			説明	：	デストラクタ
			引数	：	なし
			戻り値	：　なし
		========================================================*/
		~SkinAnimationModel();

		/*=======================================================
			関数名	：	ORG_LoadMesh
			説明	：	メッシュのロード
			引数	：	ファイルの名前
			戻り値	：　結果
		========================================================*/
		HRESULT ORG_LoadMesh(char *pFilename);


		/*=======================================================
			関数名	：	ORG_DrawMesh
			説明	：	メッシュの描画
			引数	：	メッシュインフォのポインタ
						アニメーションコントローラ
						アルファ
			戻り値	：　結果
		========================================================*/
		HRESULT ORG_DrawMesh( D3DXMATRIX world , AnimController* pAnimController, float alpha );
		
		/*=======================================================
			関数名	：	ORG_ReleaseMesh
			説明	：	メッシュの解放
			引数	：	メッシュインフォのポインタ
			戻り値	：　結果
		========================================================*/
		HRESULT ORG_ReleaseMesh();

		/*=======================================================
			関数名	：	Update
			説明	：	更新
			引数	：	アニメーションコントローラ
						ワールド行列
			戻り値	：　なし
		========================================================*/
		void Update(AnimController* pAnimController , LPD3DXMATRIX pWorld);

		/*=======================================================
			関数名	：	ReleaseAnimationMesh
			説明	：	アニメーションメッシュの解放
			引数	：	アニメーションメッシュインフォのポインタ
			戻り値	：　なし
		========================================================*/
		void ReleaseAnimationMesh(LPD3DXFRAME pFrame);

		/*=======================================================
			関数名	：	Draw
			説明	：	メッシュの描画
			引数	：	ワールドマトリックス
						アニメーションコントローラ
						アルファ
			戻り値	：　なし
		========================================================*/
		void Draw( D3DXMATRIX world , AnimController* pAnimController , float alpha = 1.0f) { ORG_DrawMesh( world , pAnimController , alpha); }

		/*=======================================================
			関数名	：	SetTruck
			説明	：	トラックの設定
			引数	：	トラック番号
						(ループ情報)
						(移行時間)
						(１回ループ時間)
						(ループ回数)
			戻り値	：　なし
		========================================================*/
		void SetTruck( int num , AnimController::LOOP_TYPE loopType = AnimController::NORMAL , float shiftTime = 0.5f , float loopTime = 1.0f , unsigned int loopNum = 0 ) 
		{ 
			m_pInfo->animController->ChangeAnimation(num ,loopType, loopNum);

			m_truck = num;
		}

		/*=======================================================
			関数名	：	SetTruck
			説明	：	トラックの設定
			引数	：	アニメーション情報
						トラック番号
						(ループ情報)
						(移行時間)
						(１回ループ時間)
						(ループ回数)
			戻り値	：　なし
		========================================================*/
		void SetTruck(AnimController* pAnimController , int num , AnimController::LOOP_TYPE loopType = AnimController::NORMAL , float shiftTime = 0.5f , float loopTime = 1.0f , unsigned int loopNum = 0 ) 
		{ 
			pAnimController->SetLoopTime(num , loopTime);
			pAnimController->SetShiftTime(num , shiftTime);
			pAnimController->ChangeAnimation(num , loopType , loopNum);

			m_truck = num;
		}

		/*=======================================================
			関数名	：	GetTruck
			説明	：	トラックの取得
			引数	：	なし
			戻り値	：　トラック番号
		========================================================*/
		int GetTruck() { return m_truck; }

		/*=======================================================
			関数名	：	GetAnimationController
			説明	：	アニメーション情報の取得
			引数	：	なし
			戻り値	：　アニメーション情報
		========================================================*/
		LPD3DXANIMATIONCONTROLLER GetAnimationController() { return m_pInfo->pAnimController; }

		/*=======================================================
			関数名	：	UpdateFrameMatrices
			説明	：	フレーム内のメッシュ毎にワールド変換行列を更新
			引数	：	LPD3DXFRAME		pFrameBase
						LPD3DXMATRIX	pParentMatrix
			戻り値	：　なし
		========================================================*/
		void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);

		/*=======================================================
			関数名	：	DrawFrame
			説明	：	複数のメッシュから成るフレームをレンダリングする。
			引数	：	LPD3DXFRAME pFrame
			戻り値	：　なし
		========================================================*/
		void DrawFrame( LPD3DXFRAME pFrameBase);

		/*=======================================================
			関数名	：	RenderMeshContainer
			説明	：	フレーム内のそれぞれのメッシュをレンダリングする
			引数	：	LPD3DXMESHCONTAINER pMeshContainerBase
						LPD3DXFRAME pFrameBase
			戻り値	：　なし
		========================================================*/
		void RenderMeshContainer(MYMESHCONTAINER* pMeshContainer, MYFRAME* pFrame);

		/*=======================================================
			関数名	：	AllocateBoneMatrix
			説明	：	ボーンのマトリクスを割り当てる
			引数	：	SkinAnimationModel	pMeshContainerBase
					LPD3DXMESHCONTAINER pMeshContainerBase
			戻り値	：　結果
		========================================================*/
		HRESULT AllocateBoneMatrix( LPANIMATIONMESHINFO	*pInfo, LPD3DXMESHCONTAINER pMeshContainerBase );

		/*=======================================================
			関数名	：	AllocateAllBoneMatrices
			説明	：	ボーンのマトリクスを割り当てる
			引数	：	SkinAnimationModel	pMeshContainerBase
					 LPD3DXFRAME		pFrameBase
			戻り値	：　結果
		========================================================*/
		HRESULT AllocateAllBoneMatrices( LPANIMATIONMESHINFO *pInfo,  LPD3DXFRAME pFrameBase );

		/*=======================================================
			関数名	：	CloneAnimationController
			説明	：	アニメーションコントローラを複製
			引数	：	アニメーションコントローラ
			戻り値	：　なし
		========================================================*/
		void CloneAnimationController( LPD3DXANIMATIONCONTROLLER* pAnimController );

		/*=======================================================
			関数名	：	SetAnimationSet
			説明	：	アニメーションセットを設定
			引数	：	アニメーションセット
			戻り値	：　なし
		========================================================*/
		void SetAnimationSet( LPD3DXANIMATIONSET* pAnimSet );

		/*=======================================================
			関数名	：	GetMesh
			説明	：	メッシュ情報の取得
			引数	：	なし
			戻り値	：　メッシュ情報
		========================================================*/
		LPD3DXMESH GetMesh() { return m_pInfo->pMesh; }

		/*=======================================================
			関数名	：	FindFrameMatrices
			説明	：	フレーム内のメッシュ毎のワールド変換行列を見つける
			引数	：	const char* boneName
					LPD3DXFRAME		pFrameBase
					LPD3DXMATRIX	pParentMatrix
			戻り値	：　なし
		========================================================*/
		void FindFrameMatrices( const char* boneName , LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);

		/*=======================================================
			関数名	：	FindMatrices
			説明	：	フレーム内のメッシュ毎のワールド変換行列を見つける
			引数	：	const char* boneName
					LPD3DXMATRIX worldMatrix
			戻り値	：　マトリクス
		========================================================*/
		LPD3DXMATRIX FindMatrices(const char* boneName , LPD3DXMATRIX world);

};


}