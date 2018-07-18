/**
*	@file	HardwareDefine.h
*
*	@brief	�n�[�h�E�F�A�̃V�X�e����`
*
*	@date	2013/08/20
*
*	@author	�F�� ����
*/

#pragma once
	
///  �X�N���[���T�C�Y ����
#define UPPER_SCREEN_WIDTH	400
#define UPPER_SCREEN_HEIGHT	240
///  �X�N���[���T�C�Y �����
#define LOWER_SCREEN_WIDTH	320
#define LOWER_SCREEN_HEIGHT 240
#define LOWER_SCREEN_OFFSET_WIDTH ((UPPER_SCREEN_WIDTH - LOWER_SCREEN_WIDTH) / 2.0f)
#define LOWER_SCREEN_OFFSET_HEIGHT (UPPER_SCREEN_HEIGHT)

// ��ʔԍ�
enum SCREEN_NUMBER
{
	SCREEN_UNKNOWN = -1, // ��ʖ���
	SCREEN_UPPER = 0   , // ����
	SCREEN_LOWER       , // �����
	
	SCREEN_NUM
};



