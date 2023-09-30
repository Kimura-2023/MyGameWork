// ===�}�b�v�����Ǘ��N���X ���� MapManager.cpp
// ---Author kimura

// ===�C���N���[�h===
#include "MapManager.h"
#include "DefineData.h"
#include "Calc.h"
#include "SceneManager.h"
#include <string>

// ===�ÓI�����o===
MapManager* MapManager::m_pInstance = nullptr;

// ===�萔�E�}�N����`===
#define ROOM_ONE_STAGE_MAX		(1)			// ������X�e�[�W��
#define ROOM_I_STAGE_MAX		(1)			// I���X�e�[�W��
#define ROOM_L_STAGE_MAX		(1)			// L���X�e�[�W��
#define ROOM_T_STAGE_MAX		(1)			// T���X�e�[�W��
#define ROOM_CROSS_STAGE_MAX	(2)			// �\���X�e�[�W��

// ===�X�e�[�W�t�@�C���p�X===
// ---������X�e�[�W---
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
// ---I���X�e�[�W---
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
// ---L���X�e�[�W---
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
// ---T���X�e�[�W---
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
// ---�\���X�e�[�W---
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

// ===�R���X�g���N�^===
MapManager::MapManager()
{
	m_nLevel = 0;
	CreateStage();						// �X�e�[�W����
}

// ===�f�X�g���N�^===
MapManager::~MapManager()
{
	DeleteRooms();						// �X�e�[�W���
}

// ===����===
void MapManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new MapManager;		// �C���X�^���X�𐶐�
	}
}

// ===�j��===
void MapManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;					// �C���X�^���X��j��
		m_pInstance = nullptr;				// null���i�[
	}
}

// ===�擾===
MapManager* MapManager::Get()
{
	return m_pInstance;						// �Q�[���}�l�[�W���̃C���X�^���X��Ԃ�
}

// ===�X�e�[�W����===
void MapManager::CreateStage()
{
	nMaxHeight = Random<int>(4, MIN_ROOM + m_nLevel);			// �c�̕����̐������߂�

	// ---�ϐ�---
	int NumRooms = 0;					// ������
	int Horizontal = 1;					// ��

	// ---�X�^�[�g�n�_���쐬����---
	m_pCurrentRoom = new Room();				// ��������
	m_pCurrentRoom->distance_x = 0;			// �X�^�[�g�n�_����̂���
	m_pCurrentRoom->distance_y = 0;			// �X�^�[�g�n�_����̂���
	m_pCurrentRoom->clear = true;				// �N���A����
	RegistRoom(0, 0, m_pCurrentRoom);			// �����o�^
	m_RoomWidth.push_back(1);			// ������̉���
	m_MinRoomPos.push_back(0);			// ���[�����̈ʒu

	// ---����ȊO�̕�������---
	CreateRoom(&Horizontal);			// ��������

	// ---�S�[�������𐶐�---
	Horizontal = Random<int>(1, nMaxHeight);			// �ǂ̕����ԍ��ɍ쐬���邩�������_���Ŕ��f����
	if (nMaxHeight <= Horizontal)			// ---�c�̍ő吔�𒴂��Ă�����
	{
		int x = Random<int>(m_MinRoomPos[nMaxHeight - 1], m_RoomWidth[nMaxHeight - 1] + m_MinRoomPos[nMaxHeight - 1]);		// 1�O�̂ւ₩��ǂ̈ʒu�ɍ쐬���邩�������_���ŎZ�o

		m_MinRoomPos.push_back(x);			// �������[�ʒu��ǉ�
		m_RoomWidth.push_back(1);			// ��������ǉ�
		nMaxHeight = Horizontal + 1;
		Room* goal = new Room;				// �����𐶐�
		goal->distance_x = x;				// x�ʒu��ݒ�
		goal->distance_y = Horizontal;		// y�ʒu��ݒ�
		goal->type = ROOM_GOAL;				// �����̌^���S�[���ɐݒ�
		RegistRoom(Horizontal, x, goal);	// ������o�^

		goal_x = x;
		goal_y = Horizontal;
	}
	else									// ---����ȊO
	{
		int rnd = Random<int>(0, 2);		// ���E�ǂ���̒[�ɐ������邩���߂�
		int x;
		(rnd > 0) ? x = m_MinRoomPos[Horizontal] - 1 : x = m_MinRoomPos[Horizontal] + m_RoomWidth[Horizontal];		// �ʒu���Z�o

		// ---���������X�V---
		if (x < m_MinRoomPos[Horizontal])
		{
			m_MinRoomPos[Horizontal] = x;		// ���[�ʒu���X�V
		}
		m_RoomWidth[Horizontal] = m_RoomWidth[Horizontal] + 1;		// �����̕����X�V

		Room* goal = new Room;				// �����𐶐�
		goal->distance_x = x;				// x�ʒu��ݒ�
		goal->distance_y = Horizontal;		// y�ʒu��ݒ�
		goal->type = ROOM_GOAL;				// �����̌^���S�[���ɐݒ�
		RegistRoom(Horizontal, x, goal);	// ������o�^

		goal_x = x;
		goal_y = Horizontal;
	}

	// ---�S�Ă̕�����A��---
	Horizontal = 0;						// ��
	ChainRoom(&Horizontal);				// �������m��A��
}

