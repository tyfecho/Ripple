/***
*	@file	model.cpp
*
*	@brief	3Dモデル用クラス
*
*	@date	2012/08/01
*
*	@author	吉田　将也
*/

#include "NW_Lib_model.h"
#include "d3dx9.h"
#include <string>

#include "../Utility/NW_Lib_utility.h"

using namespace std;


namespace NW_Lib
{
	// 白色になるマテリアル
	static const D3DMATERIAL9 WHITEMATERIAL = 
	{
		1.0f,1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,1.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,

		10.0f
	};
	// 黒色にするマテリアル
	static const D3DMATERIAL9 BLACKMATERIAL = 
	{
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,

		10.0f
	};
	// 灰色にするマテリアル
	static const D3DMATERIAL9 GLAYMATERIAL = 
	{
		0.5f,0.5f,0.5f,0.5f,
		0.5f,0.5f,0.5f,0.5f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,

		10.0f
	};


	/**
	*	コンストラクタ
	*/
	Model::Model()
	:m_Pos   (0.0f,0.0f,0.0f)
	,m_Angle (0.0f,0.0f,0.0f)
	,m_Radian(0.0f,0.0f,0.0f)
	,m_Scale (1.0f,1.0f,1.0f)
	,m_pParentWorld(NULL)
	,m_pInitWorld(NULL)
	,m_NoMaterial(false)
	,m_MaterialGlay(false)
	{
	}
	/**
	*	コンストラクタ
	*	@param filename  読み込むファイル名
	*/
	Model::Model(char* filename)
	:m_Pos   (0.0f,0.0f,0.0f)
	,m_Angle (0.0f,0.0f,0.0f)
	,m_Radian(0.0f,0.0f,0.0f)
	,m_Scale (1.0f,1.0f,1.0f)
	,m_pParentWorld(NULL)
	,m_pInitWorld(NULL)
	,m_NoMaterial(false)
	,m_MaterialGlay(false)
	{
		LoadMesh(filename);
	}

	/**
	*	デストラクタ
	*/
	Model::~Model()
	{
		Release();
		//NoTexture();
	}

	/**
	*	メッシュのロード
	*	@param filename  読み込むファイル名
	*/
	void Model::LoadMesh(const char* filename)
	{
		LPD3DXBUFFER pD3DXMtrlBuffer;       // マテリアルデータ
		LPD3DXBUFFER pAdjacencyBuffer;      // 隣接データ
		LPD3DXBUFFER pEffectInstanceBuffer; // エフェクト

		// 情報初期化
		//ZeroMemory(m_mesh,sizeof(MESHOBJECT));

		string meshname = filename;
		//meshname.insert(0,"data//mesh//");

		// メッシュをロード
		if (FAILED(D3DXLoadMeshFromX(
			meshname.c_str(),
							D3DXMESH_SYSTEMMEM,
							m_pD3DDevice,
							&pAdjacencyBuffer,
							&pD3DXMtrlBuffer,
							&pEffectInstanceBuffer,
							&m_numMaterials,
							&m_mesh))){
								// モデル作成失敗

				// spriteファイルの中にもなかった
				// エラー処理
				MessageBox(NULL, "モデルが読み込めない", "class Model", MB_OK);
		}

		// とりあえず使わないため解放しておく
		pEffectInstanceBuffer->Release();

		D3DXMATERIAL* d3dxMaterials = static_cast<D3DXMATERIAL*>(pD3DXMtrlBuffer->GetBufferPointer());
		m_meshMaterials  = new D3DMATERIAL9[m_numMaterials];
		m_meshtexture    = new Texture*[m_numMaterials];
		m_ppSetTex       = new Texture*[m_numMaterials];
		m_pMasterAmbient = new D3DCOLORVALUE[m_numMaterials];

		for(DWORD i = 0;i < m_numMaterials;++i){
			// マテリアルのコピー
			m_meshMaterials[i] = d3dxMaterials[i].MatD3D;

			// アンビエントカラーの設定
			m_meshMaterials[i].Ambient = m_meshMaterials[i].Diffuse;
			m_pMasterAmbient[i] = m_meshMaterials[i].Ambient;

			if(d3dxMaterials[i].pTextureFilename){
			//	// テクスチャの設定
			//	D3DXCreateTextureFromFile(pD3DDevice,d3dxMaterials[i].pTextureFilename,&m_meshtexture[i]);
				string map = d3dxMaterials[i].pTextureFilename;
				map.insert(0,"data//Resorse//");
				m_meshtexture[i] = new Texture(map.c_str());
			}else{
				// テクスチャなし
				m_meshtexture[i] = NULL;
			}
			m_ppSetTex[i] = NULL;
		}

		// バッファの解放
		pD3DXMtrlBuffer->Release();

		LPD3DXMESH MeshTemp = NULL;
		HRESULT hr;

		// メッシュ最適化
		hr = m_mesh->Optimize( 
									D3DXMESHOPT_VERTEXCACHE | D3DXMESH_SYSTEMMEM,
									(DWORD*)pAdjacencyBuffer->GetBufferPointer(),
									NULL,
									NULL,
									NULL,
									&MeshTemp
								  );

		pAdjacencyBuffer->Release();

		m_mesh->Release();
		m_mesh = MeshTemp;
	}

