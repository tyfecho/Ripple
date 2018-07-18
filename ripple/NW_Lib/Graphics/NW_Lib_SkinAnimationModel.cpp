/*-------------------------------------------------------------------------

	クラス名　：　スキンアニメーションクラス
	作成者　　：　皆光　健太
	最終更新日：　2012 / 05 / 25

-------------------------------------------------------------------------*/
#include "NW_Lib_SkinAnimationModel.h"
#include "NW_Lib_Texture.h"

#include "../Utility/NW_Lib_utility.h"

#include <windows.h>
#include <sstream>

//#define _DEBUG
//#ifdef _DEBUG
//#define   new                   new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define   malloc(s)             _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
//#define   calloc(c, s)          _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
//#define   realloc(p, s)         _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
//#define   _recalloc(p, c, s)    _recalloc_dbg(p, c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
//#define   _expand(p, s)         _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
//#endif

namespace NW_Lib
{

/*=======================================================
	関数名	：	SkinAnimationModel
	説明	：	コンストラクタ
	引数	：	ファイル名
	戻り値	：　なし
========================================================*/
SkinAnimationModel::SkinAnimationModel( char* pFilename )
: m_AnimeTime(0.0f)
{
	D3DXMatrixIdentity( &m_findBoneMatrix );

	ORG_LoadMesh( pFilename );
}

/*=======================================================
	関数名	：	~SkinAnimationModel
	説明	：	デストラクタ
	引数	：	なし
	戻り値	：　なし
========================================================*/
SkinAnimationModel::~SkinAnimationModel()
{
	ORG_ReleaseMesh();
}

/*=======================================================
	関数名	：	ORG_LoadMesh
	説明	：	メッシュのロード
	引数	：	ファイルの名前
	戻り値	：　結果
========================================================*/
HRESULT SkinAnimationModel::ORG_LoadMesh(char *pFilename)
{

	//メッシュの情報構造体の確保
	m_pInfo = new ANIMATIONMESHINFO;
	if( m_pInfo == NULL )
	{
		MessageBox(NULL , "エラー１", "class SkinAnimationModel",MB_OK);
		return E_FAIL;
	}

	//メッシュの初期化
	ZeroMemory( m_pInfo,sizeof(ANIMATIONMESHINFO) );
	
	//アニメーショントラックの初期化
	for( int i = 0 ; i < 10 ; i++ )
	{
		m_pInfo->m_pAnimSet[i] = 0;
	}
	m_truck = 0;

	m_pInfo->animController = new AnimController();
	m_pInfo->pAnimController = m_pInfo->animController->GetAnimationController();

	//メッシュのロード
	if( FAILED( D3DXLoadMeshHierarchyFromX( pFilename, D3DXMESH_MANAGED
								   , m_pD3DDevice , &m_cHierarchy 
								   , NULL
								   , &m_pInfo->pFrameRoot
								   , &m_pInfo->pAnimController ) ) )
	{
		ORG_ReleaseMesh();

		MessageBox(NULL , "エラー２", "class SkinAnimationModel",MB_OK);
		return E_FAIL;
	}

	//if( m_pInfo->animController.GetAnimationController()->GetNumAnimationSets() > 0 )
	{
		m_pInfo->animController->CreateAnimController( this );
	}

	//アニメーショントラックを得る
	//for(DWORD i = 0 ; i < m_pInfo->pAnimController->GetNumAnimationSets() ; i++ )
	//{
	//	m_pInfo->pAnimController->GetAnimationSet(i,&m_pInfo->m_pAnimSet[( m_pInfo->pAnimController->GetNumAnimationSets() - 1 ) - i]);
	//}

	AllocateAllBoneMatrices(&m_pInfo,m_pInfo->pFrameRoot);

	//m_pInfo->pMesh->Release();	//読み込んだメッシュを解放
	//m_pInfo->pMesh = pMeshTemp;	//最適化したメッシュに変更

	return S_OK;

}

/*=======================================================
	関数名	：	Update
	説明	：	更新
	引数	：	アニメーションコントローラ
				ワールド行列
	戻り値	：　なし
========================================================*/
void SkinAnimationModel::Update(AnimController* pAnimController , LPD3DXMATRIX pWorld)
{

	#define ANIM_SPEED (60.0f / 4800.0f)
	//#define ANIM_SPEED (120.0f / 4800.0f)

	if( pAnimController != NULL)
	{
		pAnimController->AdvanceTime(ANIM_SPEED);
		m_AnimeTime += static_cast<float>(ANIM_SPEED);
	}
	else
	{
		m_pInfo->animController->AdvanceTime(ANIM_SPEED);
		m_AnimeTime += static_cast<float>(ANIM_SPEED);
	}
	if( pWorld )
	{
		UpdateFrameMatrices(m_pInfo->pFrameRoot, pWorld);
	}

}

/*=======================================================
	関数名	：	ORG_DrawMesh
	説明	：	メッシュの描画
	引数	：	ワールド行列
				アニメーションコントローラ
				アルファ
	戻り値	：　結果
========================================================*/
HRESULT SkinAnimationModel::ORG_DrawMesh( D3DXMATRIX world , AnimController* pAnimController , float alpha )
{
	//透明度の設定
	m_alpha = alpha;

	//マテリアルの設定
	for( DWORD i = 0 ; i < m_pInfo->dwNumMaterial ; i++ )
	{
		m_pD3DDevice->SetMaterial( &m_pInfo->pMaterials[i] );
		m_pD3DDevice->SetTexture( 0 , m_pInfo->pTexture[i].GetTexture() );

		m_pInfo->pMesh->DrawSubset( i );
	}
	
	//アニメーションの準備
	if(pAnimController)
	{
		pAnimController->DrawSetUp();
	}
	else
	{
		m_pInfo->animController->DrawSetUp();
	}
	if( world )
	{
		UpdateFrameMatrices(m_pInfo->pFrameRoot, &world);
	}

	//レンダリング

	DrawFrame( m_pInfo->pFrameRoot);

	m_pD3DDevice->SetRenderState( D3DRS_VERTEXBLEND , D3DVBF_DISABLE );

	return S_OK;
}

/*=======================================================
	関数名	：	ReleaseAnimationMesh
	説明	：	アニメーションメッシュの解放
	引数	：	アニメーションメッシュインフォのポインタ
	戻り値	：　なし
========================================================*/
void SkinAnimationModel::ReleaseAnimationMesh(LPD3DXFRAME pFrame)
{

	if(pFrame->pMeshContainer != NULL)
	{			
		m_cHierarchy.DestroyMeshContainer(pFrame->pMeshContainer);
	}	
	
    if (pFrame->pFrameSibling != NULL)
    {
		ReleaseAnimationMesh(pFrame->pFrameSibling);
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
		ReleaseAnimationMesh(pFrame->pFrameFirstChild);
    }

}

/*=======================================================
	関数名	：	ORG_ReleaseMesh
	説明	：	メッシュの解放
	引数	：	メッシュインフォのポインタ
	戻り値	：　結果
========================================================*/
HRESULT SkinAnimationModel::ORG_ReleaseMesh()
{
	
	if( m_pInfo == NULL )
	{
		return S_OK;
	}

	if( m_pInfo->pFrameRoot != NULL )
	{
		ReleaseAnimationMesh(m_pInfo->pFrameRoot);

		m_cHierarchy.DestroyFrame(m_pInfo->pFrameRoot);

		m_pInfo->pFrameRoot = NULL;
	}
	
	//if( m_pInfo->pTexture != NULL )
	//{
	//	SafeDelete(m_pInfo->pTexture);
	//}

	//if( m_pInfo->pTexture != NULL )
	//{
	//	SafeDelete(m_pInfo->pTexture);
	//}

	//メッシュの解放
	SafeDelete(m_pInfo);

	//delete m_pInfo;

	return S_OK;
}

/*=======================================================
	関数名	：	UpdateFrameMatrices
	説明	：	フレーム内のメッシュ毎にワールド変換行列を更新
	引数	：	LPD3DXFRAME		pFrameBase
				LPD3DXMATRIX	pParentMatrix
	戻り値	：　なし
========================================================*/
void SkinAnimationModel::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
    MYFRAME *pFrame = (MYFRAME*)pFrameBase;	

