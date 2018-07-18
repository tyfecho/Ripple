/*-------------------------------------------------------------------------

	クラス名　：　アニメーション管理クラス
	作成者　　：　皆光　健太
	最終更新日：　2012 / 03 / 15

-------------------------------------------------------------------------*/
#include "NW_Lib_AnimController.h"

#include "NW_Lib_SkinAnimationModel.h"

namespace NW_Lib
{

/*=======================================================
	関数名	：	AnimController
	説明	：	コンストラクタ
	引数	：	なし
	戻り値	：　なし
========================================================*/
AnimController::AnimController()
{
	m_currentId = 0;
	m_prevId = 0;
	m_animeFrag = true;
}

/*=======================================================
	関数名	：	~AnimController
	説明	：	デストラクタ
	引数	：	なし
	戻り値	：　なし
========================================================*/
AnimController::~AnimController()
{
	m_pAnimController->Release();
}

/*=======================================================
	関数名	：	CreateAnimController
	説明	：	新たにアニメーションコントローラを作成する
	引数	：	スキンモデル情報
	戻り値	：　なし
========================================================*/
void AnimController::CreateAnimController(NW_Lib::SkinAnimationModel* obj)
{
	m_pAnimController = NULL;
	if( obj->GetAnimationController() )
	{
		obj->CloneAnimationController( &m_pAnimController );

		if( m_pAnimController )
		{
			//アニメーショントラックを得る
			int num = 0;
			for(DWORD i = 0 ; i < m_pAnimController->GetNumAnimationSets() ; i++ )
			{
				num = ( m_pAnimController->GetNumAnimationSets() - 1 ) - i;

				m_pAnimController->GetAnimationSet(i,&m_animeDesc[num].pAnimSet);
				m_animeDesc[num].id = num;
				m_animeDesc[num].loopTime = 1.0f;
				m_animeDesc[num].shiftTime = 0.1f;
				m_animeDesc[num].trackSpeed = 1.0f;
				m_animeDesc[num].currentWeightTime = 1.0f;
			}
		}
	}
}


/*=======================================================
	関数名	：	SetLoopTime
	説明	：	ループ時間を設定
	引数	：	アニメーションID
				時間（秒）
	戻り値	：　なし
========================================================*/
void AnimController::SetLoopTime( unsigned int animId, float time )
{
	//指定のアニメーションIDの存在するか
	if( m_pAnimController->GetNumAnimationSets() < animId )
	{
		return;
	}

	//トラックスピード調節値を計算
	float defTime = static_cast<float>(m_animeDesc[animId].pAnimSet->GetPeriod());
	m_animeDesc[animId].loopTime = time;
	m_animeDesc[animId].trackSpeed = defTime / time;

}

/*=======================================================
	関数名	：	SetShiftTime
	説明	：	動作開始にかかる時間を設定
	引数	：	アニメーションID
				時間（秒）
	戻り値	：　なし
========================================================*/
void AnimController::SetShiftTime( unsigned int animId, float interval )
{
	//指定のアニメーションIDの存在するか
	if( m_pAnimController->GetNumAnimationSets() < animId )
	{
		return;
	}
	// シフト時間を登録
	m_animeDesc[animId].shiftTime = interval;
}

/*=======================================================
	関数名	：	ChangeAnimation
	説明	：	アニメーションを切り替え
	引数	：	アニメーションID
				（ループ情報）
				（ループ回数）
	戻り値	：　なし
========================================================*/
void AnimController::ChangeAnimation( unsigned int animId , LOOP_TYPE loopType , unsigned int loopNum )
{
	//指定のアニメーションIDの存在するか
	if( m_pAnimController->GetNumAnimationSets() < animId )
	{
		return;
	}
	// 異なるアニメーションであるかをチェック
	if( m_currentId == animId )
	{
		return;
	}
	//ループ回数を設定
	m_animeDesc[animId].loopNum = loopNum;

	// 現在のアニメーションセットの設定値
	D3DXTRACK_DESC TD;		// トラック
	m_pAnimController->GetTrackDesc( 0, &TD );

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	m_pAnimController->SetTrackAnimationSet( 1, m_animeDesc[m_currentId].pAnimSet );
	m_pAnimController->SetTrackDesc( 1, &TD );

	// 新しいアニメーションセットをトラック0に設定
	m_pAnimController->SetTrackAnimationSet( 0, m_animeDesc[animId].pAnimSet );

	// トラックのスピードの設定
	m_pAnimController->SetTrackSpeed( 0, m_animeDesc[animId].trackSpeed );

	// トラックの合成を許可
	m_pAnimController->SetTrackEnable( 0, true );
	m_pAnimController->SetTrackEnable( 1, true );

	// ウェイト時間を初期化
	m_animeDesc[animId].currentWeightTime = 0.0f;

	// 現在のアニメーション番号を切り替え
	m_prevId = m_currentId;
	m_currentId = animId;

	// 時間をリセット
	m_pAnimController->ResetTime();
	m_pAnimController->AdvanceTime( 0 , NULL );
	m_currentAnimTime = 0;

	//ループ情報を設定
	m_loopType = loopType;
}

/*=======================================================
	関数名	：	SetAnimationPos
	説明	：	アニメーション位置を設定
	引数	：	アニメーションID
				アニメーション位置
	戻り値	：　なし
========================================================*/
void AnimController::SetAnimationPos( unsigned int animId , float time )
{

}

/*=======================================================
	関数名	：	AdvanceTime
	説明	：	更新
	引数	：	なし
	戻り値	：　なし
========================================================*/
void AnimController::AdvanceTime( float time )
{
	if(m_animeFrag)
	{
		// 合成中か否かを判定
		m_animeDesc[m_currentId].currentWeightTime += fabs(time);
		if( m_animeDesc[m_currentId].currentWeightTime <= m_animeDesc[m_currentId].shiftTime )
		{
			// 合成中。ウェイトを算出
			FLOAT weight = m_animeDesc[m_currentId].currentWeightTime / m_animeDesc[m_currentId].shiftTime;

			// ウェイトを登録
			m_pAnimController->SetTrackWeight( 0, weight );       // 現在のアニメーション
			m_pAnimController->SetTrackWeight( 1, 1 - weight );   // 前のアニメーション
		}
		else
		{
			// 合成終了中。通常アニメーションをするTrack0のウェイトを最大値に
			m_pAnimController->SetTrackWeight( 0, 1.0f );       // 現在のアニメーション
			m_pAnimController->SetTrackEnable( 1, false );      // 前のアニメーションを無効にする
		}

		// ループ回数が指定されており、
		// 指定ループ回数回った場合は、元のアニメーションに戻す
		double currentTime = m_pAnimController->GetTime();

		if( m_animeDesc[m_currentId].loopNum &&
			( currentTime >= m_animeDesc[m_currentId].loopTime * m_animeDesc[m_currentId].loopNum ))
		{
			ChangeAnimation(m_prevId);
		}

		//ループ情報によって切り替える
		if( m_loopType == NORMAL )
		{
			m_currentAnimTime += time;
			// 時間を更新
			m_pAnimController->AdvanceTime( time, NULL );
		}
		else if( m_loopType == INVERSE )
		{
			
			m_currentAnimTime += time;
			// 時間を更新
			m_pAnimController->SetTrackPosition( m_currentId , m_animeDesc[m_currentId].loopTime - m_currentAnimTime);
			m_pAnimController->AdvanceTime( 0, NULL );
		}
	}
}
		
/*=======================================================
	関数名	：	DrawSetUp
	説明	：	描画準備
	引数	：	なし
	戻り値	：　なし
========================================================*/
void AnimController::DrawSetUp()
{
	m_pAnimController->AdvanceTime( 0, NULL );
}

}