/**
*	@file	Constant.h
*
*	@brief	�R�c�r�}���`�v���b�g�t�H�[���p���L��`
*
*	@date	2013/08/27
*
*	@author	�F���@����
*/

#pragma once
#include <d3dx9.h>

//#define _3DS
#define _DirectX

namespace NW_Lib
{
	class Texture;
	class Camera;

	typedef enum DrawingScreen
	{
		UPPER_SCREEN_DRAWING = 0,
		LOWER_SCREEN_DRAWING,
		NUM_SCREEN_DRAWING,
	};
}

//�{�^���̔z�u
enum Button_Key{
	//BUTTON_A         = 3,               /* @brief A�{�^�� */
	//BUTTON_B         = 2,               /* @brief B�{�^�� */
	//BUTTON_X         = 4,               /* @brief X�{�^�� */
	//BUTTON_Y         = 1,               /* @brief Y�{�^�� */
	BUTTON_A         = 1,               /* @brief A�{�^�� */
	BUTTON_B         = 2,               /* @brief B�{�^�� */
	BUTTON_X         = 3,               /* @brief X�{�^�� */
	BUTTON_Y         = 4,               /* @brief Y�{�^�� */

	BUTTON_R         = 6,               /* @brief R�{�^�� */
	BUTTON_L         = 5,               /* @brief L�{�^�� */

	BUTTON_UP        = VK_UP,              /* @brief �\�����{�^�� */
	BUTTON_DOWN      = VK_DOWN,            /* @brief �\�����{�^�� */
	BUTTON_RIGHT     = VK_RIGHT,           /* @brief �\�����{�^�� */
	BUTTON_LEFT      = VK_LEFT,            /* @brief �\�����{�^�� */

	//BUTTON_PAD_UP    =dnwdwadadwadwa /* @brief �X���C�h�p�b�h���{�^�� */
	//BUTTON_PAD_DOWN  =dnwdwadadwadwa /* @brief �X���C�h�p�b�h���{�^�� */
	//BUTTON_PAD_RIGHT =dnwdwadadwadwa /* @brief �X���C�h�p�b�h���{�^�� */
	//BUTTON_PAD_LEFT  =dnwdwadadwadwa /* @brief �X���C�h�p�b�h���{�^�� */
	
	//BUTTON_START     = 10,           /* @brief START�{�^�� */
	//BUTTON_SELECT    = 9			/* @brief SELECT�{�^�� */
	BUTTON_START     = 8,           /* @brief START�{�^�� */
	BUTTON_SELECT    = 7			/* @brief SELECT�{�^�� */
};

//�e�N�X�`�����ʒ�`
typedef NW_Lib::Texture TEXTURE;

//�J�������ʒ�`
typedef NW_Lib::Camera CAMERA;

//�x�N�g���N���X���ʒ�`
typedef D3DXVECTOR2 VECTOR2;
typedef D3DXVECTOR3 VECTOR3;

//�s��N���X���ʒ�`
typedef D3DXMATRIX MATRIX;
typedef D3DXMATRIX* MATRIX33;

static unsigned long Random()
{ 
    static unsigned long x=123456789,y=362436069,z=521288629,w=88675123; 
    unsigned long t; 
    t=(x^(x<<11));x=y;y=z;z=w; return( w=(w^(w>>19))^(t^(t>>8)) ); 
} 

