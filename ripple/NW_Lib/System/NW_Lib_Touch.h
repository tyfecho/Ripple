/**
 * @file    Touch.h
 * @brief   �^�b�`��S������փb�_�ł�
 *
 * @author  Kenta Minamitsu
 * @date    2013/09/18
 * @version 1.0
 */

#pragma once

#include "../System/NW_Lib_Common.h"

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
	class Touch : public Common
	{
	private:
		
		VECTOR2 m_touchPos;
		bool m_prevTouchFlag;
		bool m_touchFlag;
		
	public:
		/**
		 * @brief   �R���X�g���N�^
		 * 
		 * @see     
		 */
		Touch();

		/**
		 * @brief   �f�X�g���N�^
		 * 
		 * @see     
		 */
		~Touch();

		/**
			@brief ���������s���܂�
		*/
		void Initialize();

		/**
			@brief �^�b�`���͂��A�b�v�f�[�g���܂�
		*/
		void Update();

		/**
			@brief ���������J�����܂�
		*/
		void Finalize();
		
		/**
		*	@brief �^�b�`����Ă��邩�ǂ���
		*/
		bool IsTouch() const;
		
		/**
		*	@brief �^�b�`����Ă��邩�ǂ���
		*/
		bool IsTrigger() const;

		/**
		*	@brief �^�b�`����Ă��邩�ǂ���
		*/
		bool IsRelease() const;

		/**
		*	@brief X���W�擾
		*/
		float GetTouchX() const;

		/**
		*	@brief Y���W�擾
		*/
		float GetTouchY() const;
	};
}// end namespace System
}// end namespace NW_Lib
