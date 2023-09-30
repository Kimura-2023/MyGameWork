#pragma once
// ===シーンベースクラス定義===
// シーンベースクラスを定義
// Author kimura
// =================================

// ===インクルード===
#include "main.h"
#include "Object.h"
#include <map>
#include "DepthStencil.h"

// ===クラス定義===
class SceneBase
{
	// ---メンバ関数---
public:
	SceneBase();					// コンストラクタ
	virtual ~SceneBase();			// デストラクタ
	virtual void Update() = 0;		// 更新処理
	virtual void Draw();			// 描画処理

	// **シーンデータ関連関数**
	Object* FindGameObject(std::string);								// オブジェクト探索
	Object* CreateGameObject(std::string, ObjType, int num = 0);		// オブジェクト生成
	bool DeleteGameObject(std::string);									// オブジェクト消去
	void ReleaseAllGameObject();										// 全オブジェクト解放

	int& RangeObjectCount(XMFLOAT3&, XMFLOAT3&);						// 特定範囲内のオブジェクト総数をカウント

	void SaveSceneData();							// シーンデータを保存
	void SaveSceneData(std::string);				// シーンデータを保存(ファイル名指定)
	bool LoadSceneData();							// シーンデータ読込
	bool LoadSceneData(std::string);				// シーンデータ読込(ファイル名指定)

	void LoadStageData(std::string,float rot,int level);		// ステージデータ読込

	std::string GetSceneName() { return m_Name; }		// シーン名取得
	std::map<std::string, Object*>* GetAllObjects();	// 全オブジェクト取得

	Object* FindObjectWithTag(ObjType);						// オブジェクトタグからオブジェクト探索・取得
	std::vector<Object*> FindObjectListWithTag(ObjType);	// オブジェクトタグからオブジェクトリスト取得

	void SetColorBuffer(XMFLOAT4* color);				// 輪郭線描画色セット

	ConstantBuffer* GetSunBuffer() { return m_pCBSun; }	// DirectionalLightバッファ取得

	// ---メンバ変数---
protected:
	std::string m_Name;								// シーン名
	XMFLOAT4 m_LightDir;							// ライト(太陽光)
	XMFLOAT4 Color;									// 輪郭線色
private:
	std::map<std::string, Object*> m_pObjectList;	// シーン内のオブジェクトリスト

	// **影の描画用**
	Texture* m_pCanvas;
	DepthStencil* m_pDepth;

	// **プレイヤー強調表示用**
	Texture* m_pCanvasObject;

	// **定数バッファ**
	ConstantBuffer* m_pCBlight;						// ライト情報用定数バッファ
	ConstantBuffer* m_pCBCamera;					// カメラ用定数バッファ
	ConstantBuffer* m_pCBColor;						// 色情報用バッファ
	ConstantBuffer* m_pCBSun;						// 影用の太陽用バッファ
	ConstantBuffer* m_pWVP;							// 影をテクスチャに書き込むためのバッファ
};