    if (pParentMatrix != NULL)
	{
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
    else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}

    if (pFrame->pFrameSibling != NULL)
    {
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
    }
}

/*=======================================================
	関数名	：	DrawFrame
	説明	：	複数のメッシュから成るフレームをレンダリングする。
	引数	：	LPD3DXFRAME pFrame
	戻り値	：　なし
========================================================*/
void SkinAnimationModel::DrawFrame( LPD3DXFRAME pFrameBase)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
    MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;	

    while(pMeshContainer != NULL)
    {
		RenderMeshContainer( pMeshContainer, pFrame );

		pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
    }

    if(pFrame->pFrameSibling != NULL)
    {
		DrawFrame( pFrame->pFrameSibling);
    }

    if(pFrame->pFrameFirstChild != NULL)
    {
		DrawFrame( pFrame->pFrameFirstChild);
    }
}

/*=======================================================
	関数名	：	DrawFrame
	説明	：	フレーム内のそれぞれのメッシュをレンダリングする
	引数	：	LPD3DXMESHCONTAINER pMeshContainerBase
				LPD3DXFRAME pFrameBase
	戻り値	：　なし
========================================================*/
void SkinAnimationModel::RenderMeshContainer( MYMESHCONTAINER* pMeshContainer, MYFRAME* pFrame)
{

	DWORD i,k;
	DWORD dwBlendMatrixNum;    
    DWORD dwPrevBoneID;
    LPD3DXBONECOMBINATION pBoneCombination;
    UINT iMatrixIndex;
	D3DXMATRIX mStack[4];
	int blendNum = 0;


	
	//スキンメッシュの場合
	if(pMeshContainer->pSkinInfo != NULL)
	{	
		
		//MessageBox(NULL , "スキンメッシュの描画", "class SkinAnimationModel",MB_OK);
		pBoneCombination = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();

		dwPrevBoneID=UINT_MAX;
		for(i = 0; i < pMeshContainer->dwBoneNum; i++)
		{
			dwBlendMatrixNum=0;
			for(k = 0 ; k < pMeshContainer->dwWeight ; k++)
			{
				if(pBoneCombination[i].BoneId[k] != UINT_MAX) 
				{
					dwBlendMatrixNum = k;
				}
			}
			
			
			m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD );
			m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixNum);
		
			D3DXMATRIX mWorld;
			for( int j = 0 ; j < 4 ; j++)
			{
				D3DXMatrixIdentity( &mStack[j] );
			}

			for(k = 0; k < pMeshContainer->dwWeight; k++) 
			{
				iMatrixIndex = pBoneCombination[i].BoneId[k];
				if (iMatrixIndex != UINT_MAX)
				{

					mWorld = pMeshContainer->pBoneOffsetMatrices[iMatrixIndex] * (*pMeshContainer->ppBoneMatrix[iMatrixIndex]);
					

					m_pD3DDevice->SetTransform( D3DTS_WORLDMATRIX(k), &mWorld );

					blendNum++;
				}
			}

			pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D.Ambient.r = 1.0f;
			pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D.Ambient.g = 1.0f;
			pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D.Ambient.b = 1.0f;

			//透明度の設定
			pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D.Diffuse.a = m_alpha;

			//描画
			m_pD3DDevice->SetRenderState(D3DRS_COLORVERTEX,TRUE);
			m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);

			m_pD3DDevice->SetMaterial( &pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D );
			m_pD3DDevice->SetTexture( 0, NULL );
			m_pD3DDevice->SetTexture( 0, pMeshContainer->ppTextures[pBoneCombination[i].AttribId] );
			dwPrevBoneID = pBoneCombination[i].AttribId;
			pMeshContainer->MeshData.pMesh->DrawSubset(i);	

		}	
	}
	//通常メッシュの場合
	else
	{

	
		//MessageBox(NULL , "通常メッシュの描画", "class SkinAnimationModel",MB_OK);
		m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
		m_pD3DDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);
		for (i = 0; i < pMeshContainer->NumMaterials; i++)
		{
			m_pD3DDevice->SetRenderState(D3DRS_COLORVERTEX,TRUE);
			m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
			//透明度の設定
			pMeshContainer->pMaterials[i].MatD3D.Diffuse.a  = m_alpha;
			m_pD3DDevice->SetMaterial( &pMeshContainer->pMaterials[i].MatD3D );
			m_pD3DDevice->SetTexture( 0, pMeshContainer->ppTextures[i] );
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}
}



