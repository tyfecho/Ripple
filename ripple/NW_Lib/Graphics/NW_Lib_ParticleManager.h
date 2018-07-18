/**
 * @file    NW_Lib_ParticleManager.h
 * @brief   パーティクルを操作するクラス
 * 
 * @author  Masaya Yoshida
 * @date    2013/09/19
 * @version 1.0
 */
#ifndef PARTICLE_MANAGER
#define PARTICLE_MANAGER

#include "../System/NW_Lib_ResourceManager.h"
#include "NW_Lib_Particle.h"

namespace NW_Lib
{
	namespace Graphics
	{
		class ParticleManager
		{
			private:

			private:
				/// コンストラクタ
				ParticleManager(){}
				ParticleManager(const ParticleManager& val){(void)val;}
				ParticleManager& operator=(const ParticleManager& val){(void)val;return *this;}

			public:
				/// シングルトン化させる
				static ParticleManager& GetInstance()
				{
					static ParticleManager instance;
					return instance;
				}

				void Initialize(){}


				/// ファイル読み込み
				void LoadFile(int loadHandle,char* filename){}
				/// パーティクルの生成
				void Entry(int handle,int loadHandle){}

				/// 位置の設定
				void SetPosision(int handle,VECTOR3 position){}
				/// 回転の設定
				void SetRotate(int handle,VECTOR3 rotation){}
				/// サイズの設定
				void SetScale(int handle,VECTOR3 scale){}

				/// 放出量の設定
				void SetEmission(int handle,float emission){}

			private:
				/// 更新
				void Update(DrawingScreen drawingScreen,int handle){}

			public:
				/// 描画
				void Draw(DrawingScreen drawingScreen,int handle){}

				void Remove(int handle){}

				void AllRemove(){}
		};
	}
}


#endif
