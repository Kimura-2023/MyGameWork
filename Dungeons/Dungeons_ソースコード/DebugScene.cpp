// ===デバッグ用シーンクラス実装===
// デバッグ用シーンクラスを実装
// ---Author kimura
// =================================

// ===インクルード===
#include "DebugScene.h"
#include "SceneManager.h"
#include "DebugProc.h"
#include "Input.h"
#include "GameManager.h"
#include "GameCamera.h"
#include "TimeController.h"
#include "Calc.h"
#include "MapManager.h"
#include "CameraManager.h"
#include "ImGuiTest.h"
#include "ImGuiSceneEditer.h"
#include "CollisionManager.h"
#include "Fade.h"
#include "Sound.h"

// ===構造体定義===
struct ViewSetting					// ---カメラ設定
{
	DirectX::XMFLOAT3 camPos;		// カメラ座標
	DirectX::XMFLOAT3 camLook;		// カメラ注視点
	DirectX::XMFLOAT3 camUp;		// カメラのアップベクトル
	char SceneName[50];				// シーン名
};

// ===コンストラクタ===
DebugScene::DebugScene()
{
	m_Name = "DebugScene";
	m_HitZ = 0.0f;

	MapManager::Create();
	CameraManager::Get()->RegisterCamera("DebugCamera", DEBUG_CAMERA);
	CameraBase* pCamera = CameraManager::Get()->FindCamera("DebugCamera");

	ViewSetting setting =
	{
		pCamera->GetPos(),
		pCamera->GetLook(),
		pCamera->GetUp()
	};

	strcpy(setting.SceneName, m_Name.c_str());

	CameraManager::Get()->SetCamera(CameraManager::Get()->FindCamera("DebugCamera"));

	std::vector<std::vector<Room*>> pRooms = MAP->GetRooms();

	new ImGuiSceneEditer;
	LoadSceneData();

	CSound::SetVolume(BGM_GAME, 0.05f);
	CSound::SetVolume(SE_SWORD, 0.05f);
	CSound::SetVolume(SE_EXPLOSION, 0.05f);
	CSound::SetVolume(SE_DEFEND, 0.05f);
	CSound::SetVolume(SE_GAMEOVER, 0.05f);
	CSound::SetVolume(SE_SHOT, 0.05f);
	CSound::SetVolume(SE_REPEL, 0.05f);
	CSound::SetVolume(SE_AVOID, 0.05f);
}

// ===デストラクタ===
DebugScene::~DebugScene()
{
	MapManager::Destroy();		// マップ破棄
	SaveSceneData();			// シーンデータを保存
}

// ===更新処理===
void DebugScene::Update()
{
	// ---マップクリア---
	if (CInput::GetKeyTrigger(VK_RETURN)) MAP->SetClear(true);

	// ---カメラ座標データ保存---
	if (CInput::GetKeyPress(VK_CONTROL))
	{
		if(CInput::GetKeyTrigger(VK_S))
		{
			CameraBase* pCamera = CameraManager::Get()->GetCurrentCamera();		// カメラ取得

			// ---カメラ情報取得---
			ViewSetting setting =
			{
				pCamera->GetPos(),		// 座標
				pCamera->GetLook(),		// 注視点
				pCamera->GetUp()		// 上方ベクトル
			};

			strcpy(setting.SceneName, m_Name.c_str());			// シーン名をコピー
			FILE* fp;
			fopen_s(&fp, "data/camera/setting.dat", "wb");		// ファイルオープン
			if (fp)
			{
				fwrite(&setting, sizeof(ViewSetting), 1, fp);	// ファイル書き込み
				fclose(fp);										// ファイルクローズ
			}
		}
	}

	CheckSelectObject();		// オブジェクト選択更新
}

// ===描画処理===
void DebugScene::Draw()
{
	SceneBase::Draw();		// 通常描画
}