//フレームを作成する
HRESULT MY_ANIME_HIERARCHY::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
{
    HRESULT hr = S_OK;
    MYFRAME *pFrame;
    *ppNewFrame = NULL;

    pFrame = new MYFRAME;
    if (pFrame == NULL)
    {
		return E_OUTOFMEMORY;    
    }
	if( Name != NULL)
	{
		pFrame->Name=new TCHAR[lstrlen(Name) + 1];	
		if (!pFrame->Name)
		{
			MessageBox(NULL , "エラー３", "class SkinAnimationModel",MB_OK);
			return E_FAIL;
		}

			strcpy_s(pFrame->Name,sizeof(Name),Name);
	}
	else
	{
		pFrame->Name = new TCHAR[1];	
		pFrame->Name[0] = '\0';
	}

    D3DXMatrixIdentity(&pFrame->TransformationMatrix);
    D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);
    pFrame->pMeshContainer = NULL;
    pFrame->pFrameSibling = NULL;
    pFrame->pFrameFirstChild = NULL;
    *ppNewFrame = pFrame;
    pFrame = NULL;
    //SafeDelete pFrame;

    return S_OK;
}
//
//HRESULT MY_ANIME_HIERARCHY::CreateMeshContainer
//メッシュコンテナーを作成する
HRESULT MY_ANIME_HIERARCHY::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
                            CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
							DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, 
                            LPD3DXMESHCONTAINER *ppMeshContainer) 
{

    MYMESHCONTAINER *pMeshContainer = NULL;
    int iFacesAmount;
    int iMaterial;
    LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
    LPD3DXMESH pMesh = NULL;
    *ppMeshContainer = NULL;
	DWORD dwBoneNum=0;

    pMesh = pMeshData->pMesh;
    pMeshContainer = new MYMESHCONTAINER;
    if (pMeshContainer == NULL)
    {
        return E_OUTOFMEMORY;
    }
    ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));

	pMeshContainer->Name=new TCHAR[lstrlen(Name) + 1];	
    if (!pMeshContainer->Name)
	{
		MessageBox(NULL , "エラー４", "class SkinAnimationModel",MB_OK);
		return E_FAIL;
	}
	strcpy_s(pMeshContainer->Name,sizeof(Name),Name);
    pMesh->GetDevice(&m_pD3DDevice);
    iFacesAmount = pMesh->GetNumFaces();  
   
	pMeshContainer->MeshData.pMesh = pMesh;   
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;	

	if (pSkinInfo == NULL)
	{
		pMesh->AddRef();//通常メッシュの場合はこれが必要。スキンの場合、これをするとなぜかメモリリーク
	} 
	//メッシュのマテリアル設定
    pMeshContainer->NumMaterials = max(1, NumMaterials);
    pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
    pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
    pMeshContainer->pAdjacency = new DWORD[iFacesAmount*3];
    if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
    {
		MessageBox(NULL , "エラー５", "class SkinAnimationModel",MB_OK);
        return E_FAIL;
    }
    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount*3);
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

    if (NumMaterials > 0)            
    {
        memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

        for (iMaterial = 0; iMaterial < static_cast<int>(NumMaterials); iMaterial++)
        {
            if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
            {
                TCHAR strTexturePath[MAX_PATH];
				strcpy_s( strTexturePath, sizeof("works_romfiles/model/") , "works_romfiles/model/" );
                strcat_s( strTexturePath, sizeof(pMeshContainer->pMaterials[iMaterial].pTextureFilename) , pMeshContainer->pMaterials[iMaterial].pTextureFilename );
                if( FAILED( D3DXCreateTextureFromFile( m_pD3DDevice, strTexturePath, 
                                                        &pMeshContainer->ppTextures[iMaterial] ) ) )
				{
                    pMeshContainer->ppTextures[iMaterial] = NULL;
				}

                pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
            }
        }
    }
    else
    {
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }
	//当該メッシュがスキン情報を持っている場合（スキンメッシュ固有の処理）
	if (pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();
		dwBoneNum = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];

		for (DWORD i= 0; i < dwBoneNum; i++)
		{	
			memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
				pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i),sizeof(D3DMATRIX));	
		}
		if(FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
			pMesh,                                       
			NULL,	pMeshContainer->pAdjacency,NULL, NULL, NULL,
			&pMeshContainer->dwWeight,
			&pMeshContainer->dwBoneNum,
			&pMeshContainer->pBoneBuffer,
			&pMeshContainer->MeshData.pMesh)
			))
		{
			MessageBox(NULL , "エラー６", "class SkinAnimationModel",MB_OK);
			return E_FAIL;
		}
	}
	//ローカルに生成したメッシュコンテナーを呼び出し側にコピーする
	*ppMeshContainer = pMeshContainer;
	//参照カウンタを増やしたので減らす
	SafeRelease(m_pD3DDevice);

    return S_OK;
}
//
//HRESULT MY_ANIME_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree) 
//フレームを破棄する
HRESULT MY_ANIME_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
	if(pFrameToFree->pFrameFirstChild)
	{    
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
	if(pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}

	//SAFE_DELETE_ARRAY( pFrameToFree->Name );
 //   SAFE_DELETE( pFrameToFree );

	if( pFrameToFree->Name != NULL )
	{
		SafeDeleteArray(pFrameToFree->Name);
	}

	if( pFrameToFree != NULL )
	{
		SafeDelete(pFrameToFree);
	}

    return S_OK; 
}
//
//HRESULT MY_ANIME_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
//メッシュコンテナーを破棄する
HRESULT MY_ANIME_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
    int iMaterial;
    MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	//SAFE_RELEASE(pMeshContainer->pSkinInfo);
 //   SAFE_DELETE_ARRAY( pMeshContainer->Name );
 //   SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
	//SAFE_DELETE_ARRAY( pMeshContainer->pMaterials );

	if( pMeshContainer->pSkinInfo != NULL )
	{
		pMeshContainer->pSkinInfo->Release();
	}
	if( pMeshContainer->Name != NULL )
	{
		SafeDeleteArray(pMeshContainer->Name);
	}
	if( pMeshContainer->pAdjacency != NULL )
	{
		SafeDeleteArray(pMeshContainer->pAdjacency);
	}
	if( pMeshContainer->pMaterials != NULL )
	{
		SafeDeleteArray(pMeshContainer->pMaterials);
	}




    if (pMeshContainer->ppTextures != NULL)
    {
        for (iMaterial = 0; iMaterial <  static_cast<int>( pMeshContainer->NumMaterials ); iMaterial++)
        {
            //SAFE_RELEASE( pMeshContainer->ppTextures[iMaterial] );
			if( pMeshContainer->ppTextures[iMaterial] != NULL )
			{
				pMeshContainer->ppTextures[iMaterial]->Release();
			}
        }
		//SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
		if( pMeshContainer->ppTextures != NULL )
		{
			SafeDeleteArray(pMeshContainer->ppTextures);
		}
    }
    //SAFE_RELEASE( pMeshContainer->MeshData.pMesh);
    //SAFE_DELETE( pMeshContainer );

	if( pMeshContainer->MeshData.pMesh != NULL )
	{
		pMeshContainer->MeshData.pMesh->Release();
	}

	SafeDelete(pMeshContainer);

    return S_OK;
}

