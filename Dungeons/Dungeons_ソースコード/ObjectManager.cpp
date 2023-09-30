// ===オブジェクト管理クラス実装 ================
// オブジェクトを一元管理するクラスを実装
// Author kimura
// ===============================================

// ===インクルード===
#include "ObjectManager.h"
#include "DebugProc.h"

// ===静的メンバ===
ObjectManager* ObjectManager::m_pInstance = nullptr;	// インスタンス

// ===コンストラクタ===
ObjectManager::ObjectManager()
{
	m_3dObject.resize(0);	// 3Dオブジェクトリスト初期化
	m_2dObject.resize(0);	// 2Dオブジェクトリスト初期化
	m_UiObject.resize(0);	// UIオブジェクトリスト初期化
	m_nCount = 0;			// オブジェクト総数
}

// ===デストラクタ===
ObjectManager::~ObjectManager()
{
	Release();				// 所持しているオブジェクトを全て破棄する
}

// ===生成===
void ObjectManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new ObjectManager;	// インスタンスを生成
	}
	else
	{
		return;
	}
}

// ===破棄===
void ObjectManager::Destroy()
{
	delete m_pInstance;						// インスタンスを削除
	m_pInstance = nullptr;					// null格納
}

// ===取得===
ObjectManager* ObjectManager::Get()
{
	return m_pInstance;						// ObjectManagerのインスタンスを返す
}

// ===更新===
void ObjectManager::Update()
{
	O3DIT it3dobj;				// 3Dオブジェクトリスト(以下3Dlist)のイテレーター宣言
	it3dobj = m_3dObject.begin();		// 3Dlistイテレーターをリストの先頭に移動

	// ===条件ループ===
	while (it3dobj != m_3dObject.end())	// --3Dlistの終端まで
	{
		// nullチェック
		if ((*it3dobj) == nullptr)
		{
			it3dobj = m_3dObject.erase(it3dobj);	// 2Dlistから要素を削除
			m_nCount--;								// オブジェクト総数減少
			continue;
		}
		(*it3dobj)->Update();		// 3Dオブジェクト更新
		// オブジェクト破棄した場合にイテレーターの指す場所がなくなるので補正
		if ((*it3dobj) == nullptr)
		{
			it3dobj = m_3dObject.erase(it3dobj);	// 2Dlistから要素を削除
			m_nCount--;								// オブジェクト総数減少
			continue;
		}
		// ===現存オブジェクトのワールドマトリクス更新===
		//(*it3dobj)->UpdateMatrix();		// マトリクス更新
		++it3dobj;						// イテレーターをずらす
	}

	O2DIT it2dobj;				// 2Dオブジェクトリスト(以下2Dlist)のイテレーター宣言
	it2dobj = m_2dObject.begin();		// 2Dlistイテレーターをリストの先頭に移動

	// ===条件ループ===
	while (it2dobj != m_2dObject.end())
	{
		// nullチェック
		if ((*it2dobj) == nullptr)
		{
			it2dobj = m_2dObject.erase(it2dobj);	// 2Dlistから要素を削除
			m_nCount--;								// オブジェクト総数減少
			continue;
		}
		(*it2dobj)->Update();		// 2Dオブジェクト更新

		// オブジェクト破棄した場合にイテレーターの指す場所がなくなるので補正
		if ((*it2dobj) == nullptr)
		{
			it2dobj = m_2dObject.erase(it2dobj);	// 2Dlistから要素を削除
			m_nCount--;								// オブジェクト総数減少
			continue;
		}
		// ===現存オブジェクトのワールドマトリクス更新===
		(*it2dobj)->UpdateMatrix();		// マトリクス更新
		++it2dobj;						// イテレーターをずらす
	}

	auto ituiobj = m_UiObject.begin();
	while (ituiobj != m_UiObject.end())
	{
		// nullチェック
		if ((*ituiobj) == nullptr)
		{
			ituiobj = m_UiObject.erase(ituiobj);	// リストから要素を削除
			m_nCount--;
			continue;
		}
		(*ituiobj)->Update();
		// オブジェクト破棄した場合にイテレーターの指す場所がなくなるので補正
		if ((*ituiobj) == nullptr)
		{
			ituiobj = m_UiObject.erase(ituiobj);	// 2Dlistから要素を削除
			m_nCount--;								// オブジェクト総数減少
			continue;
		}

		++ituiobj;
	}
}

