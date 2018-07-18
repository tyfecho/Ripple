////////////////////////////////////////////////////////////////////////////////
/// @file Task.h
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
#pragma once


////////////////////////////////////////////////////////////////////////////////
/// @brief タスクのインターフェイス
////////////////////////////////////////////////////////////////////////////////
struct ITask
{
	// 仮想デストラクタ
	virtual ~ITask(){}
	// 実行関数
	virtual bool Run(){return true;}
	// 更新
	virtual void Update(){};
	virtual void Update(bool* pGimickOn){};
	// 描画
	virtual void Draw(){};

	// リスト構造
	ITask *next; // 次のタスクのポインタ
	ITask *prev; // 前のタスクのポインタ
};

////////////////////////////////////////////////////////////////////////////////
/// @brief タスクのデータ管理
////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Task : public ITask
{
	// タスクデータ
	T data;
	// 実行関数
	bool Run() { return data.Run(); }
	// 更新
	void Update(){ data.Update(); };
	void Update(bool* pGimickOn) { data.Update(pGimickOn); }
	// 描画
	void Draw(){ data.Draw(); };
};

////////////////////////////////////////////////////////////////////////////////
/// @brief 最低限のタスク機能
////////////////////////////////////////////////////////////////////////////////
struct TaskManager
{
	// アクティブなタスク
	ITask active;

	// コンストラクタ
	TaskManager();

	// デストラクタ
	~TaskManager();

	// タスクを作成する
	template<class T>
	T* Add()
	{
		// タスクの作成
		Task<T>* task = new Task<T>;
		// タスクをリストに追加
		AddTask(task);
		// タスクのデータを返す
		return &task->data;
	}

	// タスクを実行する
	//  ※Run関数実行時呼び出し禁止
	void Run();

	// タスクを追加する
	//  ※Run関数実行時呼び出し禁止
	void AddTask(ITask* task);

	// タスクを更新
	void Update();
	void Update(bool* pGimickOn);
	// タスクを描画
	void Draw();

	// タスクを解放する
	//  ※Run関数実行時呼び出し禁止
	ITask* FreeTask(ITask* task);

	// すべてのタスクを開放
	//  ※Run関数実行時呼び出し禁止
	void AllEreas();
};
