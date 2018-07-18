
/**
*	@file	ResourceManager.h
*
*	@brief	リソースマネージメント抽象クラス
*
*	@date	2013/08/15
*
*	@author	皆光　健太
*/

#pragma once

//#include <d3dx9.h>
#include <map>

#include "../Utility/NW_Lib_utility.h"

namespace NW_Lib
{
	using namespace std;

	//シーンマネージャクラス
	template <typename T>

	class ResourceManager
	{

	private:

		//マネージャ
		map<int , T> m_manager;
		
	public:

		/*=======================================================
			関数名	：	ResourceManager
			説明	：	コンストラクタ
			引数	：	なし
			戻り値	：　なし
		========================================================*/
		ResourceManager()
		{

		}

		/*=======================================================
			関数名	：	~ResourceManager
			説明	：	デストラクタ
			引数	：	なし
			戻り値	：　なし
		========================================================*/
		~ResourceManager()
		{
			AllRemove();
		}

		/*=======================================================
			関数名	：	Entry
			説明	：	共有データの取得
			引数	：	登録ID
						オブジェクト
			戻り値	：　結果
		========================================================*/
		bool Entry(int entryID , T obj)
		{
			if( !m_manager.count(entryID) )
			{
				m_manager.insert( map<int , T>::value_type( entryID , obj));

				return true;
			}

			return false;
		}

		/*=======================================================
			関数名	：	Get
			説明	：	データの取得
			引数	：	登録ID
			戻り値	：　オブジェクト
		========================================================*/
		T Get(int ID)
		{
			return m_manager[ID];
		}

		/*=======================================================
			関数名	：	Remove
			説明	：	データの消去
			引数	：	登録ID
			戻り値	：　結果
		========================================================*/
		bool Remove(int ID)
		{
			SafeDelete(m_manager[ID]);
			return m_manager.erase(ID);
		}

		/*=======================================================
			関数名	：	AllRemove
			説明	：	データの消去
			引数	：	登録ID
			戻り値	：　オブジェクト
		========================================================*/
		void AllRemove()
		{
			if( !m_manager.empty() )
			{
				for( map<int,T>::iterator it = m_manager.begin() ; it != m_manager.end() ; it++ )
				{
					SafeDelete((*it).second);
				}
			}
			if( !m_manager.empty() )
			{
				m_manager.clear();
			}
		}
		
		/*=======================================================
			関数名	：	GetSize
			説明	：	サイズの取得
			引数	：	なし
			戻り値	：　登録済み要素数
		========================================================*/
		int GetSize()
		{
			return m_manager.size();
		}
		/*=======================================================
			関数名	：	GetId
			説明	：	IDの取得
			引数	：	インデックス
			戻り値	：　登録済みID
		========================================================*/
		int GetId(int index)
		{
			int temp = 0;
			if( !m_manager.empty() )
			{
				for( map<int,T>::iterator it = m_manager.begin() ; it != m_manager.end() ; it++ )
				{
					if( temp == index)
					{
						return it->first;
					}
					temp++;
				}
			}

			return 0;
		}

	};

}