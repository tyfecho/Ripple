#include "NW_Lib_Particle.h"



namespace NW_Lib
{
	ParticleManager::~ParticleManager()
	{
		int a = 0;
		a++;
		a--;
	}
	/**
	*	����������
	*/
	void ParticleManager::Initialize()
	{
		// �A�N�e�B�u���X�g�̏����ݒ�i��ɂ���j
		activeList.next = NULL;
		// �t���[���X�g�̏����ݒ�i��ɂ���j
		freeList.next = NULL;

		// �����̈��͒l
		gx = 0.0f;
		gy = 0.0f;
		gz = 0.0f;

		// �r���{�[�h�̏����ݒ�
		D3DXMatrixIdentity(&matBillbord);

		// �G���g���[���̔z������ׂăt���[���X�g�ɐڑ�����
		//  �e�[�u���̐������J��Ԃ�
		for(int i = 0 ; i < tableNum ; i++ )
		{
			// �t���[���X�g�ɐڑ�
			infoTable[i].next = freeList.next;
			infoTable[i].prev = &freeList;
			if( freeList.next ) freeList.next->prev = &infoTable[i];
			freeList.next = &infoTable[i];
		}
	}


	/**
	*	�e�N�X�`���̓ǂݍ���
	*	@param filename  �ǂݍ��ރt�@�C���̖��O
	*/
	bool ParticleManager::LoadTexture(LPCTSTR filename)
	{
		// �e�N�X�`�����ݒ肳��Ă�����������
		texture.Release();
		// �e�N�X�`�����t�@�C������ǂݍ���
		return SUCCEEDED( D3DXCreateTextureFromFile(m_pD3DDevice,filename,&texture) );
	}

	/**
	*	�A�N�e�B�u���X�g�ɃG���g���[����ǉ�����
	*/
	Info* ParticleManager::AddInfo()
	{
		// �t���[�̃G���g���[��񂪂Ȃ��ꍇNULL��Ԃ�
		if( freeList.next == NULL ) return NULL;

		// �t���[�̃G���g���[�����擾
		Info *info = freeList.next;

		// �擾�����G���g���[�����A�t���[���X�g����؂藣��
		freeList.next = freeList.next->next;
		if( freeList.next ) freeList.next->prev = &freeList;

		// �G���g���[�����A�N�e�B�u���X�g�ɐڑ�
		info->next = activeList.next;
		info->prev = &activeList;
		if( activeList.next) activeList.next->prev = info;
		activeList.next = info;

		// �擾�����G���g���[���̃|�C���^��Ԃ�
		return info;
		
	}

	/**
	*	�p�[�e�B�N���̃G���g���[
	*	@param x
	*	@param y
	*	@param z
	*	@param vx
	*	@param vy
	*	@param vz
	*	@param ax
	*	@param ay
	*	@param az
	*	@param start_angle   �J�n�e
	*	@param end_angle
	*	@param start_scaleX  �J�n��X�̊g�嗦
	*	@param start_scaleY  �J�n��Y�̊g�嗦
	*	@param end_scaleX
	*	@param end_scaleY
	*	@param start_r
	*	@param start_g
	*	@param start_b
	*	@param start_a
	*	@param end_r
	*	@param end_g
	*	@param end_b
	*	@param end_a
	*/
	void ParticleManager::Entry(int frame,	// �t���[����
		float x,   float y,   float z,	// ���W
		float vx,  float vy,  float vz,	// ���x
		float ax, float ay, float az,	// �����x
		float start_angle, // �J�n�p�x
		float end_angle,   // �I���p�x
		float start_scaleX,float start_scaleY, // �g�k�E�J�n�{��
		float end_scaleX,float end_scaleY,     // �g�k�E�I���{��
		float start_r,float start_g,float start_b,float start_a, // �J�n�J���[
		float end_r,  float end_g,  float end_b,  float end_a,    // �I���J���[
		D3DXMATRIX* ParentMat
		)
	{
		// �G���g���[�����擾����
		Info* info = AddInfo();

		// �G���g���[�����擾�ł��Ȃ������ꍇ�A�G���g���[���Ȃ�
		if( info == NULL )  return ;

		// �������W�ݒ�
		info->x = x;
		info->y = y;
		info->z = z;

		// �����x�ݒ�
		info->vx = vx;
		info->vy = vy;
		info->vz = vz;

		// �����x�ݒ�
		info->ax = ax;
		info->ay = ay;
		info->az = az;

		// �J�n�X�P�[���ƁA�I���X�P�[����ݒ�
		info->scaleX = start_scaleX;
		info->scaleY = start_scaleY;
		info->s_scaleX = start_scaleX;
		info->s_scaleY = start_scaleY;
		info->e_scaleX = end_scaleX;
		info->e_scaleY = end_scaleY;

		// �J�n�A���O���ƁA�I���A���O���̐ݒ�
		info->angle = start_angle;
		info->s_angle = start_angle;
		info->e_angle = end_angle;

		// �J�n�J���[�̐ݒ�i���݂̃J���[�j
		info->a = start_a;
		info->r = start_r;
		info->g = start_g;
		info->b = start_b;

		// �J�n�J���[�̐ݒ�
		info->s_a = start_a;
		info->s_r = start_r;
		info->s_g = start_g;
		info->s_b = start_b;

		// �I������̐ݒ�
		info->e_a = end_a;
		info->e_r = end_r;
		info->e_g = end_g;
		info->e_b = end_b;

		// �J�n�t���[���̐ݒ�
		info->frame = 0;
		// �I���t���[���̐ݒ�
		info->num_frame = frame;

		info->pParentMat = ParentMat;
	}

