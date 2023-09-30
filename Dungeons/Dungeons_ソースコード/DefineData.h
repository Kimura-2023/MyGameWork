// ===定数定義用ヘッダ===
// パラメータで扱う実数などを定義
#pragma once

#define GRAVITY (0.0098f)		// 9.8m/s^2 ?

#define PLAYER_SPEED		(10 / 360.0f) // 秒速10メートルをフレーム単位にする
#define MAX_FIELD_WIDTH		(2500.0f)	// 最大フィールド幅
#define MAX_FIELD_HEIGHT	(2500.0f)	// 最大フィールド奥行

#define MIN_FIELD_WIDTH		(128.0f)	// 最小部屋幅
#define MIN_FIELD_HEIGHT	(128.0f)	// 最小部屋奥行

#define MIN_ROOM			(5)			// 最小部屋数
#define MAX_ROOM			(32)		// 最大部屋数

#define MAX_WIDTH			(5)			// 横部屋数
#define MAX_HEIGHT			(5)			// 縦部屋数

#define DEAD_STICK_X		(0.3f)		// X軸入力無視絶対値
#define DEAD_STICK_Y		(0.3f)		// Y軸入力無視絶対値