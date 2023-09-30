/*
作成者: Kimura Yusaku

編集者：

更新履歴
	 2022/01/21		ファイル作成
*/
#pragma once

#include <vector>

#define SELECT (StageSelectManager::Get())		// マネージャ取得置き換え
#define STAGE_SIZE (4)							// ステージ数

class StageSelectManager
{
private:
	StageSelectManager();						// コンストラクタ
	~StageSelectManager();						// デストラクタ

public:
	static void Create();						// 初期化
	static void Destroy();						// 終了
	static StageSelectManager* Get();			// 取得

	void AddStage(bool);						// ステージ情報追加
	bool GetStage(int);							// ステージ情報取得(ステージの関数呼び出しなど
	void SetClear(int);
	void SetStageNum(int);						// ステージ番号セット
	int GetStageNum();							// ステージ番号取得

private:
	static StageSelectManager* m_pInstance;		// マネージャ
	std::vector<bool> m_pSelect;				// ステージクリア情報
	int m_num;									// 現在ステージ番号
};