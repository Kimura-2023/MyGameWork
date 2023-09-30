#pragma once
// ===マップ管理クラス定義===
// マップ管理クラスを定義
// Author kimura
// ======================

// ===インクルード===
#include "main.h"
#include <vector>
#include <map>

// ===定数・マクロ定義===
#define MAP (MapManager::Get())			// インスタンス取得

// ===部屋の型===
enum RoomType
{
	ROOM_NONE = 0,
	ROOM_ONE,				// 1つの部屋だけに連結している部屋
	ROOM_I,					// L型に連結している部屋
	ROOM_L,					// I型に連結している部屋
	ROOM_T,					// T型に連結している部屋
	ROOM_CROSS,				// 十字型に連結している部屋

	ROOM_GOAL,				// ゴール部屋
	ROOM_MAX,
};

// ===連結方向===
enum BitDirection
{
	NBit = 1 << 0,			// 北
	EBit = 1 << 1,			// 東
	SBit = 1 << 2,			// 南
	WBit = 1 << 3,			// 西
};

// ===部屋===
struct Room
{
	bool clear;					// クリア状態
	float rot;					// 回転量
	Room* chainRooms[4];		// 連結している部屋
	RoomType type;				// 部屋の型
	std::string pass;			// ステージファイルのパス
	int distance_x;				// x位置
	int distance_y;				// y位置

	// ---初期化---
	Room()
	{
		clear = false;					// クリア状態
		rot = 0.0f;						// 回転量
		type = ROOM_NONE;
		distance_x = 0.0f;
		distance_y = 0.0f;
		for (int i = 0; i < 4; ++i)
		{
			chainRooms[i] = nullptr;	// 連結している部屋
		}
	}
};

// ===クラス定義===
class MapManager
{
	// ---メンバ関数---
public:
	MapManager();							// コンストラクタ
	~MapManager();							// デストラクタ
	static MapManager* Get();				// インスタンス取得
	static void Destroy();					// インスタンス破棄
	static void Create();					// インスタンス生成
	void CreateStage();						// ステージ生成
	void CreateRoom(int*);					// 部屋生成

	std::vector<std::vector<Room*>> GetRooms();			// 部屋取得

	void DeleteRooms();						// 全部屋解放

	Room* GetCurrentRoom() { return m_pCurrentRoom; }				// 現在部屋取得
	bool GetClear() { return m_pCurrentRoom->clear; }				// 現在部屋のクリア状態取得
	void SetClear(bool flg) { m_pCurrentRoom->clear = flg; }		// クリア状態を設定
	void ChangeRoom(int);											// 部屋移動
	void AddLevel();												// 階層を更新
private:
	void RegistRoom(const int&, const int&, Room*);			// 部屋登録
	void ChainRoom(int*);									// 部屋連結
	Room* FindRoom(const int&, const int&);					// 部屋探索

	// ---メンバ変数---
public:
	int goal_x;
	int goal_y;
private:
	static MapManager* m_pInstance;				// インスタンス
	std::vector<std::vector<Room*>> m_pRooms;	// 部屋

	int nMaxWidth;		// 横最大分割数
	int nMaxHeight;		// 縦最大分割数
	int m_nLevel;		// マップ難易度

	Room* m_pCurrentRoom;

	std::map<std::tuple<int, int>, Room*> m_Room;
	std::vector<int> m_RoomWidth;
	std::vector<int> m_MinRoomPos;


};