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
	*	初期化処理
	*/
	void ParticleManager::Initialize()
	{
		// アクティブリストの初期設定（空にする）
		activeList.next = NULL;
		// フリーリストの初期設定（空にする）
		freeList.next = NULL;

		// 初期の引力値
		gx = 0.0f;
		gy = 0.0f;
		gz = 0.0f;

		// ビルボードの初期設定
		D3DXMatrixIdentity(&matBillbord);

		// エントリー情報の配列をすべてフリーリストに接続する
		//  テーブルの数だけ繰り返す
		for(int i = 0 ; i < tableNum ; i++ )
		{
			// フリーリストに接続
			infoTable[i].next = freeList.next;
			infoTable[i].prev = &freeList;
			if( freeList.next ) freeList.next->prev = &infoTable[i];
			freeList.next = &infoTable[i];
		}
	}


	/**
	*	テクスチャの読み込み
	*	@param filename  読み込むファイルの名前
	*/
	bool ParticleManager::LoadTexture(LPCTSTR filename)
	{
		// テクスチャが設定されていたら解放する
		texture.Release();
		// テクスチャをファイルから読み込む
		return SUCCEEDED( D3DXCreateTextureFromFile(m_pD3DDevice,filename,&texture) );
	}

	/**
	*	アクティブリストにエントリー情報を追加する
	*/
	Info* ParticleManager::AddInfo()
	{
		// フリーのエントリー情報がない場合NULLを返す
		if( freeList.next == NULL ) return NULL;

		// フリーのエントリー情報を取得
		Info *info = freeList.next;

		// 取得したエントリー情報を、フリーリストから切り離す
		freeList.next = freeList.next->next;
		if( freeList.next ) freeList.next->prev = &freeList;

		// エントリー情報をアクティブリストに接続
		info->next = activeList.next;
		info->prev = &activeList;
		if( activeList.next) activeList.next->prev = info;
		activeList.next = info;

		// 取得したエントリー情報のポインタを返す
		return info;
		
	}

	/**
	*	パーティクルのエントリー
	*	@param x
	*	@param y
	*	@param z
	*	@param vx
	*	@param vy
	*	@param vz
	*	@param ax
	*	@param ay
	*	@param az
	*	@param start_angle   開始各
	*	@param end_angle
	*	@param start_scaleX  開始のXの拡大率
	*	@param start_scaleY  開始のYの拡大率
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
	void ParticleManager::Entry(int frame,	// フレーム数
		float x,   float y,   float z,	// 座標
		float vx,  float vy,  float vz,	// 速度
		float ax, float ay, float az,	// 加速度
		float start_angle, // 開始角度
		float end_angle,   // 終了角度
		float start_scaleX,float start_scaleY, // 拡縮・開始倍率
		float end_scaleX,float end_scaleY,     // 拡縮・終了倍率
		float start_r,float start_g,float start_b,float start_a, // 開始カラー
		float end_r,  float end_g,  float end_b,  float end_a,    // 終了カラー
		D3DXMATRIX* ParentMat
		)
	{
		// エントリー情報を取得する
		Info* info = AddInfo();

		// エントリー情報を取得できなかった場合、エントリーしない
		if( info == NULL )  return ;

		// 初期座標設定
		info->x = x;
		info->y = y;
		info->z = z;

		// 初速度設定
		info->vx = vx;
		info->vy = vy;
		info->vz = vz;

		// 加速度設定
		info->ax = ax;
		info->ay = ay;
		info->az = az;

		// 開始スケールと、終了スケールを設定
		info->scaleX = start_scaleX;
		info->scaleY = start_scaleY;
		info->s_scaleX = start_scaleX;
		info->s_scaleY = start_scaleY;
		info->e_scaleX = end_scaleX;
		info->e_scaleY = end_scaleY;

		// 開始アングルと、終了アングルの設定
		info->angle = start_angle;
		info->s_angle = start_angle;
		info->e_angle = end_angle;

		// 開始カラーの設定（現在のカラー）
		info->a = start_a;
		info->r = start_r;
		info->g = start_g;
		info->b = start_b;

		// 開始カラーの設定
		info->s_a = start_a;
		info->s_r = start_r;
		info->s_g = start_g;
		info->s_b = start_b;

		// 終了からの設定
		info->e_a = end_a;
		info->e_r = end_r;
		info->e_g = end_g;
		info->e_b = end_b;

		// 開始フレームの設定
		info->frame = 0;
		// 終了フレームの設定
		info->num_frame = frame;

		info->pParentMat = ParentMat;
	}

	/**
	*	すべてのアクティブなエントリー情報の更新
	*/
	void ParticleManager::Update()
	{
		// アクティブなエントリー情報の先頭ポインタを取得する
		Info *i = activeList.next;

		// リストの終端まで繰り返す
		while( i != NULL  )
		{
			// 更新の実行
			if( Update(i) )
			{
				// 処理続行(true)の場合
				// 次のエントリー情報へ
				i = i->next;
			}
			else
			{
				// 処理終了(false)の場合
				// アクティブリストから切り離す
				i = Free(i);
			}
		}
	}

	/**
	*	エントリー情報の更新
	*	@param pInfo  一つの情報
	*/
	bool ParticleManager::Update(Info *pInfo)
	{
		// フレームカウントを進める(時間経過)
		pInfo->frame ++;

		// 終了フレームカウントに達していたら、終了する
		if( pInfo->frame > pInfo->num_frame ) return false;

		// 時間経過を係数(0.0f～1.0f)に変換
		float f = (float)pInfo->frame / pInfo->num_frame;

		// 速度に加速度を加える
		pInfo->vx += pInfo->ax;
		pInfo->vy += pInfo->ay;
		pInfo->vz += pInfo->az;

		// 速度に重力加速度を加える
		pInfo->vx += gx;
		pInfo->vy += gy;
		pInfo->vz += gz;

		// 現在の座標に速度を加える
		pInfo->x += pInfo->vx;
		pInfo->y += pInfo->vy;
		pInfo->z += pInfo->vz;

		// 経過時間の角度を求める（線形補間）
		pInfo->angle = (pInfo->e_angle - pInfo->s_angle) * f + pInfo->s_angle;
		// 経過時間のスケールを求める（線形補間）
		pInfo->scaleX = (pInfo->e_scaleX - pInfo->s_scaleX) * f + pInfo->s_scaleX;
		pInfo->scaleY = (pInfo->e_scaleY - pInfo->s_scaleY) * f + pInfo->s_scaleY;

		// 経過時間の色を求める（線形補間）
		pInfo->r = (pInfo->e_r - pInfo->s_r) * f + pInfo->s_r;
		pInfo->g = (pInfo->e_g - pInfo->s_g) * f + pInfo->s_g;
		pInfo->b = (pInfo->e_b - pInfo->s_b) * f + pInfo->s_b;
		pInfo->a = (pInfo->e_a - pInfo->s_a) * f + pInfo->s_a;

		// 継続する
		return true;
	}

	/**
	*	指定のエントリー情報をアクティブリストから、フリーリストへ移す
	*	@param pInfo  一つの情報
	*/
	Info* ParticleManager::Free(Info *pInfo)
	{
		// エントリー情報の次のエントリー情報を取得する
		Info *temp = pInfo->next;

		// アクティブリストから、エントリー情報を切り離す
		if( pInfo->next ) pInfo->next->prev = pInfo->prev;
		pInfo->prev->next = pInfo->next;

		// エントリー情報をフリーリストに接続する
		pInfo->next = freeList.next;
		pInfo->prev = &freeList;
		if( freeList.next ) freeList.next->prev = pInfo;
		freeList.next = pInfo;

		// 切り離した、次のエントリー情報を返す
		return temp;
	}

	/**
	*	ビルボード情報の更新
	*/
	void ParticleManager::SetBillbord()
	{
		// ビュー行列の取得
		D3DXMATRIX matView;
		m_pD3DDevice->GetTransform(D3DTS_VIEW,&matView);

		// ビュー行列の逆行列を求める
		D3DXMatrixInverse(&matBillbord,NULL,&matView);
		matBillbord._41 = 0.0f;
		matBillbord._42 = 0.0f;
		matBillbord._43 = 0.0f;
	}

	/**
	*	アクティブなエントリー情報をすべて描画する
	*/
	void ParticleManager::Draw()
	{
		// ブレンディングの設定(加算)
		//m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		//m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
		//m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// Zバッファへの書き込みを停止
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

		// カリングを停止（ポリゴンの両面描画）
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

		// ライティング停止
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

		// テクスチャの拡大縮小設定（線形処理でぼかし）
		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

		// 頂点フォーマットの設定（3D座標、色、テクスチャ１枚）
		m_pD3DDevice->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1);

		// テクスチャの設定
		m_pD3DDevice->SetTexture(0,texture);

		// 描画処理
		
		// アクティブなエントリー情報の先頭ポインタを取得する
		Info *i = activeList.next;
		// リストの末尾まで繰り返す
		while( i != NULL )
		{
			// エントリー情報の内容を描画する
			Draw(i);

			// 次のエントリーへ
			i = i->next;
		}

		// テクスチャの設定を消す
		m_pD3DDevice->SetTexture(0,NULL);
		// ブレンディングの停止
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
		// Zバッファへの書き込み有効
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
		// カリングを有効
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
		// ライティングを有効
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING,TRUE);

		// テクスチャの拡大縮小の設定（デフォルトに戻す：ぼかし無し）
		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
		m_pD3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);

	}


	/**
	*	１つのエントリ情報を描画する
	*	@param pInfo  一つの情報
	*/
	void ParticleManager::Draw(Info* pInfo)
	{
		// 色情報を頂点データの形式に変換
		DWORD color = D3DXCOLOR(pInfo->r,pInfo->g,pInfo->b,pInfo->a);

		// 頂点データの設定（ビルボード用の四角ポリゴン）
		VERTEX v[4]={
			// X座標,Y座標,Z座標,色,U座標,V座標
			{-0.5f, 0.5f,0, color, 0,0}, // 左上
			{ 0.5f, 0.5f,0, color, 1,0}, // 右上
			{-0.5f,-0.5f,0, color, 0,1}, // 左下
			{ 0.5f,-0.5f,0, color, 1,1}, // 右下
		};

		// 平行移動変換行列
		D3DXMATRIX trans;
		D3DXMatrixTranslation(&trans,pInfo->x,pInfo->y,pInfo->z);

		// Z回転変換行列 
		D3DXMATRIX angle;
		D3DXMatrixRotationZ(&angle,pInfo->angle);

		// スケール変換行列
		D3DXMATRIX scale;
		D3DXMatrixScaling(&scale,pInfo->scaleX,pInfo->scaleY,1.0f);

		// 親行列の設定
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

		// ワールド変換行列 （スケール＊回転＊ビルボード＊平行移動）
		D3DXMATRIX world;
		world = scale * angle * matBillbord * trans;

		// ワールド変換行列の設定
		m_pD3DDevice->SetTransform(D3DTS_WORLD,&world);

		// 描画
		m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,v,sizeof(VERTEX));
	}

	/**
	*	すべての情報を消す
	*/
	void ParticleManager::Erase()
	{
		// アクティブなエントリー情報の先頭ポインタを取得する
		Info *i = activeList.next;
		// リストの末尾まで繰り返す
		while( i != NULL )
		{
			i = Free(i);
		}
	}
}


