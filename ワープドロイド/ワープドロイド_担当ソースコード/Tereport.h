/*
作成者 : KimuraYusaku

更新者 : Osaki Noriaki

更新履歴
		2021/12/11		作成
		2021/12/18		挙動変更のため戻り値変更
		2022/1/3		クラス化、画像設定
*/
#pragma once

#include "OriginalType.h"
#include <list>
#include "ObjectBase2D.h"
#include "EnemyManager.h"
#include "ArrowManager.h"

class Tereport : public ObjectBase2D
{
public:
	Tereport();
	~Tereport();

	void Update();

	static OFLOAT3 Do(OFLOAT3 *);/*, std::list<Enemy*>, OFLOAT2*/				// テレポート関数
	static bool GetTereport(void);											// テレポートフラグ取得
	static void SetTereport(bool);											// テレポートフラグ設定
	static void SetTarget(void);

private:
	ObjectBase2D* m_pWave;
	void UpdateWave();



	Enemy* Serch(OFLOAT3, std::list<Enemy*>, OFLOAT2);
	static Enemy* m_pActive;
	OFLOAT2 m_input;
	EnemyManager *m_pEM;

	ArrowManager *m_pAM;
};