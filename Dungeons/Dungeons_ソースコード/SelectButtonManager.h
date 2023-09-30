#pragma once
// ===ボタン選択管理クラス定義===
// ボタン選択管理クラスを定義
// Author kimura
// ======================

// ===インクルード===
#include "main.h"
#include "UIButton.h"
#include <vector>

// ===定数・マクロ定義===
#define BUTTON (SelectButtonManager::Get())			// インスタンス取得

// ===クラス定義===
class SelectButtonManager
{
	// ---メンバ関数---
public:
	SelectButtonManager();					// コンストラクタ
	~SelectButtonManager();					// デストラクタ

	static void Create();					// 生成
	static void Destroy();					// 破棄
	static SelectButtonManager* Get();		// 取得

	void Update();							// 入力読み取り
	void SetSelect(int);					// 選択更新

	int AddButtonManager(UIButton*);		// 管理下に追加
	void Release(UIButton*);				// 管理から除外

	// ---メンバ変数---
private:
	static SelectButtonManager* m_pInstance; // インスタンス
	std::vector<UIButton*> m_pButtonList;	// ボタン一覧

	UIButton* m_pCurrentSelect;				// 現在選択中のボタン
	int m_nButtonCnt;						// 現在ボタン数
};