// ===選択オブジェクト判定===
void DebugScene::CheckSelectObject()
{
	// ---変数宣言---
	XMFLOAT3	PQ, P, V;
	XMVECTOR	vPQ, vV;
	float		Zval, PQv, d, PQd, Vd;

	// ---座標取得---
	POINT mouse = GAME->GetMousePoint();					// カーソル位置(スクリーン上)取得
	CameraBase* pCamera = CAMERA->GetCurrentCamera();		// 現在のカメラを取得

	XMFLOAT3& E = pCamera->GetPos();						// カメラの位置を取得
	Object* pObj = ImGuiSceneEditer::m_pSelectObj;			// 選択中のオブジェクトを取得
	Object* pNear = nullptr;								// マウスカーソルで触れているオブジェクト(後に判断するため初期化)

	if (pCamera)
	{
		// マウスカーソルの位置をワールド座標化(ベクトル算出)
		XMStoreFloat3(&P, XMVector3Unproject(XMVectorSet((float)mouse.x, (float)mouse.y, 0.0f, 1.0f),
			GetViewPort().TopLeftX, GetViewPort().TopLeftY, GetViewPort().Width, GetViewPort().Height, GetViewPort().MinDepth, GetViewPort().MaxDepth,
			XMLoadFloat4x4(&pCamera->GetProjMatrix()), XMLoadFloat4x4(&pCamera->GetViewMatrix()),
			XMMatrixIdentity()));
	}

	// ---オブジェクト取得---
	auto objmap = GetAllObjects();				// シーン上のオブジェクトをすべて取得
	auto it = objmap->begin();					// イテレーター初期化

	// ---判定---
	while (it != objmap->end())
	{
		// ---除外判定---
		if (!it->second->GetVisible())			// 可視状態かどうか
		{
			++it;
			continue;
		}

		// ---カメラ視点から見たオブジェクトとの距離算出---
		XMFLOAT3 Q = it->second->GetTransform().Pos;		// オブジェクトの位置を取得
		XMStoreFloat(&Zval, XMVector3Length(XMVectorSet(Q.x - E.x, Q.y - E.y, Q.z - E.z, 1.0f)));		// 距離を三主塔

		// ---既に設定されている目標より後ろだった場合---
		if (pNear != nullptr && Zval >= m_HitZ) {
			++it;											// 選択対象にならない
			continue;
		}

		// ---マウス位置へのベクトル算出---
		V.x = P.x - E.x;
		V.y = P.y - E.y;
		V.z = P.z - E.z;

		// ---点から直線までの距離を算出---
		PQ.x = Q.x - P.x;
		PQ.y = Q.y - P.y;
		PQ.z = Q.z - P.z;

		vPQ = XMLoadFloat3(&PQ);
		vV = XMLoadFloat3(&V);
		XMStoreFloat(&PQv, XMVector3Dot(vPQ, vV));
		XMStoreFloat(&PQd, XMVector3Dot(vPQ, vPQ));
		XMStoreFloat(&Vd, XMVector3Dot(vV, vV));
		d = sqrtf(PQd - PQv * PQv / Vd);

		// ---オブジェクト半径より小さい(カーソルがオブジェクトに合っている)---
		if (d < it->second->GetRad()) {
			m_HitZ = Zval;				// 視点からの距離を更新
			pNear = it->second;			// 近くのオブジェクトを更新


			// ---入力---
			if (CInput::GetMouseTrigger(MOUSEBUTTON_R))		// 右クリック時
			{
				// ---選択しているオブジェクトがあるか判断---
				if (pObj)									// 選択中のオブジェクトがある場合
				{
					pObj->SetSelect(false);
					// 選択オブジェクト更新
					pObj = it->second;
					pObj->SetSelect(true);
					ImGuiSceneEditer::m_pSelectObj = pObj;
				}
				else										// ない場合
				{
					pObj = it->second;						// 選択オブジェクト更新
					pObj->SetSelect(true);
					ImGuiSceneEditer::m_pSelectObj = pObj;	// エディタにも更新を反映
				}
			}

			if (CInput::GetKeyPress(VK_SHIFT) || CInput::GetKeyPress(VK_CONTROL))
			{
				if (CInput::GetMouseTrigger(MOUSEBUTTON_L))		// 左クリック時
				{
					// ---選択しているオブジェクトがあるか判断---
					if (pObj)									// 選択中のオブジェクトがある場合
					{
						pObj->SetSelect(false);
						// 選択オブジェクト更新
						pObj = it->second;
						pObj->SetSelect(true);
						ImGuiSceneEditer::m_pSelectObj = pObj;
					}
					else										// ない場合
					{
						pObj = it->second;						// 選択オブジェクト更新
						pObj->SetSelect(true);
						ImGuiSceneEditer::m_pSelectObj = pObj;	// エディタにも更新を反映
					}
				}
			}
		}
		++it;
	}

	// ---マウスカーソルがどのオブジェクトにも接地していない場合---
	if (pNear)
		return;

	// ---入力---
	if (CInput::GetMouseTrigger(MOUSEBUTTON_R))				// 右クリック時
	{
		if (pObj)											// セレクト状態のオブジェクトがあれば
		{
			pObj->SetSelect(false);							// 選択解除
			// 選択オブジェクト更新
			ImGuiSceneEditer::m_pSelectObj = nullptr;		// エディタ上でも選択をはずす
		}
	}
	if (CInput::GetKeyPress(VK_SHIFT) || CInput::GetKeyPress(VK_CONTROL))		// 左クリックはウィンドウを触るときと重複することがあるのでキー入力をはさむ
	{
		if (CInput::GetMouseTrigger(MOUSEBUTTON_L))			// 左クリック時
		{
			if (pObj)										// セレクト状態のオブジェクトがあれば
			{
				pObj->SetSelect(false);						// 選択を解除
				// 選択オブジェクト更新
				ImGuiSceneEditer::m_pSelectObj = nullptr;	// エディタ上で選択をはずす
			}
		}
	}
}