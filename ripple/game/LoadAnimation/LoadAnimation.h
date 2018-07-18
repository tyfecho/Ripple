/*-------------------------------------------------------------------------

	クラス名　：　ロードアニメーション管理クラス
	作成者　　：　皆光　健太
	最終更新日：　2012 / 03 / 15

-------------------------------------------------------------------------*/
#pragma once

#include "../../NW_Lib/Graphics/NW_Lib_texture.h"
#include "../../NW_Lib/Utility/NW_Lib_Constant.h"

// ロードモード
typedef enum
{
	LOADUNKNOWN = -1,
	LOADOUT		,
	LOADIN		,
	LOADEND		,
}LOAD;

namespace Game
{

	class TEXTURE;

class LoadAnimation
{

	private:

		int m_loadMaxTimer;		//ロードアニメーション時間
		float m_oneFlameTime;	//一フレームの時間

		LOAD m_loadMode;		// ロードアニメーションイン　ロードアニメーションアウト
		LOAD m_beforeMode;		//前回のロードアニメーションモード
		int m_animeTime;		//アニメーションタイマー

		struct Carp
		{
			int type;	//鯉のタイプ
			int anime;	//鯉のアニメーションカウント(loop 0～32)
			float size; //鯉のサイズ
			VECTOR2 pos;//鯉のポジション（上下合同）
		};
		static const int CarpNum = 15;
		
		Carp m_carpUpper[CarpNum];
		Carp m_carpLower[CarpNum];
		
		NW_Lib::Texture* m_loadTex;				//ロードアニメーション用テクスチャ
		NW_Lib::Texture* m_loadTex2;			//ロードアニメーション用テクスチャ
		NW_Lib::Texture* m_loadTexCarp[3];		//ロードアニメーション用テクスチャ

		bool m_enable;

		float m_texPosX;

	public:

		/*=======================================================
			関数名	：	LoadAnimation
			説明	：	コンストラクタ
			引数	：	なし
			戻り値	：　なし
		========================================================*/
		LoadAnimation();

		/*=======================================================
			関数名	：	~LoadAnimation
			説明	：	デストラクタ
			引数	：	なし
			戻り値	：　なし
		========================================================*/
		~LoadAnimation();

		/*=======================================================
			関数名	：	Initialize
			説明	：	初期化処理
			引数	：	なし
			戻り値	：　なし
		========================================================*/
		void Initialize();

		/*=======================================================
			関数名	：	isLoadAnimationEnd
			説明	：	ロードアニメーションの終わりか
			引数	：	なし
			戻り値	：　結果
		========================================================*/
		bool isLoadAnimationEnd();

		/*=======================================================
			関数名	：	isLoadAnimationNow
			説明	：	ロードアニメーション中か
			引数	：	なし
			戻り値	：　結果
		========================================================*/
		bool isLoadAnimationNow();

		/*=======================================================
			関数名	：	LoadAnimationModeChanger
			説明	：	ロードアニメーション状態を変える
			引数	：	変更する状態
			戻り値	：　なし
		========================================================*/
		void LoadAnimationModeChanger(LOAD next_load );

		/*=======================================================
			関数名	：	LoadAnimationDraw
			説明	：	描画関数
			引数	：	スクリーン
			戻り値	：　なし
		========================================================*/
		void LoadAnimationDraw( NW_Lib::DrawingScreen screenNum );
		
		/*=======================================================
			関数名	：	isLoadAnimationNow
			説明	：	ロードアニメーション中か
			引数	：	なし
			戻り値	：　ロードアニメーション状態
		========================================================*/
		LOAD GetLoadAnimationMode() { return m_loadMode; }

		/*=======================================================
			関数名	：	SetEnable
			説明	：	存在の設定
			引数	：	フラグ
			戻り値	：　なし
		========================================================*/
		void SetEnable(bool flag) { m_enable = flag; }

		/*=======================================================
			関数名	：	SetBeforeLoadAnimationMode
			説明	：	前回のロードアニメーション状態を変える
			引数	：	変更する状態
			戻り値	：　なし
		========================================================*/
		void SetBeforeLoadAnimationMode(LOAD loadMode ){ m_beforeMode = loadMode; };

		/*=======================================================
			関数名	：	SetLoadAnimationTime
			説明	：	ロードアニメーションにかかる時間
			引数	：	ロードアニメーションにかかる時間
			戻り値	：　なし
		========================================================*/
		void SetLoadAnimationTime(int num = 30 );
};

}