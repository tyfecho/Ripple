/**
 * @file    Touch.h
 * @brief   �^�b�`��S������փb�_�ł�
 *
 * @author  Kenta Minamitsu
 * @date    2013/09/18
 * @version 1.0
 */

#include "NW_Lib_Touch.h"

#include "../Utility/NW_Lib_HardwareDefine.h"

/**
 * @namespace NW_Lib
 * @brief ���C�u�������O���
 */
namespace NW_Lib
{
/**
 * @namespace NW_Lib::System
 * @brief �V�X�e�����O���
 */
namespace System
{
	/**
	* @brief   �R���X�g���N�^
	* 
	* @see     
	*/
	Touch::Touch()
		:m_touchPos(0.0f,0.0f)
		,m_touchFlag(false)
		,m_prevTouchFlag(false)
	{
	}
	
	/**
	* @brief   �f�X�g���N�^
	* 
	* @see     
	*/
	Touch::~Touch()
	{
	}

	/**
		@brief ���������s���܂�
	*/
	void Touch::Initialize()
	{
	}

	/**
		@brief �p�b�h���͂��A�b�v�f�[�g���܂�
	*/
	void Touch::Update()
	{
		if(m_pInput->PushKey(VK_LBUTTON))
		{
			m_touchPos.x = static_cast<float>(m_pInput->GetMousePos().x);
			m_touchPos.y = static_cast<float>(m_pInput->GetMousePos().y);
			
			if( m_touchPos.x < LOWER_SCREEN_OFFSET_WIDTH )
			{
				m_touchPos.x = LOWER_SCREEN_OFFSET_WIDTH;
			}
			if( m_touchPos.x > LOWER_SCREEN_WIDTH + LOWER_SCREEN_OFFSET_WIDTH )
			{
				m_touchPos.x = LOWER_SCREEN_WIDTH + LOWER_SCREEN_OFFSET_WIDTH;
			}
			if( m_touchPos.y < UPPER_SCREEN_HEIGHT )
			{
				m_touchPos.y = UPPER_SCREEN_HEIGHT;
			}
			if( m_touchPos.y > UPPER_SCREEN_HEIGHT + LOWER_SCREEN_HEIGHT )
			{
				m_touchPos.y = UPPER_SCREEN_HEIGHT + LOWER_SCREEN_HEIGHT;
			}
		}
		
	 	 m_prevTouchFlag = m_touchFlag;
	 	 m_touchFlag = m_pInput->PushKey(VK_LBUTTON);
	}

	/**
		@brief ���������J�����܂�
	*/
	void Touch::Finalize()
	{

	}
	
	///��ʂɃ^�b�`����Ă��邩�m�F
	bool Touch::IsTouch() const
	{
		return m_touchFlag;
	}

	///��ʂɈ��̂݃^�b�`����Ă��邩�ǂ�������
	bool Touch::IsTrigger() const
	{
		return m_touchFlag && !m_prevTouchFlag ? true : false;
	}

	///��ʂɃ^�b�`�������ꂽ���ǂ���
	bool Touch::IsRelease() const
	{
		return !m_touchFlag && m_prevTouchFlag ? true : false;
	}

	///X���W�擾 (5 <= x <= 314)
	float Touch::GetTouchX() const
	{
		return m_touchPos.x - LOWER_SCREEN_OFFSET_WIDTH;
	}

	///Y���W�擾 (5 <= y <= 234)
	float Touch::GetTouchY() const
	{
		return m_touchPos.y - UPPER_SCREEN_HEIGHT;
	}
}// end namespace System
}// end namespace NW_Lib
