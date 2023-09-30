// ===シーンベースクラス実装===
// シーンベースクラスを実装
// Author:	Kimura
// ==========================

// ===インクルード===
#include "SceneBase.h"
#include <string>
#include "ObjectFactory.h"
#include "CameraManager.h"
#include "GameCamera.h"
#include "ImGuiController.h"
#include "TextureFactory.h"
#include "RenderTarget.h"
#include "ShaderList.h"
#include "Door.h"
#include "MapManager.h"

// ===定数定義===
#define MAX_SCENE_OBJECT (1000)				// シーン最大オブジェクト数

// ===構造体定義===
struct SceneObjData
{
	ObjType id;					// オブジェクトの型
	Transform transform;		// 座標等
	char name[50];				// 保存した名前
	char pass[100];				// 素材のパス
	bool enable;				// 更新、描画の有無
	bool sta;					// 静的なオブジェクトかどうか(描画のみ)
};
#pragma pack()

// ===コンストラクタ===
SceneBase::SceneBase() : m_Name("Scene"),m_pCBCamera(nullptr),m_pCBColor(nullptr),m_pCBlight(nullptr),m_LightDir(0.3f,-1.0f,0.3f,0.0f),Color(0.0f,0.0f,0.0f,1.0f)
{
	// ---カメラ用定数バッファ作成---
	m_pCBCamera = new ConstantBuffer;
	m_pCBCamera->Create(sizeof(XMFLOAT4));

	// ---色用定数バッファ作成---
	m_pCBColor = new ConstantBuffer;
	m_pCBColor->Create(sizeof(XMFLOAT4));

	// ---ライト用定数バッファ作成---
	m_pCBlight = new ConstantBuffer;
	m_pCBlight->Create(sizeof(XMFLOAT4));

	// ---影用定数バッファ作成---
	m_pCBSun = new ConstantBuffer;
	m_pCBSun->Create(sizeof(XMFLOAT4X4) * 2);

	// ---ワールド変換行列用バッファ作成---
	m_pWVP = new ConstantBuffer;
	m_pWVP->Create(sizeof(XMFLOAT4X4) * 3);

	m_pCBColor->Write(&Color);			// 輪郭線色をセット

	// ---シャドウマップ生成用---
	m_pCanvas = TextureFactory::CreateRenderTarget(DXGI_FORMAT_R32_FLOAT,1280.0f, 720.0f);								// テクスチャ生成
	m_pDepth = reinterpret_cast<DepthStencil*>(TextureFactory::CreateDepthStencil(SCREEN_WIDTH, SCREEN_HEIGHT, true));	// デプスステンシル生成

	// ---プレイヤー奥行描画用---
	m_pCanvasObject = TextureFactory::CreateRenderTarget(DXGI_FORMAT_R32_FLOAT, 1280.0f, 720.0f);
}

// ===デストラクタ===
SceneBase::~SceneBase()
{
	// ---オブジェクト解放---
	ReleaseAllGameObject();
	ImGuiController::ReleaseAll();

	// ---テクスチャ解放---
	delete m_pCanvasObject;
	delete m_pDepth;
	delete m_pCanvas;

	// ---定数バッファ解放---
	delete m_pWVP;
	delete m_pCBSun;
	delete m_pCBlight;
	delete m_pCBColor;
	delete m_pCBCamera;
}


// ===オブジェクト探索===
Object* SceneBase::FindGameObject(std::string name)
{
	std::map<std::string, Object*>::iterator it;		// ワーク用
	it = m_pObjectList.find(name);		// オブジェクトリスト(連想配列)から名前をキーに探索
	if (it != m_pObjectList.end())	// ---見つかった場合
	{
		return it->second;			// そのオブジェクトを返す
	}
	else							// ---見つからなかった場合
		return nullptr;				// nullptrを返す
}

// ===オブジェクト探索(タグから)===
Object* SceneBase::FindObjectWithTag(ObjType id)
{
	auto it = m_pObjectList.begin();		// オブジェクトの先頭イテレータ取得
	while (it != m_pObjectList.end())	// 終端まで繰り返し
	{
		if (it->second->GetObjID() == id)	// 引数のタグと一致した場合
			return it->second;			// ヒットしたオブジェクトを返す

		++it;		// イテレータをずらす
	}

	return nullptr;		// 終端までたどり着いているのでタグが一致するオブジェクトなし
}

