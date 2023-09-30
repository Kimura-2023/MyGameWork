#pragma once
// ===�}�b�v�Ǘ��N���X��`===
// �}�b�v�Ǘ��N���X���`
// Author kimura
// ======================

// ===�C���N���[�h===
#include "main.h"
#include <vector>
#include <map>

// ===�萔�E�}�N����`===
#define MAP (MapManager::Get())			// �C���X�^���X�擾

// ===�����̌^===
enum RoomType
{
	ROOM_NONE = 0,
	ROOM_ONE,				// 1�̕��������ɘA�����Ă��镔��
	ROOM_I,					// L�^�ɘA�����Ă��镔��
	ROOM_L,					// I�^�ɘA�����Ă��镔��
	ROOM_T,					// T�^�ɘA�����Ă��镔��
	ROOM_CROSS,				// �\���^�ɘA�����Ă��镔��

	ROOM_GOAL,				// �S�[������
	ROOM_MAX,
};

// ===�A������===
enum BitDirection
{
	NBit = 1 << 0,			// �k
	EBit = 1 << 1,			// ��
	SBit = 1 << 2,			// ��
	WBit = 1 << 3,			// ��
};

// ===����===
struct Room
{
	bool clear;					// �N���A���
	float rot;					// ��]��
	Room* chainRooms[4];		// �A�����Ă��镔��
	RoomType type;				// �����̌^
	std::string pass;			// �X�e�[�W�t�@�C���̃p�X
	int distance_x;				// x�ʒu
	int distance_y;				// y�ʒu

	// ---������---
	Room()
	{
		clear = false;					// �N���A���
		rot = 0.0f;						// ��]��
		type = ROOM_NONE;
		distance_x = 0.0f;
		distance_y = 0.0f;
		for (int i = 0; i < 4; ++i)
		{
			chainRooms[i] = nullptr;	// �A�����Ă��镔��
		}
	}
};

// ===�N���X��`===
class MapManager
{
	// ---�����o�֐�---
public:
	MapManager();							// �R���X�g���N�^
	~MapManager();							// �f�X�g���N�^
	static MapManager* Get();				// �C���X�^���X�擾
	static void Destroy();					// �C���X�^���X�j��
	static void Create();					// �C���X�^���X����
	void CreateStage();						// �X�e�[�W����
	void CreateRoom(int*);					// ��������

	std::vector<std::vector<Room*>> GetRooms();			// �����擾

	void DeleteRooms();						// �S�������

	Room* GetCurrentRoom() { return m_pCurrentRoom; }				// ���ݕ����擾
	bool GetClear() { return m_pCurrentRoom->clear; }				// ���ݕ����̃N���A��Ԏ擾
	void SetClear(bool flg) { m_pCurrentRoom->clear = flg; }		// �N���A��Ԃ�ݒ�
	void ChangeRoom(int);											// �����ړ�
	void AddLevel();												// �K�w���X�V
private:
	void RegistRoom(const int&, const int&, Room*);			// �����o�^
	void ChainRoom(int*);									// �����A��
	Room* FindRoom(const int&, const int&);					// �����T��

	// ---�����o�ϐ�---
public:
	int goal_x;
	int goal_y;
private:
	static MapManager* m_pInstance;				// �C���X�^���X
	std::vector<std::vector<Room*>> m_pRooms;	// ����

	int nMaxWidth;		// ���ő啪����
	int nMaxHeight;		// �c�ő啪����
	int m_nLevel;		// �}�b�v��Փx

	Room* m_pCurrentRoom;

	std::map<std::tuple<int, int>, Room*> m_Room;
	std::vector<int> m_RoomWidth;
	std::vector<int> m_MinRoomPos;


};