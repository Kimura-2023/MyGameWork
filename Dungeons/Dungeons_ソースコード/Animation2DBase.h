#pragma once
// ===2Dアニメーションベースクラス定義===
// 2Dアニメーションベースクラスを定義
// Author:	Kimura
// ==============================

// ===インクルード===
#include "Object2DBase.h"

// ===クラス定義===
class Animation2DBase : public Object2DBase
{
	// ---メンバ関数---
public:
	Animation2DBase();
	~Animation2DBase();

	virtual void Update();		// 更新

	bool GetEnd();				// 再生終了したかどうか
	int GetFrame();				// 現在フレーム取得
	void SetSpeed(float);		// スピード変更
	void SetMaxAnim(int);		// アニメ枚数
	void SetStartFrame(int);	// アニメーションの開始フレーム
	void SetnEnd();				// アニメーション終了

	// ---メンバ変数---
protected:
	int m_nFrame;		// 再生中フレーム数
	int m_nOld;			// 前フレーム
	int m_nStart;		// アニメーションの開始uv
	int m_nAnimCnt;		// アニメーションする枚数
	float m_fSpd;		// 再生スピード(計算用)
	float m_fdefault;	// スピードリセット用

	bool m_Loop;		// 再生し続ける
	bool m_bEnd;		// 再生終了フラグ
	bool m_bDestroy;	// 再生終了で破棄するかどうか
};
