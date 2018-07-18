////////////////////////////////////////////////////////////////////////////////
/// @file Task.h
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
#pragma once


////////////////////////////////////////////////////////////////////////////////
/// @brief �^�X�N�̃C���^�[�t�F�C�X
////////////////////////////////////////////////////////////////////////////////
struct ITask
{
	// ���z�f�X�g���N�^
	virtual ~ITask(){}
	// ���s�֐�
	virtual bool Run(){return true;}
	// �X�V
	virtual void Update(){};
	virtual void Update(bool* pGimickOn){};
	// �`��
	virtual void Draw(){};

	// ���X�g�\��
	ITask *next; // ���̃^�X�N�̃|�C���^
	ITask *prev; // �O�̃^�X�N�̃|�C���^
};

////////////////////////////////////////////////////////////////////////////////
/// @brief �^�X�N�̃f�[�^�Ǘ�
////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Task : public ITask
{
	// �^�X�N�f�[�^
	T data;
	// ���s�֐�
	bool Run() { return data.Run(); }
	// �X�V
	void Update(){ data.Update(); };
	void Update(bool* pGimickOn) { data.Update(pGimickOn); }
	// �`��
	void Draw(){ data.Draw(); };
};

////////////////////////////////////////////////////////////////////////////////
/// @brief �Œ���̃^�X�N�@�\
////////////////////////////////////////////////////////////////////////////////
struct TaskManager
{
	// �A�N�e�B�u�ȃ^�X�N
	ITask active;

	// �R���X�g���N�^
	TaskManager();

	// �f�X�g���N�^
	~TaskManager();

	// �^�X�N���쐬����
	template<class T>
	T* Add()
	{
		// �^�X�N�̍쐬
		Task<T>* task = new Task<T>;
		// �^�X�N�����X�g�ɒǉ�
		AddTask(task);
		// �^�X�N�̃f�[�^��Ԃ�
		return &task->data;
	}

	// �^�X�N�����s����
	//  ��Run�֐����s���Ăяo���֎~
	void Run();

	// �^�X�N��ǉ�����
	//  ��Run�֐����s���Ăяo���֎~
	void AddTask(ITask* task);

	// �^�X�N���X�V
	void Update();
	void Update(bool* pGimickOn);
	// �^�X�N��`��
	void Draw();

	// �^�X�N���������
	//  ��Run�֐����s���Ăяo���֎~
	ITask* FreeTask(ITask* task);

	// ���ׂẴ^�X�N���J��
	//  ��Run�֐����s���Ăяo���֎~
	void AllEreas();
};
