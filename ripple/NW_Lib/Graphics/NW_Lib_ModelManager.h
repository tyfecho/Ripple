/**
 * @file    ModelManager.h
 * @brief   モデルを管理するためのクラス
 * 
 * @author  Masaya Yoshida
 * @date    2013/08/28
 * @version 1.0
 */
#ifndef MODELMANAGER_H_
#define MODELMANAGER_H_

#include "../Utility/NW_Lib_Constant.h"
#include "../Graphics/NW_Lib_texture.h"
#include "../System//NW_Lib_ResourceManager.h"
#include "NW_Lib_SkinAnimationModel.h"
#include "NW_Lib_AnimController.h"
#include "../System/NW_Lib_Common.h"

namespace NW_Lib
{
	struct UseModel
	{
		IModel* m_pModel;
		AnimController* m_pAnimController;

		VECTOR3 m_Pos;
		VECTOR3 m_Rot;
		VECTOR3 m_Scale;
		MATRIX  m_Mat;

		float m_alpha;

		UseModel()
		:m_pModel(NULL)
		,m_pAnimController(NULL)
		,m_Pos(0.0f,0.0f,0.0f)
		,m_Rot(0.0f,0.0f,0.0f)
		,m_Scale(1.0f,1.0f,1.0f)
		,m_alpha(1.0f)
		{
		}
			
		~UseModel()
		{
			SafeDelete(m_pAnimController);
		}

	};

	struct LoadModelInfo
	{
		IModel* model;
		bool animFlag;

		~LoadModelInfo()
		{
			SafeDelete(model);
		}
	};

	class ModelManager : public Common
	{
		private:
			// ファイル名の長さ
			static const int FILENAME_LENGTH = 64;

			ResourceManager<LoadModelInfo*> m_pLoadModelManager;
			ResourceManager<UseModel*> m_pModelManager;


		private:
			/// コンストラクタ
			ModelManager(){}
			ModelManager(const ModelManager& val){(void)val;}
			ModelManager& operator=(const ModelManager& val){(void)val;return *this;}

		public:
			/// シングルトン化させる
			static ModelManager& GetInstance()
			{
				static ModelManager instance;
				return instance;
			}

			/// ファイル読み込み
			void LoadModel(int loadHandle,char* filename);

			/// モデルの生成
			void Entry(int handle,int loadHandle);

			/// モデルの位置の設定
			void SetPosision(int handle,VECTOR3 position);
			/// モデルの回転の設定
			void SetRotate(int handle,VECTOR3 rotation);
			/// モデルのサイズの設定
			void SetScale(int handle,VECTOR3 scale);
			/// モデルの行列設定
			void SetMatrix(int handle,MATRIX* pMat);

			/// モデルの更新
			void Update(int hundle);

			/// モデルの描画
			void Draw(DrawingScreen drawingScreen,int handle);

			/// モデルの透明度の設定
			void SetAlpha(int handle,float alpha);

			/// モデルのアニメーショントラックの設定
			void SetAnimationTruck(int handle, int truck);

			/// モデルのワールド座標を設定
			void SettingMatrix(int handle);

			/// 確保しているモデルを消す
			void Remove(int handle);
 
			/// 確保しているモデルを全て消す
			void AllRemove();
	};
}

#endif
