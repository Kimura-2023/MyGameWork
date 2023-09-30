/*
作成者 : Tokuda Ryo

更新者：　Osaki Noriaki
		  Kimura Yusaku

更新履歴
     2021/12/09		ファイル作成
	 2021/12/12	　　プレイヤー更新変更
	 2021/12/19		挙動ごとのアクションで停止時間の作成
	 2021/12/21		エフェクト確認
	 2021/12/25		効果音追加
	 2022/01/06		テレポート実行の引数変更
	 2022/01/07		プレイヤーがテレポートの実体を持つように変更
	 2022/01/08		死んだときのフラグとリスポーン中のフラグを別で扱うように変更
*/

#include "Player.h"
#include "input.h"
#include "GlobalConst.h"
#include "debugproc.h"
#include "ButtonData.h"
#include "Camera.h"
#include "ArrowManager.h"
#include "TeleportFlash.h"
#include "HitStop.h"

#define JUMP_SUPPORTER (0.6f)
#define JUMP_PLAYER  (15.0f)
#define SPEED (0.9f)											// 進行速度
#define INVISIBLEFRAME (12)
#define STOP_TEREPORTTIMER	(3 + INVISIBLEFRAME)				// テレポート位置停止タイマー
#define STOP_ATTACKTIMER	(18)								// 攻撃後停止タイマー

#define HITSTOP_BUFFER (-5)

bool Attackflg;
OFLOAT3 Player::m_Pos = {0.0f, 0.0f, 0.0f};
bool Player::m_Respawnflg = true;
bool Player::m_Deadflg = true;

