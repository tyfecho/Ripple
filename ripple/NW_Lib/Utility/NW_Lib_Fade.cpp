///***
//*	@file	Fade.cpp
//*
//*	@brief	�t�F�[�h�N���X
//*
//*	@date	2012/08/01
//*
//*	@author	�g�c�@����
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
//	*	�R���X�g���N�^
//	*/
//	Fade::Fade()
//	:m_pTexture(NULL)
//	,fade_cnt(0)
//	{
//		m_pTexture = new Texture("Fade.bmp");
//	}
//
//	/**
//	*	�f�X�g���N�^
//	*/
//	Fade::~Fade()
//	{
//		SafeDelete(m_pTexture);
//	}
//
//	/**
//	*	����������
//	*/
//	void Fade::Initialize()
//	{
//		fade_cnt = 0;
//	}
//
//	/**
//	*	���ʂ̃t�F�[�h�C��
//	*	@param speed  �t�F�[�h�̃X�s�[�h
//	*	@param black  true�ō�false�Ŕ����t�F�[�h�ɂȂ�
//	*	�߂�l�F�t�F�[�h���I�������true��Ԃ�
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
//	*	���ʂ̃t�F�[�h�A�E�g
//	*	@param speed  �t�F�[�h�̃X�s�[�h
//	*	@param black  true�ō�false�Ŕ����t�F�[�h�ɂȂ�
//	*	�߂�l�F�t�F�[�h���I�������true��Ԃ�
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
//		���ʂ̃t�F�[�h
//		�C���ƃA�E�g�𗼕����܂Ƃ߂Ă��
//	*	@param speed  �t�F�[�h�̃X�s�[�h
//	*	@param black  true�ō�false�Ŕ����t�F�[�h�ɂȂ�
//	*	�߂�l�F�t�F�[�h���I�������true��Ԃ�
//	*/
//	bool Fade::FadeInOut(float speed,bool black)
//	{
//		static bool In = false,Out = false;
//
//		if(!In){
//			// �t�F�[�h�C��
//			In = FadeIn(speed,black);
//		}else{
//			// �t�F�[�h�A�E�g
//			Out = FadeIn(speed,black);
//		}
//
//
//
//		if(In && Out){
//			// �t���O�̏�����
//			In = false;
//			Out = false;
//			// �t�F�[�h�I��
//			return true;
//		}
//
//		// �t�F�[�h��
//		return false;
//	}
//
//	/**
//	*	�h��Ԃ�
//	*	@param black  true�ō�false�Ŕ����t�F�[�h�ɂȂ�
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
