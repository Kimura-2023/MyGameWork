// ===カメラ管理クラス実装===
// カメラ管理クラスを実装
// Author Kimura
// ==============================

// ===インクルード===
#include "CameraManager.h"
#include "DebugCamera.h"
#include "GameCamera.h"
#include "TitleCamera.h"

// ===グローバル宣言===
CameraManager* CameraManager::m_pInstance = nullptr;			// インスタンス初期化

// ===コンストラクタ===
CameraManager::CameraManager() : m_CurrentCamera(nullptr)
{
}

// ===デストラクタ===
CameraManager::~CameraManager()
{
	Release();			// カメラ解放処理
}

// ===インスタンス生成===
void CameraManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new CameraManager;		// マネージャのインスタンス生成
	}
}

// ===インスタンス破棄===
void CameraManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;						// マネージャのインスタンス破棄
		m_pInstance = nullptr;					// nullptrセット
	}
}

// ===インスタンス取得===
CameraManager* CameraManager::Get()
{
	return m_pInstance;							// インスタンスを返す
}

// ===カメラ管理追加===
void CameraManager::RegisterCamera(std::string name,CameraType type)
{
	auto it = m_CameraList.find(name);			// カメラリストからカメラを探索
	if (it == m_CameraList.end())				// 存在しない場合
	{
		m_CameraList.insert(std::pair<std::string, CameraBase*>(name, CreateCamera(type)));		// カメラを生成し、登録
	}
}

// ===カメラ解放===
void CameraManager::Release()
{
	auto it = m_CameraList.begin();				// カメラリストの先頭イテレータを取得
	while (it != m_CameraList.end())			// 終端イテレーターまで繰り返し
	{
		delete it->second;		// 解放処理
		++it;					// イテレーターをずらす
	}
}

// ===更新===
void CameraManager::Update()
{
	// ---現在カメラの更新---
	if(m_CurrentCamera)				// 現在のカメラが存在しない場合は何もしない
		m_CurrentCamera->Update();	// 更新処理
}

// ===描画===
void CameraManager::Draw()
{
	if (m_CurrentCamera)			// 現在のカメラが存在しない場合は何もしない
		m_CurrentCamera->Clear();	// 描画処理(クリア)
}

// ===カメラ探索処理===
CameraBase* CameraManager::FindCamera(std::string name)
{
	// ---カメラ探索---
	auto it = m_CameraList.find(name);		// カメラのリストから探索
	if (it != m_CameraList.end())			// 存在している場合
	{
		return it->second;		// 見つかったカメラを返す
	}
	else									// 存在しない場合
	{
		return nullptr;			// nullptrを返す
	}
}

// ===カメラ生成===
CameraBase* CameraManager::CreateCamera(CameraType type)
{
	CameraBase* pCamera;		// ローカル

	// ---種別から分岐---
	switch (type)
	{
	case GAME_CAMERA:			// 種別がGAME_CAMERAの場合
		pCamera = new GameCamera();		// ゲームシーンカメラを生成
		return pCamera;					// 生成したカメラを返す
		break;
	case DEBUG_CAMERA:			// 種別がDEBUG_CAMERAの場合
		pCamera = new DebugCamera();	// デバッグシーンカメラを生成
		return pCamera;					// 生成したカメラを返す
		break;
	case TITLE_CAMERA:			// 種別がTITLE_CAMERAの場合
		pCamera = new TitleCamera();	// タイトルシーンカメラを生成
		return pCamera;					// 生成したカメラを返す
		break;
	default:					// 種別が定義したもの以外の場合
		break;
	}
	return nullptr;		// どの定義にも該当しないのでnullptrを返す
}