	/**
	*	���ׂẴA�N�e�B�u�ȃG���g���[���̍X�V
	*/
	void ParticleManager::Update()
	{
		// �A�N�e�B�u�ȃG���g���[���̐擪�|�C���^���擾����
		Info *i = activeList.next;

		// ���X�g�̏I�[�܂ŌJ��Ԃ�
		while( i != NULL  )
		{
			// �X�V�̎��s
			if( Update(i) )
			{
				// �������s(true)�̏ꍇ
				// ���̃G���g���[����
				i = i->next;
			}
			else
			{
				// �����I��(false)�̏ꍇ
				// �A�N�e�B�u���X�g����؂藣��
				i = Free(i);
			}
		}
	}

	/**
	*	�G���g���[���̍X�V
	*	@param pInfo  ��̏��
	*/
	bool ParticleManager::Update(Info *pInfo)
	{
		// �t���[���J�E���g��i�߂�(���Ԍo��)
		pInfo->frame ++;

		// �I���t���[���J�E���g�ɒB���Ă�����A�I������
		if( pInfo->frame > pInfo->num_frame ) return false;

		// ���Ԍo�߂��W��(0.0f�`1.0f)�ɕϊ�
		float f = (float)pInfo->frame / pInfo->num_frame;

		// ���x�ɉ����x��������
		pInfo->vx += pInfo->ax;
		pInfo->vy += pInfo->ay;
		pInfo->vz += pInfo->az;

		// ���x�ɏd�͉����x��������
		pInfo->vx += gx;
		pInfo->vy += gy;
		pInfo->vz += gz;

		// ���݂̍��W�ɑ��x��������
		pInfo->x += pInfo->vx;
		pInfo->y += pInfo->vy;
		pInfo->z += pInfo->vz;

		// �o�ߎ��Ԃ̊p�x�����߂�i���`��ԁj
		pInfo->angle = (pInfo->e_angle - pInfo->s_angle) * f + pInfo->s_angle;
		// �o�ߎ��Ԃ̃X�P�[�������߂�i���`��ԁj
		pInfo->scaleX = (pInfo->e_scaleX - pInfo->s_scaleX) * f + pInfo->s_scaleX;
		pInfo->scaleY = (pInfo->e_scaleY - pInfo->s_scaleY) * f + pInfo->s_scaleY;

		// �o�ߎ��Ԃ̐F�����߂�i���`��ԁj
		pInfo->r = (pInfo->e_r - pInfo->s_r) * f + pInfo->s_r;
		pInfo->g = (pInfo->e_g - pInfo->s_g) * f + pInfo->s_g;
		pInfo->b = (pInfo->e_b - pInfo->s_b) * f + pInfo->s_b;
		pInfo->a = (pInfo->e_a - pInfo->s_a) * f + pInfo->s_a;

		// �p������
		return true;
	}

	/**
	*	�w��̃G���g���[�����A�N�e�B�u���X�g����A�t���[���X�g�ֈڂ�
	*	@param pInfo  ��̏��
	*/
	Info* ParticleManager::Free(Info *pInfo)
	{
		// �G���g���[���̎��̃G���g���[�����擾����
		Info *temp = pInfo->next;

		// �A�N�e�B�u���X�g����A�G���g���[����؂藣��
		if( pInfo->next ) pInfo->next->prev = pInfo->prev;
		pInfo->prev->next = pInfo->next;

		// �G���g���[�����t���[���X�g�ɐڑ�����
		pInfo->next = freeList.next;
		pInfo->prev = &freeList;
		if( freeList.next ) freeList.next->prev = pInfo;
		freeList.next = pInfo;

		// �؂藣�����A���̃G���g���[����Ԃ�
		return temp;
	}

