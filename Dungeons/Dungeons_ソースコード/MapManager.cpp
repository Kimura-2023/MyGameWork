// ===マップ生成管理クラス 実装 MapManager.cpp
// ---Author kimura

// ===インクルード===
#include "MapManager.h"
#include "DefineData.h"
#include "Calc.h"
#include "SceneManager.h"
#include <string>

// ===静的メンバ===
MapManager* MapManager::m_pInstance = nullptr;

// ===定数・マクロ定義===
#define ROOM_ONE_STAGE_MAX		(1)			// 一方向ステージ数
#define ROOM_I_STAGE_MAX		(1)			// I字ステージ数
#define ROOM_L_STAGE_MAX		(1)			// L字ステージ数
#define ROOM_T_STAGE_MAX		(1)			// T字ステージ数
#define ROOM_CROSS_STAGE_MAX	(2)			// 十字ステージ数

// ===ステージファイルパス===
// ---一方向ステージ---
namespace RoomOneStage
{
	LPCSTR normal[ROOM_ONE_STAGE_MAX] = {
		"data/stage/Stage1.dat",
	};

	LPCSTR goal[1] =
	{
		"data/stage/Goal1.dat",
	};
}
// ---I字ステージ---
namespace RoomIStage
{
	LPCSTR normal[ROOM_I_STAGE_MAX] = {
		"data/stage/Stage2.dat",
	};

	LPCSTR goal[1] =
	{
		"data/stage/Goal2.dat",
	};
}
// ---L字ステージ---
namespace RoomLStage
{
	LPCSTR normal[ROOM_L_STAGE_MAX] = {
		"data/stage/Stage3.dat",
	};

	LPCSTR goal[1] =
	{
		"data/stage/Goal3.dat",
	};
}
// ---T字ステージ---
namespace RoomTStage
{
	LPCSTR normal[ROOM_T_STAGE_MAX] = {
		"data/stage/Stage4.dat",
	};

	LPCSTR goal[1] =
	{
		"data/stage/Goal4.dat",
	};
}
// ---十字ステージ---
namespace RoomCrossStage
{
	LPCSTR normal[ROOM_CROSS_STAGE_MAX] = {
		"data/stage/Stage5.dat",
		"data/stage/Stage6.dat",
	};

	LPCSTR goal[1] =
	{
		"data/stage/Goal4.dat",
	};
}

// ===コンストラクタ===
MapManager::MapManager()
{
	m_nLevel = 0;
	CreateStage();						// ステージ生成
}

// ===デストラクタ===
MapManager::~MapManager()
{
	DeleteRooms();						// ステージ解放
}

// ===生成===
void MapManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new MapManager;		// インスタンスを生成
	}
}

// ===破棄===
void MapManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;					// インスタンスを破棄
		m_pInstance = nullptr;				// nullを格納
	}
}

// ===取得===
MapManager* MapManager::Get()
{
	return m_pInstance;						// ゲームマネージャのインスタンスを返す
}