// ===オブジェクトリスト探索(タグから)===
std::vector<Object*> SceneBase::FindObjectListWithTag(ObjType id)
{
	std::vector<Object*> Objects;			// オブジェクト格納用動的配列
	auto it = m_pObjectList.begin();		// オブジェクトリストの先頭イテレータ取得
	while (it != m_pObjectList.end())	// 終端まで繰り返し
	{
		if (it->second->GetObjID() == id)	// 引数のタグと一致する場合
			Objects.push_back(it->second);	// 対応オブジェクトリストに追加
		++it;			// イテレータをずらす
	}
	return Objects;			// 格納したオブジェクトを返す
}

// ===オブジェクト生成===
Object* SceneBase::CreateGameObject(std::string name, ObjType type,int num)
{
	std::string ReName = name;		// 生成名
	// ---オブジェクト名から既存のオブジェクトかどうか判断する---
	int index = num;		// 個数
	if (index != 0)
	{
		ReName = name + std::to_string(num);	// 名前に番号を追加
		++index;								// 番号を1増やす(例)Object0という名前のオブジェクトがある状態でObject0という名前で生成するとObject1という名前にリネームさせる
	}

	Object* pObj = nullptr;
	std::map<std::string, Object*>::iterator it = m_pObjectList.begin();

	// ---生成済みで使用していないオブジェクトを再利用する---
	while (it != m_pObjectList.end())
	{
		// ---生成するオブジェクトのタグが一致するオブジェクトを探索---
		if(type != it->second->GetObjID())	// 一致しない場合は次のオブジェクトを探索
		{
			++it;		// イテレータをずらす
			continue;	// ループ
		}

		// ---タグが一致するオブジェクトを発見した後、更新・描画をしていないオブジェクトかどうかを判定する---
		if (!it->second->GetEnable())		// ---更新・描画をしていないオブジェクトの場合
		{
			pObj = it->second;				// 生成する代わりにそのオブジェクトを再利用する
			break;		// 再利用するオブジェクトが決まったのでループから抜ける
		}
		++it;			// イテレータをずらす
	}

	// ---オブジェクトを生成---
	if (it == m_pObjectList.end())						// 再利用するオブジェクトが見つからなかった場合新規でオブジェクトを生成する
	{
		while (FindGameObject(ReName))					// 生成済みの名前かどうかを探索
		{
			ReName = name + std::to_string(index);		// リネームするためにオブジェクト名に番号を付け足す
			++index;									// 番号を加算
		}

		pObj = ObjectFactrory::CreateObject(type);								// オブジェクトを生成
		pObj->SetName(ReName);													// リネーム
		m_pObjectList.insert(std::pair<std::string, Object*>(ReName, pObj));	// オブジェクトリスト(連想配列)に登録する
	}

	// ---例外処理---
	if (!pObj)
		return nullptr;			// オブジェクトが生成できなかったのでnullptrを返す

	pObj->SetEnable(true);		// 更新・描画を開始させる
	return pObj;				// 生成したオブジェクトを返す
}

// ===オブジェクト破棄===
bool SceneBase::DeleteGameObject(std::string name)
{
	// --- オブジェクトリストから探索---
	auto it = m_pObjectList.find(name);

	// ---オブジェクトがシーンに存在していない場合---
	if (it == m_pObjectList.end())
		return false;		// 破棄失敗

	// ---オブジェクトがカメラの追従オブジェクト対象だった場合---
	if (it->second == ((GameCamera*)CAMERA->GetCurrentCamera())->GetTargetObj())
	{
		((GameCamera*)CAMERA->GetCurrentCamera())->SetTargetObj(nullptr);			// そのカメラの追従オブジェクトにnullptrをセットする
	}

	// ---オブジェクト削除---
	delete it->second;				// メモリ解放
	it->second = nullptr;
	m_pObjectList.erase(name);		// リストから除外

	return true;			// 破棄成功
}

