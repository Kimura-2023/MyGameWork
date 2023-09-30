// ===ゲーム管理クラス 実装 GameManager.cpp
// ゲーム管理クラスを実装
// ---Author kimura
// =======================================

// ===インクルード===
#include "GameManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "Fade.h"
#include "ModelManager.h"
#include <time.h>
#include "TextureManager.h"
#include "DebugProc.h"
#include "TimeController.h"
#include "ShaderList.h"
#include "CameraManager.h"
#include "EffectManager.h"
#include "SelectButtonManager.h"
#include "ScoreManager.h"

// ===グローバル宣言===
GameManager* GameManager::m_pInstance = nullptr;		// インスタンス初期化

// ===コンストラクタ===
GameManager::GameManager()
{
	srand(time(NULL));				// 乱数初期化
	InitShaderList();				// 使用シェーダーリスト初期化
	TimeController::Create();		// 時間マネージャ生成
	TextureManager::Create();		// テクスチャマネージャ生成
	ModelManager::Create();			// モデルマネージャ生成
	EffectManager::Create();		// エフェクトマネージャ生成生成
	CollisionManager::Create();		// 当たり判定マネージャ生成
	ObjectManager::Create();		// オブジェクトマネージャ生成
	CameraManager::Create();		// カメラマネージャ生成
	SelectButtonManager::Create();	// ボタンマネージャ生成
	ScoreManager::Create();			// スコアマネージャ生成
	SceneManager::Create();			// シーンマネージャ生成
	CFade::Init();					// フェード状態初期化
}

// ===デストラクタ===
GameManager::~GameManager()
{
	SceneManager::Destroy();		// シーンマネージャ破棄
	ScoreManager::Destroy();		// スコアマネージャ破棄
	SelectButtonManager::Destroy();	// ボタンマネージャ破棄
	CameraManager::Destroy();		// カメラマネージャ破棄
	ObjectManager::Destroy();		// オブジェクトマネージャ破棄
	CollisionManager::Destroy();	// 当たり判定マネージャ破棄
	EffectManager::Destroy();		// エフェクトマネージャ破棄
	ModelManager::Destroy();		// モデルマネージャ破棄
	TextureManager::Destroy();		// テクスチャマネージャ破棄
	TimeController::Destroy();		// 時間マネージャ破棄
	UninitShaderList();				// シェーダー解放
}

// ===生成===
void GameManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new GameManager;		// インスタンスを生成
	}
}

// ===破棄===
void GameManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;					// インスタンスを破棄
		m_pInstance = nullptr;				// nullを格納
	}
}

// ===取得===
GameManager* GameManager::Get()
{
	return m_pInstance;						// ゲームマネージャのインスタンスを返す
}

// ===更新===
void GameManager::Update()
{
	CFade::Update();						// フェード状態更新
	CAMERA->Update();						// カメラ更新処理
	TIME->Update();							// 時間更新処理

	// ---停止処理---
	if (!TIME->GetUpdateFlg())				// 停止時間が動作中なら更新しない
		return;

	// ---マウス座標更新---
	m_Point = *CInput::GetMousePosition();					// カーソル位置取得
	CameraBase* pCamera = CAMERA->GetCurrentCamera();		// 現カメラ情報取得
	if(pCamera)
	{
		// マウスカーソルの位置をワールド座標化(ベクトル算出)
		XMStoreFloat3(&m_MouseWorldPos, XMVector3Unproject(XMVectorSet((float)m_Point.x, (float)m_Point.y, 0.0f, 1.0f),
			GetViewPort().TopLeftX, GetViewPort().TopLeftY, GetViewPort().Width, GetViewPort().Height, GetViewPort().MinDepth, GetViewPort().MaxDepth,
			XMLoadFloat4x4(&pCamera->GetProjMatrix()), XMLoadFloat4x4(&pCamera->GetViewMatrix()),
			XMMatrixIdentity()));
	}

	// ---ゲーム関連更新---
	OBJECT->Update();						// オブジェクトの更新
	SCENE->Update();						// シーンの更新
	COLLISION->Update();					// 当たり判定の更新
	EFFECT->Update();						// エフェクト更新
	BUTTON->Update();						// ボタン更新
}

// ===描画処理===
void GameManager::Draw()
{
	SCENE->Draw();							// シーン事前描画(深度値、シャドウマップ)
	OBJECT->Draw();							// オブジェクト描画					
	EFFECT->Draw();							// エフェクト描画
	COLLISION->Draw();						// 当たり判定描画(デバッグ表示)
	CFade::Draw();							// フェード描画
}

// ===XZ平面上のマウス座標(ワールド座標)取得===
XMFLOAT3 GameManager::MousePos()
{
	// ---ローカル定義---
	XMFLOAT3 cam, mouse, vec;						// カメラ座標、マウス座標、カメラ座標からマウス座標へのベクトル
	cam = CAMERA->GetCurrentCamera()->GetPos();		// 現在カメラから座標取得
	mouse = m_MouseWorldPos;						// マウス座標取得
	XMVECTOR cpos = XMLoadFloat3(&cam);				// XMVECTOR型に変換
	XMFLOAT3 normal = { 0.0f,1.0f,0.0f };			// 平面の法線
	XMVECTOR n = XMLoadFloat3(&normal);				// 正規化
	XMFLOAT3 floorpos = { 0.0f,0.0f,0.0f };			// 平面座標

	Object* obj = SCENE->GetCurrentScene()->FindGameObject("Player");		// プレイヤーオブジェクトを取得
	if (obj != nullptr)
	{
		floorpos.y = obj->GetPos().y;			// 基準の座標をプレイヤーのy座標に設定
	}
	XMVECTOR f = XMLoadFloat3(&floorpos);		// XMVECTOR型に変換

	vec.x = mouse.x - cam.x;					// カメラ座標からマウス座標へのベクトル(x方向)
	vec.y = mouse.y - cam.y;					// カメラ座標からマウス座標へのベクトル(y方向)
	vec.z = mouse.z - cam.z;					// カメラ座標からマウス座標へのベクトル(z方向)
	XMVECTOR v = XMLoadFloat3(&vec);			// XMVECTOR型に変換
	v = DirectX::XMVector3Normalize(v);			// ベクトルを正規化
	f = f - cpos;								// カメラ座標から平面座標へのベクトル
	XMFLOAT3 pos;								// 格納用変数
	XMStoreFloat3(&pos, cpos + ((XMVector3Dot(n, f)) / (XMVector3Dot(n, v)) * v));		// マウス座標(ワールド空間)計算

	return pos;		// 求めた座標を返す
}