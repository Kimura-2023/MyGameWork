// BGM/SE定義 [SoundData.h]
#pragma once
enum eBGM {
	BGM_GAME = 0,	// ゲームBGM
	BGM_TITLE,		// タイトルBGM

	MAX_BGM
};

enum eSE {
	SE_SWORD = 0,	// 攻撃ヒット音
	SE_EXPLOSION,	// 爆発音
	SE_DEFEND,		// 衝突音
	SE_GAMEOVER,	// ゲームオーバーBGM
	SE_SHOT,		// 弾発射音
	SE_REPEL,		// 反射音
	SE_AVOID,		// 回避音
	SE_DUSH,		// ダッシュ音
	SE_ENEMY,		// 敵攻撃音
	SE_WARP,		// 階層クリア音

	MAX_SE
};
