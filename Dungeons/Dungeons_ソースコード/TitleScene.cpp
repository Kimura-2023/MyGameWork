// ===タイトルシーンクラス実装===
// タイトルシーンクラスを実装
// Author:	Kimura
// ==========================

// ===インクルード===
#include "TitleScene.h"
#include "SceneManager.h"
#include "DebugProc.h"
#include "Input.h"
#include "CameraManager.h"
#include "Sound.h"

// ===カメラ設定===
struct ViewSetting
{
	DirectX::XMFLOAT3 camPos;		// 視点
	DirectX::XMFLOAT3 camLook;		// 注視点
	DirectX::XMFLOAT3 camUp;		// 上方ベクトル
	char SceneName[50];				// シーン名
};

// ===定数定義===
#define CAMERA_FILENAME ("data/camera/setting.dat")			// カメラパラメータファイル名

// ===コンストラクタ===
TitleScene::TitleScene()
{
	// ---初期化---
	m_Name = "TitleScene";			// シーン名

	CameraManager::Get()->RegisterCamera("TitleCamera", TITLE_CAMERA);			// カメラ生成
	CameraBase* pCamera = CameraManager::Get()->FindCamera("TitleCamera");		// 生成したカメラのアドレスを取得

	// ---カメラのパラメータ初期化---
	ViewSetting setting =
	{
		pCamera->GetPos(),		// 座標
		pCamera->GetLook(),		// 注視点
		pCamera->GetUp()		// 上方ベクトル
	};

	strcpy(setting.SceneName, m_Name.c_str());

	// ---ファイル読込---
	FILE* fp;
	fopen_s(&fp, CAMERA_FILENAME, "rb");
	if (fp)
	{
		fread(&setting, sizeof(ViewSetting), 1, fp);
		fclose(fp);
	}
	pCamera->SetPos(setting.camPos);			// 読み込んだデータから座標を設定
	pCamera->SetTarget(setting.camLook);		// 読み込んだデータから注視点を設定
	pCamera->SetUpVector(setting.camUp);		// 読み込んだデータから上方ベクトルを設定
	pCamera->UpdateMatrix();					// 設定したパラメータで更新
	CameraManager::Get()->SetCamera(CameraManager::Get()->FindCamera("TitleCamera"));		// カメラをセット

	LoadSceneData();													// シーンのデータを読込
	CreateGameObject("ButtonTest",UI_START_BUTTON);						// ゲーム開始ボタン生成
	Object* button = CreateGameObject("ButtonTest2", UI_FIN_BUTTON);	// ゲーム終了ボタン生成
	button->SetPos(XMFLOAT3(0.0f, -100.0f, 0.0f));						// ボタン座標設定

	CreateGameObject("HiScore", UI_SCORE);								// ハイスコアUI生成
}

// ===デストラクタ===
TitleScene::~TitleScene()
{
}

// ===更新処理===
void TitleScene::Update()
{
}

// ===描画処理===
void TitleScene::Draw()
{
	SceneBase::Draw();		// 描画
}