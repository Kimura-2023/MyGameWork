/*
作成者 : KimuraYusaku

更新者 : Osaki Noriaki

更新履歴
		2021/12/11		作成
		2021/12/15		挙動変更(敵の向きを取得し目の前に出現するように変更。)
		2021/12/18		フラグをリターンするのではなく目標座標をリターンするように変更(要検討)
		2021/12/21		テレポート先の計算を変更
		2021/12/22		入力がない場合のテレポート先の計算を変更
		2022/1/3		クラス化、画像設定
		2022/1/6		テレポート先の敵を常に判定するように変更(ターゲット先表示のため)
						それに従い、Do関数内のテレポート判定をSerch関数に移行。		更新者:Kimura
		2022/1/7		テレポート先の敵にカーソルが表示されるように　更新者:Kimura
		2022/1/9		矢印追加
		2022/1/13		テレポートの挙動変更(ベクトルから角度を求め、斜めにも移動できるように変更)
*/

#include "Tereport.h"
#include "EnemyManager.h"
#include "main.h"
#include <vector>
#include <math.h>
#include "HitStop.h"
#include "Effect.h"
#include "Player.h"
#include "Sound.h"
#include "input.h"
#include "debugproc.h"

#define RADIUS			(30.0f)					// テレポート可能範囲(半径)
#define MOVE			(7.0f)						// 移動量(攻撃時に進行する量)

#define WAVE_EXPANSION_SPEED (0.015f)

// ===グローバル宣言===
bool g_Terflg = false;
Enemy *pEnemy = nullptr;
Enemy * Tereport::m_pActive = nullptr;

Tereport::Tereport() :ObjectBase2D(OFLOAT2(RADIUS * 2, RADIUS * 2), "data/texture/tereport_area.png")
{
	m_color.w = 0.3f;
	m_pEM = EnemyManager::GetInstance();
	ArrowManager::Create();
	m_pAM = ArrowManager::GetInstance();
	m_pWave = new ObjectBase2D(OFLOAT2(RADIUS * 2, RADIUS * 2), "data/texture/tereport_area.png");
	Transform wk = m_pWave->GetTransform();
	wk.scale *= 0.0f;
	m_pWave->SetTransform(wk);
	m_pWave->SetColor(OFLOAT4(1.0f, 1.0f, 1.0f, 0.9f));
}

Tereport::~Tereport()
{
	ArrowManager::Destroy();
}

void Tereport::Update()
{
	//UpdateWave();
	//プレイヤーに追従
	m_transform.pos = Player::GetPos();

	m_input.x = GetStick((float)GetJoyX(JOYSTICKID1));
	m_input.y = -GetStick((float)GetJoyY(JOYSTICKID1));

	if (GetKeyPress(VK_LEFT) && !Tereport::GetTereport())
	{
		m_input.x = -1;
	}
	if (GetKeyPress(VK_RIGHT) && !Tereport::GetTereport())
	{
		m_input.x = 1;
	}
	if (GetKeyPress(VK_UP) && !Tereport::GetTereport())
	{
		m_input.y = 1;
	}
	if (GetKeyPress(VK_DOWN) && !Tereport::GetTereport())
	{
		m_input.y = -1;
	}

	m_pActive = Serch(m_transform.pos, m_pEM->GetAlive(), m_input);

	if (!g_Terflg && m_pActive != nullptr)
	{
		m_pAM->SetPos(m_transform.pos, m_pActive);
	}
	//m_pAM->SetFlg(m_pTarget->GetEnable());
	m_pAM->Update();

	PrintDebugProc("X:[%f]\n", m_input.x);
	PrintDebugProc("Y:[%f]\n", m_input.y);
}

void Tereport::UpdateWave()
{

	Transform wk = m_pWave->GetTransform();
	if (wk.scale.x < 1.0f)
	{
		wk.scale.x += WAVE_EXPANSION_SPEED;
		wk.scale.y += WAVE_EXPANSION_SPEED;
		wk.scale.z += WAVE_EXPANSION_SPEED;

	}
	else
	{
		wk.scale *= 0.0f;
	}
	wk.pos = m_transform.pos;
	m_pWave->SetTransform(wk);

	OFLOAT4 wkcol = m_pWave->GetColor();
	wkcol.w = (1.0f - wk.scale.x) * 0.8f;
	m_pWave->SetColor(wkcol);
}

