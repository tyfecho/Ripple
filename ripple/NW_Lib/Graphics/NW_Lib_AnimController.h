/*-------------------------------------------------------------------------

	クラス名　：　アニメーションコントローラークラス
	作成者　　：　皆光　健太
	最終更新日：　2012 / 12 / 17

-------------------------------------------------------------------------*/
#pragma once

#include <d3dx9.h>
#include <d3d9.h>

namespace NW_Lib
{
	
	class SkinAnimationModel;

	static const int MOTION_MAX = 20;	//モーション設定の最大数

typedef struct 
{
	LPD3DXANIMATIONSET	pAnimSet;			// アニメーションセット
	unsigned int		id;					// アニメーションID
	unsigned int		loopNum;			// アニメーションのループ回数
	float				loopTime;			// 1ループの時間
	float				trackSpeed;			// トラックスピード調節値
	float				shiftTime;			// シフトするのにかかる時間
	float				currentWeightTime;	// 現在のウェイト時間
} ANIMATION_DESC;

class AnimController
{

	public:

		//アニメーションのループタイプ
		typedef enum 
		{
			NORMAL = 0,
			INVERSE,
		}LOOP_TYPE;

	private:

		LPD3DXANIMATIONCONTROLLER m_pAnimController;	//アニメーションコントローラ
		//LPD3DXANIMATIONSET	m_pAnimSet;					// アニメーションセット

		ANIMATION_DESC m_animeDesc[MOTION_MAX];			//上位トラック情報
		unsigned int m_currentId;						//現在のID
		unsigned int m_prevId;							//前回のID

		bool m_animeFrag;								//アニメーションするか

		LOOP_TYPE m_loopType;							//アニメーションのループ情報
		float m_currentAnimTime;						//現在のアニメーション時間
		
	public:

		/*=======================================================
			関数名	：	AnimController
			説明	：	コンストラクタ
			引数	：	なし
			戻り値	：　なし
		========================================================*/
		AnimController();

		/*=======================================================
			関数名	：	~AnimController
			説明	：	デストラクタ
			引数	：	なし
			戻り値	：　なし
		========================================================*/
		~AnimController();

		/*=======================================================
			関数名	：	GetAnimationController
			説明	：	アニメーション情報の取得
			引数	：	なし
			戻り値	：　アニメーション情報
		========================================================*/
		LPD3DXANIMATIONCONTROLLER GetAnimationController() { return m_pAnimController; }

		/*=======================================================
			関数名	：	CreateAnimController
			説明	：	新たにアニメーションコントローラを作成する
			引数	：	スキンモデル情報
			戻り値	：　なし
		========================================================*/
		void CreateAnimController(NW_Lib::SkinAnimationModel* obj);

		/*=======================================================
			関数名	：	SetLoopType
			説明	：	ループ情報を設定
			引数	：	アニメーション情報
			戻り値	：　なし
		========================================================*/
		void SetLoopType( LOOP_TYPE loopType){ m_loopType = loopType;}

		/*=======================================================
			関数名	：	GetLoopType
			説明	：	ループ情報を設定
			引数	：	なし
			戻り値	：　アニメーション情報 
		========================================================*/
		LOOP_TYPE GetLoopType(){ return m_loopType;}

		/*=======================================================
			関数名	：	SetLoopTime
			説明	：	ループ時間を設定
			引数	：	アニメーションID
						時間（秒）
			戻り値	：　なし
		========================================================*/
		void SetLoopTime( unsigned int animId, float time );

		/*=======================================================
			関数名	：	SetShiftTime
			説明	：	動作開始にかかる時間を設定
			引数	：	アニメーションID
						時間（秒）
			戻り値	：　なし
		========================================================*/
		void SetShiftTime( unsigned int animId, float interval );

		/*=======================================================
			関数名	：	ChangeAnimation
			説明	：	アニメーションを切り替え
			引数	：	アニメーションID
						（ループ情報）
						（ループ回数）
			戻り値	：　なし
		========================================================*/
		void ChangeAnimation( unsigned int animId , LOOP_TYPE loopType = NORMAL, unsigned int loopNum = 0 );

		/*=======================================================
			関数名	：	SetAnimationPos
			説明	：	アニメーション位置を設定
			引数	：	アニメーションID
						アニメーション位置
			戻り値	：　なし
		========================================================*/
		void SetAnimationPos( unsigned int animId , float time );

		/*=======================================================
			関数名	：	StopAnimation
			説明	：	アニメーション始める
			引数	：	なし
			戻り値	：　なし
		========================================================*/
		void StartAnimation() { m_animeFrag = true;}

		/*=======================================================
			関数名	：	StopAnimation
			説明	：	アニメーションをとめる
			引数	：	なし
			戻り値	：　なし
		========================================================*/
		void StopAnimation() { m_animeFrag = false;}
	
		/*=======================================================
			関数名	：	AdvanceTime
			説明	：	更新
			引数	：	なし
			戻り値	：　なし
		========================================================*/
		void AdvanceTime( float time );

		/*=======================================================
			関数名	：	GetRemainingTime
			説明	：	ループ残り時間の取得
						(ループ回数を設定していない場合は進行時間の取得)
			引数	：	なし
			戻り値	：　残り時間（秒）
		========================================================*/
		double GetRemainingTime() { return m_animeDesc[m_currentId].loopTime * m_animeDesc[m_currentId].loopNum - m_currentAnimTime; }
		
		/*=======================================================
			関数名	：	DrawSetUp
			説明	：	描画準備
			引数	：	なし
			戻り値	：　なし
		========================================================*/
		void DrawSetUp();

};

}