// ===全オブジェクト解放===
void SceneBase::ReleaseAllGameObject()
{
	// ---全オブジェクト破棄---
	std::map<std::string, Object*>::iterator it = m_pObjectList.begin();
	while (it != m_pObjectList.end())
	{
		// ---オブジェクトが見つかった際にカメラの追従オブジェクト対象だった場合---
		if (it->second == ((GameCamera*)CAMERA->GetCurrentCamera())->GetTargetObj())
		{
			((GameCamera*)CAMERA->GetCurrentCamera())->SetTargetObj(nullptr);			// そのカメラの追従オブジェクトにnullptrをセットする
		}

		delete it->second;				// オブジェクト解放
		it = m_pObjectList.erase(it);	// イテレータをずらす
		continue;
	}
}

// ===範囲内のオブジェクトの数を数える===
int& SceneBase::RangeObjectCount(XMFLOAT3& min, XMFLOAT3& max)
{
	int cnt = 0;							// カウント数
	auto it = m_pObjectList.begin();		// オブジェクトリスト先頭イテレータ取得
	while (it != m_pObjectList.end())	// 終端まで
	{
		// ---判定---
		if (min.x < it->second->GetTransform().Pos.x && it->second->GetTransform().Pos.x <= max.x)
		{
			if (min.y < it->second->GetTransform().Pos.y && it->second->GetTransform().Pos.y <= max.y)
			{
				if (min.z < it->second->GetTransform().Pos.z && it->second->GetTransform().Pos.y <= max.z)
				{
					cnt++;		// カウント増加
				}
			}
		}

		++it;	// イテレータをずらす
	}
	return cnt;		// カウント数を返す
}

// ===シーンデータ保存===
void SceneBase::SaveSceneData(std::string filename)
{
	// ---オブジェクトデータ取得---
	std::vector<SceneObjData> Objects;			// オブジェクト保存用配列
	SceneObjData work;							// 変換用
	auto it = m_pObjectList.begin();			// オブジェクトリストの先頭イテレータ取得
	while (it != m_pObjectList.end())		// 終端まで繰り返し
	{
		work.id = it->second->GetObjID();								// タグを取得
		work.transform = it->second->GetTransform();					// 座標・回転・拡縮を取得
		strcpy(work.name,it->first.c_str());							// 名前を取得
		strcpy(work.pass, it->second->GetResourcePass().c_str());		// 素材のパスを取得
		work.enable = it->second->GetEnable();							// 更新・描画可否を取得
		work.sta = it->second->GetStatic();								// 静的オブジェクトかどうか取得
		Objects.push_back(work);										// オブジェクト保存用配列に追加
		++it;											// イテレータをずらす
	}

	// ---ファイル操作---
	FILE* fp;
	fopen_s(&fp, filename.c_str(), "wb");			// ファイルオープン

	if (fp)
	{
		fwrite(&Objects[0], sizeof(SceneObjData), Objects.size() , fp);		// ファイルに書き込み
	}
	fclose(fp);										// ファイルクローズ
}

// ===シーンデータ保存===
void SceneBase::SaveSceneData()
{
	// ---オブジェクトデータ取得---
	std::vector<SceneObjData> Objects;			// オブジェクト保存用配列
	SceneObjData work;							// 変換用
	auto it = m_pObjectList.begin();			// オブジェクトリストの先頭イテレータ取得
	while (it != m_pObjectList.end())		// 終端まで繰り返し
	{
		work.id = it->second->GetObjID();				// タグを取得
		work.transform = it->second->GetTransform();	// 座標・回転・拡縮を取得
		strcpy(work.name, it->first.c_str());			// 名前を取得
		strcpy(work.pass, it->second->GetResourcePass().c_str());		// 素材のパスを取得
		work.enable = it->second->GetEnable();			// 更新・描画可否を取得
		work.sta = it->second->GetStatic();				// 静的オブジェクトかどうか取得
		Objects.push_back(work);						// オブジェクト保存用配列に追加
		++it;											// イテレータをずらす
	}

	// ---ファイル操作---
	FILE* fp;
	std::string file;
	file = "data/scene/" + m_Name + ".dat";
	fopen_s(&fp, file.c_str(), "wb");				// ファイルオープン

	if (fp)
	{
		fwrite(&Objects[0], sizeof(SceneObjData), Objects.size(), fp);		// ファイルに書き込み
	}
	fclose(fp);										// ファイルクローズ
}

