// ===�Q�[���V�[���N���X����===
// �Q�[���V�[���̃N���X������
// Author:	Kimura
// ==========================

// ===�C���N���[�h===
#include "GameScene.h"
#include "SceneManager.h"
#include "DebugProc.h"
#include "Input.h"
#include "MapManager.h"
#include "GameManager.h"
#include "GameCamera.h"
#include "CameraManager.h"
#include "Sound.h"
#include "Door.h"
#include "ScoreManager.h"

// ===�R���X�g���N�^===
GameScene::GameScene()
{
	// ---����������---
	CSound::SetVolume(0.5f);					// ���ʐݒ�
	CSound::Play(BGM_GAME);						// BGM�Đ�

	m_Name = "GameScene";						// �V�[����

	MapManager::Create();						// �}�b�v����
	Room* pRoom = MAP->GetCurrentRoom();		// �����̕����擾
	LoadStageData(pRoom->pass, pRoom->rot, 0);		// �����̌`����f�[�^��ǂݍ���

	CameraManager::Get()->RegisterCamera("GameCamera", GAME_CAMERA);					// �J��������
	CameraManager::Get()->SetCamera(CameraManager::Get()->FindCamera("GameCamera"));	// �J�������Z�b�g

	// ---�I�u�W�F�N�g����---
	Object* player = CreateGameObject("Player", OT_PLAYER);		// �v���C���[����

	((GameCamera*)(CameraManager::Get()->GetCurrentCamera()))->SetTargetObj(player);	// �J�����̒Ǐ]�Ώۂ��v���C���[�ɃZ�b�g
	XMFLOAT3 startPos = { 0.0f,1.0f,0.0f };		// �������W
	player->SetPos(startPos);					// ���W��ݒ�
}

// ===�f�X�g���N�^===
GameScene::~GameScene()
{
	CSound::Stop(BGM_GAME);						// BGM�X�g�b�v
	MAP->Destroy();								// �}�b�v�j��
	SCORE->UpdateHiScore();						// �n�C�X�R�A�X�V
	SCORE->Reset();								// ���݃X�R�A�����Z�b�g
}

// ===�X�V����===
void GameScene::Update()
{
	// ---�S�[�����B����---
	if (MAP->GetCurrentRoom()->type == ROOM_GOAL)
	{
		// SCENE->SetScene(SCENE_GAMEOVER);
	}
	else
	{
		CDebugProc::Print("%d,%d\n", MAP->goal_x, MAP->goal_y);
	}

	// ---�X�e�[�W�N���A����---
	std::vector<Object*>enemy = FindObjectListWithTag(OT_ENEMY);			// �ʏ�G���X�g���擾
	std::vector<Object*>enemy2 = FindObjectListWithTag(OT_ENEMY_BULLET);	// �������G���X�g���擾
	auto it = enemy.begin();			// �ʏ�G���X�g�̐擪�C�e���[�^���擾
	bool clear = true;					// �N���A����
	while (it != enemy.end())
	{
		if ((*it)->GetEnable())			// �������Ă���G�L�����N�^�[������Δ�N���A
			clear = false;			// �N���A�t���O���I�t�ɂ���
		++it;							// �C�e���[�^�����炷
	}

	auto bit = enemy2.begin();			// �������G���X�g�̐擪�C�e���[�^���擾
	while (bit != enemy2.end())
	{
		if ((*bit)->GetEnable())		// �������Ă���G�L�����N�^�[������Δ�N���A
			clear = false;			// �N���A�t���O���I�t�ɂ���
		++bit;							// �C�e���[�^�����炷
	}

	// ---�N���A������---
	if (clear && enemy.size() > 0)			// �������Ă���G�L�����N�^�[�����Ȃ��ꍇ
	{
		std::vector<Object*> door = FindObjectListWithTag(OT_DOOR);		// �h�A�̃��X�g���擾
		auto dit = door.begin();			// �h�A���X�g�̐擪�C�e���[�^���擾
		while (dit != door.end())
		{
			((Door*)(*dit))->Open();		// �h�A�J����
			++dit;							// �C�e���[�^�����炷
		}
		MAP->SetClear(true);				// �N���A������Z�b�g
	}
}

// ===�`�揈��===
void GameScene::Draw()
{
	SceneBase::Draw();						// �V�[���`��
}