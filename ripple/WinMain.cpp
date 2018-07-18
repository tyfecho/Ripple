#include "NW_Lib/system/NW_Lib_system.h"
#include "GameMain.h"

/****************************************************************************************
*	@brief メイン関数
*
*	@param[in] hInst アプリケーションのインスタンスハンドル
*	@param[in] hPrevInst 常にNULL
*	@param[in] lpCmdLine コマンドラインが格納された文字列
*	@param[in] nCmdShow ウィンドウの表示方法の指定
*
*	@return メッセージのパラメータ
****************************************************************************************/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	srand(static_cast<unsigned int>(time(NULL)));
	// システム生成
	NW_Lib::DirectXSystem* pDirectXSystem = new NW_Lib::DirectXSystem();
	// システム初期化
	pDirectXSystem->Initialize();

	// ゲーム生成
	Game::GameMain* pGamemain = new Game::GameMain();

	// 更新処理
	pDirectXSystem->Update(pGamemain);

	delete pGamemain;
	delete pDirectXSystem;

	//_CrtDumpMemoryLeaks();
	return 0;
}