Player::Player() : ObjectBase3D("data/model/Wait4.fbx"), Collision3D(this)
{
	AddAnimModel("data/model/Down7.fbx");
	AddAnimModel("data/model/Attack11_test.fbx");

	m_transform.pos = OFLOAT3(0.0f, 0.0f, 0.0f);
	m_transform.scale = OFLOAT3(0.2f, 0.2f, 0.2f);
	m_transform.rot.y = -90;
	m_pEnemyList = EnemyManager::GetInstance();
	m_pSpawnBase = SpawnBase::GetInstance();
	m_gravity = 0.0f;
	m_jump = JUMP_PLAYER;
	m_Deadflg = true;
	m_stopTimer = 0;
	m_movePos = OFLOAT3(0.0f, 0.0f, 0.0f);
	m_rigidbody.isGravity = false;
	m_tereportTimer = HITSTOP_BUFFER;
	m_dir = 1;
	Attackflg = false;
	m_Respawnflg = true;
	m_pSpawnBase->DeadPlayer(m_Respawnflg);
	//CCamera::Get()->SetFollowTarget(&m_transform.pos);
	OFLOAT3 size = { 5.0f,5.0f,5.0f };
	OFLOAT3 ofset = { 0.0f,0.0f,0.0f };
	AddCollisionBox(size, ofset);
	m_isPauseAnimation = false;
	m_pEffect = Effect::GetInstance();
	TraceManager::Create();
	m_pTM = TraceManager::GetInstance();
	m_input = { 0.0f,0.0f };
	m_pTereport = new Tereport;
	m_tag = OBJECTTAG_PLAYER;
	m_height = false;

	m_firstTp = false;
	TeleportFlash::Create();
}
Player::~Player()
{
	TeleportFlash::Destroy();
	TraceManager::Destroy();
	delete m_pTereport;
}
void Player::Update()
{
	m_transform.rot.y = -90.0f * m_dir;

	m_pTM->Generate(m_transform.pos);
	m_pTM->Update();

	TeleportFlash::GetInstance()->Update();

	if (m_Respawnflg == false)
	{
		if (m_rigidbody.isGravity)
		{
			SetAnimState(1);
		}

		// 目的座標まで進行した場合(右)
		if (m_dir == 1 && m_movePos.x < m_transform.pos.x && Tereport::GetTereport())
		{
			Tereport::SetTereport(false);			// テレポートアクション終了
		}

		// 目的座標まで進行した場合(左)
		if (m_dir == -1 && m_movePos.x > m_transform.pos.x && Tereport::GetTereport())
		{
			Tereport::SetTereport(false);			// テレポートアクション終了
		}
		if (!m_height && m_movePos.y >= m_transform.pos.y && Tereport::GetTereport())
		{
			Tereport::SetTereport(false);			// テレポートアクション終了
		}

		if (m_height && m_movePos.y <= m_transform.pos.y && Tereport::GetTereport())
		{
			Tereport::SetTereport(false);			// テレポートアクション終了
		}
		m_input = { 0.0f, 0.0f };

		// テレポートアクション中でない場合
		if (Tereport::GetTereport() == false)
		{
			m_movePos = m_transform.pos;					// 目的座標を現在座標に
			if (m_stopTimer >= 0)
			{
				--m_stopTimer;									// 停止時間更新

			}		
			m_rigidbody.spd = { 0.0f,0.0f,0.0f };			// 移動速度初期化
			m_rigidbody.accel = { 0.0f,0.0f,0.0f };			// 加速度初期化

			// 停止時間終了時
			if (m_stopTimer < 0)
			{
				m_rigidbody.isGravity = true;				// 重力を有効化
			}
		}

		if (m_stopTimer < 0)
		{
			m_input.x = GetStick((float)GetJoyX(JOYSTICKID1));
			m_input.y = -GetStick((float)GetJoyY(JOYSTICKID1));

			//向き取得
			if ((GetKeyPress(VK_LEFT) || m_input.x < -0.2f) && !Tereport::GetTereport() )
			{
				m_dir = -1;
				m_input.x = -1;
			}
			if ((GetKeyPress(VK_RIGHT) || m_input.x > 0.2f) && !Tereport::GetTereport())
			{
				m_input.x = 1;
				m_dir = 1;
			}
			if (GetKeyPress(VK_UP) && !Tereport::GetTereport())
			{
				m_input.y = 1;
			}
			if (GetKeyPress(VK_DOWN) && !Tereport::GetTereport())
			{
				m_input.y = -1;
			}

			// エンターキーが押されたらテレポート
			if ((GetKeyTrigger(VK_RETURN) || GetJoyTrigger(JOYSTICKID1, A) || m_firstTp) && !Tereport::GetTereport())
			{
				m_movePos = Tereport::Do(&m_transform.pos);
				m_firstTp = false;

				// テレポートが成立した時に目的座標まで進行する
				if (Tereport::GetTereport())
				{
					m_rigidbody.isGravity = false;
					m_tereportTimer = STOP_TEREPORTTIMER;			// テレポートした位置で停止する時間を指定
					//FadeOut(0.1f);
					m_color.w = 0.0f;
					//TeleportFlash::GetInstance()->Activation(INVISIBLEFRAME);
					if (m_movePos.x <= m_transform.pos.x)
					{
						m_dir = -1;									// 左に進む
					}
					else
					{
						m_dir = 1;									// 右に進む
					}
					if (m_movePos.y <= m_transform.pos.y)
					{
						m_height = false;
					}
					else
					{
						m_height = true;
					}
				}
			}

			if (m_tereportTimer == STOP_TEREPORTTIMER - INVISIBLEFRAME + 4)
			{

				Effect::GetInstance()->Play(EFFECT_WARP, m_transform.pos, 3.0f);

				m_color.w = 1.0f;
			}
			if (m_tereportTimer == STOP_TEREPORTTIMER - INVISIBLEFRAME)
			{
				//FadeIn(0.1f);
				//HitStop::StartSlow(0.25f, 3);
				SetAnimState(2, 3.0f);
			}

			// テレポート位置で停止した後目的座標まで進行する
			if (m_tereportTimer == 0)
			{
				if (Tereport::GetTereport())
				{
					//HitStop::StartSlow(0.1f, 3);
					m_stopTimer = STOP_ATTACKTIMER;					// 敵の方向に切り抜けた後、停止する時間
					m_rigidbody.accel.x = (m_movePos.x - m_transform.pos.x) / 10;				// 進行方向を設定
					m_rigidbody.accel.y = (m_movePos.y - m_transform.pos.y) / 10;				// 進行方向を設定
				}
			}


			if (m_transform.pos.y < -70.0f)
			{
				m_transform.pos.y = -70.0f;
				m_gravity = 0.0f;
				m_jump = JUMP_PLAYER;
				
				m_Deadflg = true;
				m_rigidbody.isGravity = false;

				m_Respawnflg = true;
        
		/*		m_pSpawnBase->DeadPlayer(m_Respawnflg);
				m_pEnemyList->RespawnEnemy();
				SetAnimState(0);
        
				m_pTM->Reset();*/
			}

			//if (m_transform.pos.x > 100.0f/*プレイヤーが動ける範囲の外を書き込む*/)
			//{
			//	m_rigidbody.isGravity = false;
			//	m_Respawnflg = true;
			//	m_pSpawnBase->DeadPlayer(m_Respawnflg);
			//}
		}
	}
	else
	{
		if (m_Deadflg)
		{
			m_pTM->Reset();
			m_rigidbody.isGravity = false;
			m_pSpawnBase->DeadPlayer(m_Respawnflg);
			m_pEnemyList->RespawnEnemy();
			SetAnimState(0);
			Tereport::SetTarget();
			Tereport::SetTereport(false);			// テレポートアクション終了
			m_Deadflg = false;
		}
		
		m_transform.pos = m_pSpawnBase->SpawnPos();
		m_transform.pos.y += 0.0f;

		//ArrowManager::GetInstance()->SetActive(true);
		if (GetKeyPress(VK_SPACE) || GetJoyTrigger(JOYSTICKID1, A))
		{
			if (!m_pSpawnBase->GetSpawn())
			{
				m_pSpawnBase->SpawnPlayer();
				m_rigidbody.isGravity = true;
				m_Respawnflg = false;
				m_firstTp = true;
			}
		}
	}
	
	m_Pos = m_transform.pos;

	JOYINFOEX* pad = GetJoyState(JOYSTICKID1);


	PrintDebugProc("X:[%f]\n", m_input.x);
	PrintDebugProc("Y:[%f]\n", m_input.y);
	PrintDebugProc("OPTION:[%d]\n", GetJoyButton(JOYSTICKID1, VIEW));
	PrintDebugProc("num:[%04x]\n", pad->dwButtons);
	PrintDebugProc("playerpos:[%f]\n", m_transform.pos.x);
	PrintDebugProc("playerpos:[%f]\n", m_transform.pos.y);
	PrintDebugProc("%d ashiba\n", m_Respawnflg);

	if (m_tereportTimer >= HITSTOP_BUFFER)
	{
		m_tereportTimer--;
		if (m_tereportTimer == HITSTOP_BUFFER)
		{
			HitStop::StartSlow(0.05f, 99);
			//HitStop::StartStop(0.1f);
		}
	}
	

}

OFLOAT2 Player::GetInput()
{
	return m_input;
}

bool Player::Getflg()
{
	return Attackflg;
}

void Player::SetRespawnFlg(bool Resflg, bool Dedflg)
{
	if (!m_Respawnflg)
	{
		CCamera::Get()->StartShake(30, 20.0f, 0.4f);
		Effect::GetInstance()->Play(EFFECT_SINU, GetPos(), 10.0f);
	}
	m_Respawnflg = Resflg;
	m_Deadflg = Dedflg;
}