// ---描画---
void ObjectManager::Draw()
{
	O3DIT it3dobj;				// 3Dオブジェクトリスト(以下3Dlist)のイテレーター宣言
	it3dobj = m_3dObject.begin();		// 3Dlistイテレーターをリストの先頭に移動

	// ===条件ループ===
	while (it3dobj != m_3dObject.end())	// --3Dlistの終端まで
	{
		// nullチェック
		if ((*it3dobj) == nullptr)
		{
			it3dobj = m_3dObject.erase(it3dobj);	// 2Dlistから要素を削除
			m_nCount--;								// オブジェクト総数減少
			continue;
		}
		(*it3dobj)->Draw();		// 3Dオブジェクト更新
		// オブジェクト破棄した場合にイテレーターの指す場所がなくなるので補正
		if ((*it3dobj) == nullptr)
		{
			it3dobj = m_3dObject.erase(it3dobj);	// 2Dlistから要素を削除
			m_nCount--;								// オブジェクト総数減少
			continue;
		}
		++it3dobj;						// イテレーターをずらす
	}

	O2DIT it2dobj;				// 2Dオブジェクトリスト(以下2Dlist)のイテレーター宣言
	it2dobj = m_2dObject.begin();		// 2Dlistイテレーターをリストの先頭に移動
	SetZBuffer(false);
	// ===条件ループ===
	while (it2dobj != m_2dObject.end())
	{
		// nullチェック
		if ((*it2dobj) == nullptr)
		{
			it2dobj = m_2dObject.erase(it2dobj);	// 2Dlistから要素を削除
			m_nCount--;								// オブジェクト総数減少
			continue;
		}
		(*it2dobj)->Draw();		// 2Dオブジェクト更新

		// オブジェクト破棄した場合にイテレーターの指す場所がなくなるので補正
		if ((*it2dobj) == nullptr)
		{
			it2dobj = m_2dObject.erase(it2dobj);	// 2Dlistから要素を削除
			m_nCount--;								// オブジェクト総数減少
			continue;
		}
		++it2dobj;						// イテレーターをずらす
	}

	auto ituiobj = m_UiObject.begin();
	while (ituiobj != m_UiObject.end())
	{
		// nullチェック
		if ((*ituiobj) == nullptr)
		{
			ituiobj = m_UiObject.erase(ituiobj);	// リストから要素を削除
			m_nCount--;
			continue;
		}
		(*ituiobj)->Draw();
		++ituiobj;
	}
	SetZBuffer(true);
}

// ===3Dオブジェクトを追加===
void ObjectManager::AddObjectManager(Object3DBase* obj3d)
{
	m_3dObject.push_back(obj3d);		// 3Dオブジェクトのリストに追加
	++m_nCount;							// オブジェクト総数加算
}

// ===2Dオブジェクトを追加===
void ObjectManager::AddObjectManager(Object2DBase* obj2d)
{
	m_2dObject.push_back(obj2d);		// 2Dオブジェクトのリストに追加
	++m_nCount;							// オブジェクト総数加算
}

void ObjectManager::AddObjectManager(UIBase* objui)
{
	m_UiObject.push_back(objui);		// UIオブジェクトリストに追加
	++m_nCount;							// オブジェクト総数加算
}

// ===全オブジェクト解放===
void ObjectManager::Release()
{
	// ---3Dオブジェクト解放
	O3DIT it3dobj;				// 3Dオブジェクトリスト(以下3Dlist)のイテレーター宣言
	it3dobj = m_3dObject.begin();				// 3Dlistイテレーターをリストの先頭に移動

		// ===条件ループ===
	while (it3dobj != m_3dObject.end())	// --3Dlistの終端まで
	{
		delete *it3dobj;						// オブジェクト削除
		++it3dobj;
	}

	// ---2Dオブジェクト解放
	O2DIT it2dobj;				// 2Dオブジェクトリスト(以下2Dlist)のイテレーター宣言
	it2dobj = m_2dObject.begin();				// 2Dlistイテレーターをリストの先頭に移動

		// ===条件ループ===
	while (it2dobj != m_2dObject.end())	// --2Dlistの終端まで
	{
		delete *it2dobj;						// オブジェクト削除
		++it2dobj;
	}

	// ---UIオブジェクト解放
	auto ituiobj = m_UiObject.begin();

	while (ituiobj != m_UiObject.end())
	{
		delete *ituiobj;
		++ituiobj;
	}
}

// ===指定した3Dオブジェクトを解放===
void ObjectManager::Release(Object3DBase* obj3d)
{
	if (m_3dObject.size() <= 0)
		return;
	O3DIT it3dobj;							// 3Dオブジェクトリスト(以下3Dlist)のイテレーター宣言
	it3dobj = m_3dObject.begin();			// 3Dlistイテレーターをリストの先頭に移動

	// ===条件ループ===
	while(*it3dobj != obj3d)	// --3Dlistのオブジェクトと指定のオブジェクトが一致するまで実行
	{
		++it3dobj;							// イテレーターをずらす	
	}

	it3dobj = m_3dObject.erase(it3dobj);	// 3Dlistから要素を削除
	m_nCount--;							// オブジェクト総数減少
}

// ===指定した2Dオブジェクトを解放===
void ObjectManager::Release(Object2DBase* obj2d)
{
	if (m_2dObject.size() <= 0)
		return;
	O2DIT it2dobj;							// 2Dオブジェクトリスト(以下2Dlist)のイテレーター宣言
	it2dobj = m_2dObject.begin();			// 2Dlistイテレーターをリストの先頭に移動

	// ===条件ループ===
	while (*it2dobj != obj2d)	// --2Dlistのオブジェクトと指定のオブジェクトが一致するまで実行
	{
		++it2dobj;							// イテレーターをずらす	
	}

	it2dobj = m_2dObject.erase(it2dobj);	// 2Dlistから要素を削除
	m_nCount--;								// オブジェクト総数減少
}

// ===指定したUIオブジェクト解放===
void ObjectManager::Release(UIBase* objui)
{
	// ---例外処理---
	if (m_UiObject.size() <= 0)
		return;

	// ---解放処理---
	auto ituiobj = m_UiObject.begin();		// UIオブジェクトリストの先頭イテレータ取得
	while (*ituiobj != objui)	// --2Dlistのオブジェクトと指定のオブジェクトが一致するまで実行
	{
		++ituiobj;							// イテレーターをずらす	
	}
	ituiobj = m_UiObject.erase(ituiobj);	// 2Dlistから要素を削除
	m_nCount--;								// オブジェクト総数減少
}