// ===シーンデータ読込===
bool SceneBase::LoadSceneData()
{
	FILE* fp;
	std::string file;
	file = "data/scene/" + m_Name + ".dat";

	// ---ファイルを開く---
	fopen_s(&fp, file.c_str(), "rb");

	// ---読込失敗---
	if (!fp)
		return false;

	// ファイルの末尾に位置指定子を移動---
	fseek(fp, 0, SEEK_END);
	// ファイルの末尾の位置を取得---
	size_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	size /= sizeof(SceneObjData);		// オブジェクト数を取得

	// ---例外処理---
	if (size > MAX_SCENE_OBJECT)
		return false;

	// ---データ読み込み---
	SceneObjData* op = new SceneObjData[size];
	fread(&op[0], sizeof(SceneObjData), size, fp);

	// ---読み込んだデータからオブジェクトを生成---
	for (int i = 0; i < size; i++)
	{
		Object* p = CreateGameObject(op[i].name, op[i].id);		// オブジェクト生成

		// ---例外処理
		if (p == nullptr)
			continue;

		p->SetTransform(op[i].transform);			// 読み込んだ座標、回転、拡縮をセット
		p->SetEnable(op[i].enable);					// 更新・描画の有無をセット
		p->SetStatic(op[i].sta);					// 静的オブジェクトかどうかをセット
		p->SetResource(op[i].pass);					// 素材のパスをセット
	}

	// ---ファイルを閉じる
	fclose(fp);
	delete[] op;
	return true;
}

// ===シーンデータ読込===
bool SceneBase::LoadSceneData(std::string filename)
{
	FILE* fp;
	fopen_s(&fp, filename.c_str(), "rb");		// ファイルオープン
	// ---読込失敗---
	if (!fp)
		return false;

	// ファイルの末尾に位置指定子を移動
	fseek(fp, 0, SEEK_END);
	// ファイルの末尾の位置を取得
	size_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	size /= sizeof(SceneObjData);		// オブジェクト数を取得

	// ---例外処理---
	if (size > MAX_SCENE_OBJECT)
		return false;

	// ---ファイル読込---
	SceneObjData* op = new SceneObjData[size];
	fread(&op[0], sizeof(SceneObjData), size, fp);

	// ---読み込んだデータからオブジェクトを生成---
	for (int i = 0; i < size; i++)
	{
		Object* p = CreateGameObject(op[i].name, op[i].id);		// オブジェクト生成

		// ---例外処理---
		if (p == nullptr)
			continue;

		p->SetTransform(op[i].transform);			// 読み込んだ座標、回転、拡縮をセット
		p->SetEnable(op[i].enable);					// 更新・描画の有無をセット
		p->SetStatic(op[i].sta);					// 静的オブジェクトかどうかをセット
		p->SetResource(op[i].pass);					// 素材のパスをセット
	}

	// ---終了処理---
	fclose(fp);
	delete[] op;
	return true;
}

