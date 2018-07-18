/**
*	@file	timer.h
*
*	@brief	�^�C�}�[�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#pragma once



#include <Windows.h>



namespace NW_Lib
{

	/*
		�^�C�}�[�N���X
	*/
	class Timer
	{
		private:
			/// CPU�̃N���b�N���g��
			LARGE_INTEGER m_frequency;

			/// �J�E���^�[��ێ����郏�[�N
			LARGE_INTEGER m_oldcounter;

		public:
			/// �R���X�g���N�^
			Timer();
			
			/// �w�肳�ꂽ���Ԃ��o�܂őҋ@
			void Wait(double waitTime);
	};

}
