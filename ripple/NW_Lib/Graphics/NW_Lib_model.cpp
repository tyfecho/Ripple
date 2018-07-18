/***
*	@file	model.cpp
*
*	@brief	3D���f���p�N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#include "NW_Lib_model.h"
#include "d3dx9.h"
#include <string>

#include "../Utility/NW_Lib_utility.h"

using namespace std;


namespace NW_Lib
{
	// ���F�ɂȂ�}�e���A��
	static const D3DMATERIAL9 WHITEMATERIAL = 
	{
		1.0f,1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,1.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,

		10.0f
	};
	// ���F�ɂ���}�e���A��
	static const D3DMATERIAL9 BLACKMATERIAL = 
	{
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,

		10.0f
	};
	// �D�F�ɂ���}�e���A��
	static const D3DMATERIAL9 GLAYMATERIAL = 
	{
		0.5f,0.5f,0.5f,0.5f,
		0.5f,0.5f,0.5f,0.5f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,

		10.0f
	};


	/**
	*	�R���X�g���N�^
	*/
	Model::Model()
	:m_Pos   (0.0f,0.0f,0.0f)
	,m_Angle (0.0f,0.0f,0.0f)
	,m_Radian(0.0f,0.0f,0.0f)
	,m_Scale (1.0f,1.0f,1.0f)
	,m_pParentWorld(NULL)
	,m_pInitWorld(NULL)
	,m_NoMaterial(false)
	,m_MaterialGlay(false)
	{
	}
	/**
	*	�R���X�g���N�^
	*	@param filename  �ǂݍ��ރt�@�C����
	*/
	Model::Model(char* filename)
	:m_Pos   (0.0f,0.0f,0.0f)
	,m_Angle (0.0f,0.0f,0.0f)
	,m_Radian(0.0f,0.0f,0.0f)
	,m_Scale (1.0f,1.0f,1.0f)
	,m_pParentWorld(NULL)
	,m_pInitWorld(NULL)
	,m_NoMaterial(false)
	,m_MaterialGlay(false)
	{
		LoadMesh(filename);
	}

	/**
	*	�f�X�g���N�^
	*/
	Model::~Model()
	{
		Release();
		//NoTexture();
	}

	/**
	*	���b�V���̃��[�h
	*	@param filename  �ǂݍ��ރt�@�C����
	*/
	void Model::LoadMesh(const char* filename)
	{
		LPD3DXBUFFER pD3DXMtrlBuffer;       // �}�e���A���f�[�^
		LPD3DXBUFFER pAdjacencyBuffer;      // �אڃf�[�^
		LPD3DXBUFFER pEffectInstanceBuffer; // �G�t�F�N�g

		// ��񏉊���
		//ZeroMemory(m_mesh,sizeof(MESHOBJECT));

		string meshname = filename;
		//meshname.insert(0,"data//mesh//");

		// ���b�V�������[�h
		if (FAILED(D3DXLoadMeshFromX(
			meshname.c_str(),
							D3DXMESH_SYSTEMMEM,
							m_pD3DDevice,
							&pAdjacencyBuffer,
							&pD3DXMtrlBuffer,
							&pEffectInstanceBuffer,
							&m_numMaterials,
							&m_mesh))){
								// ���f���쐬���s

				// sprite�t�@�C���̒��ɂ��Ȃ�����
				// �G���[����
				MessageBox(NULL, "���f�����ǂݍ��߂Ȃ�", "class Model", MB_OK);
		}

		// �Ƃ肠�����g��Ȃ����߉�����Ă���
		pEffectInstanceBuffer->Release();

		D3DXMATERIAL* d3dxMaterials = static_cast<D3DXMATERIAL*>(pD3DXMtrlBuffer->GetBufferPointer());
		m_meshMaterials  = new D3DMATERIAL9[m_numMaterials];
		m_meshtexture    = new Texture*[m_numMaterials];
		m_ppSetTex       = new Texture*[m_numMaterials];
		m_pMasterAmbient = new D3DCOLORVALUE[m_numMaterials];

		for(DWORD i = 0;i < m_numMaterials;++i){
			// �}�e���A���̃R�s�[
			m_meshMaterials[i] = d3dxMaterials[i].MatD3D;

			// �A���r�G���g�J���[�̐ݒ�
			m_meshMaterials[i].Ambient = m_meshMaterials[i].Diffuse;
			m_pMasterAmbient[i] = m_meshMaterials[i].Ambient;

			if(d3dxMaterials[i].pTextureFilename){
			//	// �e�N�X�`���̐ݒ�
			//	D3DXCreateTextureFromFile(pD3DDevice,d3dxMaterials[i].pTextureFilename,&m_meshtexture[i]);
				string map = d3dxMaterials[i].pTextureFilename;
				map.insert(0,"data//Resorse//");
				m_meshtexture[i] = new Texture(map.c_str());
			}else{
				// �e�N�X�`���Ȃ�
				m_meshtexture[i] = NULL;
			}
			m_ppSetTex[i] = NULL;
		}

		// �o�b�t�@�̉��
		pD3DXMtrlBuffer->Release();

		LPD3DXMESH MeshTemp = NULL;
		HRESULT hr;

		// ���b�V���œK��
		hr = m_mesh->Optimize( 
									D3DXMESHOPT_VERTEXCACHE | D3DXMESH_SYSTEMMEM,
									(DWORD*)pAdjacencyBuffer->GetBufferPointer(),
									NULL,
									NULL,
									NULL,
									&MeshTemp
								  );

		pAdjacencyBuffer->Release();

		m_mesh->Release();
		m_mesh = MeshTemp;
	}

	/**
	*	�e�N�X�`���̐ݒ�i��������new����p�^�[���j
	*	@param filename  �ǂݍ��ރt�@�C����
	*	@param tex_num   ����ւ���e�N�X�`��
	*/
	void Model::SetTexture(const char *filename,unsigned int tex_num)
	{
		if(tex_num < m_numMaterials){
			// �e�N�X�`���̉��
			if(m_meshtexture != NULL){
				if(m_meshtexture[tex_num] != NULL){
					// ���
					SafeDelete( m_meshtexture[tex_num]);
				}
			}
			string map = filename;
			map.insert(0,"data//Resorse//");
			m_meshtexture[tex_num] = new Texture(map.c_str());
		}
	}

	/**
	*	�e�N�X�`���̐ݒ�i�p�ӂ��Ă������g���p�^�[���j
	*	@param pTex    �e�N�X�`�����
	*	@param tex_num ����ւ���e�N�X�`��
	*/
	void Model::SetTexture(Texture* pTex,unsigned int tex_num)
	{
		SafeDelete(m_ppSetTex[tex_num]);
		m_ppSetTex[tex_num] = pTex;
	}
	/**
		�e�N�X�`����������
	*/
	void Model::NoTexture()
	{
		for(DWORD i = 0;i < m_numMaterials;++i){
			SafeDelete(m_ppSetTex[i]);
			SafeDelete(m_meshtexture[i]);
		}
	}

	/**
	*	�������
	*/
	void Model::Release()
	{
		// �}�e���A���̃o�b�t�@���J��
		if(m_meshtexture != NULL){
			delete[] m_meshMaterials;
			m_meshMaterials = NULL;
		}

		// �e�N�X�`���̉��
		if(m_meshtexture != NULL){
			for(DWORD i = 0;i < m_numMaterials;++i){
				if(m_meshtexture[i] != NULL){
					// ���
					//m_meshtexture[i]->Release();
					SafeDelete( m_meshtexture[i]);
				}
			}
			delete[] m_pMasterAmbient;
			delete[] m_meshtexture;
			delete[] m_ppSetTex;
			m_meshtexture = NULL;
		}

		// ���b�V�����̂����
		if(m_mesh != NULL){
			m_mesh->Release();
			m_mesh = NULL;
		}

		// ���b�V�����̉��
		SafeDelete( m_mesh);
	}

	/**
	*	�s��v�Z����
	*/
	void Model::Calc()
	{

		D3DXMATRIX matScale;    // �g�k�p
		D3DXMATRIX matMove;     // �ړ��p
		D3DXMATRIX matRotation; // ��]�p
		D3DXMatrixIdentity(&m_LocalWorld);

		if(m_pInitWorld != NULL){
			// ���[�J���v�Z�O�ɂ����Ă���
			m_LocalWorld *= *m_pInitWorld;
		}

		// �X�P�[�����O
		D3DXMatrixScaling(&matScale,m_Scale.x,m_Scale.y,m_Scale.z);
		// ��]
		D3DXMatrixRotationYawPitchRoll( &matRotation , m_Radian.y , m_Radian.x , m_Radian.z );
		// �ړ�
		D3DXMatrixTranslation( &matMove , m_Pos.x,m_Pos.y,m_Pos.z );

		// �e����
		m_LocalWorld *= matScale;
		m_LocalWorld *= matRotation;
		m_LocalWorld *= matMove;

		if(m_pParentWorld != NULL){
			// �e�̍s�񂪂���
			m_LocalWorld *= *m_pParentWorld;
		}

//		m_pD3DDevice->SetTransform(D3DTS_WORLD,&m_LocalWorld);
	}

	/**
	*	�`�揈��
	*/
	void Model::Draw(/**D3DVIEWPORT9* pViewport,int viewport,DWORD* pBackColor*/)
	{
		//// �r���[�|�[�g�̐ݒ�
		//m_pD3DDevice->SetViewport(&pViewport[viewport]);

		////	��ʂ��N���A
		//m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, pBackColor[viewport], 1.0f, 0);

		//Calc();

		for(DWORD i = 0;i < m_numMaterials;++i){

			//�A���t�@�u�����f�B���O��ݒ肷��	
			m_pD3DDevice->SetRenderState (D3DRS_ALPHABLENDENABLE,TRUE);
			m_pD3DDevice->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			m_pD3DDevice->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);


			m_pD3DDevice->SetRenderState (D3DRS_ALPHATESTENABLE,TRUE);
			m_pD3DDevice->SetRenderState (D3DRS_ALPHAREF,1);
			m_pD3DDevice->SetRenderState (D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
		
			//m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
	
			//m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);

			m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
			m_pD3DDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);

			m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);


			// �}�e���A���̐ݒ�
			if(m_MaterialGlay){
				m_pD3DDevice->SetMaterial(&GLAYMATERIAL);
			}else if(m_NoMaterial){
				m_pD3DDevice->SetMaterial(&BLACKMATERIAL);
			}else{
				m_pD3DDevice->SetMaterial(&m_meshMaterials[i]);
			}

			// �e�N�X�`���̐ݒ�
			if(m_ppSetTex[i] != NULL){
				m_pD3DDevice->SetTexture(0,m_ppSetTex[i]->GetTexture());
			}else if(m_meshtexture[i] != NULL){
				m_pD3DDevice->SetTexture(0,m_meshtexture[i]->GetTexture());
			}

			// �\��
			m_mesh->DrawSubset(i);
			m_pD3DDevice->SetTexture(0,NULL);
			m_pD3DDevice->SetMaterial(&WHITEMATERIAL);
		}
	}
}


