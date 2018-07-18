/**
*	@file	sprite.cpp
*
*	@brief	�QD�X�v���C�g�p�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#include "NW_Lib_sprite.h"
#include "NW_Lib_Texture.h"

#include "../Utility/NW_Lib_HardwareDefine.h"

#include "../System/NW_Lib_SceneManager.h"

namespace NW_Lib
{
	/**
	*	�R���X�g���N�^
	*	@param pD3DDevice  �f�o�C�X
	*/
	Sprite::Sprite(LPDIRECT3DDEVICE9 pD3DDevice)
	:m_width(1.0f)      // ��
	,m_height(1.0f)    // ����
	,m_texture(NULL)     // �e�N�X�`��
	,m_uLeftUp(0.0f)     // ����t���W
	,m_vLeftUp(0.0f)     // ����u���W
	,m_uRightDown(0.0f)  // �E���t���W
	,m_vRightDown(0.0f)  // �E���u���W
	,m_pD3DDevice(pD3DDevice)
	,m_Alpha(1.0f)
	,m_Color(0x00ffffff)
	{
	}

	/**
	*	�f�X�g���N�^
	*/
	Sprite::~Sprite()
	{
	}

	/**
	*	�e�N�X�`���̐ݒ�
	*	@param texture  �e�N�X�`�����
	*/
	void Sprite::SetTexture(const Texture *texture)
	{
		m_texture = texture;

		m_uLeftUp  = 0.0f; // ����ݒ�
		m_vLeftUp  = 0.0f; // ����ݒ�
		m_uRightDown = 1.0f; // �E���ݒ�
		m_vRightDown = 1.0f; // �E���ݒ�
	}

	/**
	*  �e�N�X�`���̐ݒ� �i�w��o�[�W�����j
	*	@param texture        �e�N�X�`�����
	*	@param textureX       �e�N�X�`����X�ʒu
	*	@param textureY       �e�N�X�`����Y�ʒu
	*	@param textureWidth   �e�N�X�`���̕�
	*	@param textureHeight  �e�N�X�`���̍���
	*/
	void Sprite::SetTexture(const Texture *texture, float textureX, float textureY, float textureWidth, float textureHeight)
	{
		m_texture = texture;

		m_uLeftUp  = static_cast<float>(textureX) / m_texture->GetWidth(); // ����ݒ�
		m_vLeftUp  = static_cast<float>(textureY) / m_texture->GetHeight(); // ����ݒ�
		m_uRightDown = static_cast<float>(textureX + textureWidth) / m_texture->GetWidth(); // �E���ݒ�
		m_vRightDown = static_cast<float>(textureY + textureHeight) / m_texture->GetHeight(); // �E���ݒ�
	}

	/**
	*	�e�N�X�`���̃T�C�Y�ύX
	*	@param width  ��
	*	@param height ����
	*/
	void Sprite::ChangeSize(float width, float height)
	{
		m_width = width;
		m_height = height;
	}

	/**
	*	�`��
	*	@param x      X�ʒu
	*	@param y      Y�ʒu
	*	@param angle  �p�x
	*	@param alfa   �����x
	*	@param center true�Œ��S���w��Afalse�ō�����w�肵�ĕ\��
	*/
	void Sprite::DrawSprite( float x, float y,float angle,float alfa,bool center)
	{
		float x_Left,x_Right,y_Up,y_Down;

		if(center){
			// �^�񒆊�_�ɍ��킹��
			x_Left  = x - m_width/2;
			x_Right = x + m_width/2;
			y_Up    = y - m_height/2;
			y_Down  = y + m_height/2;
		}else{
			// �����_�ɍ��킹��
			x_Left  = x;
			x_Right = x + m_width;
			y_Up    = y;
			y_Down  = y + m_height;
		}

		// �A���t�@�̌v�Z
		DWORD alp = static_cast<DWORD>(0xff * alfa) << ( 8 * 3 );

		/// ���̒��_�f�[�^
		VERTEX_TEXTURE2D vertices[4] = 
		{
			{D3DXVECTOR3(x_Left , y_Up ,0.0f), 1.0f, m_Color + alp, m_uLeftUp   , m_vLeftUp   },
			{D3DXVECTOR3(x_Right, y_Up ,0.0f), 1.0f, m_Color + alp, m_uRightDown, m_vLeftUp   },
			{D3DXVECTOR3(x_Left ,y_Down,0.0f), 1.0f, m_Color + alp, m_uLeftUp   , m_vRightDown},
			{D3DXVECTOR3(x_Right,y_Down,0.0f), 1.0f, m_Color + alp, m_uRightDown, m_vRightDown}
		};
		// ���S�ʒu�i�w�j�@���@ ���̔���  + �w�ʒu
		float coreX = m_width/2 + x_Left;
		// ���S�ʒu�i�x�j�@���@�����̔��� + �x�ʒu
		float coreY = m_height/2 + y_Up;
		float rad = (angle * D3DX_PI / 180.0f);

		// ��]�̒���
		for(int i = 0;i < 4;++i){

			int flgx[4] = {-1,1,-1,1};
			int flgy[4] = {1,1,-1,-1};

			vertices[i].position.x = coreX + (m_width/2*flgx[i] * cos(rad) - (m_height/2 * sin(rad) * flgy[i]));
			vertices[i].position.y = coreY - (m_width/2*flgx[i] * sin(rad) + (m_height/2 * cos(rad) * flgy[i]));
		}


		// �y�o�b�t�@�ɏ������܂Ȃ�
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// �y�o�b�t�@�Ɣ�r���Ȃ�
		m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

		//m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);

		// ������������
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


		// �e�N�X�`���X�e�[�W�̐ݒ�
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		// �e�N�X�`���̐ݒ�
		if(m_texture != NULL){
			m_pD3DDevice->SetTexture(0,m_texture->GetTexture());
		}else{
			m_pD3DDevice->SetTexture(0,NULL);
		}

		// ���_�t�H�[�}�b�g�̐ݒ�
		m_pD3DDevice->SetFVF(D3DFVF_TEXTURE2D);

		// �`��
		m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VERTEX_TEXTURE2D));

		m_pD3DDevice->SetTexture(0,NULL);

		// Z�o�b�t�@���I���ɂ���
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
		// �y�o�b�t�@�Ɣ�r����
		m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		// �y�o�b�t�@�ɏ�������
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	}

	/**
	*	�`��̂܂Ƃ�
	*	@param texture       �e�N�X�`�����
	*	@param x             X�ʒu
	*	@param y             Y�ʒu
	*	@param texturex      �e�N�X�`����X
	*	@param texturey      �e�N�X�`����Y
	*	@param textureWidth  ��
	*	@param textureHeight ����
	*	@param center true�Œ��S���w��Afalse�ō�����w�肵�ĕ\��
	*	@param angle         �p�x
	*	@param alfa          �����x
	*/
	void Sprite::DrawAll(const NW_Lib::Texture *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,bool center,float angle,float alpha)
	{
		ChangeSize(width,height);
		SetTexture(texture,textureX,textureY,textureWidth,textureHeight);
		DrawSprite(x,y,angle,alpha,center);
	}

	void Sprite::DrawUpper(const NW_Lib::Texture *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,bool center,float angle,float alpha)
	{
		ChangeSize(width,height);
		SetTexture(texture,textureX,textureY,textureWidth,textureHeight);
		DrawSprite(x,y,angle,alpha,center);
	}
	void Sprite::DrawLower(const NW_Lib::Texture *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight,bool center,float angle,float alpha)
	{
		ChangeSize(width,height);
		SetTexture(texture,textureX,textureY,textureWidth,textureHeight);
		DrawSprite(x + LOWER_SCREEN_OFFSET_WIDTH ,y + LOWER_SCREEN_OFFSET_HEIGHT,angle,alpha,center);
	}

	/**
	*	�`��̐ݒ�
	*	@param texture       �e�N�X�`�����
	*	@param x             X�ʒu
	*	@param y             Y�ʒu
	*	@param texturex      �e�N�X�`����X
	*	@param texturey      �e�N�X�`����Y
	*	@param textureWidth  ��
	*	@param textureHeight ����
	*	@param cx			 ���S�_X
	*	@param cy			 ���S�_Y
	*	@param angle         �p�x
	*	@param alfa          �����x
	*/	
	void Sprite::Draw(const NW_Lib::Texture *texture,float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight, float cx, float cy, float angle, float alpha)
	{

		float x_Left,x_Right,y_Up,y_Down;

		if( DrawingScreenID == NW_Lib::UPPER_SCREEN_DRAWING )
		{
			// �����_�ɍ��킹��
			x_Left  = x;
			x_Right = x + width;
			y_Up    = y;
			y_Down  = y + height;
		}
		else if( DrawingScreenID == NW_Lib::LOWER_SCREEN_DRAWING )
		{
			x_Left  = x + LOWER_SCREEN_OFFSET_WIDTH;
			x_Right = x + LOWER_SCREEN_OFFSET_WIDTH + width;
			y_Up    = y + LOWER_SCREEN_OFFSET_HEIGHT;
			y_Down  = y + LOWER_SCREEN_OFFSET_HEIGHT + height;
		}

		// �A���t�@�̌v�Z
		DWORD alp = static_cast<DWORD>(0xff * alpha) << ( 8 * 3 );

		float uLeftUp	   = static_cast<float>(textureX) / texture->GetWidth(); // ����ݒ�
		float vLeftUp	   = static_cast<float>(textureY) / texture->GetHeight(); // ����ݒ�
		float uRightDown = static_cast<float>(textureX + textureWidth) / texture->GetWidth(); // �E���ݒ�
		float vRightDown = static_cast<float>(textureY + textureHeight) / texture->GetHeight(); // �E���ݒ�

		/// ���̒��_�f�[�^
		VERTEX_TEXTURE2D vertices[4] = 
		{
			{D3DXVECTOR3(x_Left , y_Up ,0.0f), 1.0f, 0x00ffffff + alp, uLeftUp   , vLeftUp   },
			{D3DXVECTOR3(x_Right, y_Up ,0.0f), 1.0f, 0x00ffffff + alp, uRightDown, vLeftUp   },
			{D3DXVECTOR3(x_Left ,y_Down,0.0f), 1.0f, 0x00ffffff + alp, uLeftUp   , vRightDown},
			{D3DXVECTOR3(x_Right,y_Down,0.0f), 1.0f, 0x00ffffff + alp, uRightDown, vRightDown}
		};
		// ���S�ʒu�i�w�j�@���@ ���̔���  + �w�ʒu
		float coreX = width/2 + x_Left - cx;
		// ���S�ʒu�i�x�j�@���@�����̔��� + �x�ʒu
		float coreY = height/2 + y_Up - cy;
		float rad = (angle * D3DX_PI / 180.0f);

		// ��]�̒���
		for(int i = 0;i < 4;++i){

			int flgx[4] = {-1,1,-1,1};
			int flgy[4] = {1,1,-1,-1};

			vertices[i].position.x = coreX + (width/2*flgx[i] * cos(rad) - (height/2 * sin(rad) * flgy[i]));
			vertices[i].position.y = coreY - (width/2*flgx[i] * sin(rad) + (height/2 * cos(rad) * flgy[i]));
		}
		
		LPDIRECT3DDEVICE9 pD3DDevice = NW_Lib::System::SceneManager::GetInstance().GetDevice();

		// �y�o�b�t�@�ɏ������܂Ȃ�
		pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// �y�o�b�t�@�Ɣ�r���Ȃ�
		pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

		//pD3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);

		// ������������
		pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


		// �e�N�X�`���X�e�[�W�̐ݒ�
		pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		// �e�N�X�`���̐ݒ�
		if(texture != NULL){
			pD3DDevice->SetTexture(0,texture->GetTexture());
		}else{
			pD3DDevice->SetTexture(0,NULL);
		}

		// ���_�t�H�[�}�b�g�̐ݒ�
		pD3DDevice->SetFVF(D3DFVF_TEXTURE2D);

		// �`��
		pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VERTEX_TEXTURE2D));

		pD3DDevice->SetTexture(0,NULL);

		// Z�o�b�t�@���I���ɂ���
		pD3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
		// �y�o�b�t�@�Ɣ�r����
		pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		// �y�o�b�t�@�ɏ�������
		pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		
	}

}