// ===ステージ生成===
void MapManager::CreateStage()
{
	nMaxHeight = Random<int>(4, MIN_ROOM + m_nLevel);			// 縦の部屋の数を決める

	// ---変数---
	int NumRooms = 0;					// 部屋数
	int Horizontal = 1;					// 列

	// ---スタート地点を作成する---
	m_pCurrentRoom = new Room();				// 部屋生成
	m_pCurrentRoom->distance_x = 0;			// スタート地点からのずれ
	m_pCurrentRoom->distance_y = 0;			// スタート地点からのずれ
	m_pCurrentRoom->clear = true;				// クリア判定
	RegistRoom(0, 0, m_pCurrentRoom);			// 部屋登録
	m_RoomWidth.push_back(1);			// 部屋列の横幅
	m_MinRoomPos.push_back(0);			// 左端部屋の位置

	// ---それ以外の部屋生成---
	CreateRoom(&Horizontal);			// 部屋生成

	// ---ゴール部屋を生成---
	Horizontal = Random<int>(1, nMaxHeight);			// どの部屋番号に作成するかをランダムで判断する
	if (nMaxHeight <= Horizontal)			// ---縦の最大数を超えていたら
	{
		int x = Random<int>(m_MinRoomPos[nMaxHeight - 1], m_RoomWidth[nMaxHeight - 1] + m_MinRoomPos[nMaxHeight - 1]);		// 1つ前のへやからどの位置に作成するかをランダムで算出

		m_MinRoomPos.push_back(x);			// 部屋左端位置を追加
		m_RoomWidth.push_back(1);			// 部屋幅を追加
		nMaxHeight = Horizontal + 1;
		Room* goal = new Room;				// 部屋を生成
		goal->distance_x = x;				// x位置を設定
		goal->distance_y = Horizontal;		// y位置を設定
		goal->type = ROOM_GOAL;				// 部屋の型をゴールに設定
		RegistRoom(Horizontal, x, goal);	// 部屋を登録

		goal_x = x;
		goal_y = Horizontal;
	}
	else									// ---それ以外
	{
		int rnd = Random<int>(0, 2);		// 左右どちらの端に生成するか求める
		int x;
		(rnd > 0) ? x = m_MinRoomPos[Horizontal] - 1 : x = m_MinRoomPos[Horizontal] + m_RoomWidth[Horizontal];		// 位置を算出

		// ---部屋幅を更新---
		if (x < m_MinRoomPos[Horizontal])
		{
			m_MinRoomPos[Horizontal] = x;		// 左端位置を更新
		}
		m_RoomWidth[Horizontal] = m_RoomWidth[Horizontal] + 1;		// 部屋の幅を更新

		Room* goal = new Room;				// 部屋を生成
		goal->distance_x = x;				// x位置を設定
		goal->distance_y = Horizontal;		// y位置を設定
		goal->type = ROOM_GOAL;				// 部屋の型をゴールに設定
		RegistRoom(Horizontal, x, goal);	// 部屋を登録

		goal_x = x;
		goal_y = Horizontal;
	}

	// ---全ての部屋を連結---
	Horizontal = 0;						// 列
	ChainRoom(&Horizontal);				// 部屋同士を連結
}

// ===部屋生成===
void MapManager::CreateRoom(int* Horizontal)
{
	if (*Horizontal >= nMaxHeight)			// 最大列の場合は終了
		return;

	// ---生成---
	int wid = Random<int>(4, MIN_ROOM + m_nLevel);			// 列の幅をランダムで求める
	int distance = Random<int>(-wid / 2,0);		// 横側ずれを求める
	m_RoomWidth.push_back(wid);					// 部屋列の幅(x方向)の数を追加
	m_MinRoomPos.push_back(distance);			// 左端部屋の位置を追加
	std::vector<Room*> rooms;					// 部屋生成用
	for (int i = 0; i < wid; ++i)			// ---求めた部屋の数だけ生成
	{
		Room* room = new Room;				// 部屋を生成
		int x = distance + i;				// 左端から生成
		room->distance_x = x;				// 部屋位置x
		room->distance_y = *Horizontal;		// 部屋位置y
		RegistRoom(*Horizontal,x, room);	// 部屋登録
	}

	*Horizontal = *Horizontal + 1;			// 次の列の部屋を生成
	CreateRoom(Horizontal);					// 再帰処理
}

// ===全部屋解放
void MapManager::DeleteRooms()
{
	auto it = m_Room.begin();		// 先頭イテレータ取得
	while (it != m_Room.end())		// 終端まで繰り返し
	{
		delete it->second;			// 部屋解放
		it = m_Room.erase(it);		// リストから除外
	}
	m_Room.clear();					// 部屋マップ要素を0にする
	m_RoomWidth.clear();			// 部屋幅リストを0にする
	m_MinRoomPos.clear();			// 部屋左端位置リストを0にする
}