	/**
	*	テクスチャの設定（こっちでnewするパターン）
	*	@param filename  読み込むファイル名
	*	@param tex_num   入れ替えるテクスチャ
	*/
	void Model::SetTexture(const char *filename,unsigned int tex_num)
	{
		if(tex_num < m_numMaterials){
			// テクスチャの解放
			if(m_meshtexture != NULL){
				if(m_meshtexture[tex_num] != NULL){
					// 解放
					SafeDelete( m_meshtexture[tex_num]);
				}
			}
			string map = filename;
			map.insert(0,"data//Resorse//");
			m_meshtexture[tex_num] = new Texture(map.c_str());
		}
	}

	/**
	*	テクスチャの設定（用意してあるやつを使うパターン）
	*	@param pTex    テクスチャ情報
	*	@param tex_num 入れ替えるテクスチャ
	*/
	void Model::SetTexture(Texture* pTex,unsigned int tex_num)
	{
		SafeDelete(m_ppSetTex[tex_num]);
		m_ppSetTex[tex_num] = pTex;
	}
	/**
		テクスチャ情報を消す
	*/
	void Model::NoTexture()
	{
		for(DWORD i = 0;i < m_numMaterials;++i){
			SafeDelete(m_ppSetTex[i]);
			SafeDelete(m_meshtexture[i]);
		}
	}

	/**
	*	解放処理
	*/
	void Model::Release()
	{
		// マテリアルのバッファを開放
		if(m_meshtexture != NULL){
			delete[] m_meshMaterials;
			m_meshMaterials = NULL;
		}

		// テクスチャの解放
		if(m_meshtexture != NULL){
			for(DWORD i = 0;i < m_numMaterials;++i){
				if(m_meshtexture[i] != NULL){
					// 解放
					//m_meshtexture[i]->Release();
					SafeDelete( m_meshtexture[i]);
				}
			}
			delete[] m_pMasterAmbient;
			delete[] m_meshtexture;
			delete[] m_ppSetTex;
			m_meshtexture = NULL;
		}

		// メッシュ自体を解放
		if(m_mesh != NULL){
			m_mesh->Release();
			m_mesh = NULL;
		}

		// メッシュ情報の解放
		SafeDelete( m_mesh);
	}

	/**
	*	行列計算処理
	*/
	void Model::Calc()
	{

		D3DXMATRIX matScale;    // 拡縮用
		D3DXMATRIX matMove;     // 移動用
		D3DXMATRIX matRotation; // 回転用
		D3DXMatrixIdentity(&m_LocalWorld);

		if(m_pInitWorld != NULL){
			// ローカル計算前にかけておく
			m_LocalWorld *= *m_pInitWorld;
		}

		// スケーリング
		D3DXMatrixScaling(&matScale,m_Scale.x,m_Scale.y,m_Scale.z);
		// 回転
		D3DXMatrixRotationYawPitchRoll( &matRotation , m_Radian.y , m_Radian.x , m_Radian.z );
		// 移動
		D3DXMatrixTranslation( &matMove , m_Pos.x,m_Pos.y,m_Pos.z );

		// 各合成
		m_LocalWorld *= matScale;
		m_LocalWorld *= matRotation;
		m_LocalWorld *= matMove;

		if(m_pParentWorld != NULL){
			// 親の行列がある
			m_LocalWorld *= *m_pParentWorld;
		}

//		m_pD3DDevice->SetTransform(D3DTS_WORLD,&m_LocalWorld);
	}

	/**
	*	描画処理
	*/
	void Model::Draw(/**D3DVIEWPORT9* pViewport,int viewport,DWORD* pBackColor*/)
	{
		//// ビューポートの設定
		//m_pD3DDevice->SetViewport(&pViewport[viewport]);

		////	画面をクリア
		//m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, pBackColor[viewport], 1.0f, 0);

		//Calc();

		for(DWORD i = 0;i < m_numMaterials;++i){

			//アルファブレンディングを設定する	
			m_pD3DDevice->SetRenderState (D3DRS_ALPHABLENDENABLE,TRUE);
			m_pD3DDevice->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			m_pD3DDevice->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);


			m_pD3DDevice->SetRenderState (D3DRS_ALPHATESTENABLE,TRUE);
			m_pD3DDevice->SetRenderState (D3DRS_ALPHAREF,1);
			m_pD3DDevice->SetRenderState (D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
		
			//m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
	
			//m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);

			m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
			m_pD3DDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);

			m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);


			// マテリアルの設定
			if(m_MaterialGlay){
				m_pD3DDevice->SetMaterial(&GLAYMATERIAL);
			}else if(m_NoMaterial){
				m_pD3DDevice->SetMaterial(&BLACKMATERIAL);
			}else{
				m_pD3DDevice->SetMaterial(&m_meshMaterials[i]);
			}

			// テクスチャの設定
			if(m_ppSetTex[i] != NULL){
				m_pD3DDevice->SetTexture(0,m_ppSetTex[i]->GetTexture());
			}else if(m_meshtexture[i] != NULL){
				m_pD3DDevice->SetTexture(0,m_meshtexture[i]->GetTexture());
			}

			// 表示
			m_mesh->DrawSubset(i);
			m_pD3DDevice->SetTexture(0,NULL);
			m_pD3DDevice->SetMaterial(&WHITEMATERIAL);
		}
	}
}


