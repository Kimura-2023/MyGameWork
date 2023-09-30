// ===当たり判定管理クラス実装===
// 当たり判定管理クラスを実装
// Author Kimura
// ==============================

// ===インクルード===
#include "CollisionManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "DebugProc.h"


// ===静的メンバ===
CollisionManager* CollisionManager::m_pInstance = nullptr;

// ===コンストラクタ===
CollisionManager::CollisionManager()
{
}

// ===デストラクタ===
CollisionManager::~CollisionManager()
{
	Release();									// 全当たり判定解放
}

// ===生成===
void CollisionManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new CollisionManager;		// インスタンスを生成
	}
}

// ===破棄===
void CollisionManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;					// インスタンスを破棄
		m_pInstance = nullptr;				// nullを格納
	}
}

// ===取得===
CollisionManager* CollisionManager::Get()
{
	return m_pInstance;						// インスタンスを返す
}

// ===管理下に追加===
void CollisionManager::AddManager(Collision3D* col)
{
	m_CollisionList.push_back(col);			// 当たり判定リストに追加
}

// ===解放===
void CollisionManager::Release()
{
	COLIT colit = m_CollisionList.begin();			// 当たり判定リストの先頭イテレータ取得
	while (colit != m_CollisionList.end())			// 終端まで繰り返し
	{
		colit = m_CollisionList.erase(colit);		// リストから消去
	}
}

// ===指定した当たり判定の解放===
void CollisionManager::Release(Collision3D* col)
{
	COLIT colit = m_CollisionList.begin();			// リストの先頭イテレータ取得
	while ((*colit) != col)							// 対象の当たり判定まで繰り返し
	{
		++colit;									// イテレータをずらす
	}
	colit = m_CollisionList.erase(colit);			// リストから対象の当たり判定を消去
}

// ===更新処理===
void CollisionManager::Update()
{
	// ---例外処理---
	if (m_CollisionList.size() <= 0)
		return;

	// ---更新処理---
	COLIT colit = m_CollisionList.begin();	// 当たり判定リストの先頭にイテレーターを取得
	std::list<Collision3D*> ColVec(0);		// 当たり判定を行うリスト

	while (colit != m_CollisionList.end()) // 終端まで繰り返し
	{
		// ---当たり判定計算の除外---
		if (!(*colit)->GetFollowObject()->GetEnable())				// オブジェクトの当たり判定enableフラグがfalseの場合は処理しない
		{
			(*colit)->SetCollision(false);			// 当たり判定をとらない
			++colit;								// 次の当たり判定を探索
			continue;
		}

		// ---当たり判定座標の更新---
		(*colit)->Update();
		if ((*colit)->GetEnable() && (*colit)->GetFollowObject()->GetVisible() && !(*colit)->GetFollowObject()->GetStatic())		// 当たり判定をとるオブジェクトはリストに追加
																																	// (更新・描画を行っているオブジェクトかつ、静的オブジェクトじゃないもの)
			ColVec.push_back((*colit));				// リストに追加
		++colit;									// イテレータをずらす
	}

	// ---衝突判定---
	colit = ColVec.begin();					// 当たり判定リストの先頭にイテレーターを取得
	while (colit != ColVec.end())			// 終端まで繰り返し
	{
		// ---例外処理---
		if (m_CollisionList.size() <= 1)
			return;

		// ---判定---
		COLIT other = m_CollisionList.begin();
		while (other != m_CollisionList.end())
		{
			// ---除外設定---
			if ((*other) == (*colit))
			{
				++other;		// 次の当たり判定を探索
				continue;
			}
			if (!(*other)->GetEnable())
			{
				++other;		// 次の当たり判定を探索
				continue;
			}
			if (!(*other)->GetFollowObject()->GetVisible())
			{
				++other;		// 次の当たり判定を探索
				continue;
			}

			// ---当たり判定チェック(衝突時true)---
			if ((*colit)->CheckCollision(*other))
			{
				(*colit)->OnCollision((*other)->GetFollowObject());		// 当たったときの処理
			}
			++other;			// イテレータをずらす
		}
		(*colit)->ReflectMovement();		// めり込み量を戻す処理
		++colit;				// イテレータをずらす
	}
}

// ===デバッグ表示===
void CollisionManager::Draw()
{
	auto it = m_CollisionList.begin();			// 当たり判定リストの先頭イテレータ取得
	while (it != m_CollisionList.end())			// 終端まで繰り返し
	{
		(*it)->Draw();				// デバッグ表示処理
		++it;						// イテレータをずらす
	}
}

// ===デバッグ表示有無の切替===
void CollisionManager::SwitchVisibleCollision(Object* obj)
{
	auto it = m_CollisionList.begin();			// 当たり判定リストの先頭イテレータ取得
	while (it != m_CollisionList.end())			// 終端まで繰り返し
	{
		if ((*it)->GetFollowObject() == obj)	// 引数のオブジェクトと参照しているオブジェクトが一致していたらループから抜ける
			break;
		++it;						// イテレータをずらす
	}
	if(it != m_CollisionList.end())	// オブジェクトが見つかった場合
		(*it)->SwitchVisible();		// デバッグ表示の切替
}