// ===テレポート処理===
OFLOAT3 Tereport::Do(OFLOAT3 *aPos)
{
	g_Terflg = true;
	if (m_pActive == nullptr)
	{
		g_Terflg = false;
		return *aPos;
	}


	pEnemy = (m_pActive);							// テレポートした敵のポインタを保存

	int Dir = 1;
	if ((m_pActive)->GetPos().x > aPos->x)
	{
		Dir = -1;
	}

	Effect::GetInstance()->Play(EFFECT_WARP, *aPos, 0.08f);

	// ベクトルの長さを正規化
	OFLOAT3 vec = *aPos - m_pActive->GetPos();
	vec.x = vec.x * vec.x;
	vec.y = vec.y * vec.z;
	vec.z = vec.x + vec.y;
	vec.z = sqrtf(vec.z);
	vec.x = (aPos->x - m_pActive->GetPos().x);
	vec.y = (aPos->y - m_pActive->GetPos().y);

	aPos->x = m_pActive->GetPos().x + cosf(atan2(vec.y, vec.x)) * MOVE;				// プレイヤーを敵の目の前に移動させる
	aPos->y = m_pActive->GetPos().y + sinf(atan2(vec.y, vec.x)) * MOVE;

	//Effect::GetInstance()->Play(EFFECT_WARP, *aPos, 0.1f);
	//HitStop::StartSlow(0.25f, 3);

	CSound::Play(SE_WARP);				// 効果音再生(ワープ)

	OFLOAT3 nextPos = { m_pActive->GetPos().x - cosf(atan2(vec.y , vec.x)) * MOVE,
		m_pActive->GetPos().y - sinf(atan2(vec.y , vec.x)) * MOVE, (m_pActive)->GetPos().z };
	return nextPos;						// 進行方向の座標をリターン
}

bool Tereport::GetTereport()
{
	return g_Terflg;
}

void Tereport::SetTereport(bool flg)
{
	g_Terflg = flg;
}

void Tereport::SetTarget(void)
{
	pEnemy = nullptr;
}

Enemy* Tereport::Serch(OFLOAT3 pos, std::list<Enemy*> enemy, OFLOAT2 vec)
{
	std::list<Enemy*>::iterator it = enemy.begin();
	std::list<Enemy*> active;
	m_pAM->SetFlg(false);
	while (it != enemy.end())
	{
		if ((((*it)->GetPos().x - pos.x)*((*it)->GetPos().x - pos.x)) + (((*it)->GetPos().y - pos.y) * ((*it)->GetPos().y - pos.y)) < RADIUS * RADIUS)
		{
			active.push_back(*it);
		}

		++it;
	}
	if (active.empty())
	{
		return nullptr;
	}
	m_pAM->SetFlg(true);

	// プレイヤーから敵座標へのベクトル正規化	
	std::vector<OFLOAT3> vector;
	std::vector<float> dot;

	int a = active.size();
	vector.resize(a);								// 三平方の定理で長さを求める
	dot.resize(a);									// 内積格納用
	OFLOAT3 save;									// プレイヤーから敵のベクトル

	it = active.begin();
	int i = 0;
	while (it != active.end())
	{
		save.x = pos.x - (*it)->GetPos().x;							// プレイヤーから敵のベクトル(x方向)
		vector[i].x = save.x * save.x;								// 三平方の定理で扱うために2乗する
		save.y = pos.y - (*it)->GetPos().y;							// プレイヤーから敵のベクトル(y方向)
		vector[i].y = save.y * save.y;								// 三平方の定理で扱うために2乗する
		vector[i].z = vector[i].x + vector[i].y;					// x^2+y^2でベクトルを求める
		vector[i].z = sqrtf(vector[i].z);							// 正規化するために√2乗する
		vector[i].x = save.x / vector[i].z;							// x方向ベクトルを正規化する
		vector[i].y = save.y / vector[i].z;							// y方向ベクトルを正規化する

		// 内積を算出
		dot[i] = (vector[i].x) * (vec.x) + (vector[i].y) * (vec.y);		// 内積を求める
		++it;
		++i;
	}

	int Max = 0;
	int next = 0;
	// 方向入力が少ない場合はプレイヤーに近い敵にテレポート先を指定
	if (vec.x < 0.2f && vec.x > -0.2f && vec.y < 0.2f && vec.y > -0.2f)
	{
		for (int i = 0; i < a; i++)
		{
			if (vector[i].z < vector[Max].z)
			{
				next = Max;
				Max = i;
			}
		}
	}
	else
	{
		// 内積の値で一番大きいものを算出する

		for (int i = 0; i < a; i++)
		{
			//if(vec.xh) /*ベクトルの方向が合っている時*/
			if (dot[i] < dot[Max])
			{
				next = Max;
				Max = i;
			}
		}
	}

	it = active.begin();
	for (int i = 0; i < Max; i++)
	{
		++it;
	}

	// 前に攻撃した敵にはテレポートしないようにする
	if (pEnemy == nullptr)
	{
		//pEnemy = *it;						// 最初にテレポートした敵のポインタを格納(攻撃した敵がいない場合)
	}
	else if (pEnemy == *it)					// 一番近くの敵が攻撃済みだった場合
	{
		it = active.begin();
		for (int i = 0; i < next; i++)
		{
			++it;								// 次に近い敵を対象に
		}
		if (*it == pEnemy)
		{
			m_pAM->SetFlg(false);	// テレポートできる敵がいない場合カーソルも表示させない
			return nullptr;					// テレポート先無しにする
		}
	}
	return *it;
}