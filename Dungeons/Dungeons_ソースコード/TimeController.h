#pragma once
#include "main.h"
#include <vector>
#include <functional>
#include <map>

#define RESETINTERVAL (3600)
#define TIME (TimeController::Get())

class TimeController
{
public:
	static void Create();			// 生成
	static TimeController* Get();	// 取得
	static void Destroy();			// 破棄

	void Update();					// 更新処理
	float GetTime();				// 時間取得

	void SetTimeUpdateFlg(bool flg = false);

	void SetStop(const float& time) { if(m_stopTime <= 0.0f)m_stopTime = time * 60; }			// 更新停止時間をセット(秒間)
	void SetSlow(const float& time, const int & cut = 2) { if (m_slowTime <= 0.0)m_slowTime = time * 60; m_nCut = cut; }			// スローモーション時間をセット(秒間,割合)

	void ResetProcess(void);																	// セットされた関数を全てリセット
	void ResetProcess(int);																		// 関数削除(引数指定)
	void SetProcessTimer(const std::function<void(void)>&, float, bool flg = false);			// 時間差関数呼び出し

	const bool& GetUpdateFlg() { return m_bEnable; }
private:
	TimeController();
private:
	static TimeController* m_pInstance;

	float m_nowTime;				// 経過フレーム
	int m_Second;					// 経過時間(秒)
	bool m_bEnable;					// 更新の有無
	float m_stopTime;				// 停止時間
	float m_slowTime;				// スローモーション時間
	int m_nCut;						// スロー比率
	bool m_bPause;					// ポーズ

	std::vector <std::tuple<float, std::function<void(void)>, bool>> m_funcList;				// 実行関数リスト<<タイマー,関数>,ループ指定>
};