// ===全部屋取得===
std::vector<std::vector<Room*>> MapManager::GetRooms()
{
	return m_pRooms;
}

// ===部屋登録===
void MapManager::RegistRoom(const int& y, const int& x, Room* room)
{
	auto it = m_Room.find(std::make_tuple(y,x));			// 部屋番号で探索
	if (it == m_Room.end())							// 登録していなければmapに登録
		m_Room.insert(std::pair<std::tuple<int, int>, Room*>(std::make_tuple(y, x), room));
}

// ===部屋探索===
 Room* MapManager::FindRoom(const int& y, const int& x)
{
	auto it = m_Room.find(std::make_tuple(y, x));			// 部屋番号から探索

	if (it != m_Room.end())			// 見つかった場合
		return it->second;			// 見つかった部屋を返す
	return nullptr;
}

 // ===部屋同士を連結させる===
 void MapManager::ChainRoom(int* Holizontal)
 {
	 if (*Holizontal >= nMaxHeight)					// 最大列数を超えていれば終了
		 return;

	 int distance = m_MinRoomPos[*Holizontal];				// 列の左端部屋位置を取得
	 for (int i = 0; i < m_RoomWidth[*Holizontal]; ++i)
	 {
		 int x = i + distance;													// 部屋のx位置
		 std::tuple<int, int> index = std::make_tuple(*Holizontal, x);			// 登録用の番号を作成

		 Room* rooms[4];
		 rooms[0] = m_Room[index]->chainRooms[0] = FindRoom(*Holizontal + 1, x);				// 北方向
		 rooms[1] = m_Room[index]->chainRooms[1] = FindRoom(*Holizontal, x + 1);				// 東方向
		 rooms[2] = m_Room[index]->chainRooms[2] = FindRoom(*Holizontal - 1, x);				// 南方向
		 rooms[3] = m_Room[index]->chainRooms[3] = FindRoom(*Holizontal, x - 1);				// 西方向

		 // ---部屋の連結状態からどの形の部屋か判断する---
		 char bit = 0;
		 int cnt = 0;

		 // ---4方向探索---
		 for (int j = 0; j < 4; ++j)
		 {
			 if (rooms[j] != nullptr)			// 連結している部屋が空じゃない場合
			 {
				 bit = bit | 1 << j;			// 対象の部屋にビットを立てる
				 ++cnt;							// 連結している部屋数をカウント
			 }
		 }

		 int b = 0;

		 // ---連結している部屋の数と方向から部屋の型,向きを判定---
		 switch (cnt)
		 {
		 case 1:				// 連結部屋1つ
			 // ---どのビットが立っているか調べる
			 while ((bit & 0x1) == 0x0)
			 {
				bit = bit >> 1;
				 ++b;
			 }
			 m_Room[index]->rot = 90.0f * b;		// 連結方向の分だけ回転させる

			 if (m_Room[index]->type == ROOM_GOAL)
			 {
				 m_Room[index]->pass = RoomOneStage::goal[Random<int>(0, 1)];
			 }
			 else
			 {
				 m_Room[index]->type = ROOM_ONE;	// 連結している部屋が一つなので1方向の部屋
				 m_Room[index]->pass = RoomOneStage::normal[Random<int>(0, ROOM_ONE_STAGE_MAX)];
			 }

			 continue;
		 case 2:				// 連結部屋2つ
			 if (bit == (NBit | SBit) || bit == (EBit | WBit))			// 一直線に連結している場合はI字部屋
			 {
				 if (bit == (NBit | SBit))			// 一直線で縦方向のみの連結の場合
					 m_Room[index]->rot = 180.0f * Random<int>(0, 1);		// ランダムに回転
				 else								// 横方向のみの連結
					 m_Room[index]->rot = Random<int>(0, 1) ? 90.0f : 270.0f;		// 回転

				 if (m_Room[index]->type == ROOM_GOAL)
				 {
					 m_Room[index]->pass = RoomIStage::goal[Random<int>(0, 1)];
				 }
				 else
				 {
					 m_Room[index]->type = ROOM_I;		// 部屋の型
					 m_Room[index]->pass = RoomIStage::normal[Random<int>(0, ROOM_I_STAGE_MAX)];		// ステージファイルのパスを設定
				 }
			 }
			 else														// L字連結の場合
			 {
				 if (bit == (NBit | EBit))			// 上と右に連結している場合
					 m_Room[index]->rot = 0.0f;		// そのまま
				 if (bit == (EBit | SBit))			// 右と下に連結している場合
					 m_Room[index]->rot = 90.0f;	// 90°回転
				 if (bit == (SBit | WBit))			// 下と左に連結している場合
					 m_Room[index]->rot = 180.0f;	// 180°回転
				 if (bit == (WBit | NBit))			// 左と上に連結している場合
					 m_Room[index]->rot = 270.0f;	// 270°回転

				 if (m_Room[index]->type == ROOM_GOAL)
				 {
					m_Room[index]->pass = RoomLStage::goal[Random<int>(0, 1)];
				 }
				 else
				 {
					 m_Room[index]->type = ROOM_L;		// 部屋の型
					 m_Room[index]->pass = RoomLStage::normal[Random<int>(0, ROOM_L_STAGE_MAX)];		// ステージファイルのパスを設定
				 }
			 }

			 continue;
		 case 3:					// 連結部屋3つ
			 // ---どのビットが立っていないかを調べる---
			 while ((bit & 0x1) == 0x1)
			 {
				 bit = bit >> 1;
				 ++b;
			 }
			 m_Room[index]->rot = 90.0f * (b ^ 0x2);			// 連結している方向に合わせて回転

			 if (m_Room[index]->type == ROOM_GOAL)
			 {
				 m_Room[index]->pass = RoomTStage::goal[Random<int>(0, 1)];
			 }
			 else
			 {
				 m_Room[index]->type = ROOM_T;			// 部屋の型
				 m_Room[index]->pass = RoomTStage::normal[Random<int>(0, ROOM_T_STAGE_MAX)];			// ステージファイルのパスを設定	
			 }
			 continue;

		 case 4:					// 連結部屋4つ
			 m_Room[index]->type = ROOM_CROSS;		// 部屋の型
			 m_Room[index]->rot = 90.0f * Random<int>(0, 3);	// ランダムに回転
			 m_Room[index]->pass = RoomCrossStage::normal[Random<int>(0, ROOM_CROSS_STAGE_MAX)];	// ステージファイルのパスを設定
			 continue;
		 }

	 }

	 *Holizontal += 1;			// 次の列
	 ChainRoom(Holizontal);		// 再帰処理
 }

 // ===部屋移動===
 void MapManager::ChangeRoom(int direction)
 {
	 // ---現在位置取得---
	 int x = m_pCurrentRoom->distance_x;
	 int y = m_pCurrentRoom->distance_y;

	 switch (direction)			// 進行した方向によって位置を変更
	 {
	 case 0: y++; break;
	 case 1: x++; break;
	 case 2: y--; break;
	 case 3: x--; break;
	 default: break;
	 }

	 // ---部屋を探索---
	 std::tuple<int, int> index = std::make_tuple(y, x);		// 進行先の部屋番号を作成
	 auto it = m_Room.find(index);				// 探索
	 if (it == m_Room.end())
		 return;

	 Room* pRoom = it->second;			// 見つかった部屋を次の部屋とする
	 m_pCurrentRoom = pRoom;			// 現在部屋更新

	 SCENE->GetCurrentScene()->LoadStageData(pRoom->pass,pRoom->rot,m_nLevel);		// ステージ読込
 }

// ===階層更新===
void MapManager::AddLevel()
{
	m_nLevel++;					// 階層を更新(敵数、部屋数を増加させる
	DeleteRooms();				// 現在ステージを削除
	CreateStage();				// 再生成
}