// ===��������===
void MapManager::CreateRoom(int* Horizontal)
{
	if (*Horizontal >= nMaxHeight)			// �ő��̏ꍇ�͏I��
		return;

	// ---����---
	int wid = Random<int>(4, MIN_ROOM + m_nLevel);			// ��̕��������_���ŋ��߂�
	int distance = Random<int>(-wid / 2,0);		// ������������߂�
	m_RoomWidth.push_back(wid);					// ������̕�(x����)�̐���ǉ�
	m_MinRoomPos.push_back(distance);			// ���[�����̈ʒu��ǉ�
	std::vector<Room*> rooms;					// ���������p
	for (int i = 0; i < wid; ++i)			// ---���߂������̐���������
	{
		Room* room = new Room;				// �����𐶐�
		int x = distance + i;				// ���[���琶��
		room->distance_x = x;				// �����ʒux
		room->distance_y = *Horizontal;		// �����ʒuy
		RegistRoom(*Horizontal,x, room);	// �����o�^
	}

	*Horizontal = *Horizontal + 1;			// ���̗�̕����𐶐�
	CreateRoom(Horizontal);					// �ċA����
}

// ===�S�������
void MapManager::DeleteRooms()
{
	auto it = m_Room.begin();		// �擪�C�e���[�^�擾
	while (it != m_Room.end())		// �I�[�܂ŌJ��Ԃ�
	{
		delete it->second;			// �������
		it = m_Room.erase(it);		// ���X�g���珜�O
	}
	m_Room.clear();					// �����}�b�v�v�f��0�ɂ���
	m_RoomWidth.clear();			// ���������X�g��0�ɂ���
	m_MinRoomPos.clear();			// �������[�ʒu���X�g��0�ɂ���
}

// ===�S�����擾===
std::vector<std::vector<Room*>> MapManager::GetRooms()
{
	return m_pRooms;
}

// ===�����o�^===
void MapManager::RegistRoom(const int& y, const int& x, Room* room)
{
	auto it = m_Room.find(std::make_tuple(y,x));			// �����ԍ��ŒT��
	if (it == m_Room.end())							// �o�^���Ă��Ȃ����map�ɓo�^
		m_Room.insert(std::pair<std::tuple<int, int>, Room*>(std::make_tuple(y, x), room));
}