void SceneBase::LoadStageData(std::string filename, float rot , int level)
{
	// ---現在のオブジェクトを一旦更新、描画無しの状態にする---
	auto it = m_pObjectList.begin();
	while (it != m_pObjectList.end())
	{
		it->second->SetEnable(false);
		++it;
	}

	//---ファイルを読み込む---
	FILE* fp;
	fopen_s(&fp, filename.c_str(), "rb");
	// ---読込失敗---
	if (!fp)
		return;

	// ファイルの末尾に位置指定子を移動
	fseek(fp, 0, SEEK_END);
	// ファイルの末尾の位置を取得
	size_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	size /= sizeof(SceneObjData);		// オブジェクト数を取得

	// ---例外処理
	if (size > MAX_SCENE_OBJECT)
		return;

	SceneObjData* op = new SceneObjData[size];

	// ---ファイル読込
	fread(&op[0], sizeof(SceneObjData), size, fp);

	// ---ファイルを閉じる
	int a = fclose(fp);

	// ---読み込んだデータからオブジェクトを生成---
	for (int i = 0; i < size; i++)
	{
		Object* p = CreateGameObject(op[i].name, op[i].id);		// オブジェクト生成

		// ---例外処理---
		if (p == nullptr)
			continue;

		// ---ステージ回転---
		TransformRotate(&op[i].transform, XMFLOAT3(0.0f, -1.0f, 0.0f), rot);		// 連結している部屋の方向によって回転させる

		p->SetTransform(op[i].transform);			// 読み込んだ座標、回転、拡縮をセット
		p->SetEnable(op[i].enable);					// 更新・描画の有無をセット
		p->SetStatic(op[i].sta);					// 静的オブジェクトかどうかをセット
		p->SetResource(op[i].pass);					// 素材のパスをセット

		if (p->GetObjID() == OT_DOOR)			// 生成したオブジェクトがドアだった場合
			((Door*)p)->SetDirection();			// 向きをセット
	}

	delete[] op;		// メモリ解放

	// ---敵キャラ生成---
	if (!MAP->GetClear() && MAP->GetCurrentRoom()->type != ROOM_GOAL)						// ステージがクリア状態になっていない場合は生成
	{
		int num = Random<int>(1, 4 + level);	// 生成数をランダムに振り分け
		for (int i = 0; i < num; i++)
		{
			Object* enemy;						// 生成用
			if (Random<int>(1, 5) <= 3)			// 3/5の確立で通常敵
			{
				enemy = CreateGameObject("Enemy", OT_ENEMY);		// 通常敵生成
			}
			else								// それ以外は遠距離敵
			{
				enemy = CreateGameObject("BulletEnemy", OT_ENEMY_BULLET);		// 遠距離敵生成
			}
			enemy->SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));							// 向きを正面方向に
			enemy->SetPos(XMFLOAT3(Random<float>(-9.0f, 9.0f), 1.0f, Random<float>(-9.0f, 9.0f)));		// 座標をランダムにセット
		}
	}

	// ---プレイヤー再生成---
	Object* player = FindObjectWithTag(OT_PLAYER);		// プレイヤー取得
	if(player)
		player->SetEnable(true);
	CreateGameObject("Life", UI_LIFE);			// ライフUI再生成
	CreateGameObject("Stamina", UI_STAMINA);	// スタミナUI再生成
	CreateGameObject("Score", UI_SCORE);		// スコアUI再生成
}

// ===全オブジェクト取得===
std::map<std::string, Object*>* SceneBase::GetAllObjects()
{
	return &m_pObjectList;
}

