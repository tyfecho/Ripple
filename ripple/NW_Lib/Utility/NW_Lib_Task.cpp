////////////////////////////////////////////////////////////////////////////////
/// @file Task.cpp
/// タスクマネージャ
///
/// @author  Takafumi Murase @ freeplay inc.
///
/// @version Ver1.01 date2012.05.30  縮小版
/// @version Ver1.00 date2012.04.01  初版
///
/// @since   2012.04.01
///
///  Copyright(c) TRIDENT college 2012 , Takafumi Murase All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////
#include "NW_Lib_Task.h"

////////////////////////////////////////////////////////////////////////////////
/// @brief コンストラクタ
////////////////////////////////////////////////////////////////////////////////
TaskManager::TaskManager()
{
	// タスクは空にする
	active.next = &active;
	active.prev = &active;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief デストラクタ
////////////////////////////////////////////////////////////////////////////////
TaskManager::~TaskManager()
{
	// すべてのタスクを解放する
	AllEreas();
}

// 
//  
////////////////////////////////////////////////////////////////////////////////
/// @brief タスクを追加する
/// @param  task 追加するタスクのポインタ
///
/// ※Run関数実行時呼び出し禁止.
////////////////////////////////////////////////////////////////////////////////
void TaskManager::AddTask(ITask* task)
{
	task->prev = &active;
	task->next = active.next;
	task->prev->next = task;
	task->next->prev = task;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief タスクを解放する
/// @param  task 解放するタスクのポインタ
/// @return *説明*
///
///  ※Run関数実行時呼び出し禁止.
////////////////////////////////////////////////////////////////////////////////
ITask* TaskManager::FreeTask(ITask* task)
{
	// 次のタスクを取得
	ITask* next = task->next;

	// タスクをリストから外す
	task->prev->next = task->next;
	task->next->prev = task->prev;

	// タスクを削除
	delete task;

	// 次のタスクを返す
	return next;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief タスクを実行する
///
/// ※Run関数実行時呼び出し禁止.
////////////////////////////////////////////////////////////////////////////////
void TaskManager::Run()
{
	// 先頭のタスクを取得
	ITask *task = active.next;
	while( task != &active )
	{
		// タスクを実行
		if( task->Run() ) task = task->next;     // 次のタスク
		else              task = FreeTask(task); // タスクの開放
	}
}

////////////////////////////////////////////////////////////////////////////////
/// @brief タスクを更新する
///
/// ※Run関数実行時呼び出し禁止.
////////////////////////////////////////////////////////////////////////////////
void TaskManager::Update()
{
	// 先頭のタスクを取得
	ITask *task = active.next;
	while( task != &active )
	{
		// タスクを更新
		task->Update();
		task = task->next;
	}
}
////////////////////////////////////////////////////////////////////////////////
/// @brief タスクを更新する
///
/// ※Run関数実行時呼び出し禁止.
////////////////////////////////////////////////////////////////////////////////
void TaskManager::Update(bool* pGimickOn)
{
	// 先頭のタスクを取得
	ITask *task = active.next;
	while( task != &active )
	{
		// タスクを更新
		task->Update(pGimickOn);
		task = task->next;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// @brief すべてのタスクを開放
///
/// ※Run関数実行時呼び出し禁止.
////////////////////////////////////////////////////////////////////////////////
void TaskManager::AllEreas()
{
	for( ITask* t = active.next; t != &active ; t = FreeTask(t) );
}
