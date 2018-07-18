////////////////////////////////////////////////////////////////////////////////
/// @file Task.cpp
/// �^�X�N�}�l�[�W��
///
/// @author  Takafumi Murase @ freeplay inc.
///
/// @version Ver1.01 date2012.05.30  �k����
/// @version Ver1.00 date2012.04.01  ����
///
/// @since   2012.04.01
///
///  Copyright(c) TRIDENT college 2012 , Takafumi Murase All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////
#include "NW_Lib_Task.h"

////////////////////////////////////////////////////////////////////////////////
/// @brief �R���X�g���N�^
////////////////////////////////////////////////////////////////////////////////
TaskManager::TaskManager()
{
	// �^�X�N�͋�ɂ���
	active.next = &active;
	active.prev = &active;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief �f�X�g���N�^
////////////////////////////////////////////////////////////////////////////////
TaskManager::~TaskManager()
{
	// ���ׂẴ^�X�N���������
	AllEreas();
}

// 
//  
////////////////////////////////////////////////////////////////////////////////
/// @brief �^�X�N��ǉ�����
/// @param  task �ǉ�����^�X�N�̃|�C���^
///
/// ��Run�֐����s���Ăяo���֎~.
////////////////////////////////////////////////////////////////////////////////
void TaskManager::AddTask(ITask* task)
{
	task->prev = &active;
	task->next = active.next;
	task->prev->next = task;
	task->next->prev = task;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief �^�X�N���������
/// @param  task �������^�X�N�̃|�C���^
/// @return *����*
///
///  ��Run�֐����s���Ăяo���֎~.
////////////////////////////////////////////////////////////////////////////////
ITask* TaskManager::FreeTask(ITask* task)
{
	// ���̃^�X�N���擾
	ITask* next = task->next;

	// �^�X�N�����X�g����O��
	task->prev->next = task->next;
	task->next->prev = task->prev;

	// �^�X�N���폜
	delete task;

	// ���̃^�X�N��Ԃ�
	return next;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief �^�X�N�����s����
///
/// ��Run�֐����s���Ăяo���֎~.
////////////////////////////////////////////////////////////////////////////////
void TaskManager::Run()
{
	// �擪�̃^�X�N���擾
	ITask *task = active.next;
	while( task != &active )
	{
		// �^�X�N�����s
		if( task->Run() ) task = task->next;     // ���̃^�X�N
		else              task = FreeTask(task); // �^�X�N�̊J��
	}
}

////////////////////////////////////////////////////////////////////////////////
/// @brief �^�X�N���X�V����
///
/// ��Run�֐����s���Ăяo���֎~.
////////////////////////////////////////////////////////////////////////////////
void TaskManager::Update()
{
	// �擪�̃^�X�N���擾
	ITask *task = active.next;
	while( task != &active )
	{
		// �^�X�N���X�V
		task->Update();
		task = task->next;
	}
}
////////////////////////////////////////////////////////////////////////////////
/// @brief �^�X�N���X�V����
///
/// ��Run�֐����s���Ăяo���֎~.
////////////////////////////////////////////////////////////////////////////////
void TaskManager::Update(bool* pGimickOn)
{
	// �擪�̃^�X�N���擾
	ITask *task = active.next;
	while( task != &active )
	{
		// �^�X�N���X�V
		task->Update(pGimickOn);
		task = task->next;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// @brief ���ׂẴ^�X�N���J��
///
/// ��Run�֐����s���Ăяo���֎~.
////////////////////////////////////////////////////////////////////////////////
void TaskManager::AllEreas()
{
	for( ITask* t = active.next; t != &active ; t = FreeTask(t) );
}