/*=======================================================
	関数名	：	AllocateBoneMatrix
	説明	：	ボーンのマトリクスを割り当てる
	引数	：	SkinAnimationModel	pMeshContainerBase
			LPD3DXMESHCONTAINER pMeshContainerBase
	戻り値	：	結果
========================================================*/
HRESULT SkinAnimationModel::AllocateBoneMatrix( LPANIMATIONMESHINFO	*pInfo, LPD3DXMESHCONTAINER pMeshContainerBase )
{

	MYFRAME *pFrame=NULL;
	DWORD dwBoneNum=0;

    MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
    if (pMeshContainer->pSkinInfo == NULL)
	{
		return S_OK;
	}
	dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
    pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneNum];

    for (DWORD i=0; i<dwBoneNum; i++)
    {
		pFrame = (MYFRAME*)D3DXFrameFind( (*pInfo)->pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i) );
        if (pFrame == NULL)
		{
			MessageBox(NULL , "エラー７", "class SkinAnimationModel",MB_OK);
			return E_FAIL;
		}
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;
 
	}    
    return S_OK;
}

/*=======================================================
	関数名	：	AllocateAllBoneMatrices
	説明	：	ボーンのマトリクスを割り当てる
	引数	：	SkinAnimationModel	pMeshContainerBase
				LPD3DXFRAME		pFrameBase
	戻り値	：	結果
========================================================*/
HRESULT SkinAnimationModel::AllocateAllBoneMatrices( LPANIMATIONMESHINFO	*pInfo,  LPD3DXFRAME pFrameBase )
{
    if ( pFrameBase->pMeshContainer != NULL)
    {
        if(FAILED(AllocateBoneMatrix(pInfo,pFrameBase->pMeshContainer)))
		{
			MessageBox(NULL , "エラー８", "class SkinAnimationModel",MB_OK);
			return E_FAIL;
		}
    }
    if ( pFrameBase->pFrameSibling != NULL)
    {
        if(FAILED(AllocateAllBoneMatrices(pInfo,pFrameBase->pFrameSibling)))
		{
			MessageBox(NULL , "エラー９", "class SkinAnimationModel",MB_OK);
			return E_FAIL;
		}
    }
    if ( pFrameBase->pFrameFirstChild != NULL)
    {
        if(FAILED(AllocateAllBoneMatrices(pInfo,pFrameBase->pFrameFirstChild)))
		{
			MessageBox(NULL , "エラー１０", "class SkinAnimationModel",MB_OK);
			return E_FAIL;
		}
    }
    return S_OK;
}

