#include "NW_Lib/system/NW_Lib_system.h"
#include "GameMain.h"

/****************************************************************************************
*	@brief ���C���֐�
*
*	@param[in] hInst �A�v���P�[�V�����̃C���X�^���X�n���h��
*	@param[in] hPrevInst ���NULL
*	@param[in] lpCmdLine �R�}���h���C�����i�[���ꂽ������
*	@param[in] nCmdShow �E�B���h�E�̕\�����@�̎w��
*
*	@return ���b�Z�[�W�̃p�����[�^
****************************************************************************************/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	srand(static_cast<unsigned int>(time(NULL)));
	// �V�X�e������
	NW_Lib::DirectXSystem* pDirectXSystem = new NW_Lib::DirectXSystem();
	// �V�X�e��������
	pDirectXSystem->Initialize();

	// �Q�[������
	Game::GameMain* pGamemain = new Game::GameMain();

	// �X�V����
	pDirectXSystem->Update(pGamemain);

	delete pGamemain;
	delete pDirectXSystem;

	//_CrtDumpMemoryLeaks();
	return 0;
}
