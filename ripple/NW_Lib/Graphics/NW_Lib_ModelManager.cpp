/**
 * @file    ModelManager.cpp
 * @brief   モデルを管理するためのクラス
 * 
 * @author  皆光　健太
 * @date    2013/09/03
 */
#include "NW_Lib_ModelManager.h"

#include "NW_Lib_SkinAnimationModel.h"
#include "NW_Lib_model.h"

#include <fstream>
#include <string>


namespace NW_Lib
{
	void ModelManager::LoadModel(int handle,char* filename)
	{
		// ファイルパスをくっ付ける
		static const char* rom = "works_romfiles/model/";
		char file[FILENAME_LENGTH];
		memset(file, 0, sizeof(file));
		strcpy_s(file, sizeof(rom) ,rom);
		strcat_s(file, sizeof(filename), filename);
		strcat_s(file, sizeof(".x"), ".x");

		std::string temp;
		std::ifstream fs( file );
		bool animModelFlag = false;
		
		///////アニメーションモデルの判断///////

		while(fs >> temp)
		{
			if(temp.find("Animation") != string::npos)
			{
				animModelFlag = true;
				break;
			}
		}

		LoadModelInfo* loadModel = new LoadModelInfo();
		if( animModelFlag )
		{
			//スキンメッシュの読み込み
			loadModel->model = new SkinAnimationModel(file);
			loadModel->animFlag = true;
		}
		else
		{
			//通常メッシュの読み込み
			loadModel->model = new Model(file);
			loadModel->animFlag = false;
		}

		//////////////////////////////////////////////

		m_pLoadModelManager.Entry(handle,loadModel);

	}

	void ModelManager::Entry(int handle,int loadHandle)
	{
		UseModel* useModel = new UseModel();

		LoadModelInfo* loadModel = m_pLoadModelManager.Get(loadHandle);
		useModel->m_pModel = loadModel->model;

		useModel->m_pAnimController = NULL;
		if( loadModel->animFlag )
		{
			useModel->m_pAnimController = new AnimController();
			useModel->m_pAnimController->CreateAnimController((NW_Lib::SkinAnimationModel*)(useModel->m_pModel));
		}
		else
		{
			useModel->m_pAnimController = NULL;
		}
		
		m_pModelManager.Entry(handle,useModel);
	}

	void ModelManager::SetPosision(int handle,VECTOR3 position)
	{
		UseModel* pUseModel = m_pModelManager.Get(handle);
		pUseModel->m_Pos = position;
	}

	void ModelManager::SetRotate(int handle,VECTOR3 rotation)
	{
		UseModel* pUseModel = m_pModelManager.Get(handle);
		pUseModel->m_Rot = rotation;
	}

	void ModelManager::SetScale(int handle,VECTOR3 scale)
	{
		UseModel* pUseModel = m_pModelManager.Get(handle);
		pUseModel->m_Scale = scale;
	}

	void ModelManager::SetMatrix(int handle,MATRIX* pMat)
	{
		UseModel* pUseModel = m_pModelManager.Get(handle);
		pUseModel->m_Mat = *pMat;
	}

	/// モデルの更新
	void ModelManager::Update(int hundle)
	{
		UseModel* pUseModel = m_pModelManager.Get(hundle);

		pUseModel->m_pModel->Update(pUseModel->m_pAnimController,&pUseModel->m_Mat);
	}

	/// モデルの描画
	void ModelManager::Draw(DrawingScreen drawingScreen,int handle)
	{
		UseModel* pUseModel = m_pModelManager.Get(handle);

		SettingMatrix(handle);

		pUseModel->m_pModel->Draw( *&pUseModel->m_Mat , pUseModel->m_pAnimController , pUseModel->m_alpha);
	}

	/// モデルのワールド座標を設定
	void ModelManager::SettingMatrix(int handle)
	{
		UseModel* pUseModel = m_pModelManager.Get(handle);

		D3DXMATRIX matScale,matRot,matTrans;

		D3DXMatrixIdentity( &pUseModel->m_Mat );

		D3DXMatrixScaling( &matScale , pUseModel->m_Scale.x,pUseModel->m_Scale.y,pUseModel->m_Scale.z );
		D3DXMatrixRotationYawPitchRoll( &matRot , pUseModel->m_Rot.x,pUseModel->m_Rot.y,pUseModel->m_Rot.z);
		D3DXMatrixTranslation( &matTrans ,pUseModel->m_Pos.x,pUseModel->m_Pos.y,pUseModel->m_Pos.z);

		D3DXMatrixMultiply( &pUseModel->m_Mat , &pUseModel->m_Mat , &matScale );
		D3DXMatrixMultiply( &pUseModel->m_Mat , &pUseModel->m_Mat , &matRot );
		D3DXMatrixMultiply( &pUseModel->m_Mat , &pUseModel->m_Mat , &matTrans );
			
		m_pD3DDevice->SetTransform(D3DTS_WORLD, &pUseModel->m_Mat);

	}

	/// モデルの透明度の設定
	void ModelManager::SetAlpha(int handle,float alpha)
	{
		UseModel* pUseModel = m_pModelManager.Get(handle);

		pUseModel->m_alpha = alpha;
	}

	/// モデルのアニメーショントラックの設定
	void ModelManager::SetAnimationTruck(int handle, int truck)
	{
		UseModel* pUseModel = m_pModelManager.Get(handle);

		pUseModel->m_pAnimController->ChangeAnimation(truck);
	}

	void ModelManager::Remove(int handle)
	{
		m_pModelManager.Remove(handle);
	}

	void ModelManager::AllRemove()
	{
		m_pModelManager.AllRemove();
		m_pLoadModelManager.AllRemove();
	}
}