/*=======================================================
	関数名	：	CloneAnimationController
	説明	：	アニメーションコントローラを複製
	引数	：	アニメーションコントローラ
	戻り値	：　なし
========================================================*/
void SkinAnimationModel::CloneAnimationController( LPD3DXANIMATIONCONTROLLER* pAnimController )
{
    //アニメーションコントローラのコピーを作る
    m_pInfo->pAnimController->CloneAnimationController(
        m_pInfo->pAnimController->GetMaxNumAnimationOutputs(),
        m_pInfo->pAnimController->GetMaxNumAnimationSets(),
        m_pInfo->pAnimController->GetMaxNumTracks(),
        m_pInfo->pAnimController->GetMaxNumEvents(),
        pAnimController);

}

/*=======================================================
	関数名	：	SetAnimationSet
	説明	：	アニメーションセットを設定
	引数	：	アニメーションセット
	戻り値	：　なし
========================================================*/
void SkinAnimationModel::SetAnimationSet( LPD3DXANIMATIONSET* pAnimSet )
{
	pAnimSet = m_pInfo->m_pAnimSet;
}

/*=======================================================
	関数名	：	FindFrameMatrices
	説明	：	フレーム内のメッシュ毎のワールド変換行列を見つける
	引数	：	const char* boneName
			LPD3DXFRAME		pFrameBase
			LPD3DXMATRIX	pParentMatrix
	戻り値	： なし
========================================================*/
void SkinAnimationModel::FindFrameMatrices( const char* boneName , LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{

    MYFRAME *pFrame = (MYFRAME*)pFrameBase;	

    if (pParentMatrix != NULL)
	{
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
    else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	if( strcmp( boneName , pFrame->Name ) == 0 )
	{
		m_findBoneMatrix = pFrame->CombinedTransformationMatrix;
	}

    if (pFrame->pFrameSibling != NULL)
    {
		FindFrameMatrices( boneName , pFrame->pFrameSibling, pParentMatrix);
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
		FindFrameMatrices( boneName , pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
    }
}

/*=======================================================
	関数名	：	FindMatrices
	説明	：	フレーム内のメッシュ毎のワールド変換行列を見つける
	引数	：	const char* boneName
			LPD3DXMATRIX worldMatrix
	戻り値	：　マトリクス
========================================================*/
LPD3DXMATRIX SkinAnimationModel::FindMatrices(const char* boneName , LPD3DXMATRIX world)
{
	FindFrameMatrices( boneName , m_pInfo->pFrameRoot, world);

	return &m_findBoneMatrix;

}

}