// ===�����T��===
 Room* MapManager::FindRoom(const int& y, const int& x)
{
	auto it = m_Room.find(std::make_tuple(y, x));			// �����ԍ�����T��

	if (it != m_Room.end())			// ���������ꍇ
		return it->second;			// ��������������Ԃ�
	return nullptr;
}

 // ===�������m��A��������===
 void MapManager::ChainRoom(int* Holizontal)
 {
	 if (*Holizontal >= nMaxHeight)					// �ő�񐔂𒴂��Ă���ΏI��
		 return;

	 int distance = m_MinRoomPos[*Holizontal];				// ��̍��[�����ʒu���擾
	 for (int i = 0; i < m_RoomWidth[*Holizontal]; ++i)
	 {
		 int x = i + distance;													// ������x�ʒu
		 std::tuple<int, int> index = std::make_tuple(*Holizontal, x);			// �o�^�p�̔ԍ����쐬

		 Room* rooms[4];
		 rooms[0] = m_Room[index]->chainRooms[0] = FindRoom(*Holizontal + 1, x);				// �k����
		 rooms[1] = m_Room[index]->chainRooms[1] = FindRoom(*Holizontal, x + 1);				// ������
		 rooms[2] = m_Room[index]->chainRooms[2] = FindRoom(*Holizontal - 1, x);				// �����
		 rooms[3] = m_Room[index]->chainRooms[3] = FindRoom(*Holizontal, x - 1);				// ������

		 // ---�����̘A����Ԃ���ǂ̌`�̕��������f����---
		 char bit = 0;
		 int cnt = 0;

		 // ---4�����T��---
		 for (int j = 0; j < 4; ++j)
		 {
			 if (rooms[j] != nullptr)			// �A�����Ă��镔�����󂶂�Ȃ��ꍇ
			 {
				 bit = bit | 1 << j;			// �Ώۂ̕����Ƀr�b�g�𗧂Ă�
				 ++cnt;							// �A�����Ă��镔�������J�E���g
			 }
		 }

		 int b = 0;

		 // ---�A�����Ă��镔���̐��ƕ������畔���̌^,�����𔻒�---
		 switch (cnt)
		 {
		 case 1:				// �A������1��
			 // ---�ǂ̃r�b�g�������Ă��邩���ׂ�
			 while ((bit & 0x1) == 0x0)
			 {
				bit = bit >> 1;
				 ++b;
			 }
			 m_Room[index]->rot = 90.0f * b;		// �A�������̕�������]������

			 if (m_Room[index]->type == ROOM_GOAL)
			 {
				 m_Room[index]->pass = RoomOneStage::goal[Random<int>(0, 1)];
			 }
			 else
			 {
				 m_Room[index]->type = ROOM_ONE;	// �A�����Ă��镔������Ȃ̂�1�����̕���
				 m_Room[index]->pass = RoomOneStage::normal[Random<int>(0, ROOM_ONE_STAGE_MAX)];
			 }

			 continue;
		 case 2:				// �A������2��
			 if (bit == (NBit | SBit) || bit == (EBit | WBit))			// �꒼���ɘA�����Ă���ꍇ��I������
			 {
				 if (bit == (NBit | SBit))			// �꒼���ŏc�����݂̘̂A���̏ꍇ
					 m_Room[index]->rot = 180.0f * Random<int>(0, 1);		// �����_���ɉ�]
				 else								// �������݂̘̂A��
					 m_Room[index]->rot = Random<int>(0, 1) ? 90.0f : 270.0f;		// ��]

				 if (m_Room[index]->type == ROOM_GOAL)
				 {
					 m_Room[index]->pass = RoomIStage::goal[Random<int>(0, 1)];
				 }
				 else
				 {
					 m_Room[index]->type = ROOM_I;		// �����̌^
					 m_Room[index]->pass = RoomIStage::normal[Random<int>(0, ROOM_I_STAGE_MAX)];		// �X�e�[�W�t�@�C���̃p�X��ݒ�
				 }
			 }
			 else														// L���A���̏ꍇ
			 {
				 if (bit == (NBit | EBit))			// ��ƉE�ɘA�����Ă���ꍇ
					 m_Room[index]->rot = 0.0f;		// ���̂܂�
				 if (bit == (EBit | SBit))			// �E�Ɖ��ɘA�����Ă���ꍇ
					 m_Room[index]->rot = 90.0f;	// 90����]
				 if (bit == (SBit | WBit))			// ���ƍ��ɘA�����Ă���ꍇ
					 m_Room[index]->rot = 180.0f;	// 180����]
				 if (bit == (WBit | NBit))			// ���Ə�ɘA�����Ă���ꍇ
					 m_Room[index]->rot = 270.0f;	// 270����]

				 if (m_Room[index]->type == ROOM_GOAL)
				 {
					m_Room[index]->pass = RoomLStage::goal[Random<int>(0, 1)];
				 }
				 else
				 {
					 m_Room[index]->type = ROOM_L;		// �����̌^
					 m_Room[index]->pass = RoomLStage::normal[Random<int>(0, ROOM_L_STAGE_MAX)];		// �X�e�[�W�t�@�C���̃p�X��ݒ�
				 }
			 }

			 continue;
		 case 3:					// �A������3��
			 // ---�ǂ̃r�b�g�������Ă��Ȃ����𒲂ׂ�---
			 while ((bit & 0x1) == 0x1)
			 {
				 bit = bit >> 1;
				 ++b;
			 }
			 m_Room[index]->rot = 90.0f * (b ^ 0x2);			// �A�����Ă�������ɍ��킹�ĉ�]

			 if (m_Room[index]->type == ROOM_GOAL)
			 {
				 m_Room[index]->pass = RoomTStage::goal[Random<int>(0, 1)];
			 }
			 else
			 {
				 m_Room[index]->type = ROOM_T;			// �����̌^
				 m_Room[index]->pass = RoomTStage::normal[Random<int>(0, ROOM_T_STAGE_MAX)];			// �X�e�[�W�t�@�C���̃p�X��ݒ�	
			 }
			 continue;

		 case 4:					// �A������4��
			 m_Room[index]->type = ROOM_CROSS;		// �����̌^
			 m_Room[index]->rot = 90.0f * Random<int>(0, 3);	// �����_���ɉ�]
			 m_Room[index]->pass = RoomCrossStage::normal[Random<int>(0, ROOM_CROSS_STAGE_MAX)];	// �X�e�[�W�t�@�C���̃p�X��ݒ�
			 continue;
		 }

	 }

	 *Holizontal += 1;			// ���̗�
	 ChainRoom(Holizontal);		// �ċA����
 }

 // ===�����ړ�===
 void MapManager::ChangeRoom(int direction)
 {
	 // ---���݈ʒu�擾---
	 int x = m_pCurrentRoom->distance_x;
	 int y = m_pCurrentRoom->distance_y;

	 switch (direction)			// �i�s���������ɂ���Ĉʒu��ύX
	 {
	 case 0: y++; break;
	 case 1: x++; break;
	 case 2: y--; break;
	 case 3: x--; break;
	 default: break;
	 }

	 // ---������T��---
	 std::tuple<int, int> index = std::make_tuple(y, x);		// �i�s��̕����ԍ����쐬
	 auto it = m_Room.find(index);				// �T��
	 if (it == m_Room.end())
		 return;

	 Room* pRoom = it->second;			// �����������������̕����Ƃ���
	 m_pCurrentRoom = pRoom;			// ���ݕ����X�V

	 SCENE->GetCurrentScene()->LoadStageData(pRoom->pass,pRoom->rot,m_nLevel);		// �X�e�[�W�Ǎ�
 }

// ===�K�w�X�V===
void MapManager::AddLevel()
{
	m_nLevel++;					// �K�w���X�V(�G���A�������𑝉�������
	DeleteRooms();				// ���݃X�e�[�W���폜
	CreateStage();				// �Đ���
}