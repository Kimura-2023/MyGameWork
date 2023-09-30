// BGM/SE定義 [SoundData.hpp]
#pragma once
LPCWSTR g_pszFileBGM[MAX_BGM] = {
	L"game.mp3",			// ゲームBGM
	L"battle12.mp3",		// タイトルBGM
};

LPCWSTR g_pszFileSE[MAX_SE] = {
	L"attack.mp3",			// 攻撃ヒット音
	L"break.mp3",			// 爆発音
	L"hit.mp3",				// 衝突音
	L"jingle03.mp3",		// ゲームオーバーBGM
	L"shot0.wav",			// 弾発射音
	L"repel.mp3",			// 反射音
	L"avoid.mp3",			// 回避音
	L"dush.mp3",			// ダッシュ音
	L"boost.mp3",			// 敵攻撃音
	L"warp.mp3",			// ワープ音
};
