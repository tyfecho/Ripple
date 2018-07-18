#pragma once

#include <atlbase.h>
#include <d3dx9.h>

#include "../System/NW_Lib_Common.h"


//  テクスチャ枚数：1
//  エントリー可能数：1000
namespace NW_Lib
{
		// 描画用の頂点フォーマット
		struct VERTEX
		{
			// 3D座標
			float x,y,z;
			// 色情報
			unsigned int color;
			// テクスチャのUV座標
			float u,v;
		};

		// エントリー情報
		struct Info
		{
			// 現在の座標
			float x,y,z;
			// 速度
			float vx,vy,vz;
			// 加速度
			float ax,ay,az;

			// 現在の角度
			float angle;
			// 開始角度
			float s_angle;
			// 終了角度
			float e_angle;

			// 現在のスケール
			float scaleX,scaleY;
			// 開始スケール
			float s_scaleX,s_scaleY;
			// 終了スケール
			float e_scaleX,e_scaleY;

			// 現在の色
			float r,g,b,a;
			// 開始の色
			float s_r,s_g,s_b,s_a;
			// 終了の色
			float e_r,e_g,e_b,e_a;

			// 現在のフレームカウント
			int   frame;
			// 終了のフレームカウント
			int   num_frame;
		
			// 次のデータポインタ
			Info* next;
			// 前のデータポインタ
			Info* prev;

			// 親行列
			D3DXMATRIX* pParentMat;
		};


	class ParticleManager: public Common
	{
		private:
			// テクスチャ情報
			CComPtr<IDirect3DTexture9> texture;
			// 重力加速度
			float gx,gy,gz;

			// ビルボード変換行列
			D3DXMATRIX matBillbord;

			// エントリーの限界数
			static const int tableNum=1000;
			// エントリー情報の配列
			Info infoTable[tableNum];

			// フリーなエントリー情報
			Info freeList;
			// アクティブなエントリー情報
			Info activeList;
		public:

			~ParticleManager();
			// テクスチャの読み込み
			bool LoadTexture(LPCTSTR filename);
	
			// ビルボードの情報を更新する（デバイスに設定されたカメラ情報(ビュー行列)を使う）
			void SetBillbord();

			// エントリー情報の更新
			bool Update(Info *info);

			// １つのエントリ情報を描画する
			void Draw(Info* info);

			// 初期化
			void Initialize();

			// アクティブリストにエントリー情報を追加する
			Info* AddInfo();

			// パーティクルのエントリー
			void Entry(int frame,	// フレーム数
				float x,   float y,   float z,	// 座標
				float vx,  float vy,  float vz,	// 速度
				float ax, float ay, float az,	// 加速度
				float start_angle, // 開始角度
				float end_angle,   // 終了角度
				float start_scaleX,float start_scaleY, // 拡縮・開始倍率
				float end_scaleX,float end_scaleY,     // 拡縮・終了倍率
				float start_r,float start_g,float start_b,float start_a, // 開始カラー
				float end_r,  float end_g,  float end_b,  float end_a,    // 終了カラー
				D3DXMATRIX* ParentMat = NULL
				);

			// 指定のエントリー情報をアクティブリストから、フリーリストへ移す
			Info* Free(Info *info);

			// すべてのアクティブなエントリー情報を更新する
			void Update();

			// アクティブなエントリー情報をすべて描画する
			void Draw();

			void Erase();

			// 重力加速度の設定
			void SetGravity(float _gx,float _gy,float _gz)
			{
				gx = _gx;
				gy = _gy;
				gz = _gz;
			}
	};
}
