/*
作成者: Tokuda Ryo

編集者：Osaki Noriaki

更新履歴
     2021/12/09		ファイル作成
	 2021/12/21		エフェクト確認
	 2021/12/24		プレイヤー死亡追加

*/

#pragma once

#include "ObjectBase3D.h"
#include "EnemyManager.h"
#include "SpawnBase.h"
#include "Effect.h"
#include "TraceManager.h"
#include "Tereport.h"


class Player : public ObjectBase3D, public Collision3D
{
public:
	Player();
	~Player();
	void Update();

	OFLOAT2 GetInput();
	bool m_Attackflg;
	static bool Getflg();
	static OFLOAT3 GetPos() { return  m_Pos; };

	static void SetRespawnFlg(bool Resflg, bool Dedflg);

	bool GetRespawnFlg() { return m_Respawnflg; };
	

private:
	static bool m_Deadflg;			//死亡フラグ
	float  m_stopTimer;		//ヒットストップタイマー
	bool m_gameOver;		//ゲームオーバー
	float m_jump;
	float m_gravity;
	float m_tereportTimer;	// テレポート地点停止タイマー
	int m_dir;				// プレイヤー向き
	static bool m_Respawnflg;		// 復帰フラグ
	bool m_height;					// テレポート位置が高い
	bool m_firstTp;

	static OFLOAT3 m_Pos;	// バカ(座標渡す

	OFLOAT3 m_movePos;		// 移動量
	OFLOAT3 m_oldPos;		// 過去座標
	OFLOAT2 m_input;		// 入力
	EnemyManager *m_pEnemyList;
	SpawnBase *m_pSpawnBase;
	TraceManager *m_pTM;

	Effect* m_pEffect;		//エフェクト確認
	Tereport* m_pTereport;

};