// ===描画処理===
void SceneBase::Draw()
{
	// ---例外処理---
	auto it = m_pObjectList.begin();
	if (it == m_pObjectList.end())
		return;

	// =====各テクスチャに描画する=====
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());

	// ---書き込み先の変更---
	Texture* pCanvas = m_pCanvas;
	Texture* pMask = m_pCanvasObject;

	DepthStencil* pDepth = m_pDepth;

	// ---透過テクスチャ描画---
	RenderTarget* pRTVCanvas = reinterpret_cast<RenderTarget*>(pMask);
	ID3D11RenderTargetView* pView = pRTVCanvas->GetView();
	GetDeviceContext()->OMSetRenderTargets(1, &pView, pDepth->GetView());

	// ---描画先のクリア---
	float color[4] = { 1.0f,1.0f,1.0f,1.0f };					// 奥行比較のために、一番遠い1を初期値で設定
	float disa[4] = { 0.0f,0.0f,0.0f,0.0f };
	GetDeviceContext()->ClearRenderTargetView(pView, disa);
	GetDeviceContext()->ClearDepthStencilView(pDepth->GetView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ---シェーダーセット---
	GetVS(VS_DEPTHWRITE)->Bind();
	GetPS(PS_DEPTHWRITE)->Bind();

	// ---プレイヤーをテクスチャに書き込む---
	std::vector<Object*> Objects = FindObjectListWithTag(OT_ENEMY);
	std::vector<Object*> bullets = FindObjectListWithTag(OT_ENEMY_BULLET);
	Object* player = FindObjectWithTag(OT_PLAYER);

	for (auto bullet : bullets)
	{
		Objects.push_back(bullet);
	}
	if(player)
		Objects.push_back(player);
	
	for(auto obj : Objects)
	{
		XMStoreFloat4x4(&mat[0], XMMatrixTranspose(XMLoadFloat4x4(&obj->GetWorld())));								// ワールド変換行列
		XMStoreFloat4x4(&mat[1], XMMatrixTranspose(XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetViewMatrix())));		// ビュー行列
		XMStoreFloat4x4(&mat[2], XMMatrixTranspose(XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetProjMatrix())));		// ビュー行列

		m_pWVP->Write(mat);							// 定数バッファに書き込み
		m_pWVP->BindVS(0);							// 頂点シェーダ―にセット
		obj->DrawShadow();
	}

	// ---シャドウマップ生成---
	pRTVCanvas = reinterpret_cast<RenderTarget*>(pCanvas);
	pView = pRTVCanvas->GetView();

	GetDeviceContext()->OMSetRenderTargets(1, &pView, pDepth->GetView());

	// ---描画先のクリア---
	GetDeviceContext()->ClearRenderTargetView(pView, color);
	GetDeviceContext()->ClearDepthStencilView(pDepth->GetView(),D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	XMFLOAT3 look = CAMERA->GetCurrentCamera()->GetLook();

	// ---太陽から見た視点の設定---(太陽の位置にカメラがあると想定して、テクスチャに書き込む)
	DirectX::XMMATRIX sunView = DirectX::XMMatrixLookAtLH(			//   カメラをある程度離した位置を設定する
		DirectX::XMVectorSet(-300.0f, 1000.0f, -1000.0f, 0.0f),
		DirectX::XMVectorSet(look.x, look.y, look.z ,0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	DirectX::XMMATRIX sunProj = DirectX::XMMatrixOrthographicLH(50, (16 / 9) * 50, 0.2f, 2000.0f);

	// ---転置計算---
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixTranspose(sunView));
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(sunProj));

	// ---オブジェクトをテクスチャに書き込む---
	while (it != m_pObjectList.end())
	{
		XMMATRIX m = XMLoadFloat4x4(&it->second->GetWorld());				// オブジェクトのワールド変換マトリックスを取得
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(m));	// シェーダーに渡すために転置
		m_pWVP->Write(mat);							// 定数バッファに書き込み
		m_pWVP->BindVS(0);							// 頂点シェーダ―にセット
		if(it->second->GetObjID() != OT_WALL)		// ---壁オブジェクトは影を書き込まない(暗くなりすぎるため)
			it->second->DrawShadow();				// 描画

		++it;		// 次のオブジェクト
	}

	// ---描画先を元の画面に戻す---
	ResetRenderTarget();
	m_pCBSun->Write(&mat[1]);
	m_pCBSun->BindVS(2);

	GetVS(VS_OBJECT)->Bind();
	GetPS(PS_LAMBERT)->Bind();
	XMFLOAT3 Pos = CAMERA->GetCurrentCamera()->GetPos();
	XMFLOAT4 buf = XMFLOAT4(Pos.x, Pos.y, Pos.z, 0.0f);
	m_pCBCamera->Write(&buf);
	m_pCBCamera->BindVS(1);									// カメラ定数をシェーダーに渡す
	m_pCBCamera->BindPS(1);

	m_pCBlight->Write(&m_LightDir);							// ライト定数を渡す
	m_pCBlight->BindPS(0);

	m_pCBColor->Write(&Color);
	m_pCBColor->BindPS(2);									// 色定数を渡す

	SetTexturePS(pCanvas->GetResource(), 2);				// 書き込んだテクスチャを渡す
	SetTexturePS(pMask->GetResource(), 3);					// 書き込んだテクスチャを渡す
}

void SceneBase::SetColorBuffer(XMFLOAT4* color)
{
	 m_pCBColor->Write(color); 			// 定数バッファに色をセット(輪郭線など)
	 m_pCBColor->BindPS(2);
}