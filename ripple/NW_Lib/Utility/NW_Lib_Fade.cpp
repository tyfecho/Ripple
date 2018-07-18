///***
//*	@file	Fade.cpp
//*
//*	@brief	フェードクラス
//*
//*	@date	2012/08/01
//*
//*	@author	吉田　将也
//*/
//
//#include "Fade.h"
//
//#include "texture.h"
//
//#include "../Game/defs.h"
//
//
//namespace NW_Lib
//{
//	/**
//	*	コンストラクタ
//	*/
//	Fade::Fade()
//	:m_pTexture(NULL)
//	,fade_cnt(0)
//	{
//		m_pTexture = new Texture("Fade.bmp");
//	}
//
//	/**
//	*	デストラクタ
//	*/
//	Fade::~Fade()
//	{
//		SafeDelete(m_pTexture);
//	}
//
//	/**
//	*	初期化処理
//	*/
//	void Fade::Initialize()
//	{
//		fade_cnt = 0;
//	}
//
//	/**
//	*	普通のフェードイン
//	*	@param speed  フェードのスピード
//	*	@param black  trueで黒falseで白いフェードになる
//	*	戻り値：フェードが終わったらtrueを返す
//	*/
//	bool Fade::FadeOut(float speed,bool black)
//	{
//		float x;
//		if(black){
//			x = 0.0f;
//		}else{
//			x = 1.0f;
//		}
//		float alpha = (fade_cnt * speed) / 100.0f;
//		if(alpha > 1.0f){
//			alpha = 1.0f;
//		}
//
//		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
//		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
//
//		m_pSprite->DrawAll(m_pTexture,320.0f,240.0f,640.0f,480.0f,x,0.0f,1.0f,1.0f,true,0.0f,alpha);
//		
//		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
//		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//	
//		fade_cnt++;
//		if(alpha >= 1.0f){
//			fade_cnt = 0;
//			return true;
//		}else{
//			return false;
//		}
//	}
//	/**
//	*	普通のフェードアウト
//	*	@param speed  フェードのスピード
//	*	@param black  trueで黒falseで白いフェードになる
//	*	戻り値：フェードが終わったらtrueを返す
//	*/
//	bool Fade::FadeIn(float speed,bool black)
//	{
//		float x;
//		if(black){
//			x = 0.0f;
//		}else{
//			x = 1.0f;
//		}
//		float alpha = (fade_cnt * speed) / 100.0f;
//		if(alpha > 1.0f){
//			alpha = 1.0f;
//		}
//		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
//		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
//
//		m_pSprite->DrawAll(m_pTexture,320.0f,240.0f,640.0f,480.0f,x,0.0f,1.0f,1.0f,true,0.0f,1-alpha);
//
//		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
//		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//
//		fade_cnt++;
//		if(alpha >= 1.0f){
//			fade_cnt = 0;
//			return true;
//		}else{
//			return false;
//		}
//	}
//
//	/**
//		普通のフェード
//		インとアウトを両方をまとめてやる
//	*	@param speed  フェードのスピード
//	*	@param black  trueで黒falseで白いフェードになる
//	*	戻り値：フェードが終わったらtrueを返す
//	*/
//	bool Fade::FadeInOut(float speed,bool black)
//	{
//		static bool In = false,Out = false;
//
//		if(!In){
//			// フェードイン
//			In = FadeIn(speed,black);
//		}else{
//			// フェードアウト
//			Out = FadeIn(speed,black);
//		}
//
//
//
//		if(In && Out){
//			// フラグの初期化
//			In = false;
//			Out = false;
//			// フェード終了
//			return true;
//		}
//
//		// フェード中
//		return false;
//	}
//
//	/**
//	*	塗りつぶす
//	*	@param black  trueで黒falseで白いフェードになる
//	*/
//	void Fade::AllRender(bool black)
//	{
//		float x;
//		if(black){
//			x = 0.0f;
//		}else{
//			x = 1.0f;
//		}
//		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
//		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
//
//		m_pSprite->DrawAll(m_pTexture,320.0f,240.0f,640.0f,480.0f,x,0.0f,1.0f,1.0f,true,0.0f);
//
//		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
//		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//
//	}
//}
