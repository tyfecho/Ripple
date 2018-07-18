/**
*	@file	timer.cpp
*
*	@brief	�^�C�}�[�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#include "NW_Lib_Timer.h"


namespace NW_Lib
{
	/**
	*	�R���X�g���N�^
	*/
	Timer::Timer()
	{
		QueryPerformanceFrequency(&m_frequency); // CPU�̑������Ƃ��Ă���
		QueryPerformanceCounter(&m_oldcounter);  // �ǂꂾ�����Ԃ���������
	}



	/**
	*	�w�肳�ꂽ���Ԃ��o�܂őҋ@
	*	@param waitTime �҂�����
	*/
	void Timer::Wait(double waitTime)
	{
		if (waitTime > 1.0) waitTime = 1.0;
		if (waitTime < 0.0) waitTime = 0.0;


		LARGE_INTEGER s;
		LARGE_INTEGER w; // �P��ő҂ׂ����Ԃ��i�[

		w.QuadPart = (LONGLONG)(m_frequency.QuadPart * waitTime); // �ǂꂾ���҂ׂ��Ȃ̂�
		QueryPerformanceCounter(&s);
		s.QuadPart -= m_oldcounter.QuadPart; // �O�񂩂�ǂꂾ���o������

		// �O�t���[������̌o�ߎ��ԁ@���@�P��ő҂ׂ�����
		while (s.QuadPart < w.QuadPart)
		{
			Sleep(0);                            // �w�肵�����ԏ������Ƃ߂�
			QueryPerformanceCounter(&s);         // �����P�x���̎��Ԃ��擾
			s.QuadPart -= m_oldcounter.QuadPart; // �O�񂩂�ǂꂾ���o������
		}

		QueryPerformanceCounter(&m_oldcounter);
	}
}