	/**
	*	�r���{�[�h���̍X�V
	*/
	void ParticleManager::SetBillbord()
	{
		// �r���[�s��̎擾
		D3DXMATRIX matView;
		m_pD3DDevice->GetTransform(D3DTS_VIEW,&matView);

		// �r���[�s��̋t�s������߂�
		D3DXMatrixInverse(&matBillbord,NULL,&matView);
		matBillbord._41 = 0.0f;
		matBillbord._42 = 0.0f;
		matBillbord._43 = 0.0f;
	}

	/**
	*	�A�N�e�B�u�ȃG���g���[�������ׂĕ`�悷��
	*/
	void ParticleManager::Draw()
	{
		// �u�����f�B���O�̐ݒ�(���Z)
		//m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		//m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
		//m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// Z�o�b�t�@�ւ̏������݂��~
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

		// �J�����O���~�i�|���S���̗��ʕ`��j
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

		// ���C�e�B���O��~
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

		// �e�N�X�`���̊g��k���ݒ�i���`�����łڂ����j
		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

		// ���_�t�H�[�}�b�g�̐ݒ�i3D���W�A�F�A�e�N�X�`���P���j
		m_pD3DDevice->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1);

		// �e�N�X�`���̐ݒ�
		m_pD3DDevice->SetTexture(0,texture);

		// �`�揈��
		
		// �A�N�e�B�u�ȃG���g���[���̐擪�|�C���^���擾����
		Info *i = activeList.next;
		// ���X�g�̖����܂ŌJ��Ԃ�
		while( i != NULL )
		{
			// �G���g���[���̓��e��`�悷��
			Draw(i);

			// ���̃G���g���[��
			i = i->next;
		}

		// �e�N�X�`���̐ݒ������
		m_pD3DDevice->SetTexture(0,NULL);
		// �u�����f�B���O�̒�~
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
		// Z�o�b�t�@�ւ̏������ݗL��
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
		// �J�����O��L��
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
		// ���C�e�B���O��L��
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING,TRUE);

		// �e�N�X�`���̊g��k���̐ݒ�i�f�t�H���g�ɖ߂��F�ڂ��������j
		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);

	}


	/**
	*	�P�̃G���g������`�悷��
	*	@param pInfo  ��̏��
	*/
	void ParticleManager::Draw(Info* pInfo)
	{
		// �F���𒸓_�f�[�^�̌`���ɕϊ�
		DWORD color = D3DXCOLOR(pInfo->r,pInfo->g,pInfo->b,pInfo->a);

		// ���_�f�[�^�̐ݒ�i�r���{�[�h�p�̎l�p�|���S���j
		VERTEX v[4]={
			// X���W,Y���W,Z���W,�F,U���W,V���W
			{-0.5f, 0.5f,0, color, 0,0}, // ����
			{ 0.5f, 0.5f,0, color, 1,0}, // �E��
			{-0.5f,-0.5f,0, color, 0,1}, // ����
			{ 0.5f,-0.5f,0, color, 1,1}, // �E��
		};

		// ���s�ړ��ϊ��s��
		D3DXMATRIX trans;
		D3DXMatrixTranslation(&trans,pInfo->x,pInfo->y,pInfo->z);

		// Z��]�ϊ��s�� 
		D3DXMATRIX angle;
		D3DXMatrixRotationZ(&angle,pInfo->angle);

		// �X�P�[���ϊ��s��
		D3DXMATRIX scale;
		D3DXMatrixScaling(&scale,pInfo->scaleX,pInfo->scaleY,1.0f);

		// �e�s��̐ݒ�
		if(pInfo->pParentMat){
			trans *= *(pInfo->pParentMat);
			trans._11 = 1;
			trans._12 = 0;
			trans._13 = 0;
			trans._21 = 0;
			trans._22 = 1;
			trans._23 = 0;
			trans._31 = 0;
			trans._32 = 0;
			trans._33 = 1;
		}

		// ���[���h�ϊ��s�� �i�X�P�[������]���r���{�[�h�����s�ړ��j
		D3DXMATRIX world;
		world = scale * angle * matBillbord * trans;

		// ���[���h�ϊ��s��̐ݒ�
		m_pD3DDevice->SetTransform(D3DTS_WORLD,&world);

		// �`��
		m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,v,sizeof(VERTEX));
	}

	/**
	*	���ׂĂ̏�������
	*/
	void ParticleManager::Erase()
	{
		// �A�N�e�B�u�ȃG���g���[���̐擪�|�C���^���擾����
		Info *i = activeList.next;
		// ���X�g�̖����܂ŌJ��Ԃ�
		while( i != NULL )
		{
			i = Free(i);
		}
	}
}


